﻿#include"Sakiyary_EmojiBattle.h"

int main() {
    InitAll();
    LoadRes();
    PrintThemeBG();
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    ThemeUI(&Event, KeyValue);
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

void ThemeUI(SDL_Event *event, const Uint8 *KeyValue) {
    while (1) {
        PrintThemeBG();
        while (SDL_WaitEventTimeout(event, mSPF)) {
            if (event->type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return;
            if (KeyValue[SDL_SCANCODE_RETURN] || KeyValue[SDL_SCANCODE_KP_ENTER]) {
                printf("Game Start!\n");
                if (!GameUI(event, KeyValue))
                    return;
                PrintThemeBG();
            }
        }
    }
}

int GameUI(SDL_Event *event, const Uint8 *KeyValue) {
    if (Mix_PausedMusic())
        Mix_ResumeMusic();
    else
        Mix_PlayMusic(BGM, -1);
    Upgrade();
    while (1) {
        clock_t FStartTime = clock();
        CoolDown();
        if (!CDLevel)
            CreateEnemy(Level);
        Move();
        PrintAnime();
        if (MyAction(GetKey(KeyValue))) {
            Mix_PauseMusic();
            return 1;
        }
        ControlFPS(FStartTime);
        while (SDL_PollEvent(event))
            if (event->type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return 0;
    }
}

void ControlFPS(clock_t FStartTime) {
    clock_t FDurTime = clock() - FStartTime;
    if (FDurTime < mSPF)
        SDL_Delay(mSPF - FDurTime);
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
        for (int dir = -30; dir <= 30; dir += 15) {
            AddNode(&MyBlt,
                    CreateNode(My, SizeMyBlt, 20, My.Direction + dir,
                               HPBlt, TypeMyBlt, 1, DmgMyBlt));
        }
        CDMyBlt = 15;
    }
    if (Input & RotateLeft && My.Direction < 270)
        My.Direction += 10;
    if (Input & RotateRight && My.Direction > 90)
        My.Direction -= 10;
    if (Input & RotateReset && My.Direction != 180)
        My.Direction = 180;
    if (Input & Back2Theme)
        return 1;
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
    if (Now->Type & (TypeMyBlt | TypeEnmBlt)) {
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
        Now->FRect.x = ChangeRL > 1 ? (Width - Now->FRect.w) / 2
                                    : ChangeRL ? +(float) (rand() % ChargeRandRage)
                                               : Width - Now->FRect.w - (float) (rand() % ChargeRandRage);
        Now->FRect.y = -Now->FRect.h;
        if (Now->Type & TypeLv1Enm)
            ChangeRL += ChangeRL > 1 ? -ChangeRL : 1;
        else
            ChangeRL = !ChangeRL;
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
    if (Now->Type != TypeMyBlt) {
        if (!((Now->Type & TypeBoss) && CDUnbeatableBoss))
            Collide(List);
        RemoveNode(&MyBlt);
    }
    RemoveNode(List);
}

void OutBounds(OP **Now) {
    OP *Enm = *Now;
    if ((Enm->FRect.y < -Enm->FRect.h - 1)
        || (Enm->FRect.y >= Height)
        || (Enm->FRect.x < -Enm->FRect.w - 1)
        || (Enm->FRect.x >= Width + 1)) {
        Enm->HP = 0;
        if (Enm->Type & TypeChargeEnm)
            My.HP -= Enm->Damage / 2;
    }
}

void Collide(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    while (Now != NULL) {
        if (Now->Type & TypeEnm)
            for (OP *i = MyBlt; i != NULL; i = i->Next)
                if (XY2Dis(i->FRect.x + i->FCentre.x,
                           i->FRect.y + i->FCentre.x,
                           Now->FRect.x + Now->FCentre.x,
                           Now->FRect.y + Now->FCentre.y) < i->FRect.w + Now->FRect.w / 4) {
                    if (Now->HP)
                        i->HP = 0;
                    Now->HP -= Now->HP - i->Damage < 0 ? Now->HP : i->Damage;
                }
        if (Now->Type & (TypeLv2Enm1)) {
            if (Now->HP < HPLv2Enm1 / 10 * 7)
                Now->Status = 12;
            if (Now->HP < HPLv2Enm1 / 10 * 4)
                Now->Status = 13;
        }
        if (Now->Type & (TypeBoss)) {
            if (Now->HP < HPBoss / 5 * 3 && Now->Status == 1)
                Now->Status = 2;
            if (Now->HP < HPBoss / 2 && Now->Status == 2) {
                Now->Status = 3;
                CDUnbeatableBoss = 100;
                CDBossBlt = 70;
            }
        }
        if (!Now->HP) {
            //Props
        } else if (XY2Dis(My.FRect.x + My.FCentre.x,
                          My.FRect.y + My.FCentre.y,
                          Now->FRect.x + Now->FCentre.x,
                          Now->FRect.y + Now->FCentre.y) < My.FRect.w / 4 * 3) {
            My.HP -= Now->Damage;
            Now->HP += Now->HP - My.Damage <= 0 ? -Now->HP : -My.Damage;
        }
        Now = Now->Next;
    }
}

void RemoveNode(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    OP *Pre = NULL;
    while (Now != NULL)
        if (!Now->HP || !Lv3Cnt) {
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
    if (Enm->Type & TypeBoss) {
        Lv3Cnt--;
        RemoveNode(&BossBlt);
        RemoveNode(&ChargeEnm);
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
                                       HPLv1Enm, TypeLv1Enm, ChargeEnemyForm, DmgLv1Enm));
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
                                       HPLv2Enm2, TypeLv2Enm2, ChargeEnemyForm, DmgLv2Enm2));
                }
                CDChargeEnm += 1000;
            }
            if (!CDPeltEnm) {
                CgAngle = 20;
                AddNode(&PeltEnm,
                        CreateNode(My, SizeLv2Enm, 3, 0,
                                   HPLv2Enm1, TypeLv2Enm1, 11, DmgLv2Enm1));
                CDPeltEnm += 3000;
            }
            if (!CDPeltBlt) {
                for (int dir = -45; dir <= 45; dir += 30)
                    AddNode(&PeltBlt,
                            CreateNode(*PeltEnm, SizeLv2Blt, 10, dir,
                                       HPBlt, TypeLv2Blt, 3, DmgLv2Blt));
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

        CDBossBlt = 50;
    }
    if (!CDUnbeatableBoss && Boss->Status == 3)
        Boss->Status = 4;
    if (!CDBossBlt && Boss->Status == 4) {
        for (int dir = BossBltCnt * 3; dir <= 360 + BossBltCnt * 3; dir += 30)
            AddNode(&BossBlt,
                    CreateNode(*Boss, SizeBossBlt1, 10, dir,
                               HPBlt, TypeBossBlt3, 6 + BossBltCnt % 4, DmgBossBlt1));
        CDBossBlt = 12;
        BossBltCnt++;
        if (BossBltCnt == 12) {
            BossBltCnt = 0;
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

    Upgrade();
}

void Upgrade() {
    if (!Level) {
        Level = 1;
        CDLevel = 300;
        Lv1Cnt = 9;
        Lv2Cnt = 3;
        Lv3Cnt = 1;
        CDBoss = 0;
        CDLevel = 150;
        ChargeEnemyForm = 0;
        ChargeRandRage = 300;
    }
    if (!Lv1Cnt && Level == 1) {
        Level = 2;
        CDLevel = 150;
        CDPeltBlt = 230;
        ChargeEnemyForm = 3;
        ChargeRandRage = 200;
    }
    if (!Lv2Cnt && Level == 2) {
        Level = 3;
        CDLevel = 150;
        CDBossBlt = 250;
        ChargeEnemyForm = 5;
        ChargeRandRage = 100;
    }
    if (!Lv3Cnt && Level == 3) {
        CDLevel = 20000;
    }
}

void PrintAnime() {
    PrintGameBG();
    PrintList(&ChargeEnm, SurEnemy);
    PrintList(&MyBlt, SurBullet);
    PrintList(&PeltBlt, SurBullet);
    PrintList(&BossBlt, SurBullet);
    PrintList(&PeltEnm, SurEnemy);
    PrintList(&Boss, SurBoss);
    PrintMyself();

    if (CDLevel)
        PrintLevel();
    SDL_RenderPresent(Renderer);
}

void PrintThemeBG() {
    SDL_Texture *TexThemeBG = SDL_CreateTextureFromSurface(Renderer, SurThemeBG);
    SDL_RenderCopy(Renderer, TexThemeBG, NULL, &RectThemeBG);
    SDL_DestroyTexture(TexThemeBG);
    SDL_Surface *SurHintTheme = TTF_RenderUTF8_Blended(MiddleFont, HintTheme, FontCgColor);
    SDL_Texture *TexHintTheme = SDL_CreateTextureFromSurface(Renderer, SurHintTheme);
    SDL_Rect RectHintTheme = {(Width - SurHintTheme->w) / 2, 750, SurHintTheme->w, SurHintTheme->h};
    SDL_RenderCopy(Renderer, TexHintTheme, NULL, &RectHintTheme);
    SDL_FreeSurface(SurHintTheme);
    SDL_DestroyTexture(TexHintTheme);
    SDL_RenderPresent(Renderer);
    CgAngle -= CgAngle > 0 ? 4 : -356;
    FontCgColor.a += (int) (9 * sin(CgAngle * D2R));
}

void PrintGameBG() {
    SDL_Texture *TexGameBG = SDL_CreateTextureFromSurface(Renderer, SurGameBG);
    SDL_RenderCopy(Renderer, TexGameBG, NULL, &RectGameBG);
    SDL_DestroyTexture(TexGameBG);
    RectGameBG.y += RectGameBG.y >= 0 ? -Height + ScrollSpeed : ScrollSpeed;
}

void PrintList(OP **List, SDL_Surface *SurList[]) {
    for (OP *i = *List; i != NULL; i = i->Next) {
        SDL_Texture *TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Status]);
        SDL_RenderCopyExF(Renderer, TexList, NULL, &i->FRect, -i->Direction, &i->FCentre, 0);
        SDL_DestroyTexture(TexList);
    }
}

