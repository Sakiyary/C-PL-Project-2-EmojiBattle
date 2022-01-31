#ifndef SAKIYARY_EMOJIBATTLE_C_SAKIYARY_EMOJIBATTLE_H
#define SAKIYARY_EMOJIBATTLE_C_SAKIYARY_EMOJIBATTLE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#undef main
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define Width 1080
#define Height 960
#define D2R 0.01745329252
#define R2D 57.29577951
#define ScrollSpeed 10
#define BGMVolume 30
#define SoundVolume 40
#define mSPF 21

#define SizeMy 96.0
#define SizeLv1Enm 120.0
#define SizeLv2Enm 144.0
#define SizeLv3Enm 120.0
#define SizeBoss 216.0
#define SizeProps 120.0

#define SizeMyBlt 64.0
#define SizeLv2Blt 80.0
#define SizeBossBlt1 64.0
#define SizeBossBlt2 240.0

#define NumLv1 15
#define NumLv2 5
#define NumLv3 1

#define StatusMy 0x1FF1

enum InputAction {
    MoveUp = 0x0001,
    MoveDown = 0x0002,
    MoveLeft = 0x0004,
    MoveRight = 0x0008,
    Shoot = 0x0010,
    RotateLeft = 0x0020,
    RotateRight = 0x0040,
    RotateReset = 0x0080,
    Pause = 0x1000,
    Tab = 0x2000,
    Back2Theme = 0x8000
};

enum HP {
    HPMy = 2000,
    HPBlt = 1,
    HPProps = 1,
    HPLv1Enm = 60,
    HPLv2Enm1 = 600,
    HPLv2Enm2 = 300,
    HPLv3Enm = 500,
    HPBoss = 30000
};

enum Damage {
    DmgMy = 100,
    DmgMyBlt = 10,
    DmgLv1Enm = 200,
    DmgLv2Enm1 = HPMy,
    DmgLv2Blt = 100,
    DmgLv2Enm2 = 400,
    DmgLv3Enm = 500,
    DmgBoss = HPMy,
    DmgBossBlt1 = 200,
    DmgBossBlt2 = HPMy / 3 * 2
};

enum Type {
    TypeMy = 0x0000,
    TypeMyBlt = 0x0001,
    TypeBoss = 0x0002,
    TypeProps = 0x0004,
    TypeOut = 0x0008,
    TypeLv2Blt = 0x0010,
    TypeBossBlt1 = 0x0020,
    TypeBossBlt2 = 0x0040,
    TypeBossBlt3 = 0x0080,
    TypeLv1Enm = 0x0100,
    TypeLv2Enm2 = 0x0200,
    TypeLv2Enm1 = 0x0400,
    TypeLv3Enm = 0x0800,

    TypeChargeEnm = 0x0B00,
    TypeStayEnm = 0x0402,
    TypeEnm = 0x0F02,
    TypeEnmBlt = 0x00F0,

};

typedef struct ObjectParameters {
    SDL_FRect FRect;
    SDL_FPoint FCentre;
    float FSpeed;
    double Direction;
    int HP;
    int Type;
    int Status;
    int Damage;
    struct ObjectParameters *Next;
} OP;

void InitAll();
void ThemeUI(SDL_Event *event, const Uint8 *KeyValue);
int GameUI(SDL_Event *event, const Uint8 *KeyValue);
void ControlFPS(clock_t FStartTime);
int GetKey(const Uint8 *KeyValue);
int MyAction(int Input);
void AddNode(OP **List, OP *NewNode);
OP *CreateNode(OP who, float size, float fspeed, double direction, int hp, int type, int status, int damage);
void FixXYDir(OP **Now, OP who, double direction);
void Collide(OP **List);
void CollideMyBlt(OP **Now);
int IsCollideMe(OP **Now);
void CgEnmStatus(OP **Now);
void CreateProps(OP **Now);
void BuffMe(OP **Now);
void MoveNode(OP **List);
void RemoveNode(OP **List);
void CountLevel(OP **Now);
void CoolDown();
void BloodRage();
double XY2Dir(float x1, float y1, float x2, float y2);
float XY2Dis(float x1, float y1, float x2, float y2);
void CreateEnemy(int level);
void BossAction();
void Move();
void OutBounds(OP **Now);
void Upgrade();
void ResetFontColor();
void LoadRes();
void PrintAnime();
void PrintThemeBG();
void PrintGameBG();
void PrintFPS();
void PrintList(OP **List, SDL_Surface *SurList[]);
void PrintMyself();
void PrintHP();
void PrintInfo();
void PrintLevel();
void PrintHints(char HintGame[], int mode);

static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;
static SDL_Event Event;

static Mix_Music *BGM = NULL;
static Mix_Chunk *Boom = NULL;

static TTF_Font *LargeFont = NULL;
static TTF_Font *MiddleFont = NULL;
static TTF_Font *SmallFont = NULL;
static SDL_Color FontCgColor = {0, 0, 0, 0xFF};
static SDL_Color FontColor = {0, 0, 0, 0x99};

static SDL_Surface *SurThemeBG = NULL;
static SDL_Surface *SurGameBG = NULL;
static SDL_Surface *SurMy[10] = {NULL};
static SDL_Surface *SurEnemy[20] = {NULL};
static SDL_Surface *SurBoss[10] = {NULL};
static SDL_Surface *SurBullet[10] = {NULL};
static SDL_Surface *SurHP[10] = {NULL};
static SDL_Surface *SurProps[30] = {NULL};

static SDL_Rect RectThemeBG = {0, 0, Width, Height};
static SDL_Rect RectGameBG = {0, -Height, Width, Height * 2};
static const SDL_FRect FRectMy = {(float) (Width - SizeMy) / 2, (float) Height / 5 * 4, SizeMy, SizeMy};

static OP My = {{(float) (Width - SizeMy) / 2, (float) Height / 5 * 4, SizeMy, SizeMy},
                {(float) SizeMy / 2, (float) SizeMy / 2}, 15, 180,
                HPMy, TypeMy, StatusMy, DmgMy, NULL};
static OP *MyBlt = NULL;
static OP *ChargeEnm = NULL;
static OP *PeltEnm = NULL;
static OP *PeltBlt = NULL;
static OP *Boss = NULL;
static OP *BossBlt = NULL;
static OP *Props = NULL;

static int CDMyBlt, CDChargeEnm, CDPeltEnm, CDBoss, CDBossBlt,
        CDPeltBlt, CDLevel, CDUnbeatableBoss, CDDisplay, CDBloodRage, CDGoldenBody;
static int Level, Lv1Cnt, Lv2Cnt, Lv3Cnt, BossBltCnt;
static int ChargeEnemyForm;
static int ChargeRandRage;
static int Score;

static int CgRL, CgPN, CgDisplay;
static int CgAngle = 360;
static char HintTheme[] = "Press.\"Enter\".to.Start!";
static char Hint1Game[] = "Press.\"Space\".to.Pause!";
static char Hint2Game[] = "Press.\"Tab\".to.Switch.Display!";
static char HintPause[] = "Press.\"Space\".to.Continue!";
static char HintWin[] = "Press.\"Enter\".to.Restart!";
static char HintLose[] = "Press.\"Enter\".to.Remake!";
static char FPS[15];

static clock_t FDurTime;
#endif //SAKIYARY_EMOJIBATTLE_C_SAKIYARY_EMOJIBATTLE_H