#ifndef _WANDERER_H_
#define _WANDERER_H_

#include <ace/managers/system.h>
#include "bob_new.h"

typedef struct _tWanderer
{
    UBYTE _direction;
    UBYTE _currentFrame;
    UBYTE _frameOffset;
    UWORD _ScreenX;
    UWORD _ScreenY;
    UBYTE _Frames[4][2];
    BYTE _frameMod;
    BYTE _currentFrameStep;
    ULONG ulFrameCounter;
    tBobNew _Bob;   
} tWanderer;


void wandererProcess(tWanderer* pWanderer);

tWanderer* initWanderer(UBYTE personType);
void wandererInitBobs(tWanderer* pWanderer);
void destroyWanderer(tWanderer* pWanderer);

void wandererSetDestination(tWanderer* pWanderer, UBYTE x, UBYTE y); // Make way to zuul for example.

#endif // _WANDERER_H_