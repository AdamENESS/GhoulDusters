#include "tilemap.h"
#include "json/json.h"
#include <ace/managers/log.h>
#include <ace/managers/system.h>

UBYTE mapDataCopyFromMem(tTileMap *pMapData, const UBYTE* src, UBYTE width, BYTE height)
{
    if (src == NULL || pMapData == NULL)
        return 0;

    systemUse();
    logBlockBegin("mapDataCopyFromMem(szPath: '%d, %d')", width, height);
    pMapData->_width = width;
    pMapData->_height = height;
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            const UBYTE val = src[(y*width) + x];
            pMapData->_mapDataBase[x][y] = val;
            //pMapData->_mapDataModified[x][y] = val;
        }
    }
    logBlockEnd("mapDataInitFromFile()");
        
    systemUnuse();

    return 1;
}

UBYTE mapDataInitFromFile(tTileMap *pMapData, const char *szPath)
{
    systemUse();
	logBlockBegin("mapDataInitFromFile(szPath: '%s')", szPath);
	UBYTE isOk = 0;
	tJson *pJson = jsonCreate(szPath);
	if(pJson) {
	    mapDataClear(pMapData);
        isOk = 1;
        UWORD uwTokWidth = jsonGetDom(pJson, "width");
        UWORD uwTokHeight = jsonGetDom(pJson, "height");
        UWORD uwTokLayers = jsonGetDom(pJson, "layers[0].data");

        if(!uwTokWidth || !uwTokHeight || !uwTokLayers) {
            jsonDestroy(pJson);
        
            logWrite("ERR: couldn't find all tokens\n");
            return 0;
        }
        ULONG ulWidth = 0, ulHeight=0;
        ulWidth = jsonTokToUlong(pJson, uwTokWidth);
        ulHeight = jsonTokToUlong(pJson, uwTokHeight);
        for(int y=0; y<ulHeight; y++)
        {
            for(int x=0; x<ulWidth; x++)
            {
                UWORD tokIndex = jsonGetElementInArray(pJson, uwTokLayers, y*ulWidth + x);
                pMapData->_mapDataBase[x][y] = jsonTokToUlong(pJson, tokIndex);
               // logWrite("%d,%d = %d", x,y,pMapData->_mapDataBase[x][y]);
            }
        }
        logBlockEnd("mapDataInitFromFile()");
        jsonDestroy(pJson);
    }
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
        //memset(pMapData->_mapDataModified, 0, sizeof(UBYTE) * (MAP_WIDTH_MAX*MAP_HEIGHT_MAX));
    }
}

void mapDataReset(tTileMap *pMapData)
{
    if (pMapData != NULL)
    {
        //memcpy(pMapData->_mapDataModified, pMapData->_mapDataBase, sizeof(UBYTE) * (MAP_WIDTH_MAX*MAP_HEIGHT_MAX));
    }
}

void mapDataSetTile(tTileMap *pMapData, UBYTE x, UBYTE y, UBYTE value)
{
    if (pMapData != NULL)
    {
        pMapData->_mapDataBase[x][y] = value;
    }
}

UBYTE mapDataGetTile(tTileMap *pMapData, UBYTE x, UBYTE y)
{
    if (pMapData != NULL)
    {
        return pMapData->_mapDataBase[x][y];
    }

    return 0xFF;
}