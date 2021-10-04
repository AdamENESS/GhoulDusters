#include "player.h"
#include "assets.h"

tPlayer* initPlayer(ULONG cash, UBYTE car)
{
    tPlayer* pPlayer = NULL;
    pPlayer = memAllocFastClear(sizeof(tPlayer));
	pPlayer->_cash = cash;
	pPlayer->_carModel = car;
	pPlayer->_locX = 80;
	pPlayer->_locY = 100;
 	bobNewInit(
 		&pPlayer->_bobCarMap, 16, 16, 1,
 		g_pSprites16x, g_pSpriteMask16x, pPlayer->_locX, pPlayer->_locY);

    return pPlayer;
}

BYTE updatePlayer(tPlayer* player, BYTE joyX, BYTE joyY)
{
	player->_locX += joyX;
	player->_locY += joyY;
	if (player->_locX <= 0)
		player->_locX = 304;

	if (player->_locX > 304)
		player->_locX = 0;

	if (player->_locY <= 0)
		player->_locY = 160;

	if (player->_locY > 160)
		player->_locY = 0;

	player->_bobCarMap.sPos.uwX = player->_locX;
	player->_bobCarMap.sPos.uwY = player->_locY;
    bobNewPush(&player->_bobCarMap);

    return 0;
}