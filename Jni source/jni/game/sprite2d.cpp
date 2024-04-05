#include "../main.h"
#include "game.h"

#include "sprite2d.h"

void CSprite2d::Draw(float x, float y, float width, float height, CRGBA& color)
{
	((void (*)(CSprite2d*, float, float, float, float, CRGBA&))(g_libGTASA + 0x005525F8 + 1))(this, x, y, width, height, color);
}

CSprite2d::~CSprite2d()
{
	((int (*)(CSprite2d*))(g_libGTASA + 0x00551838 + 1))(this);
}
