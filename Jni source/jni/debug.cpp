#include "debug.h"

#include "main.h"
#include "gui/gui.h"
#include "game/game.h"
#include "net/netgame.h"
#include "game/RW/RenderWare.h"
#include "chatwindow.h"
#include "playertags.h"
#include "dialog.h"
#include "keyboard.h"
#include "settings.h"
#include "util/armhook.h"

extern CGUI* pGUI;
extern CChatWindow* pChatWindow;

uint32_t CDebugInfo::uiStreamedPeds = 0;
uint32_t CDebugInfo::uiStreamedVehicles = 0;
uint32_t CDebugInfo::m_uiDrawDebug = 0;
uint32_t CDebugInfo::m_uiDrawFPS = 0;
uint32_t CDebugInfo::m_dwSpeedMode = 0;
uint32_t CDebugInfo::m_dwSpeedStart = 0;

void CDebugInfo::ToggleDebugDraw()
{
	m_uiDrawDebug ^= 1;
}

void CDebugInfo::SetDrawFPS(uint32_t bDraw)
{
	m_uiDrawFPS = bDraw;
}

void CDebugInfo::Draw()
{
	char szStr[30];
	ImVec2 pos;
	if (m_uiDrawDebug || m_uiDrawFPS)
	{
		float* pFPS = (float*)(g_libGTASA + 0x00608E00);
		snprintf(&szStr[0], 30, "%.1f", *pFPS);
		pos = ImVec2(pGUI->ScaleX(20.0f), pGUI->ScaleY(1080.0f - pGUI->GetFontSize()));

		pGUI->RenderText(pos, (ImU32)0xFFFFFFFF, true, &szStr[0]);
	}

#ifdef DEBUG_INFO_ENABLED

	if (!m_uiDrawDebug)
	{
		return;
	}
	uint32_t msUsed = *(uint32_t*)(g_libGTASA + 0x0067067C);
	uint32_t msAvailable = *(uint32_t*)(g_libGTASA + 0x005DE734);
	snprintf(&szStr[0], 30, "%d/%d %f %d %d", msUsed, msAvailable, (float)((float)msUsed/(float)msAvailable), uiStreamedPeds, uiStreamedVehicles);
	pos = ImVec2(pGUI->ScaleX(20.0f), pGUI->ScaleY(1080.0f - pGUI->GetFontSize()));

	pGUI->RenderText(pos, (ImU32)0xFFFFFFFF, true, &szStr[0]);

#endif
}

void CDebugInfo::ApplyDebugPatches()
{
#ifdef DEBUG_INFO_ENABLED

	UnFuck(g_libGTASA + 0x008B8018);
	*(uint8_t*)(g_libGTASA + 0x008B8018) = 1;
	NOP(g_libGTASA + 0x00399EDA, 2);
	NOP(g_libGTASA + 0x00399F46, 2);
	NOP(g_libGTASA + 0x00399F92, 2);

#endif
}

void CDebugInfo::ToggleSpeedMode()
{
	m_dwSpeedMode ^= 1;
	if (m_dwSpeedMode)
	{
		pChatWindow->AddDebugMessage("Speed mode enabled");
	}
	else
	{
		pChatWindow->AddDebugMessage("Speed mode disabled");
	}
}

void CDebugInfo::ProcessSpeedMode(VECTOR* pVecSpeed)
{
	if (!m_dwSpeedMode)
	{
		return;
	}
	static uint32_t m_dwState = 0;
	float speed = sqrt((pVecSpeed->X * pVecSpeed->X) + (pVecSpeed->Y * pVecSpeed->Y) + (pVecSpeed->Z * pVecSpeed->Z)) * 2.0f * 100.0f;
	if (speed >= 1.0f)
	{
		if (!m_dwSpeedStart)
		{
			m_dwSpeedStart = GetTickCount();
			m_dwState = 0;
			pChatWindow->AddDebugMessage("Start");
		}
		if ((speed >= 119.0f) && (speed <= 121.0f) && (m_dwState == 0))
		{
			pChatWindow->AddDebugMessage("1 to 100: %d", GetTickCount() - m_dwSpeedStart);
			m_dwSpeedStart = GetTickCount();
			m_dwState = 1;
		}
		if ((speed >= 230.0f) && (speed <= 235.0f) && (m_dwState == 1))
		{
			pChatWindow->AddDebugMessage("100 to 200: %d", GetTickCount() - m_dwSpeedStart);
			m_dwSpeedStart = 0;
			m_dwState = 0;
		}
		// process for 100 and 200
	}
	else
	{
		if (m_dwSpeedStart)
		{
			m_dwSpeedStart = 0;
			m_dwState = 0;
			pChatWindow->AddDebugMessage("Reseted");
			return;
		}
	}
}
