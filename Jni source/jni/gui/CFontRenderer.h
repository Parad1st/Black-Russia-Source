#pragma once

#include "CFontInstance.h"

#define MAX_FONTS_RENDERER 5

class CFontRenderer
{
	static CFontInstance* m_pFonts[MAX_FONTS_RENDERER];
public:
	static void Initialise();
	static void Shutdown();

	static CFontInstance* AddFont(const char* szPath, int iLineHeight);

	static void RenderText(const char* szText, CFontInstance* pFont, unsigned char* bitmap, int bitWidth, int& x, int& y_max);
};