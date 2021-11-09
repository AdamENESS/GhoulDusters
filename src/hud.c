#include "hud.h"
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

void hudCreate(tView *pView, const tFont *pFont) {
    g_pVpHUD = vPortCreate(0,
							TAG_VPORT_VIEW, g_pView,
							TAG_VPORT_WIDTH, 320,
							TAG_VPORT_HEIGHT,256-176,
							TAG_VPORT_BPP, 5,
							TAG_END);
	
	g_pHudBuffer = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, g_pVpHUD,
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_INTERLEAVED,
		TAG_SIMPLEBUFFER_BOUND_HEIGHT, (256-176) * 5,
  TAG_END);

    hudReset();
}


void hudDestroy(void) {

}

void hudReset(void) {
    blitRect(g_pHudBuffer->pBack, 0,0,320,80, 0);
    vPortWaitForEnd(g_pVpHUD);
	viewUpdateCLUT(g_pView);

}

void hudUpdate(void) {

}
