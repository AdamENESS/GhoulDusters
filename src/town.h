/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TOWNMAP_H_
#define _TOWNMAP_H_

#include "tilemap.h"
#include "wanderer.h"
#include "player.h"
#include "ghost.h"

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

tTileMap g_pTownMap;

tPlayer *g_pMainPlayer = NULL;
tWanderer *g_pWanderers[2] = {NULL, NULL};
tGhost *g_pGhosts[4] = {NULL, NULL, NULL, NULL};
extern UBYTE g_buildings[40];

#endif // _TOWNMAP_H_
