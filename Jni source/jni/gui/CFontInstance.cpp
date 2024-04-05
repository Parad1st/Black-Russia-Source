#include "CFontInstance.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CFontInstance::CFontInstance(const char* szPath, int iLineHeight)
{
	m_bReady = false;
	m_pFontBuffer = nullptr;

	m_iLineHeight = iLineHeight;

	long size;

	FILE* pFontFile = fopen(szPath, "rb");

	if (!pFontFile)
	{
		return;
	}

	fseek(pFontFile, 0, SEEK_END);
	size = ftell(pFontFile); // how long is the file ?
	fseek(pFontFile, 0, SEEK_SET); // reset 

	m_pFontBuffer = new unsigned char[size];

	fread(m_pFontBuffer, size, 1, pFontFile);

	fclose(pFontFile);

	if (!stbtt_InitFont(&m_sFontInfo, m_pFontBuffer, 0))
	{
		return;
	}

	m_fScale = stbtt_ScaleForPixelHeight(&m_sFontInfo, m_iLineHeight);

	stbtt_GetFontVMetrics(&m_sFontInfo, &m_iAscent, &m_iDescent, &m_iLineGap);

	m_iAscent = roundf(m_iAscent * m_fScale);
	m_iDescent = roundf(m_iDescent * m_fScale);

	m_bReady = true;
}

CFontInstance::~CFontInstance()
{
	if (m_pFontBuffer)
	{
		delete[] m_pFontBuffer;
	}
}

bool CFontInstance::IsOk()
{
	return m_bReady;
}
