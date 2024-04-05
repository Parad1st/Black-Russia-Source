#pragma once
#include "font.h"
#include <string>

class CFirstPersonCamera
{
	static bool m_bEnabled;
public:
	static void ProcessCameraOnFoot(uintptr_t pCam, CPlayerPed* pPed);
	static void ProcessCameraInVeh(uintptr_t pCam, CPlayerPed* pPed, CVehicle* pVeh);

	static void SetEnabled(bool bEnabled);
	static void Toggle();
	static bool IsEnabled();
};