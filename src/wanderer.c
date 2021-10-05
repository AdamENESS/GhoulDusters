#include "wanderer.h"
#include "assets.h"
#include "misc.h"
#include <ace/managers/rand.h>

tWanderer *initWanderer(UBYTE personType)
{
    tWanderer *pWanderer = NULL;

    pWanderer = memAllocFastClear(sizeof(tWanderer));
    pWanderer->_frameOffset = 16;
    pWanderer->_direction = 2;
    pWanderer->_ScreenX = 8;
    pWanderer->_ScreenY = (personType ? 8 : 152);
    if (personType)
    {
        pWanderer->_currentFrame = 9;
    }
    else
    {
        pWanderer->_currentFrame = 1;
    }
    for (int dir = 0; dir < 4; dir++)
    {
        for (int sub = 0; sub < 2; sub++)
        {
            pWanderer->_Frames[dir][sub] = pWanderer->_currentFrame + ((dir * 2) + sub);
        }
    }

    bobNewInit(
        &pWanderer->_Bob, 16, 16, 1,
        g_pSprites16x, g_pSpriteMask16x, pWanderer->_ScreenX, pWanderer->_ScreenY);
    bobNewSetBitMapOffset(&pWanderer->_Bob, pWanderer->_frameOffset * pWanderer->_currentFrame);

    pWanderer->_currentFrameStep = 0;

    return pWanderer;
}

void wandererProcess(tWanderer *pWanderer)
{

    if (pWanderer)
    {
        /*//pWanderer->_currentFrame +=1;
        pWanderer->_currentFrameStep +=1;
        if (pWanderer->_currentFrameStep > 1)
            pWanderer->_currentFrameStep = 0;
        */

        ULONG ulNow = timerGet();
        UBYTE isUpdate = timerGetDelta(pWanderer->ulFrameCounter, ulNow) >= 10;
        if (isUpdate)
        {
            //playerSetCursorPos(pPlayer, pPlayer->pNodeCursor->pNeighbors[eDir]);
            pWanderer->_currentFrameStep += 1;
            if (pWanderer->_currentFrameStep > 1)
                pWanderer->_currentFrameStep = 0;

            pWanderer->ulFrameCounter = ulNow;

            pWanderer->_currentFrame = pWanderer->_Frames[pWanderer->_direction][pWanderer->_currentFrameStep];
            bobNewSetBitMapOffset(&pWanderer->_Bob, pWanderer->_frameOffset * pWanderer->_currentFrame);

            switch (pWanderer->_direction)
            {
            case 0:
            {
                pWanderer->_ScreenY -= 2;
                /* code */
                break;
            }
            case 1:
            {
                pWanderer->_ScreenY += 2;
                /* code */
                break;
            }
            case 2:
            {
                pWanderer->_ScreenX -= 2;
                /* code */
                break;
            }
            case 3:
            {
                pWanderer->_ScreenX += 2;
                /* code */
                break;
            }
            default:
                break;
            }
            
            switch (pWanderer->_direction)
            {
            case 0:
            case 1:
            {
                if (canMoveX(pWanderer->_ScreenY + 4))
                {
                    pWanderer->_direction =( uwRandMinMax(0,10) > 6)? 2:3;
                }

                break;
            }
            case 2:
            case 3:
            {
                if (canMoveY(pWanderer->_ScreenX + 4))
                {
                    pWanderer->_direction = ( uwRandMinMax(0,10) > 6)? 0:1;
                }
                break;
            }
            default:
                break;
            }

            if (pWanderer->_ScreenX <= 0)
                pWanderer->_ScreenX = 304;

            if (pWanderer->_ScreenX > 304)
                pWanderer->_ScreenX = 0;

            if (pWanderer->_ScreenY <= 0)
                pWanderer->_ScreenY = 160;

            if (pWanderer->_ScreenY > 160)
                pWanderer->_ScreenY = 0;

            pWanderer->_Bob.sPos.uwX = pWanderer->_ScreenX;
            pWanderer->_Bob.sPos.uwY = pWanderer->_ScreenY;
        }

        bobNewPush(&pWanderer->_Bob);
    }
}

void destroyWanderer(tWanderer *pWanderer)
{
    if (pWanderer)
    {
    }
}

void wandererSetDestination(tWanderer *pWanderer, UBYTE x, UBYTE y)
{
    if (pWanderer)
    {
    }
}