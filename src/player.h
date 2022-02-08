#ifndef _GHOSTBUSTER_PLAYER_H_
#define _GHOSTBUSTER_PLAYER_H_

#include <ace/types.h>
#include "bob_new.h"

typedef struct _tPlayer
{
    UWORD _locX;
    UWORD _locY;
    UWORD _locMapX, _locMapY;
    UBYTE _locModX, _locModY;
    UBYTE _carModel;
    UBYTE _equipment[15];
    ULONG _cash;
    char _name[16];
    tBobNew _bobCarMap;
    tBobNew _bobCarTrail;
    tBobNew _bobCarDrive;
    tBobNew _bobMan[3];
    tBobNew _bobTrap;

    ULONG ulFrameCounter;
} tPlayer;

tPlayer* initPlayer(ULONG cash, UBYTE car);
void playerInitBobs(tPlayer* pPlayer);
BYTE updatePlayer(tPlayer* player, BYTE joyX, BYTE joyY);

//void playerGetCenter(tPlayer* player)

#endif // _GHOSTBUSTER_PLAYER_H_