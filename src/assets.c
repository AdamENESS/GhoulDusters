#include "assets.h"

tBitMap *g_pSprites;
tBitMap *g_pSpriteMask;
tBitMap *g_pTiles;

tPtplayerMod *g_pMods[MOD_COUNT];
UWORD *g_pModSamples;

void loadMapAssets(void)
{
	g_pSprites = bitmapCreateFromFile("data/sprites/mapsprites.bm", 0);
	g_pSpriteMask = bitmapCreateFromFile("data/sprites/mapsprites_masks.bm", 0);
	g_pTiles = bitmapCreateFromFile("data/maps/citytiles.bm", 0);
}

void destroyMapAssets(void)
{
    bitmapDestroy(g_pTiles);
    bitmapDestroy(g_pSpriteMask);
    bitmapDestroy(g_pSprites);
}