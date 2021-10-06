#include "town.h"
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
#include "maps/city16x.h"
#include "bob_new.h"
#include "assets.h"

#define GAME_BPP 5

static UWORD s_pPaletteRef[1 << GAME_BPP];
static UWORD *s_pColorBg;
static tView *s_pView;
static tVPort *s_pVpMain;
tTileBufferManager *g_pMainBuffer;
static UWORD s_uwDistanceTravelled;
// Sprites

UBYTE g_buildings[40] = {
						BT_1S, BT_2S, BT_3S, BT_1S, BT_2S, BT_SP, BT_2S, BT_1S, 
						BT_3S, BT_1S, BT_2S, BT_3S, BT_1S, BT_2S, BT_3S, BT_1S,
						BT_1S, BT_3S, BT_3S, BT_1S, BT_ZL, BT_3S, BT_2S, BT_1S,
						BT_2S, BT_2S, BT_1S, BT_2S, BT_3S, BT_1S, BT_3S, BT_2S,
						BT_1S, BT_2S, BT_HQ, BT_1S, BT_2S, BT_3S, BT_2S, BT_1S};


void setMapBuilding(tTileBufferManager* pMapBuffer, UBYTE building, UBYTE status)
{
	UBYTE bRow = status >> 3;
	UBYTE bCol = status - (bRow << 3);
	
	UBYTE x=0,y=0;
	g_pMainBuffer->pTileData[x][y] = g_pTownMap._mapDataBase[x][y] + (status *39);
}
void coreProcessBeforeBobs(void)
{
	//	timerOnInterrupt();
	bobNewBegin();
	// Draw pending tiles
	tileBufferQueueProcess(g_pMainBuffer);
	static UWORD lastX = 0xFFFF;
	static UWORD lastY = 0xFFFF;
	// blit any map dots.
	UWORD x = (g_pMainPlayer->_locModX << 3) + 4; //pulRandMinMax(0,19) * 16;
	UWORD y = (g_pMainPlayer->_locModY << 3) + 4; //ulRandMinMax(0,10) * 16;
	blitCopyMask(g_pSprites16x, 0, 16 * 17, g_pMainBuffer->pScroll->pBack, x, y, 16, 16, (UWORD *)g_pSpriteMask16x->Planes[0]);
	//blitCopyMask(g_pSprites16x, 0, 16 * 17, g_pMainBuffer->pScroll->pFront, x, y, 16, 16, (UWORD *)g_pSpriteMask16x->Planes[0]);
	if (x != lastX || y != lastY)
	{
		s_uwDistanceTravelled++;
		lastX = x;
		lastY = y;

		static UWORD t = 0;
		setMapBuilding(g_pMainBuffer,1, 2);
		
		t++;
		if (t > 2)
			t = 0;
	}
}

void coreProcessAfterBobs(void)
{

	// Finish bob drawing
	bobNewPushingDone();
	bobNewEnd();

	// Update HUD state machine and draw stuff
	//hudUpdate();

	viewProcessManagers(s_pView);
	copProcessBlocks();
	vPortWaitForEnd(s_pVpMain);
}

void initBuildings(void)
{
	//
}

