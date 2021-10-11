#ifndef _GHOST_H_
#define _GHOST_H_

#include <ace/managers/system.h>
#include "bob_new.h"

#define GHOST_ZULL_X 170
#define GHOST_ZULL_Y 90
#define GHOST_ZULL_STEPS 100

typedef struct _tGhost
{
    UBYTE _corner;
    WORD _ScreenX;
    WORD _ScreenY;
    WORD _StartX;
    WORD _StartY;

    UBYTE _frameOffset, _currentFrame;
    tBobNew _Bob;   
    ULONG ulFrameCounter;
    BYTE _currentFrameStep;
    BYTE _currentZullStep;
    BYTE _respawnDelay;
} tGhost;



tGhost* initGhost(UBYTE corner);
void ghostInitBobs(tGhost* pGhost);
void destroyGhost(tGhost* pGhost);
void ghostProcess(tGhost* pGhost);
void ghostReset(tGhost* pGhost); // Make way to zuul for example.

#endif // _GHOST_H_