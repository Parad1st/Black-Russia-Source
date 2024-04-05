#include "../main.h"
#include "game.h"
#include "CFirstPersonCamera.h"
#include "..//chatwindow.h"

#include "..//net/netgame.h"

extern CNetGame* pNetGame;

bool CFirstPersonCamera::m_bEnabled = false;

extern CChatWindow* pChatWindow;
MATRIX4X4* RwMatrixMultiplyByVector(VECTOR* out, MATRIX4X4* a2, VECTOR* in);
void CFirstPersonCamera::ProcessCameraOnFoot(uintptr_t pCam, CPlayerPed* pPed)
{
	if (!m_bEnabled || *(uint8_t*)(g_libGTASA + 0x8B147E) || *(uint8_t*)(g_libGTASA + 0x8B147F))
	{
		return;
	}

	VECTOR* pVec = (VECTOR*)(pCam + 372);

	VECTOR vecOffset;
	vecOffset.X = 0.35f;
	vecOffset.Y = 0.1f;
	vecOffset.Z = 0.1f;
	if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON).X != -1)
	{
		vecOffset.X += (float)(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON).X - 200) / 100.0f;
	}
	if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON).Y != -1)
	{
		vecOffset.Y += (float)(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON).Y - 200) / 100.0f;
	}
	if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_FIRSTPERSON).Y != -1)
	{
		vecOffset.Z += (float)(CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_FIRSTPERSON).Y - 200) / 100.0f;
	}

	VECTOR vecOut;
	RwMatrixMultiplyByVector(&vecOut, &(pPed->m_HeadBoneMatrix), &vecOffset);

	if (vecOut.X != vecOut.X || vecOut.Y != vecOut.Y || vecOut.Z != vecOut.Z)
	{
		pPed->GetBonePosition(4, &vecOut);
	}
	if (vecOut.X != vecOut.X || vecOut.Y != vecOut.Y || vecOut.Z != vecOut.Z)
	{
		return;
	}

	pVec->X = vecOut.X;
	pVec->Y = vecOut.Y;
	pVec->Z = vecOut.Z;

	((RwCamera*(*)(RwCamera*, float))(g_libGTASA + 0x001AD6F4 + 1))(*(RwCamera**)(g_libGTASA + 0x95B064), 0.2f);
}

VECTOR vecAtSaved;
VECTOR vecUpSaved;
bool bsaved = false;

void CFirstPersonCamera::ProcessCameraInVeh(uintptr_t pCam, CPlayerPed* pPed, CVehicle* pVeh)
{
	if (!m_bEnabled || !pPed->GetGtaVehicle())
	{
		return;
	}

	VECTOR* pVec = (VECTOR*)(pCam + 372);

	VECTOR vecOffset;
	vecOffset.X = 0.0f;
	vecOffset.Y = 0.0f;
	vecOffset.Z = 0.6f;

	uint16_t modelIndex = pPed->GetGtaVehicle()->entity.nModelIndex;

#ifdef GAME_EDITION_CR
	if (modelIndex >= 15072 && modelIndex <= 15080)
	{
		vecOffset.X = 0.05f;
		vecOffset.Y = 0.3f;
		vecOffset.Z = 0.45f;
		((RwCamera * (*)(RwCamera*, float))(g_libGTASA + 0x001AD6F4 + 1))(*(RwCamera * *)(g_libGTASA + 0x95B064), 0.3f);
	}
	else
	{
		((RwCamera * (*)(RwCamera*, float))(g_libGTASA + 0x001AD6F4 + 1))(*(RwCamera * *)(g_libGTASA + 0x95B064), 0.01f);
	}
#else
	if (modelIndex == 581 || modelIndex == 509 || modelIndex == 481 || modelIndex == 462 || modelIndex == 521 || modelIndex == 463 || modelIndex == 510 ||
		modelIndex == 522 || modelIndex == 461 || modelIndex == 468 || modelIndex == 448 || modelIndex == 586)
	{
		vecOffset.X = 0.05f;
		vecOffset.Y = 0.3f;
		vecOffset.Z = 0.45f;
		((RwCamera * (*)(RwCamera*, float))(g_libGTASA + 0x001AD6F4 + 1))(*(RwCamera * *)(g_libGTASA + 0x95B064), 0.3f);
	}
	else
	{
		((RwCamera * (*)(RwCamera*, float))(g_libGTASA + 0x001AD6F4 + 1))(*(RwCamera * *)(g_libGTASA + 0x95B064), 0.01f);
	}
#endif
	if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON_VEH).X != -1)
	{
		vecOffset.X += (float)(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON_VEH).X - 200) / 100.0f;
	}
	if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON_VEH).Y != -1)
	{
		vecOffset.Y += (float)(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIRSTPERSON_VEH).Y - 200) / 100.0f;
	}
	if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_FIRSTPERSON_VEH).Y != -1)
	{
		vecOffset.Z += (float)(CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_FIRSTPERSON_VEH).Y - 200) / 100.0f;
	}

	VECTOR vecOut;
	MATRIX4X4 mat;

	memcpy(&mat, pPed->m_pPed->entity.mat, sizeof(MATRIX4X4));

	RwMatrixMultiplyByVector(&vecOut, &mat, &vecOffset);

	if (vecOut.X != vecOut.X || vecOut.Y != vecOut.Y || vecOut.Z != vecOut.Z)
	{
		pPed->GetBonePosition(4, &vecOut);
	}
	if (vecOut.X != vecOut.X || vecOut.Y != vecOut.Y || vecOut.Z != vecOut.Z)
	{
		return;
	}

	pVec->X = vecOut.X;
	pVec->Y = vecOut.Y;
	pVec->Z = vecOut.Z;

	//pPed->SetArmedWeapon(0);

	if (!pVeh)
	{
		if (!pPed->IsAPassenger())
		{
			*(uint16_t*)(pCam + 14) = 16;
		}
		return;
	}

	if (!pPed->IsAPassenger())
	{
		VECTOR vecSpeed;
		pVeh->GetMoveSpeedVector(&vecSpeed);
		float speed = sqrt((vecSpeed.X * vecSpeed.X) + (vecSpeed.Y * vecSpeed.Y) + (vecSpeed.Z * vecSpeed.Z)) * 2.0f * 100.0f;

		
		
	}
}
#include "..//util/armhook.h"
void CFirstPersonCamera::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
	//UnFuck(g_libGTASA + 0x0037C3B8);
	//*(float*)(g_libGTASA + 0x0037C3E0) = 1.0f;
	//*(float*)(g_libGTASA + 0x0037C3E4) = 1000.0f;
	//*(float*)(g_libGTASA + 0x0037C3E8) = 10.0f;
	//*(float*)(g_libGTASA + 0x0037C3D8) = 1000.0f;
	//*(float*)(g_libGTASA + 0x0037C3C4) = 1.0f;
	//*(float*)
}

void CFirstPersonCamera::Toggle()
{
	m_bEnabled ^= 1;
}

bool CFirstPersonCamera::IsEnabled()
{
	return m_bEnabled;
}
