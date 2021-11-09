#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/state.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/blit.h>
#include <ace/utils/palette.h>
#include <ace/managers/rand.h>

#include <ace/managers/viewport/tilebuffer.h>
#include "tilemap.h"
#include "wanderer.h"
#include "player.h"
#include "ghost.h"

typedef struct _tMarshMan tMarshMan;

#define BT_HQ 0 //  Ghost HQ
#define BT_SP 1 //  Shoppie
#define BT_ZL 2 //  ZULL

#define BT_1S 3 // Twin Towers
#define BT_2S 4 // Grassy
#define BT_3S 5 // Single Tower

#define BT_1H 6
#define BT_2H 7
#define BT_3H 8

#define BT_1A 9
#define BT_2A 10
#define BT_3A 11

#define BT_1D 12
#define BT_2D 13
#define BT_3D 14

#define BT_ZA 15

#define BT_UNK 0xFF // reserved for random map generation.

extern tTileMap g_pTownMap;
extern UBYTE g_bBuildings[40];
extern tPlayer *g_pMainPlayer ;
extern tWanderer *g_pWanderers[2];
extern tGhost *g_pGhosts[4];
extern tGhost *g_pMarshGhosts[4];
extern tMarshMan *g_pMarshMellowMan;

typedef struct _tGameStats
{
    USHORT _cityPKEnergy;
    UBYTE* _buildings;
} tGameStats;

extern tView *g_pView;
extern tVPort *g_pVpMain;
extern tVPort *g_pVpHUD;
extern tTileBufferManager *g_pMainBuffer;
extern tSimpleBufferManager *g_pHudBuffer;
extern UWORD s_pPaletteRef[32];
void ResetGameStats(void);

tGameStats* GetCurrentStats(void);
#endif // _GAME_STATE_H_