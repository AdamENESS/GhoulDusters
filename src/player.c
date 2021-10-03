#include "player.h"
#include "assets.h"

tPlayer* initPlayer(ULONG cash, UBYTE car)
{
    tPlayer* pPlayer = NULL;
    pPlayer = memAllocFastClear(sizeof(tPlayer));
	pPlayer->_cash = cash;
	pPlayer->_carModel = car;
	
 	bobNewInit(
 		&pPlayer->_bobCarMap, 16, 16, 1,
 		g_pSprites, g_pSpriteMask, 80, 100);

    return pPlayer;
}

BYTE updatePlayer(tPlayer* player, BYTE joyX, BYTE joyY)
{
	player->_bobCarMap.sPos.uwX += joyX;
	player->_bobCarMap.sPos.uwY += joyY;
	if (player->_bobCarMap.sPos.uwX <= 0)
		player->_bobCarMap.sPos.uwX = 304;

	if (player->_bobCarMap.sPos.uwX > 304)
		player->_bobCarMap.sPos.uwX = 0;

	if (player->_bobCarMap.sPos.uwY <= 0)
		player->_bobCarMap.sPos.uwY = 160;

	if (player->_bobCarMap.sPos.uwY > 160)
		player->_bobCarMap.sPos.uwY = 0;

     bobNewPush(&player->_bobCarMap);

     return 0;
}