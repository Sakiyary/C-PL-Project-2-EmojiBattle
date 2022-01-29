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
#define Width 1080
#define Height 960
#define D2R 0.01745329252
#define R2D 57.29577951
#define ScrollSpeed 10
#define BGMVolume 30
#define mSPF 10

#define MySize 96.0
#define Lv1EnmSize 120.0
#define Lv2EnmSize 144.0
#define Lv3EnmSize 120.0
#define Lv3BossSize 216.0
#define PropsSize 120.0

#define MyBltSize 64.0
#define Lv2BltSize 80.0
#define Lv3Blt1Size 80.0
#define Lv3Blt2Size 240.0

#define MyHP 1000
#define Lv1EnmHP 30
#define Lv2Enm1HP 200
#define Lv2Enm2HP 40
#define Lv3EnmHP 50
#define Lv3BossHP 200000

#define MyDmg 100
#define MyBltDmg 10
#define Lv1EnmDmg 200
#define Lv2Enm1Dmg 1000
#define Lv2BltDmg 100
#define Lv2Enm2Dmg 300
#define Lv3EnmDmg 400
#define Lv3BossDmg 1000
#define Lv3Blt1Dmg 200
#define Lv3Blt2Dmg 200

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

enum Type {
    MyType = 0x0000,
    BossType = 0x0001,
    MyBltType = 0x0010,
    Lv2BltType = 0x0020,
    Lv3Blt1Type = 0x0040,
    Lv3Blt2Type = 0x0080,
    Lv1EnmType = 0x0100,
    Lv2Enm1Type = 0x0200,
    Lv2Enm2Type = 0x0400,
    Lv3EnmType = 0x0800,
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
int GetKey(const Uint8 *KeyValue);
int MyAction(int Input);
void ListAdd(OP **List, OP *NewNode);
OP *CreateObject(OP who, float size, float fspeed, double direction, int hp, int type, int status, int damage);
void Collision(OP **List);
void ListMove(OP **List);
void ListRemove(OP **List);
void CoolDown();
double XY2Dir(float x1, float y1, float x2, float y2);
float XY2Dis(float x1, float y1, float x2, float y2);
void CreateEnemy(int level);
void Move();
void Upgrade();
void LoadRes();
void PrintAnime();
void PrintThemeBG();
void PrintGameBG();
void PrintList(OP **List, SDL_Surface *ListSurface[]);
void PrintMyself();
void PrintLevel();

static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;
static SDL_Event TotleEvent;

static Mix_Music *BGM = NULL;
static Mix_Chunk *Boom = NULL;

static TTF_Font *LargeFont = NULL;
static TTF_Font *MiddleFont = NULL;
static TTF_Font *SmallFont = NULL;
static SDL_Color FontTwinkColor = {0, 0, 0, 0xFF};

static SDL_Surface *ThemeBGSurface = NULL;
static SDL_Surface *GameBGSurface = NULL;
static SDL_Surface *MySurface[10] = {NULL};
static SDL_Surface *EnemySurface[20] = {NULL};
static SDL_Surface *BossSurface[10] = {NULL};
static SDL_Surface *BulletSurface[10] = {NULL};
static SDL_Surface *PropsSurface[30] = {NULL};

static SDL_Rect ThemeBGRect = {0, 0, Width, Height};
static SDL_Rect GameBGRect = {0, -Height, Width, Height * 2};

static OP My = {{(float) (Width - MySize) / 2, (float) Height / 5 * 4, MySize, MySize},
                {(float) MySize / 2, (float) MySize / 2}, 10, 180,
                MyHP, MyType, 0x0, MyDmg, NULL};
static OP *MyBlt = NULL;
static OP *ChargeEnemy = NULL;
static OP *PeltEnemy = NULL;
static OP *PeltBlt = NULL;
static OP *Boss = NULL;

static int MyBltCD, ChargeCD, PeltEnemyCD, PeltBltCD, LevelCD;
static int RorL;
static int Level, Level1Cnt, Level2Cnt;
static int ChargeEnemyForm;
static int ChargeRandRage;

static int TwinkTime = 360;
static char ThemeTips[] = "Press_\"Enter\"_to_Start!";
static char PauseTips[] = "Press_\"Space\"_to_Continue!";
static char WinTips[] = "Press_\"Enter\"_to_Restart!";
static char LoseTips[] = "Press_\"Enter\"_to_Remake!";

#endif //SAKIYARY_EMOJIBATTLE_C_SAKIYARY_EMOJIBATTLE_H