void PrintMyself() {
    SDL_Texture *TexMy = SDL_CreateTextureFromSurface(Renderer, SurMy[My.HP > HPMy * 0.6 ? 1 : My.HP > HPMy * 0.3 ? 2 : 3]);
    SDL_RenderCopyExF(Renderer, TexMy, NULL, &My.FRect, 180 - My.Direction, &My.FCentre, 0);
    SDL_DestroyTexture(TexMy);
}

void PrintLevel() {
    char TitleLevel[20];
    if (CDLevel > 300)
        sprintf_s(TitleLevel, 10, "YouWin!");
    else if (CDLevel > 150)
        sprintf_s(TitleLevel, 10, "Start");
    else
        sprintf_s(TitleLevel, 10, "Level%d", Level);
    SDL_Surface *SurTitleLevel = TTF_RenderUTF8_Blended(LargeFont, TitleLevel, FontCgColor);
    SDL_Texture *TexTitleLevel = SDL_CreateTextureFromSurface(Renderer, SurTitleLevel);
    SDL_Rect RectTitleLevel = {(Width - SurTitleLevel->w) / 2, (Height - SurTitleLevel->h) / 2, SurTitleLevel->w, SurTitleLevel->h};
    SDL_RenderCopy(Renderer, TexTitleLevel, NULL, &RectTitleLevel);
    SDL_FreeSurface(SurTitleLevel);
    SDL_DestroyTexture(TexTitleLevel);
    FontCgColor.a += (int) (8 * sin(CgAngle * D2R));
}

void LoadRes() {
    char FileName[40];
    BGM = Mix_LoadMUS("res/Enemy.mp3");
    Mix_VolumeMusic(BGMVolume);
    Mix_Volume(-1, SoundVolume);

    LargeFont = TTF_OpenFont("res/GenshinDefault.ttf", 64);
    MiddleFont = TTF_OpenFont("res/GenshinDefault.ttf", 40);
    SmallFont = TTF_OpenFont("res/GenshinDefault.ttf", 28);

    SurGameBG = IMG_Load("res/GameBG.png");
    SurThemeBG = IMG_Load("res/ThemeBG.png");
    for (int i = 1; i <= 5; ++i) {
        sprintf_s(FileName, 30, "res/I%d.png", i);
        SurMy[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 15; ++i) {
        sprintf_s(FileName, 30, "res/Enemy%d.png", i);
        SurEnemy[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 5; ++i) {
        sprintf_s(FileName, 30, "res/Boss%d.png", i);
        SurBoss[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 10; ++i) {
        sprintf_s(FileName, 30, "res/Bullet%d.png", i);
        SurBullet[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 30; ++i) {
        sprintf_s(FileName, 30, "res/Props%d.png", i);
        SurProps[i] = IMG_Load(FileName);
    }
}