#include "town.h"
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/state.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/blit.h>
#include <ace/utils/palette.h>
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

// Sprites

// tBobNew ghostCar;
// tBobNew ghosts[4];
// tBobNew gateKeeper;
// tBobNew keyMaster;

void coreProcessBeforeBobs(void)
{
	// Undraw all bobs
	//debugColor(0x008);
	bobNewBegin();

	// Draw pending tiles
	tileBufferQueueProcess(g_pMainBuffer);

	// Draw dino bones before anything else
	//	dinoProcess();
}

void coreProcessAfterBobs(void)
{

	// Finish bob drawing
	bobNewPushingDone();
	bobNewEnd();

	// Update HUD state machine and draw stuff
	//hudUpdate();

	// Load next base tiles, if needed
	//	baseTileProcess();

	// Update palette for new ground layers, also take into account fade level
	//fadeProcess();
	//UBYTE ubFadeLevel = fadeGetLevel();
	//groundLayerProcess(g_pMainBuffer->pCamera->uPos.uwY, ubFadeLevel);
	//mainPaletteProcess(ubFadeLevel);

	//debugColor(0x800);
	viewProcessManagers(s_pView);
	copProcessBlocks();
	//debugColor(*s_pColorBg);
	vPortWaitForEnd(s_pVpMain);
}


static void townGsCreate(void)
{
	logBlockBegin("townGsCreate()");
	
	loadMapAssets();

	s_pView = viewCreate(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_END);

	s_pVpMain = vPortCreate(0,
							TAG_VPORT_VIEW, s_pView,
							TAG_VPORT_WIDTH, 320,
							TAG_VPORT_HEIGHT, 200,
							TAG_VPORT_BPP, GAME_BPP,
							TAG_END);
	g_pMainBuffer = tileBufferCreate(0,
									 TAG_TILEBUFFER_VPORT, s_pVpMain,
									 TAG_TILEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
									 TAG_TILEBUFFER_BOUND_TILE_X, CITY16X_WIDTH,
									 TAG_TILEBUFFER_BOUND_TILE_Y, CITY16X_HEIGHT,
									 TAG_TILEBUFFER_IS_DBLBUF, 1,
									 TAG_TILEBUFFER_TILE_SHIFT, 4,
									 TAG_TILEBUFFER_REDRAW_QUEUE_LENGTH, 100,
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

// 	bobNewInit(
// 		&ghostCar, 16, 16, 1,
// 		s_pMapSprites, s_pMapMask, 80, 100);

// 	bobNewInit(
// 		&keyMaster, 16, 16, 1,
// 		s_pMapSprites, s_pMapMask, 0, 30);

// bobNewSetBitMapOffset(&keyMaster, 16*1);
// 	bobNewInit(
// 		&gateKeeper, 16, 16, 1,
// 		s_pMapSprites, s_pMapMask, 0, 160);
// bobNewSetBitMapOffset(&gateKeeper, 16*9);
	
	g_pMainPlayer = initPlayer(10000, 1);

	g_pWanderers[0] = initWanderer(0);
	g_pWanderers[1] = initWanderer(1);
	
	bobNewAllocateBgBuffers();

	systemUnuse();

	g_pMainBuffer->pCamera->uPos.uwX = 0;
	// Initial background
	tileBufferRedrawAll(g_pMainBuffer);
	// Load the view
	viewLoad(s_pView);
	logBlockEnd("townGsCreate()");
}

static void townGsLoop(void)
{
	//MCOP_NOP;
	//g_pMainBuffer->pCamera->uPos.uwX += 1;
	//g_pMainBuffer->pCamera->uPos.uwY += 1;
	BYTE bDirX = 0, bDirY = 0;
	//if(g_is1pKbd) {
	if (keyCheck(KEY_D))
	{
		bDirX += 1;
	}
	if (keyCheck(KEY_A))
	{
		bDirX -= 1;
	}
	if (keyCheck(KEY_S))
	{
		bDirY += 1;
	}
	if (keyCheck(KEY_W))
	{
		bDirY -= 1;
	}
	//}
	//else {
	if (joyCheck(JOY1_RIGHT))
	{
		bDirX += 1;
	}
	if (joyCheck(JOY1_LEFT))
	{
		bDirX -= 1;
	}
	if (joyCheck(JOY1_DOWN))
	{
		bDirY += 1;
	}
	if (joyCheck(JOY1_UP))
	{
		bDirY -= 1;
	}
	//}
	//vehicleMove(&g_pVehicles[0], bDirX, bDirY);

	coreProcessBeforeBobs();
	//tileBufferProcess(g_pMainBuffer);
	//ghostCar.sPos.uwX= +1;
	updatePlayer(g_pMainPlayer, bDirX, bDirY);
		
	// bobNewPush(&keyMaster);
	// bobNewPush(&gateKeeper);
	// bobNewPush(&ghostCar);
	coreProcessAfterBobs();
}

static void townGsDestroy(void)
{
	logBlockBegin("townGsDestroy()");
	systemUse();
	destroyMapAssets();
	viewDestroy(s_pView);
	logBlockEnd("townGsDestroy()");
}

tState g_sStateGameTown = {
	.cbCreate = townGsCreate, .cbLoop = townGsLoop, .cbDestroy = townGsDestroy};
