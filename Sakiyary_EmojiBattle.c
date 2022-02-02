#include"Sakiyary_EmojiBattle.h"

int main() {
    InitAll();
    LoadRes();
    PrintBG(1, HintTheme);
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    ThemeUI(KeyValue);
    return 0;
}

void InitAll() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);
    Window = SDL_CreateWindow("Sakiyary$ Emoji Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    srand((unsigned) time(NULL));
}

void ThemeUI(const Uint8 *KeyValue) {
    ResetFontColor();
    while (1) {
        PrintBG(1, HintTheme);
        CgAngle -= CgAngle > 0 ? 4 : -356;
        ChangeFontColor();
        while (SDL_WaitEventTimeout(&Event, mSPF)) {
            if (Event.type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return;
            if (KeyValue[SDL_SCANCODE_RETURN] || KeyValue[SDL_SCANCODE_KP_ENTER]) {
                if (!GameUI(KeyValue))
                    return;
                ResetFontColor();
            }
        }
    }
}

int GameUI(const Uint8 *KeyValue) {
    if (Mix_PausedMusic())
        Mix_ResumeMusic();
    else
        Mix_PlayMusic(BGM[1], -1);
    Upgrade();
    while (1) {
        clock_t FStartTime = clock();
        if (!My.HP) {
            if (GameOver())
                return 0;
            else
                Restart();
        }
        if (Boss != NULL && !Boss->HP) {
            if (IsVictory())
                return 0;
            else
                Restart();
        }
        CoolDown();
        if (!CDLevel)
            CreateEnemy(Level);
        Move();
        int action = MyAction(GetKey(KeyValue));
        if (action) {
            Mix_PauseMusic();
            return action == 1 ? 1 : 0;
        }
        PrintAnime();
        while (SDL_PollEvent(&Event))
            if (Event.type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return 0;
        ControlFPS(FStartTime);
    }
}

int GameOver() {
    Mix_HaltMusic();
    Mix_PlayMusic(BGM[2], 1);
    ResetFontColor();
    PrintMyself();
    int CDDeath = 80;
    while (CDDeath) {
        SDL_Delay(mSPF);
        CDDeath--;
    }
    while (1) {
        PrintBG(4, HintLose);
        int MsgReturn = MsgAction();
        if (MsgReturn) {
            if (MsgReturn == 2)
                return 1;
            return 0;
        }
    }
}

int IsVictory() {
    Mix_HaltMusic();
    Mix_FadeInMusic(BGM[4], 1, 500);
    ResetFontColor();
    ChangeEnmStatus(&Boss);
    PrintList(&Boss, SurBoss);
    PrintMyself();
    int CDVictory = 100;
    while (CDVictory) {
        SDL_Delay(mSPF);
        CDVictory--;
    }
    while (1) {
        PrintBG(5, HintWin);
        int MsgReturn = MsgAction();
        if (MsgReturn) {
            if (MsgReturn == 2)
                return 1;
            return 0;
        }
    }
}

int MsgAction() {
    CgAngle -= CgAngle > 0 ? 4 : -356;
    ChangeFontColor();
    while (SDL_WaitEventTimeout(&Event, mSPF)) {
        if (Event.type == SDL_QUIT || Event.key.keysym.sym == SDLK_ESCAPE)
            return 2;
        if (Event.key.keysym.sym == SDLK_RETURN || Event.key.keysym.sym == SDLK_KP_ENTER) {
            Mix_HaltMusic();
            return 1;
        }
    }
    return 0;
}

void Restart() {
    RemoveNode(&ChargeEnm, 1);
    RemoveNode(&PeltEnm, 1);
    RemoveNode(&PeltBlt, 1);
    RemoveNode(&Boss, 1);
    RemoveNode(&BossBlt, 1);
    RemoveNode(&MyBlt, 1);
    RemoveNode(&Props, 1);
    Level = 0;
    for (int i = 0; i < 1000; ++i)
        CoolDown();
    Upgrade();
    My.FRect.x = (float) (Width - SizeMy) / 2;
    My.FRect.y = (float) Height / 5 * 4;
    My.FRect.w = My.FRect.h = SizeMy;
    My.FCentre.x = My.FCentre.y = (float) SizeMy / 2;
    My.FSpeed = 15;
    My.Direction = 180;
    My.HP = HPMy;
    My.Status = StatusMy;
    Mix_PlayMusic(BGM[1], -1);
}

int TheWorld() {
    Mix_PauseMusic();
    ResetFontColor();
    while (1) {
        PrintBG(3, HintPause);
        CgAngle -= CgAngle > 0 ? 4 : -356;
        ChangeFontColor();
        while (SDL_WaitEventTimeout(&Event, mSPF)) {
            if (Event.type == SDL_QUIT || Event.key.keysym.sym == SDLK_ESCAPE)
                return 1;
            if (Event.key.keysym.sym == SDLK_SPACE) {
                Mix_ResumeMusic();
                return 0;
            }
        }
    }
}

void ControlFPS(clock_t FStartTime) {
    FDurTime = clock() - FStartTime;
    if (FDurTime < mSPF)
        SDL_Delay(mSPF - FDurTime);
    FDurTime = clock() - FStartTime;
}

int GetKey(const Uint8 *KeyValue) {
    int Input = 0;
    if (KeyValue[SDL_SCANCODE_UP] || KeyValue[SDL_SCANCODE_W])
        Input |= MoveUp;
    if (KeyValue[SDL_SCANCODE_DOWN] || KeyValue[SDL_SCANCODE_S])
        Input |= MoveDown;
    if (KeyValue[SDL_SCANCODE_LEFT] || KeyValue[SDL_SCANCODE_A])
        Input |= MoveLeft;
    if (KeyValue[SDL_SCANCODE_RIGHT] || KeyValue[SDL_SCANCODE_D])
        Input |= MoveRight;
    if (KeyValue[SDL_SCANCODE_LSHIFT] || KeyValue[SDL_SCANCODE_RSHIFT])
        Input |= Shoot;
    if (KeyValue[SDL_SCANCODE_COMMA])
        Input |= RotateLeft;
    if (KeyValue[SDL_SCANCODE_PERIOD])
        Input |= RotateRight;
    if (KeyValue[SDL_SCANCODE_SLASH])
        Input |= RotateReset;
    if (KeyValue[SDL_SCANCODE_SPACE])
        Input |= Pause;
    if (KeyValue[SDL_SCANCODE_TAB])
        Input |= Tab;
    if (KeyValue[SDL_SCANCODE_BACKSPACE])
        Input |= Back2Theme;
    return Input;
}

int MyAction(int Input) {
    if (Input & MoveUp && My.FRect.y >= 0)
        My.FRect.y -= My.FSpeed;
    if (Input & MoveDown && My.FRect.y <= Height - My.FRect.h)
        My.FRect.y += My.FSpeed;
    if (Input & MoveLeft && My.FRect.x >= 0)
        My.FRect.x -= My.FSpeed;
    if (Input & MoveRight && My.FRect.x <= Width - My.FRect.w)
        My.FRect.x += My.FSpeed;
    if (Input & Shoot && CDMyBlt == 0) {
        int Twigs = ((My.Status & 0x1F00) >> 8) > 3 ? 3 : ((My.Status & 0x1F00) >> 8);
        for (int dir = -Twigs * 15; dir <= Twigs * 15; dir += 15) {
            AddNode(&MyBlt,
                    CreateNode(My, SizeMyBlt, 20, My.Direction + dir,
                               HPBlt, TypeMyBlt, (CDBloodRage ? 2 : 1),
                               (DmgMyBlt + ((My.Status & 0x1C00) >> 10) * 5) * (CDBloodRage ? 3 : 1)));
        }
        CDMyBlt = 18 - ((My.Status & 0x00F0) >> 4);
    }
    if (Input & RotateLeft && My.Direction < 270 && (My.Status & 0x0001))
        My.Direction += 10;
    if (Input & RotateRight && My.Direction > 90 && (My.Status & 0x0001))
        My.Direction -= 10;
    if (Input & RotateReset && My.Direction != 180)
        My.Direction = 180;
    if (Input & Tab && !CDDisplay) {
        CDDisplay = 20;
        CgDisplay = !CgDisplay;
    }
    if (Input & Pause && !CDPause) {
        CDPause = 20;
        if (TheWorld())
            return 2;
    }
    if (Input & Back2Theme)
        return 1;
    if (CDBloodRage)
        BloodRage();
    return 0;
}

void AddNode(OP **List, OP *NewNode) {
    if (*List == NULL) {
        *List = NewNode;
        return;
    }
    OP *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}

OP *CreateNode(OP who, float size, float fspeed, double direction, int hp, int type, int status, int damage) {
    OP *New = malloc(sizeof(OP));
    New->FRect.w = size;
    New->FRect.h = size;
    New->FCentre.x = size / 2;
    New->FCentre.y = size / 2;
    New->Type = type;
    FixXYDir(&New, who, direction);
    New->FSpeed = fspeed;
    New->HP = hp;
    New->Status = status;
    New->Damage = damage;
    New->Next = NULL;
    return New;
}

void FixXYDir(OP **New, OP who, double direction) {
    OP *Now = *New;
    if (Now->Type & (TypeMyBlt | TypeEnmBlt | TypeProps)) {
        Now->FRect.x = who.FRect.x + who.FCentre.x - Now->FCentre.x;
        Now->FRect.y = who.FRect.y + who.FCentre.y - Now->FCentre.y;
        Now->Direction = direction;
    }
    if (Now->Type & TypeStayEnm) {
        Now->FRect.x = (Width - Now->FRect.w) / 2;
        Now->FRect.y = -Now->FRect.h;
        Now->Direction = direction;
    }
    if (Now->Type & TypeChargeEnm) {
        Now->FRect.x = CgRL > 1 ? (Width - Now->FRect.w) / 2
                                : CgRL ? +(float) (rand() % ChargeRandRage)
                                       : Width - Now->FRect.w - (float) (rand() % ChargeRandRage);
        Now->FRect.y = -Now->FRect.h;
        if (Now->Type & TypeLv1Enm)
            CgRL += CgRL > 1 ? -CgRL : 1;
        else
            CgRL = !CgRL;
        double FollowDir = XY2Dir(Now->FRect.x + Now->FCentre.x,
                                  Now->FRect.y + 0,
                                  who.FRect.x + who.FCentre.x,
                                  who.FRect.y + who.FRect.h);
        Now->Direction = fabs(FollowDir) > 15 ?
                         FollowDir > 0 ? 15 : -15 : FollowDir;
    }
}

void MoveNode(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    while (Now != NULL) {
        if (Now->Type & TypeStayEnm) {
            if (Now->FRect.y > Now->FRect.h / 2) {
                Now->FRect.x += (float) sin(CgAngle * D2R) * 3;
                Now->FRect.y += Now->FSpeed * (float) cos(CgAngle * D2R) / 2;
                Now->FRect.y = Now->FRect.y > Now->FRect.h / 2 ? Now->FRect.y : Now->FRect.h / 2 + 1;
            } else
                Now->FRect.y += Now->FSpeed;
        } else if (Now->Type & TypeProps) {
            Now->FRect.x += (float) sin(CgAngle * D2R);
            Now->FRect.y += Now->FSpeed;
            OutBounds(&Now);
        } else {
            if (Now->Type & (TypeLv2Blt | TypeChargeEnm)) {
                double dDir = Now->Type & TypeLv1Enm ? 0.5 : 0.2;
                double FollowDir = XY2Dir(Now->FRect.x + Now->FCentre.x,
                                          Now->FRect.y + 0,
                                          My.FRect.x + My.FCentre.x,
                                          My.FRect.y + My.FRect.h);
                Now->Direction += FollowDir - Now->Direction > 1 ? dDir : -dDir;
            }
            Now->FRect.x += Now->FSpeed * (float) sin(Now->Direction * D2R);
            Now->FRect.y += Now->FSpeed * (float) cos(Now->Direction * D2R);
            OutBounds(&Now);
        }
        Now = Now->Next;
    }
    Now = *List;
    if (Now->Type & (~TypeMyBlt & ~TypeOut & ~TypeProps)) {
        if (!((Now->Type & TypeBoss) && CDUnbeatableBoss))
            Collide(List);
        RemoveNode(&MyBlt, 0);
    }
    if (Now->Type & TypeProps)
        for (OP *i = Now; i != NULL; i = i->Next)
            if (IsCollideMe(&i))
                BuffMe(&i);
    if (!(Now->Type & TypeBoss))
        RemoveNode(List, 0);
}

void OutBounds(OP **Now) {
    OP *Enm = *Now;
    if ((Enm->FRect.y < -Enm->FRect.h - 1)
        || (Enm->FRect.y >= Height)
        || (Enm->FRect.x < -Enm->FRect.w - 1)
        || (Enm->FRect.x >= Width + 1)) {
        Enm->HP = 0;
        if (Enm->Type & TypeChargeEnm)
            My.HP -= My.HP - Enm->Damage / 2 <= 0 ? My.HP : Enm->Damage / 2;
        Enm->Type = TypeOut;
    }
}

void Collide(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    while (Now != NULL) {
        CollideMyBlt(&Now);
        ChangeEnmStatus(&Now);
        if (!Now->HP) {
            Score += (Now->Type >> 8) * 9;
            CreateProps(&Now);
        } else if (IsCollideMe(&Now)) {
            if (!CDGoldenBody)
                My.HP -= My.HP - Now->Damage <= 0 ? My.HP : Now->Damage;
            Now->HP -= Now->HP - My.Damage <= 0 ? Now->HP : My.Damage;

        }
        Now = Now->Next;
    }
}

void CollideMyBlt(OP **Now) {
    OP *Enm = *Now;
    if (Enm->Type & TypeEnm)
        for (OP *i = MyBlt; i != NULL; i = i->Next)
            if (XY2Dis(i->FRect.x + i->FCentre.x,
                       i->FRect.y + i->FCentre.x,
                       Enm->FRect.x + Enm->FCentre.x,
                       Enm->FRect.y + Enm->FCentre.y) < i->FRect.w + Enm->FRect.w / 4) {
                if (Enm->HP)
                    i->HP = 0;
                Enm->HP -= Enm->HP - i->Damage < 0 ? Enm->HP : i->Damage;
            }
}

int IsCollideMe(OP **Now) {
    OP *Object = *Now;
    float dis = XY2Dis(My.FRect.x + My.FCentre.x,
                       My.FRect.y + My.FCentre.y,
                       Object->FRect.x + Object->FCentre.x,
                       Object->FRect.y + Object->FCentre.y);
    return dis < My.FRect.h;
}

void ChangeEnmStatus(OP **Now) {
    OP *Enm = *Now;
    if (Enm->Type & (TypeLv2Enm1)) {
        if (Enm->HP < HPLv2Enm1 / 10 * 7)
            Enm->Status = 12;
        if (Enm->HP < HPLv2Enm1 / 10 * 4)
            Enm->Status = 13;
    }
    if (Enm->Type & (TypeBoss)) {
        if (Enm->HP < HPBoss / 40 * 23 && Enm->Status == 1) {
            Enm->Status = 2;
            Mix_PlayMusic(BGM[3], 1);
        }
        if (Enm->HP < HPBoss / 2 && Enm->Status == 2) {
            Enm->Status = 3;
            CDUnbeatableBoss = 150;
            CDBossBlt = 100;
        }
        if (!Enm->HP)
            Enm->Status = 5;
    }
}

void CreateProps(OP **Now) {
    OP *Enm = *Now;
    if (rand() < RAND_MAX / 10 * 7) {
        int status = 0;
        int Ran = rand();
        switch (Enm->Type) {
            case TypeLv1Enm:
                if (Ran < RAND_MAX / 10 * 2)
                    status = Enm->Status == 1 ? 1 : 2;
                else if (Ran < RAND_MAX / 10 * 5 && !(My.Status & 0x1000))
                    status = 11;
                else if (Ran < RAND_MAX / 10 * 6 && My.FRect.w > 72)
                    status = 12;
                else if (Ran < RAND_MAX / 10 * 8 && !(My.Status & 0x0080))
                    status = 13;
                break;
            case TypeLv2Enm1:
                if (Ran < RAND_MAX / 10 * 2)
                    status = 4;
                else if (Ran < RAND_MAX / 10 * 5 && !(My.Status & 0x1000))
                    status = 11;
                else if (Ran < RAND_MAX / 10 * 6 && !(My.Status & 0x0001))
                    status = 14;
                break;
            case TypeLv2Enm2:
                if (Ran < RAND_MAX / 10 * 3)
                    status = 3;
                else if (Ran < RAND_MAX / 10 * 5 && !(My.Status & 0x1000))
                    status = 11;
                else if (Ran < RAND_MAX / 10 * 7 && My.FRect.w > 72)
                    status = 12;
                else if (Ran < RAND_MAX / 10 * 9 && !(My.Status & 0x0080))
                    status = 13;
                break;
            case TypeLv3Enm:
                if (Ran < RAND_MAX / 10 * 4)
                    status = Enm->Status == 7 ? 6 : 5;
                else if (Ran < RAND_MAX / 10 * 5)
                    status = 21;
                else if (Ran < RAND_MAX / 10 * 7)
                    status = 22;
                break;
            default:
                break;
        }
        if (status)
            AddNode(&Props,
                    CreateNode(*Enm, SizeProps, 6, 0,
                               HPProps, TypeProps, status, 0));
    }
}

void BuffMe(OP **Now) {
    OP *Buff = *Now;
    Buff->HP = 0;
    if (Buff->Status < 10) {
        My.HP += My.HP / 40 * (int) pow(Buff->Status, 1.6);
        My.HP = My.HP > HPMy ? HPMy : My.HP;
    }
    if (Buff->Status == 11)
        My.Status += 1 << 8;
    if (Buff->Status == 12) {
        My.FRect.w -= 6;
        My.FRect.h -= 6;
        My.FCentre.x -= 3;
        My.FCentre.y -= 3;
        My.Status += 1 << 1;
    }
    if (Buff->Status == 13)
        My.Status += 1 << 4;
    if (Buff->Status == 14)
        My.Status += 1;
    if (Buff->Status == 21)
        CDBloodRage += 200;
    if (Buff->Status == 22)
        CDGoldenBody += 200;
}

void RemoveNode(OP **List, int mode) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    OP *Pre = NULL;
    while (Now != NULL)
        if (!Now->HP || mode) {
            CountLevel(&Now);
            if (*List == Now) {
                *List = Now->Next;
                free(Now);
                Now = *List;
            } else {
                Pre->Next = Now->Next;
                free(Now);
                Now = Pre;
            }
        } else {
            Pre = Now;
            Now = Now->Next;
        }

}

void CountLevel(OP **Now) {
    OP *Enm = *Now;
    if (Enm->Type & TypeLv1Enm)
        Lv1Cnt--;
    if (Enm->Type & TypeLv2Enm1) {
        Lv2Cnt--;
        CDPeltBlt = 80;
    }
}

void CoolDown() {
    CDMyBlt -= CDMyBlt > 0 ? 1 : 0;
    CDChargeEnm -= CDChargeEnm > 0 ? 1 : 0;
    CDPeltEnm -= CDPeltEnm > 0 ? 1 : 0;
    CDPeltBlt -= CDPeltBlt > 0 ? 1 : 0;
    CDBossBlt -= CDBossBlt > 0 ? 1 : 0;
    CDUnbeatableBoss -= CDUnbeatableBoss > 0 ? 1 : 0;
    CDLevel -= CDLevel > 0 ? 1 : 0;
    CDBloodRage -= CDBloodRage > 0 ? 1 : 0;
    CDGoldenBody -= CDGoldenBody > 0 ? 1 : 0;
    CDDisplay -= CDDisplay > 0 ? 1 : 0;
    CDPause -= CDPause > 0 ? 1 : 0;
    CgAngle -= CgAngle > 0 ? 4 : -356;
}

double XY2Dir(float x1, float y1, float x2, float y2) {
    return atan((double) ((x2 - x1) / (y2 - y1))) * R2D;
}

float XY2Dis(float x1, float y1, float x2, float y2) {
    return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

void CreateEnemy(int level) {
    if (ChargeEnm == NULL)
        CDChargeEnm = 0;
    if (PeltEnm == NULL)
        CDPeltEnm = 0;
    switch (level) {
        case 1:
            if (!CDChargeEnm) {
                ChargeEnemyForm += ChargeEnemyForm < 3 ? 1 : -2;
                for (int num = 0; num < 3; ++num) {
                    AddNode(&ChargeEnm,
                            CreateNode(My, SizeLv1Enm, 2, 0,
                                       HPLv1Enm + (NumLv1 - Lv1Cnt) * 6, TypeLv1Enm, ChargeEnemyForm, DmgLv1Enm + (NumLv1 - Lv1Cnt) * 5));
                }
                CDChargeEnm += 1000;
            }
            break;
        case 2:
            if (!CDChargeEnm) {
                ChargeEnemyForm += ChargeEnemyForm < 5 ? 1 : -1;
                for (int num = 0; num < 2; ++num) {
                    AddNode(&ChargeEnm,
                            CreateNode(My, SizeLv2Enm, 3, 0,
                                       HPLv2Enm2 + (NumLv2 - Lv2Cnt) * 6, TypeLv2Enm2, ChargeEnemyForm, DmgLv2Enm2 + (NumLv2 - Lv2Cnt) * 5));
                }
                CDChargeEnm += 1000;
            }
            if (!CDPeltEnm) {
                CgAngle = 20;
                AddNode(&PeltEnm,
                        CreateNode(My, SizeLv2Enm, 3, 0,
                                   HPLv2Enm1 + (NumLv2 - Lv2Cnt) * 10, TypeLv2Enm1, 11, DmgLv2Enm1));
                CDPeltEnm += 3000;
            }
            if (!CDPeltBlt) {
                for (int dir = -45; dir <= 45; dir += 30)
                    AddNode(&PeltBlt,
                            CreateNode(*PeltEnm, SizeLv2Blt, 10, dir,
                                       HPBlt, TypeLv2Blt, 3, DmgLv2Blt + (NumLv2 - Lv2Cnt) * 10));
                CDPeltBlt += 60;
            }
            break;
        case 3:
            if (!CDChargeEnm) {
                ChargeEnemyForm += ChargeEnemyForm < 8 ? 1 : -2;
                for (int num = 0; num < 2; ++num) {
                    AddNode(&ChargeEnm,
                            CreateNode(My, SizeLv3Enm, 4, 0,
                                       HPLv3Enm, TypeLv3Enm, ChargeEnemyForm, DmgLv3Enm));
                }
                CDChargeEnm += 1000;
            }
            BossAction();
            break;
        default:
            break;
    }
}

void BossAction() {
    if (!CDBoss) {
        CgAngle = 20;
        AddNode(&Boss,
                CreateNode(My, SizeBoss, 2, 0,
                           HPBoss, TypeBoss, 1, DmgBoss));
        CDBoss = 1;
    }
    if (!CDBossBlt && Boss->Status <= 2) {
        for (int dir = -50; dir <= 50; dir += 25)
            AddNode(&BossBlt,
                    CreateNode(*Boss, SizeBossBlt1, 10, dir,
                               HPBlt, TypeBossBlt1, 4, DmgBossBlt1));
        CDBossBlt = 10;
        BossBltCnt++;
        if (BossBltCnt == 3) {
            BossBltCnt = 0;
            CDBossBlt = 60;
        }
    }
    if (!CDBossBlt && CDUnbeatableBoss && Boss->Status == 3) {
        AddNode(&BossBlt,
                CreateNode(*Boss, SizeBossBlt2, 30, XY2Dir
                                   (Boss->FRect.x + Boss->FCentre.x, Boss->FRect.y,
                                    My.FRect.x + My.FCentre.x, My.FRect.y + My.FRect.h),
                           HPBlt, TypeBossBlt2, 5, DmgBossBlt2));

        CDBossBlt = 80;
    }
    if (!CDUnbeatableBoss && Boss->Status == 3)
        Boss->Status = 4;
    if (!CDBossBlt && Boss->Status == 4) {
        for (int dir = CgPN * BossBltCnt * 8; dir <= 360 + CgPN * BossBltCnt * 8; dir += 30)
            AddNode(&BossBlt,
                    CreateNode(*Boss, SizeBossBlt1, 10, dir,
                               HPBlt, TypeBossBlt3, 6 + BossBltCnt % 4, DmgBossBlt1));
        CDBossBlt = 15;
        BossBltCnt++;
        if (BossBltCnt == 12) {
            BossBltCnt = 0;
            CgPN *= -1;
            CDBossBlt = 80;
        }
    }
}

void Move() {
    MoveNode(&MyBlt);
    MoveNode(&PeltBlt);
    MoveNode(&BossBlt);
    MoveNode(&ChargeEnm);
    MoveNode(&PeltEnm);
    MoveNode(&Boss);
    MoveNode(&Props);

    Upgrade();
}

void Upgrade() {
    if (!Level) {
        Level = 1;
        Score = 0;
        CgRL = 0;
        CgPN = 0;
        CgDisplay = 0;
        Lv1Cnt = NumLv1;
        Lv2Cnt = NumLv2;
        CDBoss = 0;
        BossBltCnt = 0;
        CDLevel = 250;
        ResetFontColor();
        ChargeEnemyForm = 0;
        ChargeRandRage = 300;
    }
    if (!Lv1Cnt && Level == 1) {
        Level = 2;
        CDLevel = 100;
        CDPeltBlt = 230;
        ResetFontColor();
        ChargeEnemyForm = 3;
        ChargeRandRage = 200;
    }
    if (!Lv2Cnt && Level == 2) {
        Level = 3;
        CDLevel = 100;
        CDBossBlt = 250;
        CgPN = 1;
        ResetFontColor();
        ChargeEnemyForm = 5;
        ChargeRandRage = 100;
    }
}

void BloodRage() {
    if (My.HP > 200)
        My.HP -= 2;
    else
        My.HP -= My.HP * 10 / 1000;
}

void ChangeFontColor() {
    FontCgColor.a += (int) (9 * sin(CgAngle * D2R));
}

void ResetFontColor() {
    CgAngle = 360;
    FontCgColor.a = 0xFF;
}

void PrintAnime() {
    PrintGameBG();
    if (CgDisplay) {
        PrintFPS();
        PrintHP();
        PrintInfo();
        PrintStatus();
    }
    PrintList(&Props, SurProps);
    PrintList(&ChargeEnm, SurEnemy);
    PrintList(&MyBlt, SurBullet);
    PrintList(&PeltBlt, SurBullet);
    PrintList(&BossBlt, SurBullet);
    PrintList(&PeltEnm, SurEnemy);
    PrintList(&Boss, SurBoss);
    PrintMyself();

    if (CDLevel) {
        PrintLevel();
        ChangeFontColor();
    }
    SDL_RenderPresent(Renderer);
}

void PrintBG(int mode, char *HintName) {
    SDL_Texture *TexThemeBG = SDL_CreateTextureFromSurface(Renderer, SurBG[mode]);
    SDL_RenderCopy(Renderer, TexThemeBG, NULL, &RectBG);
    SDL_DestroyTexture(TexThemeBG);
    PrintHints(HintName, mode > 3 ? 2 : 4 + mode);
    SDL_RenderPresent(Renderer);
}

void PrintGameBG() {
    SDL_Texture *TexGameBG = SDL_CreateTextureFromSurface(Renderer, SurBG[2]);
    SDL_RenderCopy(Renderer, TexGameBG, NULL, &RectGameBG);
    SDL_DestroyTexture(TexGameBG);
    RectGameBG.y += RectGameBG.y >= 0 ? -Height + ScrollSpeed : ScrollSpeed;
}

void PrintFPS() {
    sprintf_s(FPS, 15, "FPS:%.lf", 1000 / (double) FDurTime);
    SDL_Surface *SurFPS = TTF_RenderUTF8_Blended(SmallFont, FPS, FontColor);
    SDL_Texture *TexFPS = SDL_CreateTextureFromSurface(Renderer, SurFPS);
    SDL_Rect RectFPS = {0, 0, SurFPS->w, SurFPS->h};
    SDL_RenderCopy(Renderer, TexFPS, NULL, &RectFPS);
    SDL_FreeSurface(SurFPS);
    SDL_DestroyTexture(TexFPS);
}

void PrintList(OP **List, SDL_Surface *SurList[]) {
    for (OP *i = *List; i != NULL; i = i->Next) {
        SDL_Texture *TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Status]);
        SDL_RenderCopyExF(Renderer, TexList, NULL, &i->FRect, -i->Direction, &i->FCentre, 0);
        SDL_DestroyTexture(TexList);
    }
}

