#include "../main.h"
#include "game.h"
#include "CAdjustableHudColors.h"
#include "..//chatwindow.h"

#include <sstream>
#include <iomanip>

CRGBA CAdjustableHudColors::m_Colors[E_HUD_ELEMENT::HUD_SIZE];
bool CAdjustableHudColors::m_bUseColors[E_HUD_ELEMENT::HUD_SIZE];

void CAdjustableHudColors::SetHudColorFromString(E_HUD_ELEMENT id, std::string szColor)
{
	if (id < E_HUD_ELEMENT::HUD_HP || id >= E_HUD_ELEMENT::HUD_SIZE)
	{
		return;
	}

	std::istringstream converter(szColor);
	uint32_t dwColor;
	converter >> std::hex >> dwColor;

	m_Colors[id].B = dwColor & 0xFF; dwColor >>= 8;
	m_Colors[id].G = dwColor & 0xFF; dwColor >>= 8;
	m_Colors[id].R = dwColor & 0xFF; dwColor >>= 8;
	m_Colors[id].A = dwColor & 0xFF;

	m_bUseColors[id] = true;
}
#include "..//chatwindow.h"
extern CChatWindow* pChatWindow;
void CAdjustableHudColors::SetHudColorFromRGBA(E_HUD_ELEMENT id, int r, int g, int b, int a)
{
	if (id < E_HUD_ELEMENT::HUD_HP || id >= E_HUD_ELEMENT::HUD_SIZE)
	{
		return;
	}

	if (r == -1 || g == -1 || b == -1 || a == -1)
	{
		m_bUseColors[id] = false;
		m_Colors[id].R = 0;
		m_Colors[id].G = 0;
		m_Colors[id].B = 0;
		m_Colors[id].A = 0;
		return;
	}

	m_Colors[id].R = r;
	m_Colors[id].G = g;
	m_Colors[id].B = b;
	m_Colors[id].A = a;

	m_bUseColors[id] = true;
}

bool CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT id)
{
	if (id < E_HUD_ELEMENT::HUD_HP || id >= E_HUD_ELEMENT::HUD_SIZE)
	{
		return false;
	}
	return m_bUseColors[id];
}

CRGBA CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT id)
{
	if (id < E_HUD_ELEMENT::HUD_HP || id >= E_HUD_ELEMENT::HUD_SIZE)
	{
		return CRGBA();
	}
	return m_Colors[id];
}

std::string CAdjustableHudColors::GetHudColorString(E_HUD_ELEMENT id)
{
	if (id < E_HUD_ELEMENT::HUD_HP || id >= E_HUD_ELEMENT::HUD_SIZE)
	{
		return std::string("");
	}

	char buff[30];
	snprintf(buff, sizeof(buff), "#%02x%02x%02x%02x", m_Colors[id].A, m_Colors[id].R, m_Colors[id].G, m_Colors[id].B);
	
	return std::string(buff);
}
