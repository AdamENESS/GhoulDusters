#include "assets.h"

tBitMap *g_pSprites16x;
tBitMap *g_pSpriteMask16x;
tBitMap *g_pTiles;

tPtplayerMod *g_pMods[MOD_COUNT];
UWORD *g_pModSamples;

void loadMapAssets(void)
{
	g_pSprites16x = bitmapCreateFromFile("data/sprites/mapsprites.bm", 0);
	g_pSpriteMask16x = bitmapCreateFromFile("data/sprites/mapsprites_masks.bm", 0);
	g_pTiles = bitmapCreateFromFile("data/maps/citytiles.bm", 0);
}

void destroyMapAssets(void)
{
    bitmapDestroy(g_pTiles);
    bitmapDestroy(g_pSpriteMask16x);
    bitmapDestroy(g_pSprites16x);
}