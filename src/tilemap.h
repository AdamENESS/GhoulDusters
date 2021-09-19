/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TILEMAP_H_
#define _TILEMAP_H_
#include <ace/types.h>
#include <ace/managers/viewport/tilebuffer.h>

#define MAP_WIDTH_MAX 20
#define MAP_HEIGHT_MAX 16
#define MAP_TILESIZE_X 16
#define MAP_TILESIZE_Y 16

typedef struct _tTileSet
{
    UBYTE _countX;
    UBYTE _countY;
    tBitMap* _tileBitmap;
} tTileSet;

typedef struct _tTileMap
{
    UBYTE _mapDataBase[MAP_WIDTH_MAX][MAP_HEIGHT_MAX];
    UBYTE _mapDataModified[MAP_WIDTH_MAX][MAP_HEIGHT_MAX];
    tTileSet* _tileSet;

} tTileMap;

UBYTE mapDataInitFromFile(tTileMap *pMapData, const char *szPath);

UBYTE mapDataSaveToFile(const tTileMap *pMapData, const char *szPath);

void mapDataClear(tTileMap *pMapData);

void mapDataReset(tTileMap *pMapData);

void mapDataSetTile(tTileMap *pMapData, UBYTE x, UBYTE y, UBYTE value);

#endif // _TILEMAP_H_
