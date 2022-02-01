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
#include "gamestate.h"
#include "ghostbusters.h"
#include "fade.h"
#include "hud.h"

typedef void (*tCbLogo)(void);
typedef UBYTE (*tCbFadeOut)(void);

void townFadeIn(void);
void townWait(void);
UBYTE townFadeOut(void);

static UBYTE s_ubFrame = 0;
static tFadeState s_eFadeState;

static UBYTE s_ubFadeoutCnt;
static tCbLogo s_cbFadeIn = 0, s_cbWait = 0;
static tCbFadeOut s_cbFadeOut = 0;
static UBYTE s_isAnyPressed = 0;
static UBYTE s_isEntryFade = 1;
#define GAME_BPP 5

//static UWORD s_pPaletteRef[1 << GAME_BPP];
static UWORD *s_pColorBg;

static UWORD s_uwDistanceTravelled;
// Sprites

void setMapBuilding(tTileBufferManager *pMapBuffer, UBYTE building, UBYTE status)
{
	UBYTE bRow = status >> 3;
	UBYTE bCol = status - (bRow << 3);

	UBYTE x = 0, y = 0;
	g_pMainBuffer->pTileData[x][y] = g_pTownMap._mapDataBase[x][y] + (status * 39);
}

void coreProcessBeforeBobs(void)
{
	//	timerOnInterrupt();

	// Draw pending tiles
	tileBufferQueueProcess(g_pMainBuffer);
	if (s_eFadeState == FADE_STATE_IDLE)
	{
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
			//setMapBuilding(g_pMainBuffer, 1, 2);

			t++;
			if (t > 2)
				t = 0;
		}
	}
}

void coreProcessAfterBobs(void)
{

	// Finish bob drawing
	bobNewPushingDone();
	bobNewEnd();

	// Update HUD state machine and draw stuff
	//hudUpdate();

	viewProcessManagers(g_pView);
	copProcessBlocks();
	vPortWaitForEnd(g_pVpMain);
}

void initBuildings(void)
{
	//
}

static void townGsCreate(void)
{
	logBlockBegin("townGsCreate()");

	g_pView = viewCreate(0,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_END);

	g_pVpMain = vPortCreate(0,
							TAG_VPORT_VIEW, g_pView,
							TAG_VPORT_WIDTH, 320,
							TAG_VPORT_HEIGHT, 176,
							TAG_VPORT_BPP, GAME_BPP,
							TAG_END);

	g_pMainBuffer = tileBufferCreate(0,
									 TAG_TILEBUFFER_VPORT, g_pVpMain,
									 TAG_TILEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
									 TAG_TILEBUFFER_BOUND_TILE_X, CITY16X_WIDTH,
									 TAG_TILEBUFFER_BOUND_TILE_Y, 24, // two extra tiles above and below for town.
									 TAG_TILEBUFFER_IS_DBLBUF, 1,
									 TAG_TILEBUFFER_TILE_SHIFT, 4,
									 TAG_TILEBUFFER_REDRAW_QUEUE_LENGTH, 24,
									 TAG_TILEBUFFER_TILESET, g_pTiles,
									 TAG_END);

	hudCreate(g_pView, NULL);
	systemReleaseBlitterToOs();
	paletteLoad("data/maps/GB-Game.plt", g_pVpMain->pPalette, 1 << GAME_BPP);
	for (int p = 0; p < 32; p++)
		s_pPaletteRef[p] = g_pVpMain->pPalette[p];

	//memset(g_pVpMain->pPalette, 0, 1 << GAME_BPP);
	systemGetBlitterFromOs();
	s_pColorBg = &g_pVpMain->pPalette[1];

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
	
	for (int i = 0; i < 4; i++)
	{
		if (g_pGhosts[i] == NULL)
		{
			g_pGhosts[i] = initGhost(i);
		}
		ghostInitBobs(g_pGhosts[i]);
	}
	//initBuildings();
	bobNewAllocateBgBuffers();

	systemUnuse();
	paletteDim(s_pPaletteRef, g_pVpMain->pPalette, 32, (15 * s_ubFadeoutCnt) / 20);
	vPortWaitForEnd(g_pVpMain);
	viewUpdateCLUT(g_pView);
	g_pMainBuffer->pCamera->uPos.uwX = 0;
	// Initial background
	tileBufferRedrawAll(g_pMainBuffer);
	// Load the view
	viewLoad(g_pView);
	s_ubFadeoutCnt = 0;
	s_eFadeState = FADE_STATE_IN;

	s_cbFadeIn = townFadeIn;
	s_cbFadeOut = townFadeOut;
	s_cbWait = townWait;

	logBlockEnd("townGsCreate()");
}

