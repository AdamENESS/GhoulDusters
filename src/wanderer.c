#include "wanderer.h"
#include "assets.h"
tWanderer* initWanderer(UBYTE personType)
{
    tWanderer* pWanderer = NULL;
    
    pWanderer = memAllocFastClear(sizeof(tWanderer));
    pWanderer->_frameOffset = 16;
    pWanderer->_direction =0;
    if (personType)
    {
        pWanderer->_currentFrame = 9;
    }
    else
    {
        pWanderer->_currentFrame = 1;
    }
    for(int dir=0; dir<4;dir++)
    {
        for(int sub=0; sub<2; sub++)
        {
            pWanderer->_Frames[dir][sub] = pWanderer->_currentFrame + ((dir*2)+ sub);
        }
    }

    bobNewInit(
    &pWanderer->_Bob, 16, 16, 1,
    g_pSprites16x, g_pSpriteMask16x, 0, (personType ? 0 : 160));
    bobNewSetBitMapOffset(&pWanderer->_Bob, pWanderer->_frameOffset *pWanderer->_currentFrame);
    
    pWanderer->_currentFrameStep =0;
    
    return pWanderer;
}


void wandererProcess(tWanderer* pWanderer)
{
    
    if (pWanderer)
    {
        /*//pWanderer->_currentFrame +=1;
        pWanderer->_currentFrameStep +=1;
        if (pWanderer->_currentFrameStep > 1)
            pWanderer->_currentFrameStep = 0;
        */

        ULONG ulNow = timerGet();
	    UBYTE isRepeat = timerGetDelta(pWanderer->ulRepeatCounter, ulNow) >= 25;
        if(isRepeat)
        {
                //playerSetCursorPos(pPlayer, pPlayer->pNodeCursor->pNeighbors[eDir]);
                pWanderer->_currentFrameStep +=1;
                if (pWanderer->_currentFrameStep > 1)
                    pWanderer->_currentFrameStep = 0;
                
                pWanderer->ulRepeatCounter = ulNow;
        }
        pWanderer->_currentFrame = pWanderer->_Frames[pWanderer->_direction][pWanderer->_currentFrameStep];
        bobNewSetBitMapOffset(&pWanderer->_Bob, pWanderer->_frameOffset *pWanderer->_currentFrame);
        bobNewPush(&pWanderer->_Bob);
    }
}


void destroyWanderer(tWanderer* pWanderer)
{
    if (pWanderer)
    {
        
    }
}

void wandererSetDestination(tWanderer* pWanderer, UBYTE x, UBYTE y)
{
    if (pWanderer)
    {

    }
}