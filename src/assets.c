#include <ace/managers/memory.h>
#include <ace/managers/system.h>
#include "assets.h"

tBitMap *g_pSprites16x;
tBitMap *g_pSpriteMask16x;
tBitMap *g_pSprites32x;
tBitMap *g_pSpriteMask32x;
tBitMap *g_pTiles;
tBitMap *g_pTilesMask;
tBitMap *g_pCarSprites;
tBitMap *g_pCarSpritesMask;
tPtplayerMod *g_pMods[MOD_COUNT];
UWORD *g_pModSamples;

void loadMapAssets(void)
{
	//systemDisownBlitter();
	g_pSprites16x = bitmapCreateFromFile("data/sprites/mapsprites.bm", 0);
	g_pSpriteMask16x = bitmapCreateFromFile("data/sprites/mapsprites_masks.bm", 0);
	g_pSprites32x = bitmapCreateFromFile("data/sprites/ghosts.bm", 0);
	g_pSpriteMask32x = bitmapCreateFromFile("data/sprites/ghosts_mask.bm", 0);
	g_pCarSprites = bitmapCreateFromFile("data/sprites/cars.bm", 0);
	g_pCarSpritesMask = bitmapCreateFromFile("data/sprites/cars_mask.bm", 0);

	g_pTiles = bitmapCreateFromFile("data/maps/citytiles.bm", 0);
	g_pTilesMask = bitmapCreateFromFile("data/maps/citytilesMasked.bm", 0);
	
//	systemOwnBlitter();
}

void destroyMapAssets(void)
{
    bitmapDestroy(g_pTilesMask);
	bitmapDestroy(g_pTiles);
    bitmapDestroy(g_pSpriteMask16x);
    bitmapDestroy(g_pSprites16x);
	bitmapDestroy(g_pCarSpritesMask);
	bitmapDestroy(g_pCarSprites);
	bitmapDestroy(g_pSpriteMask32x);
	bitmapDestroy(g_pSprites32x);
}

void loadMusicAssets(void)
{
    ptplayerCreate(1);
	g_pMods[0] = ptplayerModCreate("data/ghostbusters.mod");
	//g_pMods[1] = ptplayerModCreate("data/ghostbusters.mod");
	ULONG ulSampleSize = fileGetSize("data/samples.samplepack");
	g_pModSamples = memAllocChip(ulSampleSize);
	tFile *pFileSamples = fileOpen("data/samples.samplepack", "rb");
	fileRead(pFileSamples, g_pModSamples, ulSampleSize);
	fileClose(pFileSamples);

	ptplayerLoadMod(g_pMods[0], g_pModSamples, 0);
	//ptplayerConfigureSongRepeat(0, onSongEnd);
	ptplayerEnableMusic(1);
}