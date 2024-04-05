#include "../main.h"
#include "font.h"

void CFont::AsciiToGxtChar(const char* ascii, uint16_t* gxt)
{
	return ((void(*)(const char*, uint16_t*))(g_libGTASA + 0x532D00 + 1))(ascii, gxt);
}

void CFont::SetScale(float x, float y)
{
	*(float*)(g_libGTASA + 0x99D750) = x;
	*(float*)(g_libGTASA + 0x99D754) = y;
}

void CFont::SetColor(uint32_t* dwColor)
{
	return ((void(*)(uint32_t*))(g_libGTASA + 0x5336F4 + 1))(dwColor);
}

void CFont::SetJustify(uint8_t justify)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5339D0 + 1))(justify);
}

void CFont::SetOrientation(uint8_t orientation)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5339E8 + 1))(orientation);
}

void CFont::SetWrapX(float wrapX)
{
	return ((void(*)(float))(g_libGTASA + 0x53381C + 1))(wrapX);
}

void CFont::SetCentreSize(float size)
{
	return ((void(*)(float))(g_libGTASA + 0x533834 + 1))(size);
}
void Font_SetRightJustifyWrap(float wrap)
{
	((int (*)(float))(g_libGTASA + 0x0053384C + 1))(wrap);
	//0053384C
}

void CFont::SetBackground(uint8_t bBackground, uint8_t bOnlyText)
{
	return ((void(*)(uint8_t, uint8_t))(g_libGTASA + 0x533988 + 1))(bBackground, bOnlyText);
}

void CFont::SetBackgroundColor(uint32_t* dwColor)
{
	return ((void(*)(uint32_t*))(g_libGTASA + 0x5339A4 + 1))(dwColor);
}

void CFont::SetProportional(uint8_t prop)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x533970 + 1))(prop);
}

void CFont::SetDropColor(uint32_t* dwColor)
{
	return ((void(*)(uint32_t*))(g_libGTASA + 0x53387C + 1))(dwColor);
}

void CFont::SetDropShadowPosition(uint8_t pos)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5338DC + 1))(pos);
}

/*
void CFont::PrintString(float fX, float fY, const uint16_t* szText)
{
	return ((void(*)(float, float, const uint16_t*))(g_libGTASA + 0x5353B4 + 1))(fX, fY, szText);
}
*/

void CFont::PrintString(float posX, float posY, const char* string)
{
	uint16_t* gxt_string = new uint16_t[0xFF];
	CFont::AsciiToGxtChar(string, gxt_string);
	((void (*)(float, float, uint16_t*))(g_libGTASA + 0x5353B4 + 1))(posX, posY, gxt_string);
	delete gxt_string;
	((void (*)())(g_libGTASA + 0x53411C + 1))();//53411C ; _DWORD CFont::RenderFontBuffer(CFont *__hidden this)
}

void CFont::SetFontStyle(uint8_t style)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x533748 + 1))(style);
}

void CFont::SetEdge(uint8_t edge)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x53394C + 1))(edge);
}