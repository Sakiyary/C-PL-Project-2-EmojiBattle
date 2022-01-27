#include"Sakiyary_EmojiBattle.h"

int main() {
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
    Load();
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    InitialUI(&TotleEvent, KeyValue);
    return 0;
}

void InitialUI(SDL_Event *event, const Uint8 *KeyValue) {
    while (1) {
        while (SDL_WaitEvent(event)) {
            if (event->type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return;
            if (KeyValue[SDL_SCANCODE_RETURN] || KeyValue[SDL_SCANCODE_KP_ENTER]) {
                printf("Game Start!\n");
                if (!GameUI(event, KeyValue))
                    return;
            }
        }
    }
}

int GameUI(SDL_Event *event, const Uint8 *KeyValue) {
    Mix_PlayMusic(BGM, -1);
    while (1) {
        Cooldown();
        PrintAnime();
        if (MyAction(GetKey(KeyValue))) {
            Mix_PauseMusic();
            return 1;
        }
        SDL_Delay(KDFPS);
        while (SDL_PollEvent(event))
            if (event->type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return 0;
    }
}

int GetKey(const Uint8 *KeyValue) {
    int Input = 0;
    if (KeyValue[SDL_SCANCODE_UP] || KeyValue[SDL_SCANCODE_W])
        Input |= 0x1;
    if (KeyValue[SDL_SCANCODE_DOWN] || KeyValue[SDL_SCANCODE_S])
        Input |= 0x2;
    if (KeyValue[SDL_SCANCODE_LEFT] || KeyValue[SDL_SCANCODE_A])
        Input |= 0x4;
    if (KeyValue[SDL_SCANCODE_RIGHT] || KeyValue[SDL_SCANCODE_D])
        Input |= 0x8;
    if (KeyValue[SDL_SCANCODE_LSHIFT] || KeyValue[SDL_SCANCODE_RSHIFT])
        Input |= 0x10;
    if (KeyValue[SDL_SCANCODE_COMMA])
        Input |= 0x20;
    if (KeyValue[SDL_SCANCODE_PERIOD])
        Input |= 0x40;
    if (KeyValue[SDL_SCANCODE_SLASH])
        Input |= 0x80;
    if (KeyValue[SDL_SCANCODE_BACKSPACE])Input |= 0x800;
    return Input;
}

int MyAction(int Input) {
    if (Input & 0x1 && My.FRect.y >= My.FSpeed)
        My.FRect.y -= My.FSpeed;
    if (Input & 0x2 && My.FRect.y <= Height - My.FSpeed - My.FRect.h)
        My.FRect.y += My.FSpeed;
    if (Input & 0x4 && My.FRect.x >= My.FSpeed)
        My.FRect.x -= My.FSpeed;
    if (Input & 0x8 && My.FRect.x <= Width - My.FSpeed - My.FRect.w)
        My.FRect.x += My.FSpeed;
    if (Input & 0x10 && MyBulletCD == 0) {
        ListPush(&MyBullets, CreateBullet(My, 0x10, MyBulletA, 20, My.Direction, 10));
        ListPush(&MyBullets, CreateBullet(My, 0x10, MyBulletA, 20, My.Direction + 20, 10));
        ListPush(&MyBullets, CreateBullet(My, 0x10, MyBulletA, 20, My.Direction - 20, 10));
        MyBulletCD = 10;
    }
    if (Input & 0x20 && My.Direction < 270)
        My.Direction += 10;
    if (Input & 0x40 && My.Direction > 90)
        My.Direction -= 10;
    if (Input & 0x80 && My.Direction != 180)
        My.Direction = 180;
    if (Input & 0x800)
        return 1;
    return 0;
}

void ListPush(OP **List, OP *NewNode) {
    if (*List == NULL) {
        *List = NewNode;
        return;
    }
    OP *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}

OP *CreateBullet(OP who, int type, float size, float fspeed, double direction, int damage) {
    OP *New = malloc(sizeof(OP));
    New->FRect.x = who.FRect.x + who.FCentre.x - size / 2;
    New->FRect.y = who.FRect.y + who.FCentre.y - size / 2;
    New->FRect.w = size;
    New->FRect.h = size;
    New->FCentre.x = size / 2;
    New->FCentre.y = size / 2;
    New->FSpeed = fspeed;
    New->Direction = direction;
    New->HP = 1;
    New->Type = type;
    New->Status = 0x0;
    New->Damage = damage;
    New->Next = NULL;
    return New;
}

void BulletListMove(OP **BulletList) {
    if (*BulletList == NULL)
        return;
    OP *Now = *BulletList;
    while (Now != NULL) {
        Now->FRect.x += Now->FSpeed * (float) sin(Now->Direction * D2R);
        Now->FRect.y += Now->FSpeed * (float) cos(Now->Direction * D2R);
        if ((Now->FRect.y <= -Now->FRect.h)
            || (Now->FRect.y >= Height)
            || (Now->FRect.x <= -Now->FRect.w)
            || (Now->FRect.x >= Width))
            Now->HP = 0;
        Now = Now->Next;
    }
    BulletListRemove(BulletList);
}

void BulletListRemove(OP **BulletList) {
    if (*BulletList == NULL)
        return;
    OP *Now = *BulletList;
    OP *Pre = NULL;
    while (Now != NULL)
        if (!Now->HP)
            if (*BulletList == Now) {
                *BulletList = Now->Next;
                free(Now);
                Now = *BulletList;
            } else {
                Pre->Next = Now->Next;
                free(Now);
                Now = Pre;
            }
        else {
            Pre = Now;
            Now = Now->Next;
        }
}

void Cooldown() {
    MyBulletCD -= MyBulletCD > 0 ? 1 : 0;
}

void PrintAnime() {
//    SDL_RenderClear(Renderer);

    PrintBG();

    SDL_Texture *MyTexture = SDL_CreateTextureFromSurface(Renderer, My1Surface);
    SDL_RenderCopyExF(Renderer, MyTexture, NULL, &My.FRect, 180 - My.Direction, &My.FCentre, 0);
    SDL_DestroyTexture(MyTexture);

    BulletListMove(&MyBullets);
    for (OP *i = MyBullets; i != NULL; i = i->Next) {
        SDL_Texture *MyBulletTexture = SDL_CreateTextureFromSurface(Renderer, MyBullet1Sureface);
        SDL_RenderCopyExF(Renderer, MyBulletTexture, NULL, &i->FRect, -i->Direction, &i->FCentre, 0);
        SDL_DestroyTexture(MyBulletTexture);
    }

    SDL_RenderPresent(Renderer);
}

void PrintBG() {
    SDL_Texture *BackGroundTexture = SDL_CreateTextureFromSurface(Renderer, BackGroundSurface);
    SDL_RenderCopy(Renderer, BackGroundTexture, NULL, &BackGroundRect);
    SDL_DestroyTexture(BackGroundTexture);
    BackGroundRect.y += BackGroundRect.y >= 0 ? -Height + ScrollSpeed : ScrollSpeed;
}


void Load() {
    BGM = Mix_LoadMUS("res/Enemy.mp3");
    Mix_VolumeMusic(BGMVolume);
    BackGroundSurface = IMG_Load("res/BackGround.png");
    My1Surface = IMG_Load("res/I1.png");
    MyBullet1Sureface = IMG_Load("res/MyBullet1.png");
}