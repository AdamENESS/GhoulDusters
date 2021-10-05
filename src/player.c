#include "player.h"
#include "assets.h"
#include <ace/managers/log.h>
#include "misc.h"

tPlayer *initPlayer(ULONG cash, UBYTE car)
{
	tPlayer *pPlayer = NULL;
	pPlayer = memAllocFastClear(sizeof(tPlayer));
	pPlayer->_cash = cash;
	pPlayer->_carModel = car;
	pPlayer->_locX = 80;
	pPlayer->_locY = 156;
	pPlayer->_locModX = pPlayer->_locX >> 3;
	pPlayer->_locModY = pPlayer->_locY >> 3;
	bobNewInit(
		&pPlayer->_bobCarMap, 16, 16,1,
		g_pSprites16x, g_pSpriteMask16x, pPlayer->_locX, pPlayer->_locY);


// bobNewInit(
// 		&pPlayer->_bobCarTrail, 16, 16, 0,
// 		g_pSprites16x, g_pSpriteMask16x, -16, -16);



// 	bobNewSetBitMapOffset(&pPlayer->_bobCarTrail, 16*17);

	return pPlayer;
}


BYTE updatePlayer(tPlayer *pPlayer, BYTE joyX, BYTE joyY)
{
	//logWrite("%uw %uw\n", pPlayer->_locX,pPlayer->_locY);
	ULONG ulNow = timerGet();
	//UBYTE isRepeat = timerGetDelta(pPlayer->ulFrameCounter, ulNow) >= 1;
	//if (isRepeat)
	{
		if (canMoveX(pPlayer->_locY-4))
		{
			pPlayer->_locX += joyX;
			if (pPlayer->_locX <= 0)
				pPlayer->_locX = 304;

			if (pPlayer->_locX > 304)
				pPlayer->_locX = 0;
		}

		if (canMoveY(pPlayer->_locX+4))
		{
			pPlayer->_locY += joyY;
	

			if (pPlayer->_locY <= 0)
				pPlayer->_locY = 160;

			if (pPlayer->_locY > 160)
				pPlayer->_locY = 0;
		}
		pPlayer->_locModX = pPlayer->_locX >> 3;
		pPlayer->_locModY = pPlayer->_locY >> 3;

		pPlayer->_bobCarMap.sPos.uwX = pPlayer->_locX;
		pPlayer->_bobCarMap.sPos.uwY = pPlayer->_locY;

		// pPlayer->_bobCarTrail.sPos.uwX = pPlayer->_locModX << 3;
		// pPlayer->_bobCarTrail.sPos.uwY = pPlayer->_locModY << 3;
		// pPlayer->_bobCarTrail.sPos.uwX += 4;
		// pPlayer->_bobCarTrail.sPos.uwY += 4;


		pPlayer->ulFrameCounter = ulNow;
	}

	
	bobNewPush(&pPlayer->_bobCarMap);
	//bobNewPush(&pPlayer->_bobCarTrail);
	return 0;
}