void PrintMyself() {
    SDL_Texture *TexMy = SDL_CreateTextureFromSurface(Renderer, SurMy[(Boss != NULL && !Boss->HP) ?
                                                                      7 : My.HP <= 0 ?
                                                                          6 : CDBloodRage ?
                                                                              4 : My.HP > HPMy * 0.6 ?
                                                                                  1 : My.HP > HPMy * 0.3 ?
                                                                                      2 : 3]);
    SDL_RenderCopyExF(Renderer, TexMy, NULL, &My.FRect, 180 - My.Direction, &My.FCentre, 0);
    SDL_DestroyTexture(TexMy);
    if (CDGoldenBody) {
        SDL_Texture *TexGoldenBody = SDL_CreateTextureFromSurface(Renderer, SurMy[5]);
        SDL_RenderCopyExF(Renderer, TexGoldenBody, NULL, &My.FRect, 180 - My.Direction, &My.FCentre, 0);
        SDL_DestroyTexture(TexGoldenBody);
    }
}

void PrintHP() {
    SDL_Texture *TexHPSide = SDL_CreateTextureFromSurface(Renderer, SurHP[1]);
    SDL_Rect RectHPSide = {10, Height - SurHP[1]->h - 10, SurHP[1]->w, SurHP[1]->h};
    SDL_Texture *TexHP = SDL_CreateTextureFromSurface(Renderer, SurHP[My.HP > HPMy * 0.6 ? 2 : My.HP > HPMy * 0.3 ? 3 : 4]);
    SDL_Rect RectHP = {10, Height - SurHP[2]->h - 10, SurHP[2]->w * My.HP / HPMy, SurHP[2]->h};
    SDL_RenderCopy(Renderer, TexHP, NULL, &RectHP);
    SDL_RenderCopy(Renderer, TexHPSide, NULL, &RectHPSide);
    SDL_DestroyTexture(TexHPSide);
    SDL_DestroyTexture(TexHP);
}

