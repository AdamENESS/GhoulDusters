#include "ghost.h"

#include "assets.h"
#include "misc.h"
#include <ace/managers/rand.h>

void ghostProcess(tGhost *pGhost)
{
    ULONG ulNow = timerGet();
    UBYTE isUpdate = timerGetDelta(pGhost->ulFrameCounter, ulNow) >= 10;
    if (isUpdate)
    {
        pGhost->ulFrameCounter = ulNow;
        if (pGhost->_respawnDelay > 0)
        {
            pGhost->_respawnDelay--;
            pGhost->_Bob.sPos.uwX = 330;
            pGhost->_Bob.sPos.uwY = 210;
            bobNewPush(&pGhost->_Bob);
            return;
        }
        pGhost->_currentFrameStep += 1;
        pGhost->_currentZullStep += 1;
        if (pGhost->_currentZullStep >= GHOST_ZULL_STEPS)
        {
            ghostReset(pGhost);
        }

        if (pGhost->_currentFrameStep > 1)
            pGhost->_currentFrameStep = 0;

        //m_fValueEnd = m_fMin + fRandom * (m_fMax - m_fMin);

        pGhost->_ScreenX = pGhost->_StartX + ((pGhost->_currentZullStep * (GHOST_ZULL_X - pGhost->_StartX)) / GHOST_ZULL_STEPS);
        pGhost->_ScreenY = pGhost->_StartY + ((pGhost->_currentZullStep * (GHOST_ZULL_Y - pGhost->_StartY)) / GHOST_ZULL_STEPS);
        pGhost->_Bob.sPos.uwX = pGhost->_ScreenX;
        pGhost->_Bob.sPos.uwY = pGhost->_ScreenY;
    }
    bobNewSetBitMapOffset(&pGhost->_Bob, pGhost->_frameOffset * (pGhost->_currentFrame + pGhost->_currentFrameStep));
    bobNewPush(&pGhost->_Bob);
}

tGhost *initGhost(UBYTE corner)
{
    tGhost *pGhost = NULL;

    pGhost = memAllocFastClear(sizeof(tGhost));
    pGhost->_corner = corner;
    pGhost->_frameOffset = 32;
    ghostReset(pGhost);

    return pGhost;
}

void ghostInitBobs(tGhost *pGhost)
{
    bobNewInit(
        &pGhost->_Bob, 32, 32, 1,
        g_pSprites32x, g_pSpriteMask32x, 330, 200);
    //bobNewSetBitMapOffset(&pWanderer->_Bob, pWanderer->_frameOffset * pWanderer->_currentFrame);
}

void destroyGhost(tGhost *pGhost);

void ghostReset(tGhost *pGhost)
{
    pGhost->_currentZullStep = 0;
    pGhost->_respawnDelay = uwRandMinMax(50, 100);
    pGhost->_Bob.sPos.uwX = 330;
    pGhost->_Bob.sPos.uwY = 210;
    
    switch (pGhost->_corner)
    {
    case 0:
    default:
    {
        pGhost->_currentFrame = 9;
        pGhost->_StartX = 0;
        pGhost->_StartY = 0;
        break;
    }

    case 1:
    {
        pGhost->_currentFrame = 9;
        pGhost->_StartX = 0;
        pGhost->_StartY = 150;
        break;
    }
    case 2:
    {
        pGhost->_currentFrame = 7;
        pGhost->_StartX = 320;
        pGhost->_StartY = 0;
        break;
    }
    case 3:
    {
        pGhost->_currentFrame = 7;
        pGhost->_StartX = 320;
        pGhost->_StartY = 150;
        break;
    }
    }
}