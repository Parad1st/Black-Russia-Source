#pragma once

#include <stdint.h>
#include "game/common.h"

#define DEBUG_INFO_ENABLED

class CDebugInfo
{
	static uint32_t m_uiDrawDebug;
	static uint32_t m_dwSpeedMode;
	static uint32_t m_uiDrawFPS;

	static uint32_t m_dwSpeedStart;
public:
	static uint32_t uiStreamedPeds;
	static uint32_t uiStreamedVehicles;
	
	static void ToggleDebugDraw();

	static void SetDrawFPS(uint32_t bDraw);

	static void Draw();
	static void ApplyDebugPatches();

	static void ToggleSpeedMode();
	static void ProcessSpeedMode(VECTOR* pVecSpeed);
};

