#include "../main.h"
#include "game.h"
#include "CInfoBarText.h"
#include "..//chatwindow.h"
#include "font.h"
#include "..//gui/gui.h"

bool CInfoBarText::m_bIsEnabled = false;

extern CGUI* pGUI;

void CInfoBarText::Draw(int fHP, int fArmour)
{
	if (!m_bIsEnabled || !pGUI)
	{
		return;
	}

	CRGBA color = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP_TEXT);
	uint32_t black = 0;
	uint32_t t1 = color.ToInt();
	CFont::SetColor(&t1);
	CFont::SetProportional(0);
	CFont::SetBackground(0, 0);
	CFont::SetOrientation(2);
	Font_SetRightJustifyWrap(0);
	CFont::SetFontStyle(2);
	CFont::SetDropShadowPosition(0);
	CFont::SetDropColor(&black);
	CFont::SetEdge(1);

	char buff[30];
	snprintf(buff, sizeof(buff), "%dHP", fHP);

	CFont::SetScale((float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_HP_TEXT).X / 100.0f, 0);
	CFont::PrintString(pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP_TEXT).X),
		pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP_TEXT).Y), buff);

	color = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR_TEXT);
	t1 = color.ToInt();
	CFont::SetColor(&t1);
	CFont::SetProportional(0);
	CFont::SetBackground(0, 0);
	CFont::SetOrientation(2);
	Font_SetRightJustifyWrap(0);
	CFont::SetFontStyle(2);
	CFont::SetDropShadowPosition(0);
	CFont::SetDropColor(&black);
	CFont::SetEdge(1);

	snprintf(buff, sizeof(buff), "%d", fArmour);

	CFont::SetScale((float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_ARMOR_TEXT).X / 100.0f, 0);
	CFont::PrintString(pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR_TEXT).X),
		pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR_TEXT).Y), buff);

}

void CInfoBarText::SetEnabled(bool bEnable)
{
	m_bIsEnabled = bEnable;
}