static void townGsCreate(void)
{
	logBlockBegin("townGsCreate()");

	s_pView = viewCreate(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_END);

	s_pVpMain = vPortCreate(0,
							TAG_VPORT_VIEW, s_pView,
							TAG_VPORT_WIDTH, 320,
							TAG_VPORT_HEIGHT, 176,
							TAG_VPORT_BPP, GAME_BPP,
							TAG_END);
	g_pMainBuffer = tileBufferCreate(0,
									 TAG_TILEBUFFER_VPORT, s_pVpMain,
									 TAG_TILEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
									 TAG_TILEBUFFER_BOUND_TILE_X, CITY16X_WIDTH,
									 TAG_TILEBUFFER_BOUND_TILE_Y, CITY16X_HEIGHT,
									 TAG_TILEBUFFER_IS_DBLBUF, 1,
									 TAG_TILEBUFFER_TILE_SHIFT, 4,
									 TAG_TILEBUFFER_REDRAW_QUEUE_LENGTH, 10,
									 TAG_TILEBUFFER_TILESET, g_pTiles,
									 TAG_END);

	paletteLoad("data/maps/GB-Game.plt", s_pVpMain->pPalette, 1 << GAME_BPP);
	//memset(s_pVpMain->pPalette, 0, 1 << GAME_BPP);

	s_pColorBg = &s_pVpMain->pPalette[1];

	// Load the town map in.
	//   mapDataInitFromFile(&g_pTownMap, "data/maps/city8x.json");
	mapDataCopyFromMem(&g_pTownMap, Tile_Layer_1, CITY16X_WIDTH, CITY16X_HEIGHT);

	UBYTE **pTiles = g_pMainBuffer->pTileData;
	//int y=0;
	for (int y = 0; y < g_pTownMap._height; y++)
	{
		for (int x = 0; x < g_pTownMap._width; x++)
		{
			pTiles[x][y] = mapDataGetTile(&g_pTownMap, x, y);
		}
	}

	bobNewManagerCreate(
		g_pMainBuffer->pScroll->pFront, g_pMainBuffer->pScroll->pBack,
		g_pMainBuffer->pScroll->uwBmAvailHeight);
#ifdef GAME_DEBUG
	randInit(2184);
#else
	// Seed from beam pos Y & X
	randInit((getRayPos().bfPosY << 8) | getRayPos().bfPosX);
#endif
	if (g_pMainPlayer == NULL)
	{
		g_pMainPlayer = initPlayer(10000, 1);
		g_pWanderers[0] = initWanderer(0);
		g_pWanderers[1] = initWanderer(1);
	}
	playerInitBobs(g_pMainPlayer);
	wandererInitBobs(g_pWanderers[0]);
	wandererInitBobs(g_pWanderers[1]);
	//initBuildings();
	bobNewAllocateBgBuffers();

	systemUnuse();

	g_pMainBuffer->pCamera->uPos.uwX = 0;
	// Initial background
	tileBufferRedrawAll(g_pMainBuffer);
	// Load the view
	viewLoad(s_pView);
	logBlockEnd("townGsCreate()");
}

void handleInput(BYTE *bDirX, BYTE *bDirY)
{
	if (joyCheck(JOY1_FIRE))
	{
		s_uwDistanceTravelled = 0;
		tileBufferRedrawAll(g_pMainBuffer);
		bobNewDiscardUndraw();
	}
	if (keyCheck(KEY_D))
	{
		*bDirX += 1;
	}
	if (keyCheck(KEY_A))
	{
		*bDirX -= 1;
	}
	if (keyCheck(KEY_S))
	{
		*bDirY += 1;
	}
	if (keyCheck(KEY_W))
	{
		*bDirY -= 1;
	}

	if (joyCheck(JOY1_RIGHT))
	{
		*bDirX += 1;
	}
	if (joyCheck(JOY1_LEFT))
	{
		*bDirX -= 1;
	}
	if (joyCheck(JOY1_DOWN))
	{
		*bDirY += 1;
	}
	if (joyCheck(JOY1_UP))
	{
		*bDirY -= 1;
	}
}

static void townGsLoop(void)
{

	BYTE bDirX = 0, bDirY = 0;
	handleInput(&bDirX, &bDirY);

	coreProcessBeforeBobs();

	wandererProcess(g_pWanderers[0]);
	wandererProcess(g_pWanderers[1]);

	updatePlayer(g_pMainPlayer, bDirX, bDirY);

	coreProcessAfterBobs();

	//vPortWaitForEnd(s_pVpMain);
	//viewUpdateCLUT(s_pView);
}

static void townGsDestroy(void)
{
	logBlockBegin("townGsDestroy()");
	systemUse();

	viewDestroy(s_pView);
	logBlockEnd("townGsDestroy()");
}

tState g_sStateGameTown = {
	.cbCreate = townGsCreate, .cbLoop = townGsLoop, .cbDestroy = townGsDestroy};
