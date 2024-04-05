#include "CRenderTarget.h"

#include "../main.h"
#include "game.h"
#include "../util/armhook.h"

bool CRenderTarget::InitialiseScene()
{
	// RpLightCreate
	m_pLight = ((struct RpLight* (*)(int))(g_libGTASA + 0x1E3860 + 1))(2);
	if (m_pLight == nullptr)
	{
		return false;
	}

	// RpLightSetColor
	float rwColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	((struct RpLight* (*)(struct RpLight*, float*))(g_libGTASA + 0x1E333C + 1))(m_pLight, rwColor);

	// RwCameraCreate
	m_pCamera = ((struct RwCamera* (*)())(g_libGTASA + 0x1ADA1C + 1))();

	if (m_pCamera == nullptr)
	{
		return false;
	}

	// RwFrameCreate
	m_pFrame = ((struct RwFrame* (*)())(g_libGTASA + 0x1AE9E0 + 1))();

	if (m_pFrame == nullptr)
	{
		return false;
	}

	// RwFrameTranslate
	float v[3] = { 0.0f, 0.0f, 50.0f };
	((RwFrame*(*)(struct RwFrame*, float*, int))(g_libGTASA + 0x1AED7C + 1))(m_pFrame, v, 1);

	// RwFrameRotate
	v[0] = 1.0f; v[1] = 0.0f; v[2] = 0.0f;
	((RwFrame*(*)(struct RwFrame*, float*, float, int))(g_libGTASA + 0x1AEDC4 + 1))(m_pFrame, v, 90.0f, 1);

	m_zBuffer = RwRasterCreate(m_dwResultSizeX, m_dwResultSizeY, 0, rwRASTERTYPEZBUFFER);

	if (m_zBuffer == nullptr)
	{
		return false;
	}

	*(RwRaster**)((uintptr_t)m_pCamera + 0x64) = m_zBuffer;

	// RwObjectHasFrameSetFrame
	((void(*)(struct RwCamera*, struct RwFrame*))(g_libGTASA + 0x1B2988 + 1))(m_pCamera, m_pFrame);

	// RwCameraSetFarClipPlane
	((struct RwCamera*(*)(struct RwCamera*, float))(g_libGTASA + 0x1AD710 + 1))(m_pCamera, 300.0f);

	// RwCameraSetNearClipPlane
	((struct RwCamera* (*)(struct RwCamera*, float))(g_libGTASA + 0x1AD6F4 + 1))(m_pCamera, 0.01f);

	// RwCameraSetViewWindow
	float view[2] = { 0.5f, 0.5f };
	((struct RwCamera* (*)(struct RwCamera*, float*))(g_libGTASA + 0x1AD924 + 1))(m_pCamera, view);

	// RwCameraSetProjection
	((struct RwCamera* (*)(struct RwCamera*, int))(g_libGTASA + 0x1AD8DC + 1))(m_pCamera, 1);

	// RpWorldAddCamera
	uintptr_t pRwWorld = *(uintptr_t*)(g_libGTASA + 0x95B060);
	if (pRwWorld)
	{
		((uintptr_t(*)(uintptr_t, struct RwCamera*))(g_libGTASA + 0x1EB118 + 1))(pRwWorld, m_pCamera);
	}
	else
	{
		return false;
	}

	return true;
}

