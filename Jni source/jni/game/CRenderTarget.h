#pragma once

#include <vector>
#include <stdint.h>

class CRenderTarget
{
	struct RwTexture* m_pResultTexture;

	struct RwCamera* m_pCamera;
	struct RpLight* m_pLight;
	struct RwFrame* m_pFrame;
	struct RwRaster* m_zBuffer;

	uint32_t m_dwResultSizeX;
	uint32_t m_dwResultSizeY;

	bool m_bReady;

	bool m_b2D;

	bool InitialiseScene();

	void PreProcessCamera();
	void ProcessCamera();
	void PostProcessCamera();

	bool m_bSucessfull;
public:
	CRenderTarget(uint32_t dwResultSizeX, uint32_t dwResultSizeY, bool b2D);
	~CRenderTarget();

	void Begin();
	struct RwTexture* End();
};