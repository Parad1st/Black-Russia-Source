#pragma once
#include "font.h"
#include <string>

enum E_HUD_ELEMENT
{
	HUD_HP,
	HUD_ARMOR,
	HUD_MONEY,
	HUD_WANTED,
	HUD_HP_TEXT,
	HUD_ARMOR_TEXT,
	HUD_RADAR,
	HUD_CHAT,
	HUD_FIST,
	HUD_AMMO,
	HUD_FIRSTPERSON,
	HUD_FIRSTPERSON_VEH,
	HUD_WEAPONSPOS,
	HUD_WEAPONSROT,
	HUD_SNOW,
	HUD_SIZE
};

class CAdjustableHudColors
{
	static CRGBA m_Colors[E_HUD_ELEMENT::HUD_SIZE];
	static bool m_bUseColors[E_HUD_ELEMENT::HUD_SIZE];
public:
	static void SetHudColorFromString(E_HUD_ELEMENT id, std::string szColor);
	static void SetHudColorFromRGBA(E_HUD_ELEMENT id, int r, int g, int b, int a);
	static bool IsUsingHudColor(E_HUD_ELEMENT id);
	static CRGBA GetHudColor(E_HUD_ELEMENT id);
	static std::string GetHudColorString(E_HUD_ELEMENT id);
};