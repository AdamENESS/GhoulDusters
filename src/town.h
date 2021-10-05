/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TOWNMAP_H_
#define _TOWNMAP_H_

#include "tilemap.h"
#include "wanderer.h"
#include "player.h"

#define BT_HQ 0
#define BT_SP 1
#define BT_ZL 2

#define BT_1S 3
#define BT_2S 4
#define BT_3S 5

#define BT_1H 6
#define BT_1H 7
#define BT_1H 8

#define BT_1A 9
#define BT_1A 10
#define BT_1A 11

#define BT_1D 12
#define BT_1D 13
#define BT_1D 14

#define BT_ZA 15


tTileMap g_pTownMap;

tPlayer* g_pMainPlayer;
tWanderer* g_pWanderers[2];
extern UBYTE g_buildings[40];

#endif // _TOWNMAP_H_