void PrintInfo() {
    char SubLvScore[30];
    sprintf_s(SubLvScore, 30, "Level:%d Score:%d", Level, Score);
    SDL_Surface *SurScore = TTF_RenderUTF8_Blended(SmallFont, SubLvScore, FontColor);
    SDL_Texture *TexScore = SDL_CreateTextureFromSurface(Renderer, SurScore);
    SDL_Rect RectScore = {10, Height - 15 - 27 - SurScore->h, SurScore->w, SurScore->h};
    SDL_RenderCopy(Renderer, TexScore, NULL, &RectScore);
    SDL_FreeSurface(SurScore);
    SDL_DestroyTexture(TexScore);
}

void PrintStatus() {
    if (My.Status & 0x1F00)
        PrintBuff(11, (My.Status & 0x1F00) >> 8, 60);
    if (My.Status & 0x000E)
        PrintBuff(12, (My.Status & 0x000E) >> 1, 120);
    if (My.Status & 0x00F0)
        PrintBuff(13, (My.Status & 0x00F0) >> 4, 180);
    if (My.Status & 0x0001)
        PrintBuff(14, 1, 240);
    if (CDBloodRage)
        PrintBuff(21, CDBloodRage * 10 / 200, 300);
    if (CDGoldenBody)
        PrintBuff(22, CDGoldenBody * 10 / 200, 360);
}

