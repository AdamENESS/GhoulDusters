#include "misc.h"

BYTE canMoveY(UWORD val)
{
    BYTE retVal=1;
	//int modX
	for(int modX=8; modX<320; modX+=48, ++retVal)
	{
		
		if (val == modX/* && val < (modX + 8)*/)
			return retVal;
	}
	return 0;
}

BYTE canMoveX(UWORD val)
{
    BYTE retVal=1;
	for(int modY=8; modY<200; modY+=48, ++retVal)
	{
		
		if (val == modY /*&& val < (modY + 8)*/)
			return retVal;
	}
	return 0;
}