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
#define ScrollSpeed 10
#define BGMVolume 20
#define KDFPS 10
#define MyA 96
#define MyBulletA 64


typedef struct ObjectParameters{
    SDL_FRect FRect;
    SDL_FPoint FCentre;
    float FSpeed;
    double Direction;
    int HP;
    int Type;
    int Status;
    int Damage;
    struct ObjectParameters *Next;
}OP;

void InitialUI(SDL_Event *event,const Uint8 *KeyValue);
int GameUI(SDL_Event *event,const Uint8 *KeyValue);
int GetKey(const Uint8 *KeyValue);
int MyAction(int Input);
void ListPush(OP **List,OP *NewNode);
OP *CreateBullet(OP who, int type,float size, float fspeed,double direction,int damage);
void BulletListMove(OP **BulletList);
void BulletListRemove(OP **BulletList);
void Cooldown();
void Load();
void PrintAnime();
void PrintBG();

static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;
static SDL_Event TotleEvent;

static Mix_Music *BGM=NULL;
static Mix_Chunk *Boom=NULL;

static SDL_Surface *BackGroundSurface=NULL;
static SDL_Surface *My1Surface=NULL;
static SDL_Surface *MyBullet1Sureface=NULL;

static SDL_Rect BackGroundRect={0,-Height,Width,Height*2};
static OP My={{(float)(Width-MyA)/2, (float)Height/4*3, MyA, MyA},
              {(float)MyA/2,(float)MyA/2},10, 180,
              1000,0x0,0x0,0,NULL};
static OP *MyBullets=NULL;

static int MyBulletCD;



#endif //SAKIYARY_EMOJIBATTLE_C_SAKIYARY_EMOJIBATTLE_H