#include"Sakiyary_EmojiBattle.h"

int main() {
    InitAll();
    LoadRes();
    PrintThemeBG();
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    ThemeUI(&TotleEvent, KeyValue);
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
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
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
    Mix_PlayMusic(BGM, -1);
    Upgrade();
    while (1) {
        CoolDown();
        if (!LevelCD)
            CreateEnemy(Level);
        Move();
        PrintAnime();
        if (MyAction(GetKey(KeyValue))) {
            Mix_PauseMusic();
            return 1;
        }
        SDL_Delay(mSPF);
        while (SDL_PollEvent(event))
            if (event->type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return 0;
    }
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
    if (Input & Shoot && MyBltCD == 0) {
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction - 10, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction + 10, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction - 20, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction + 20, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction - 30, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction + 30, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction - 40, 1, MyBltType, 1, MyBltDmg));
        ListAdd(&MyBlt, CreateObject(My, MyBltSize, 20, My.Direction + 40, 1, MyBltType, 1, MyBltDmg));
        MyBltCD = 5;
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

void ListAdd(OP **List, OP *NewNode) {
    if (*List == NULL) {
        *List = NewNode;
        return;
    }
    OP *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}

OP *CreateObject(OP who, float size, float fspeed, double direction, int hp, int type, int status, int damage) {
    OP *New = malloc(sizeof(OP));
    New->FRect.w = size;
    New->FRect.h = size;
    New->FCentre.x = size / 2;
    New->FCentre.y = size / 2;
    if (type & (MyBltType | Lv2BltType | Lv3Blt1Type | Lv3Blt2Type)) {
        New->FRect.x = who.FRect.x + who.FCentre.x - size / 2;
        New->FRect.y = who.FRect.y + who.FCentre.y - size / 2;
        New->Direction = direction;
    }
    if (type & (Lv2Enm1Type)) {
        New->FRect.x = (Width - size) / 2;
        New->FRect.y = -size;
        New->Direction = direction;
    }
    if (type & (Lv1EnmType | Lv2Enm2Type | Lv3EnmType)) {
        New->FRect.x = RorL > 1 ? (Width - size) / 2
                                : RorL ? +(float) (rand() % ChargeRandRage)
                                       : Width - size - (float) (rand() % ChargeRandRage);
        New->FRect.y = -size;
        if (type & Lv1EnmType)
            RorL += RorL > 1 ? -RorL : 1;
        else
            RorL = !RorL;
        double FollowDir = XY2Dir(New->FRect.x + New->FCentre.x,
                                  New->FRect.y + 0,
                                  who.FRect.x + who.FCentre.x,
                                  who.FRect.y + who.FRect.h);
        New->Direction = fabs(FollowDir) > 15 ?
                         FollowDir > 0 ? 15 : -15 : FollowDir;
    }
    New->FSpeed = fspeed;
    New->HP = hp;
    New->Type = type;
    New->Status = status;
    New->Damage = damage;
    New->Next = NULL;
    return New;
}

void ListMove(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    while (Now != NULL) {
        if (Now->Type & (Lv2Enm1Type | BossType)) {
            if (Now->FRect.y > Now->FRect.h / 2) {
                Now->FRect.x += Now->FSpeed * (float) sin(TwinkTime * D2R) * 3 / 2;
                Now->FRect.y += Now->FSpeed * (float) cos(TwinkTime * D2R) / 2;
                Now->FRect.y = Now->FRect.y > Now->FRect.h / 2 ? Now->FRect.y : Now->FRect.h / 2 + 1;
            } else
                Now->FRect.y += Now->FSpeed;
        } else {
            if (Now->Type & (Lv2BltType | Lv1EnmType | Lv2Enm2Type | Lv3EnmType)) {
                double dDir = Now->Type & Lv1EnmType ? 0.5 : 0.2;
                double FollowDir = XY2Dir(Now->FRect.x + Now->FCentre.x,
                                          Now->FRect.y + 0,
                                          My.FRect.x + My.FCentre.x,
                                          My.FRect.y + My.FRect.h);
                Now->Direction += FollowDir - Now->Direction > 1 ? dDir : -dDir;
            }
            Now->FRect.x += Now->FSpeed * (float) sin(Now->Direction * D2R);
            Now->FRect.y += Now->FSpeed * (float) cos(Now->Direction * D2R);
            if ((Now->FRect.y < -Now->FRect.h - 10)
                || (Now->FRect.y >= Height)
                || (Now->FRect.x < -Now->FRect.w - 10)
                || (Now->FRect.x >= Width + 10)) {
                Now->HP = 0;
                if (Now->Type & (Lv1EnmType | Lv2Enm2Type | Lv3EnmType))
                    My.HP -= Now->Damage / 2;
            }
        }
        Now = Now->Next;
    }
    Now = *List;
    if (Now->Type != MyBltType) {
        Collision(List);
        ListRemove(&MyBlt);
    }
    ListRemove(List);
}

void Collision(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    while (Now != NULL) {
        if (Now->Type & (Lv2Enm1Type | BossType | Lv1EnmType | Lv2Enm2Type | Lv3EnmType))
            for (OP *i = MyBlt; i != NULL; i = i->Next)
                if (XY2Dis(i->FRect.x + i->FCentre.x,
                           i->FRect.y + i->FCentre.x,
                           Now->FRect.x + Now->FCentre.x,
                           Now->FRect.y + Now->FCentre.y) < i->FRect.w + Now->FRect.w / 4) {
                    if (Now->HP)
                        i->HP = 0;
                    Now->HP -= Now->HP - i->Damage < 0 ? Now->HP : i->Damage;
                }
        if (Now->Type & (Lv2Enm1Type)) {
            if (Now->HP < Lv2Enm1HP / 10 * 7)Now->Status = 12;
            if (Now->HP < Lv2Enm1HP / 10 * 4)Now->Status = 13;
        }
        if (!Now->HP) {
            //Props
        } else if (XY2Dis(My.FRect.x + My.FCentre.x,
                          My.FRect.y + My.FCentre.y,
                          Now->FRect.x + Now->FCentre.x,
                          Now->FRect.y + Now->FCentre.y) < My.FRect.w / 4 * 3) {
            My.HP -= Now->Damage;
            Now->HP -= My.Damage;
        }
        Now = Now->Next;
    }
}

void ListRemove(OP **List) {
    if (*List == NULL)
        return;
    OP *Now = *List;
    OP *Pre = NULL;
    while (Now != NULL)
        if (!Now->HP) {
            if (Now->Type & Lv1EnmType)
                Level1Cnt--;
            if (Now->Type & Lv2Enm1Type) {
                Level2Cnt--;
                PeltBltCD = 100;
            }
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

void CoolDown() {
    MyBltCD -= MyBltCD > 0 ? 1 : 0;
    ChargeCD -= ChargeCD > 0 ? 1 : 0;
    PeltEnemyCD -= PeltEnemyCD > 0 ? 1 : 0;
    PeltBltCD -= PeltBltCD > 0 ? 1 : 0;
    LevelCD -= LevelCD > 0 ? 1 : 0;
    TwinkTime -= TwinkTime > 0 ? 4 : -356;
}

double XY2Dir(float x1, float y1, float x2, float y2) {
    return atan((double) ((x2 - x1) / (y2 - y1))) * R2D;
}

float XY2Dis(float x1, float y1, float x2, float y2) {
    return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

void CreateEnemy(int level) {
    if (ChargeEnemy == NULL)
        ChargeCD = 0;
    if (PeltEnemy == NULL)
        PeltEnemyCD = 0;
    switch (level) {
        case 1:
            if (!ChargeCD) {
                ChargeEnemyForm += ChargeEnemyForm < 3 ? 1 : -2;
                ListAdd(&ChargeEnemy, CreateObject(My, Lv1EnmSize, 4, 0, Lv1EnmHP, Lv1EnmType, ChargeEnemyForm, Lv1EnmDmg));
                ListAdd(&ChargeEnemy, CreateObject(My, Lv1EnmSize, 4, 0, Lv1EnmHP, Lv1EnmType, ChargeEnemyForm, Lv1EnmDmg));
                ListAdd(&ChargeEnemy, CreateObject(My, Lv1EnmSize, 4, 0, Lv1EnmHP, Lv1EnmType, ChargeEnemyForm, Lv1EnmDmg));
                ChargeCD += 1000;
            }
            break;
        case 2:
            if (!ChargeCD) {
                ChargeEnemyForm += ChargeEnemyForm < 5 ? 1 : -1;
                ListAdd(&ChargeEnemy, CreateObject(My, Lv2EnmSize, 5, 0, Lv2Enm2HP, Lv2Enm2Type, ChargeEnemyForm, Lv2Enm2Dmg));
                ListAdd(&ChargeEnemy, CreateObject(My, Lv2EnmSize, 5, 0, Lv2Enm2HP, Lv2Enm2Type, ChargeEnemyForm, Lv2Enm2Dmg));
                ChargeCD += 1000;
            }
            if (!PeltEnemyCD) {
                ListAdd(&PeltEnemy, CreateObject(My, Lv2EnmSize, 5, 0, Lv2Enm1HP, Lv2Enm1Type, 11, Lv2Enm1Dmg));
                PeltEnemyCD += 3000;
            }
            if (!PeltBltCD)
                for (OP *i = PeltEnemy; i != NULL; i = i->Next) {
                    ListAdd(&PeltBlt, CreateObject(*i, Lv2BltSize, 10, -10, 1, Lv2BltType, 3, Lv2BltDmg));
                    ListAdd(&PeltBlt, CreateObject(*i, Lv2BltSize, 10, 10, 1, Lv2BltType, 3, Lv2BltDmg));
                    PeltBltCD += 80;
                }

            break;
        case 3:
            if (!ChargeCD) {
                ChargeEnemyForm += ChargeEnemyForm < 8 ? 1 : -2;
                ListAdd(&ChargeEnemy, CreateObject(My, Lv3EnmSize, 6, 0, Lv3EnmHP, Lv3EnmType, ChargeEnemyForm, Lv3EnmDmg));
                ListAdd(&ChargeEnemy, CreateObject(My, Lv3EnmSize, 6, 0, Lv3EnmHP, Lv3EnmType, ChargeEnemyForm, Lv3EnmDmg));
                ChargeCD += 1000;
            }
            break;

        default:
            break;
    }
}

void Move() {
    ListMove(&MyBlt);
    ListMove(&PeltBlt);
    ListMove(&ChargeEnemy);
    ListMove(&PeltEnemy);
    Upgrade();


}

void Upgrade() {
    if (!Level) {
        Level = 1;
        LevelCD = 300;
        Level1Cnt = 36;
        Level2Cnt = 6;
        LevelCD = 150;
        ChargeEnemyForm = 3;
        ChargeRandRage = 300;
    } else if (!Level1Cnt && Level == 1) {
        Level = 2;
        LevelCD = 150;
        ChargeEnemyForm = 3;
        ChargeRandRage = 200;
        PeltBltCD = 100;
    } else if (!Level2Cnt && Level == 2) {
        Level = 3;
        LevelCD = 150;
        ChargeEnemyForm = 5;
        ChargeRandRage = 100;
    }
}

void PrintAnime() {
//    SDL_RenderClear(Renderer);
    PrintGameBG();
    PrintList(&ChargeEnemy, EnemySurface);
    PrintList(&PeltEnemy, EnemySurface);
    PrintList(&MyBlt, BulletSurface);
    PrintList(&PeltBlt, BulletSurface);
    PrintMyself();

    if (LevelCD)
        PrintLevel();
    SDL_RenderPresent(Renderer);
}

void PrintThemeBG() {
    SDL_Texture *ThemeBGTexture = SDL_CreateTextureFromSurface(Renderer, ThemeBGSurface);
    SDL_RenderCopy(Renderer, ThemeBGTexture, NULL, &ThemeBGRect);
    SDL_DestroyTexture(ThemeBGTexture);
    SDL_Surface *ThemeTipsSurface = TTF_RenderUTF8_Blended(MiddleFont, ThemeTips, FontTwinkColor);
    SDL_Texture *ThemeTipsTexture = SDL_CreateTextureFromSurface(Renderer, ThemeTipsSurface);
    SDL_Rect ThemeTipsRect = {(Width - ThemeTipsSurface->w) / 2, 750, ThemeTipsSurface->w, ThemeTipsSurface->h};
    SDL_RenderCopy(Renderer, ThemeTipsTexture, NULL, &ThemeTipsRect);
    SDL_FreeSurface(ThemeTipsSurface);
    SDL_DestroyTexture(ThemeTipsTexture);
    SDL_RenderPresent(Renderer);
    TwinkTime -= TwinkTime > 0 ? 4 : -356;
    FontTwinkColor.a += (int) (9 * sin(TwinkTime * D2R));
}

void PrintGameBG() {
    SDL_Texture *GameBGTexture = SDL_CreateTextureFromSurface(Renderer, GameBGSurface);
    SDL_RenderCopy(Renderer, GameBGTexture, NULL, &GameBGRect);
    SDL_DestroyTexture(GameBGTexture);
    GameBGRect.y += GameBGRect.y >= 0 ? -Height + ScrollSpeed : ScrollSpeed;
}

void PrintList(OP **List, SDL_Surface *ListSurface[]) {
    for (OP *i = *List; i != NULL; i = i->Next) {
        SDL_Texture *ListTexture = SDL_CreateTextureFromSurface(Renderer, ListSurface[i->Status]);
        SDL_RenderCopyExF(Renderer, ListTexture, NULL, &i->FRect, -i->Direction, &i->FCentre, 0);
        SDL_DestroyTexture(ListTexture);
    }
}

void PrintMyself() {
    SDL_Texture *MyTexture = SDL_CreateTextureFromSurface(Renderer, MySurface[My.HP > MyHP * 0.6 ? 1 : My.HP > MyHP * 0.3 ? 2 : 3]);
    SDL_RenderCopyExF(Renderer, MyTexture, NULL, &My.FRect, 180 - My.Direction, &My.FCentre, 0);
    SDL_DestroyTexture(MyTexture);
}

void PrintLevel() {
    char TitleLevel[20];
    if (LevelCD > 150)
        sprintf_s(TitleLevel, 10, "Start");
    else
        sprintf_s(TitleLevel, 10, "Level%d", Level);
    SDL_Surface *TitleLevelSurface = TTF_RenderUTF8_Blended(LargeFont, TitleLevel, FontTwinkColor);
    SDL_Texture *TitleLevelTexture = SDL_CreateTextureFromSurface(Renderer, TitleLevelSurface);
    SDL_Rect TitleLevelRect = {(Width - TitleLevelSurface->w) / 2, (Height - TitleLevelSurface->h) / 2, TitleLevelSurface->w, TitleLevelSurface->h};
    SDL_RenderCopy(Renderer, TitleLevelTexture, NULL, &TitleLevelRect);
    SDL_FreeSurface(TitleLevelSurface);
    SDL_DestroyTexture(TitleLevelTexture);
    FontTwinkColor.a += (int) (8 * sin(TwinkTime * D2R));
}

void LoadRes() {
    char FileName[40];
    BGM = Mix_LoadMUS("res/Enemy.mp3");
    Mix_VolumeMusic(BGMVolume);

    LargeFont = TTF_OpenFont("res/GenshinDefault.ttf", 64);
    MiddleFont = TTF_OpenFont("res/GenshinDefault.ttf", 40);
    SmallFont = TTF_OpenFont("res/GenshinDefault.ttf", 28);

    GameBGSurface = IMG_Load("res/GameBG.png");
    ThemeBGSurface = IMG_Load("res/ThemeBG.png");
    for (int i = 1; i <= 5; ++i) {
        sprintf_s(FileName, 30, "res/I%d.png", i);
        MySurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 15; ++i) {
        sprintf_s(FileName, 30, "res/Enemy%d.png", i);
        EnemySurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 5; ++i) {
        sprintf_s(FileName, 30, "res/Boss%d.png", i);
        BossSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 10; ++i) {
        sprintf_s(FileName, 30, "res/Bullet%d.png", i);
        BulletSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i <= 30; ++i) {
        sprintf_s(FileName, 30, "res/Props%d.png", i);
        PropsSurface[i] = IMG_Load(FileName);
    }
}