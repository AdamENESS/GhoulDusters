/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ace/generic/main.h>
#include <ace/managers/key.h>
#include <ace/managers/joy.h>
#include <ace/managers/state.h>
#include <ace/utils/ptplayer.h>
#include "ghostbusters.h"
//#include "logo.h"
//#include "assets.h"

tStateManager *g_pStateMachineGame;
static ULONG s_ulSampleSize;
static UBYTE s_ubCurrentMod = 0;

static void onSongEnd(void) {
	if(++s_ubCurrentMod >= MOD_COUNT) {
		s_ubCurrentMod = 0;
	}
	ptplayerLoadMod(g_pMods[s_ubCurrentMod], g_pModSamples, 0);
	ptplayerEnableMusic(1);
}

void genericCreate(void) {
	systemUse();
	g_pStateMachineGame = stateManagerCreate();
	keyCreate();
	joyOpen();
	ptplayerCreate(1);
	//g_pMods[0] = ptplayerModCreate("data/ghostbusters.mod");
	//g_pMods[1] = ptplayerModCreate("data/ghostbusters.mod");
	//s_ulSampleSize = fileGetSize("data/samples.samplepack");
	//g_pModSamples = memAllocChip(s_ulSampleSize);
	//tFile *pFileSamples = fileOpen("data/samples.samplepack", "rb");
	//fileRead(pFileSamples, g_pModSamples, s_ulSampleSize);
	//fileClose(pFileSamples);

	//ptplayerLoadMod(g_pMods[s_ubCurrentMod], g_pModSamples, 0);
	//ptplayerConfigureSongRepeat(0, onSongEnd);
	statePush(g_pStateMachineGame, &g_sStateLogo);
	//statePush(g_pStateMachineGame, &g_sStateGameTown);

}

void genericProcess(void) {
	ptplayerProcess();
	keyProcess();
	joyProcess();
	stateProcess(g_pStateMachineGame);
	
}

void genericDestroy(void) {
	ptplayerDestroy();
//	assetsGlobalDestroy();
	keyDestroy();
	joyClose();
	stateManagerDestroy(g_pStateMachineGame);
}
