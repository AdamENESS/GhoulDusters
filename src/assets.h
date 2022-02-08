#ifndef _ASSETS_H_
#define _ASSETS_H_
#include <ace/managers/ptplayer.h>
#include <ace/utils/bitmap.h>

// Gfx
extern tBitMap *g_pSprites16x;
extern tBitMap *g_pSpriteMask16x;
extern tBitMap *g_pSprites32x; // Ghosts
extern tBitMap *g_pSpriteMask32x;
extern tBitMap *g_pCarSprites;
extern tBitMap *g_pCarSpritesMask;


extern tBitMap *g_pTiles;
extern tBitMap *g_pTilesMask;

// Music
#define MOD_COUNT 1

extern tPtplayerMod *g_pMods[MOD_COUNT];
extern UWORD *g_pModSamples;

void loadMapAssets(void);
void destroyMapAssets(void);
void loadMusicAssets(void);
#endif // _ASSETS_H_
