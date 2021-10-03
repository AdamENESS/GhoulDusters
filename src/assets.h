#ifndef _ASSETS_H_
#define _ASSETS_H_
#include <ace/utils/ptplayer.h>
#include <ace/utils/bitmap.h>

extern tBitMap *g_pSprites16x;
extern tBitMap *g_pSpriteMask16x;
extern tBitMap *g_pTiles;



#define MOD_COUNT 1

extern tPtplayerMod *g_pMods[MOD_COUNT];
extern UWORD *g_pModSamples;


void loadMapAssets(void);
void destroyMapAssets(void);
#endif // _ASSETS_H_
