#include "driving.h"
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
typedef void (*tCbLogo)(void);
typedef UBYTE (*tCbFadeOut)(void);

void driveFadeIn(void);
void driveWait(void);
UBYTE driveFadeOut(void);

static UBYTE s_ubFrame = 0;
static tFadeState s_eFadeState;

static UBYTE s_ubFadeoutCnt;
static tCbLogo s_cbFadeIn = 0, s_cbWait = 0;
static tCbFadeOut s_cbFadeOut = 0;
static UBYTE s_isAnyPressed = 0;
static UBYTE s_isEntryFade = 1;

static void driveGsCreate(void)
{
	s_ubFadeoutCnt = 0;
	s_eFadeState = FADE_STATE_IN;

	s_cbFadeIn = driveFadeIn;
	s_cbFadeOut = driveFadeOut;
	s_cbWait = driveWait;
	bobNewDiscardUndraw();
	UBYTE **pTiles = g_pMainBuffer->pTileData;
	//int y=0;
	for (int y = 0; y < g_pTownMap._height; y++)
	{
		for (int x = 0; x < g_pTownMap._width; x++)
		{
			pTiles[x][y] = 120;
		}
	}
	
	tileBufferRedrawAll(g_pMainBuffer);
}

static void driveGsLoop(void)
{
bobNewBegin();
	s_isAnyPressed = (keyUse(KEY_RETURN) | keyUse(KEY_ESCAPE) | keyUse(KEY_SPACE) |
					  keyUse(KEY_LSHIFT) | keyUse(KEY_RSHIFT) |
					  joyUse(JOY1 + JOY_FIRE) | joyUse(JOY2 + JOY_FIRE));
	if (s_eFadeState == FADE_STATE_IN) 
	{
		if (s_ubFadeoutCnt >= 50)
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
			paletteDim(s_pPaletteRef, g_pVpMain->pPalette, 32, (15 * s_ubFadeoutCnt) / 50);
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
			paletteDim(s_pPaletteRef, g_pVpMain->pPalette, 32, 15 * s_ubFadeoutCnt / 50);
		}
	}

	vPortWaitForEnd(g_pVpMain);
	viewUpdateCLUT(g_pView);
		// Finish bob drawing
	bobNewPushingDone();
	bobNewEnd();

	// Update HUD state machine and draw stuff
	//hudUpdate();

	viewProcessManagers(g_pView);
	copProcessBlocks();
	vPortWaitForEnd(g_pVpMain);
}

static void driveGsDestroy(void)
{
}

void driveFadeIn(void)
{
}

void driveWait(void)
{
	++s_ubFrame;
	if (s_eFadeState == FADE_STATE_IDLE)
	{
		if (s_isAnyPressed)
		{
			s_ubFadeoutCnt = 50;
			s_eFadeState = FADE_STATE_OUT;
		}
	}
	// else if(s_ubFrame == 1){
	// 	//ptplayerSfxPlay(s_pSfxLmc, -1, PTPLAYER_VOLUME_MAX, 1);
	// 	// s_eFadeState = FADE_STATE_OUT; // FOR DEBUGGING SFX GLITCHES
	// }
}

UBYTE driveFadeOut(void)
{
	//ptplayerWaitForSfx();
	//ptplayerSfxDestroy(s_pSfxdrive);

	statePop(g_pStateMachineGame);

	return 1;
}

tState g_sStateGameDrive = {
	.cbCreate = driveGsCreate, .cbLoop = driveGsLoop, .cbDestroy = driveGsDestroy};
