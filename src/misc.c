#include "misc.h"

BYTE canMoveY(UWORD val, UBYTE gap)
{
    BYTE retVal=1;
	//int modX
	for(int modX=8; modX<320; modX+=48, ++retVal)
	{
		
		if (val >= (modX - gap) && val <= (modX + gap))
			return retVal;
	}
	return 0;
}

BYTE canMoveX(UWORD val, UBYTE gap)
{
    BYTE retVal=1;
	for(int modY=8; modY<200; modY+=48, ++retVal)
	{
		
		if (val >= (modY - gap) && val <= (modY + gap))
			return retVal;
	}
	return 0;
}