void handleInput(BYTE *bDirX, BYTE *bDirY)
{
	if (joyCheck(JOY1_FIRE))
	{
		s_uwDistanceTravelled = 0;
		//tileBufferRedrawAll(g_pMainBuffer);
		//bobNewDiscardUndraw();
		//statePush(g_pStateMachineGame, &g_sStateGameDrive);
		s_eFadeState = FADE_STATE_OUT;
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
void fadeLoop(void)
{
	if (s_eFadeState == FADE_STATE_IN)
	{
		if (s_ubFadeoutCnt >= 20)
		{
			s_eFadeState = FADE_STATE_IDLE;
			s_ubFrame = 0;
		}
		else
		{
			if (s_cbFadeIn)
			{
				s_cbFadeIn();
			}
			++s_ubFadeoutCnt;
			paletteDim(s_pPaletteRef, g_pVpMain->pPalette, 32, (15 * s_ubFadeoutCnt) / 20);
		}
	}
	else if (s_eFadeState == FADE_STATE_IDLE)
	{
		if (s_cbWait)
		{
			s_cbWait();
		}
	}
	else if (s_eFadeState == FADE_STATE_OUT)
	{
		if (s_ubFadeoutCnt == 0)
		{
			if (s_cbFadeOut && s_cbFadeOut())
			{
				return;
			}
			else
			{
				s_eFadeState = FADE_STATE_IN;
			}
		}
		else
		{
			--s_ubFadeoutCnt;
			paletteDim(s_pPaletteRef, g_pVpMain->pPalette, 32, 15 * s_ubFadeoutCnt / 20);
		}
	}

	vPortWaitForEnd(g_pVpMain);
	viewUpdateCLUT(g_pView);
}
static void townGsLoop(void)
{

	BYTE bDirX = 0, bDirY = 0;

	fadeLoop();
	bobNewBegin();
	coreProcessBeforeBobs();
	if (s_eFadeState == FADE_STATE_IDLE)
	{
		handleInput(&bDirX, &bDirY);
		wandererProcess(g_pWanderers[0]);
		wandererProcess(g_pWanderers[1]);
		for (int i = 0; i < 4; i++)
		{
			ghostProcess(g_pGhosts[i]);
		}
		updatePlayer(g_pMainPlayer, bDirX, bDirY);
	}
	else
	{
		bobNewPush(&g_pMainPlayer->_bobCarMap);
		bobNewPush(&g_pWanderers[0]->_Bob);
		bobNewPush(&g_pWanderers[1]->_Bob);
		for (int i = 0; i < 4; i++)
		{
			bobNewPush(&g_pGhosts[i]->_Bob);
		}
	}

	coreProcessAfterBobs();

	//vPortWaitForEnd(g_pVpMain);
	//viewUpdateCLUT(g_pView);
}

static void townGsDestroy(void)
{
	logBlockBegin("townGsDestroy()");
	systemUse();

	viewDestroy(g_pView);
	logBlockEnd("townGsDestroy()");
}

void townFadeIn(void)
{
}

void townWait(void)
{
	++s_ubFrame;
	if (s_eFadeState == FADE_STATE_IDLE)
	{
		if (s_isAnyPressed)
		{
			s_ubFadeoutCnt = 20;
			s_eFadeState = FADE_STATE_OUT;
		}
	}
	// else if(s_ubFrame == 1){
	// 	//ptplayerSfxPlay(s_pSfxLmc, -1, PTPLAYER_VOLUME_MAX, 1);
	// 	// s_eFadeState = FADE_STATE_OUT; // FOR DEBUGGING SFX GLITCHES
	// }
}

UBYTE townFadeOut(void)
{
	//ptplayerWaitForSfx();
	//ptplayerSfxDestroy(s_pSfxdrive);
	statePush(g_pStateMachineGame, &g_sStateGameDrive);
	return 1;
}

void townResume(void)
{

	s_ubFadeoutCnt = 0;
	s_eFadeState = FADE_STATE_IN;
	UBYTE **pTiles = g_pMainBuffer->pTileData;
	//int y=0;
	for (int y = 0; y < g_pTownMap._height; y++)
	{
		for (int x = 0; x < g_pTownMap._width; x++)
		{
			pTiles[x][y] = mapDataGetTile(&g_pTownMap, x, y);
		}
	}
	tileBufferRedrawAll(g_pMainBuffer);
}
tState g_sStateGameTown = {
	.cbCreate = townGsCreate, .cbLoop = townGsLoop, .cbDestroy = townGsDestroy, .cbResume = townResume};
