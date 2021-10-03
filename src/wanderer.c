#include "wanderer.h"
#include "assets.h"
tWanderer* initWanderer(UBYTE personType)
{
    tWanderer* pWanderer = NULL;
    
    pWanderer = memAllocFastClear(sizeof(tWanderer));

    	bobNewInit(
 		&pWanderer->_Bob, 16, 16, 1,
 		g_pSprites16x, g_pSpriteMask16x, 0, (personType ? 0 : 160));
        bobNewSetBitMapOffset(&pWanderer->_Bob, 16 * (personType ? 9:1));

    return pWanderer;
}


void wandererProcess(tWanderer* pWanderer)
{
    if (pWanderer)
    {
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