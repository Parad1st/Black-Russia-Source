#pragma once

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "..//vendor/imgui/stb_truetype.h"

class CFontInstance
{
	unsigned char* m_pFontBuffer;
	stbtt_fontinfo m_sFontInfo;

	int m_iLineHeight;

	float m_fScale;
	int m_iAscent;
	int m_iDescent;
	int m_iLineGap;

	friend class CFontRenderer;

	bool m_bReady;
public:
	CFontInstance(const char* szPath, int iLineHeight);
	~CFontInstance();

	bool IsOk();
};