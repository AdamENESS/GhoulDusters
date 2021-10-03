#ifndef _GHOSTBUSTER_PLAYER_H_
#define _GHOSTBUSTER_PLAYER_H_

#include <ace/types.h>
#include "bob_new.h"

typedef struct _tPlayer
{
    UBYTE _locX;
    UBYTE _locY;
    UBYTE _carModel;
    UBYTE _equipment[15];
    ULONG _cash;
    char _name[16];
    tBobNew _bobCarMap;
    tBobNew _bobCarDrive;
    tBobNew _bobMan[3];
    tBobNew _bobTrap;
} tPlayer;

tPlayer* initPlayer(ULONG cash, UBYTE car);
BYTE updatePlayer(tPlayer* player, BYTE joyX, BYTE joyY);


#endif // _GHOSTBUSTER_PLAYER_H_