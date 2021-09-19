#include "tilemap.h"
#include "json/json.h"
#include <ace/managers/log.h>
#include <ace/managers/system.h>


UBYTE mapDataInitFromFile(tTileMap *pMapData, const char *szPath)
{
    systemUse();
	logBlockBegin("mapDataInitFromFile(szPath: '%s')", szPath);
	UBYTE isOk = 0;
	tJson *pJson = jsonCreate(szPath);
	if(pJson) {
	    mapDataClear(pMapData);

        jsonDestroy(pJson);
        isOk = 1;
    }

    logBlockEnd("mapDataInitFromFile()");
	systemUnuse();
	return isOk;
}

UBYTE mapDataSaveToFile(const tTileMap *pMapData, const char *szPath)
{
    systemUse();
	logBlockBegin("mapDataSaveToFile(szPath: '%s')", szPath);
    UBYTE isOk = 0;
    logBlockEnd("mapDataSaveToFile()");
	systemUnuse();
	return isOk;
}

void mapDataClear(tTileMap *pMapData)
{
    if (pMapData != NULL)
    {
        memset(pMapData->_mapDataBase, 0, sizeof(UBYTE) * (MAP_WIDTH_MAX*MAP_HEIGHT_MAX));
        memset(pMapData->_mapDataModified, 0, sizeof(UBYTE) * (MAP_WIDTH_MAX*MAP_HEIGHT_MAX));
    }
}

void mapDataReset(tTileMap *pMapData)
{
    if (pMapData != NULL)
    {
        memcpy(pMapData->_mapDataModified, pMapData->_mapDataBase, sizeof(UBYTE) * (MAP_WIDTH_MAX*MAP_HEIGHT_MAX));
    }
}

void mapDataSetTile(tTileMap *pMapData, UBYTE x, UBYTE y, UBYTE value)
{
    if (pMapData != NULL)
    {
        pMapData->_mapDataModified[x][y] = value;
    }
}