void PrintBuff(int buff, int num, int position) {
    SDL_Texture *TexBuff = SDL_CreateTextureFromSurface(Renderer, SurProps[buff]);
    SDL_Rect RectBuff = {10, Height - 100 - position, SizeProps / 5 * 2, SizeProps / 5 * 2};
    SDL_RenderCopy(Renderer, TexBuff, NULL, &RectBuff);
    SDL_DestroyTexture(TexBuff);
    if (num > 0) {
        char Num[10];
        sprintf_s(Num, 10, "%d", num);
        SDL_Surface *SurNum = TTF_RenderUTF8_Blended(SmallFont, Num, FontColor);
        SDL_Texture *TexNum = SDL_CreateTextureFromSurface(Renderer, SurNum);
        SDL_Rect RectNum = {10 + SizeProps / 5 * 2, Height - 90 - position, SurNum->w, SurNum->h};
        SDL_RenderCopy(Renderer, TexNum, NULL, &RectNum);
        SDL_FreeSurface(SurNum);
        SDL_DestroyTexture(TexNum);
    }
}

void PrintLevel() {
    char TitleLevel[20];
    if (CDLevel > 100) {
        sprintf_s(TitleLevel, 10, "Start");
        PrintHints(Hint1Game, 1);
        PrintHints(Hint2Game, 2);
    } else
        sprintf_s(TitleLevel, 10, "Level %d", Level);
    SDL_Surface *SurTitleLevel = TTF_RenderUTF8_Blended(LargeFont, TitleLevel, FontCgColor);
    SDL_Texture *TexTitleLevel = SDL_CreateTextureFromSurface(Renderer, SurTitleLevel);
    SDL_Rect RectTitleLevel = {(Width - SurTitleLevel->w) / 2, (Height - SurTitleLevel->h) / 2, SurTitleLevel->w, SurTitleLevel->h};
    SDL_RenderCopy(Renderer, TexTitleLevel, NULL, &RectTitleLevel);
    SDL_FreeSurface(SurTitleLevel);
    SDL_DestroyTexture(TexTitleLevel);
}

