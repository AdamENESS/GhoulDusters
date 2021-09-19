#ifndef _GHOSTBUSTERS_H_
#define _GHOSTBUSTERS_H_

#include <ace/managers/state.h>
#include <ace/utils/ptplayer.h>

#define MOD_COUNT 1

extern tPtplayerMod *g_pMods[MOD_COUNT];
extern UWORD *g_pModSamples;

extern tStateManager *g_pStateMachineGame;

extern tState
	g_sStateLogo, g_sStateMenu, g_sStateGame, g_sStateGameInit,
	g_sStateGameTown, g_sStateGameDrive, g_sStateGameBustGhost,  g_sStateGamePause;

#endif // _GHOSTBUSTERS_H_
