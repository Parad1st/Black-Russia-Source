#pragma once

class CRadarRect
{
	static bool m_bEnabled;
public:
	static RwTexture* m_pRectTexture;
	static RwTexture* m_pDiscTexture;
	static void LoadTextures();
	static float CRadar__LimitRadarPoint_hook(float* a1);

	static bool IsEnabled();

	static void SetEnabled(bool bEnabled);
};