void CRenderTarget::PreProcessCamera()
{
	struct RwRaster* pRaster = RwRasterCreate(m_dwResultSizeX, m_dwResultSizeY, 32, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);

	// RwTextureCreate
	m_pResultTexture = ((struct RwTexture*(*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pRaster);
	*(RwRaster**)((uintptr_t)m_pCamera + 0x60) = pRaster;

	// CVisibilityPlugins::SetRenderWareCamera
	((void(*)(struct RwCamera*))(g_libGTASA + 0x55CFA4 + 1))(m_pCamera);
}

void CRenderTarget::ProcessCamera()
{
	// RwCameraClear
	uint32_t dwColor = 0xFFFFFFFF;
	((struct RwCamera* (*)(struct RwCamera*, uint32_t*, int))(g_libGTASA + 0x1AD8A0 + 1))(m_pCamera, &dwColor, 3);

	if (!RwCameraBeginUpdate(m_pCamera))
	{
		m_bSucessfull = false;
		return;
	}
	m_bSucessfull = true;

	// RpWorldAddLight
	uintptr_t pRwWorld = *(uintptr_t*)(g_libGTASA + 0x95B060);
	if (pRwWorld)
	{
		((uintptr_t(*)(uintptr_t, struct RpLight*))(g_libGTASA + 0x1EB868 + 1))(pRwWorld, m_pLight);
	}

	if (m_b2D)
	{
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)0);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)0);
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
		RwRenderStateSet(rwRENDERSTATEBORDERCOLOR, (void*)0);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONGREATER);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)2);
		RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEARMIPLINEAR);
		RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);

		DefinedState2d();
	}
	else
	{
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)true);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)true);
		RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODENASHADEMODE);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)0);
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODENACULLMODE);
		RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)false);

		// DefinedState
		((void(*) (void))(g_libGTASA + 0x559008 + 1))();
	}
}

void CRenderTarget::PostProcessCamera()
{
	if (m_bSucessfull)
	{
		RwCameraEndUpdate(m_pCamera);

		uintptr_t pRwWorld = *(uintptr_t*)(g_libGTASA + 0x95B060);

		// RpWorldRemoveLight
		if (pRwWorld)
		{
			((uintptr_t(*)(uintptr_t, struct RpLight*))(g_libGTASA + 0x1EB8F0 + 1))(pRwWorld, m_pLight);
		}
	}
}

CRenderTarget::CRenderTarget(uint32_t dwResultSizeX, uint32_t dwResultSizeY, bool b2D)
{
	m_pResultTexture = nullptr;

	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pFrame = nullptr;
	m_zBuffer = 0;

	m_bReady = false;

	m_dwResultSizeX = dwResultSizeX;
	m_dwResultSizeY = dwResultSizeY;

	m_b2D = b2D;

	if (InitialiseScene())
	{
		m_bReady = true;
	}
}

CRenderTarget::~CRenderTarget()
{
	// RwBool RpLightDestroy(RpLight* light);
	((RwBool (*)(struct RpLight*))(g_libGTASA + 0x001E3810 + 1))(m_pLight);

	// RwObjectHasFrameSetFrame
	((void(*)(struct RwCamera*, struct RwFrame*))(g_libGTASA + 0x1B2988 + 1))(m_pCamera, nullptr);

	// RwBool RwFrameDestroy(RwFrame* frame);
	((RwBool(*)(struct RwFrame*))(g_libGTASA + 0x001AEC84 + 1))(m_pFrame);

	RwRasterDestroy(m_zBuffer);
	*(RwRaster**)((uintptr_t)m_pCamera + 0x64) = nullptr;

	// RwBool RwCameraDestroy(RwCamera* camera);

	((RwBool(*)(struct RwCamera*))(g_libGTASA + 0x001AD9D8 + 1))(m_pCamera);
}

void CRenderTarget::Begin()
{
	if (!m_bReady)
	{
		return;
	}

	PreProcessCamera();
	ProcessCamera();
}

RwTexture* CRenderTarget::End()
{
	if (!m_bReady)
	{
		return nullptr;
	}

	PostProcessCamera();

	struct RwTexture* pTexture = m_pResultTexture;

	((RwTexture*(*)(struct RwTexture*, const char*))(g_libGTASA + 0x001B1BB4 + 1))(pTexture, "rtarget");

	m_pResultTexture = nullptr;
	*(RwRaster * *)((uintptr_t)m_pCamera + 0x60) = nullptr;

	return pTexture;
}

