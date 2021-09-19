#include "town.h"
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/state.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/blit.h>
#include <ace/utils/palette.h>

static void townGsCreate(void)
{
	logBlockBegin("townGsCreate()");
    // Load the town map in.
    mapDataInitFromFile(&g_pTownMap, "data/maps/city.json");
    systemUnuse();
    

    logBlockEnd("townGsCreate()");

}

static void townGsLoop(void)
{
    MCOP_NOP;
}

static void townGsDestroy(void)
{
    logBlockBegin("townGsDestroy()");
    systemUse();
    logBlockEnd("townGsDestroy()");
}

tState g_sStateGameTown = {
	.cbCreate = townGsCreate, .cbLoop = townGsLoop, .cbDestroy = townGsDestroy
};
