#ifndef _GHOSTBUSTERS_H_
#define _GHOSTBUSTERS_H_

#include <ace/managers/state.h>

#include "assets.h"

extern tStateManager *g_pStateMachineGame;

extern tState
	g_sStateLogo, g_sStateMenu, g_sStateGame, g_sStateGameInit,
	g_sStateGameTown, g_sStateGameDrive, g_sStateGameBustGhost,  g_sStateGamePause;

#endif // _GHOSTBUSTERS_H_
