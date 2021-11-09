#ifndef _HUD_H_
#define _HUD_H_
#include <ace/utils/extview.h>
#include <ace/utils/font.h>

void hudCreate(tView *pView, const tFont *pFont);

void hudDestroy(void);

void hudReset(void);

void hudUpdate(void);
#endif // _HUD_H_