void PrintHints(char HintGame[], int mode) {
    SDL_Surface *SurHint = TTF_RenderUTF8_Blended(MiddleFont, HintGame, FontCgColor);
    SDL_Texture *TexHint = SDL_CreateTextureFromSurface(Renderer, SurHint);
    SDL_Rect RectHint = {(Width - SurHint->w) / 2, Height / 2 + (SurHint->h + 5) * mode, SurHint->w, SurHint->h};
    SDL_RenderCopy(Renderer, TexHint, NULL, &RectHint);
    SDL_FreeSurface(SurHint);
    SDL_DestroyTexture(TexHint);
}

void LoadRes() {
    char FileName[40];
    Mix_VolumeMusic(BGMVolume);
    Mix_Volume(-1, SoundVolume);
    LargeFont = TTF_OpenFont("res/font/GenshinDefault.ttf", 72);
    MiddleFont = TTF_OpenFont("res/font/GenshinDefault.ttf", 40);
    SmallFont = TTF_OpenFont("res/font/GenshinDefault.ttf", 28);
    for (int i = 1; i <= 25; ++i) {
        sprintf_s(FileName, 30, "res/audio/BGM%d.mp3", i);
        BGM[i] = Mix_LoadMUS(FileName);
        sprintf_s(FileName, 30, "res/image/BG%d.png", i);
        SurBG[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "res/image/I%d.png", i);
        SurMy[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "res/image/Enemy%d.png", i);
        SurEnemy[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "res/image/Boss%d.png", i);
        SurBoss[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "res/image/Bullet%d.png", i);
        SurBullet[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "res/image/HP%d.png", i);
        SurHP[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "res/image/Props%d.png", i);
        SurProps[i] = IMG_Load(FileName);
    }
}