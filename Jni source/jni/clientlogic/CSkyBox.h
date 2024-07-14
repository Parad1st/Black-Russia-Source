#pragma once

#include "../main.h"

class CObject;

class CSkyBox {
public:
	static void Initialise();

	static void Process();

	static void SetRotSpeed(float mFRotSpeed);
	static void SetTexture(const char *texName);

	static bool IsNeedRender();

	static CObject *GetSkyObject();

private:
	static CObject* CreateObjectScaled(int iModel, float fScale);

	static void ReTexture();

	static uintptr_t RwFrameForAllObjectsCallback(uintptr_t object, CObject* pObject);

private:
	static CObject* m_pSkyObject;

	static bool m_bNeedRender;

	static const char *m_TextureName;

	static float m_fRotSpeed;
};