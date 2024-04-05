#include "scrollbar.h"
#include "main.h"
#include "gui/gui.h"
#include "game/game.h"

bool CScrollbar::OnTouchEvent(int type, bool multi, int x, int y)
{
	float fX = (float)x;
	float fY = (float)y;

	static float fClickY;
	static bool bMove = false;
	if (!m_bDrawState)
	{
		return true;
	}
	switch (type)
	{
	case TOUCH_PUSH:
		if (!( (fX >= m_fX) && 
			(fX <= m_fX + m_fWidthBox) && 
			(fY >= m_fY) && 
			(fY <= m_fY + m_fHeightBox) ) )
		{
			bMove = false;
			return true;
		}
		fClickY = fY;
		bMove = true;

		return false;

	case TOUCH_MOVE:
	{
		if (!bMove) return true;
		m_fY -= fClickY - y;

		fClickY = y;

		if (m_fY + m_fHeightBox > m_fDownY)
		{
			m_fY = m_fDownY - m_fHeightBox;
		}
		if (m_fY < m_fUpY)
		{
			m_fY = m_fUpY;
		}

		float fRelativeY = m_fY - m_fUpY;
		float m_fDistance = m_fDownY - m_fUpY - m_fHeightBox;
		if (m_fDistance == 0.0f)
		{
			return false;
		}
		float m_fValue = fRelativeY / m_fDistance;
		m_iValue = (int)((m_fValue * (100.0f / (float)m_iMaxValue)) * 100.0f);
		return false;
	}
		

	case TOUCH_POP:
		
		bMove = false;
		break;
	}
	return true;
}

bool CScrollbar::IsClickedOnBar(int x, int y)
{
	return (x >= m_fX && x <= m_fX + m_fWidthBox && y >= m_fY && y <= m_fY + m_fHeightBox);
}

void CScrollbar::Draw()
{
	if (!m_bDrawState)
	{
		return;
	}
	ImVec2 a(m_fX, m_fY);
	ImVec2 b(m_fX + m_fWidthBox, m_fY + m_fHeightBox);
	ImColor co(0.0f, 0.5f, 1.0f, 0.8f);
	ImGui::GetOverlayDrawList()->AddRectFilled(a, b, co, 0.0f);
}

int CScrollbar::GetValue() const
{
	return m_iValue;
}

void CScrollbar::SetOnPos(int iPos)
{
	float fPerc = (float)iPos / (float)m_iMaxValue;
	float fDistance = m_fDownY - m_fUpY - m_fHeightBox;
	float fValue = fPerc * fDistance;
	m_fY = m_fUpY + fValue;
	m_iValue = iPos;
}

CScrollbar::CScrollbar()
{
	m_bDrawState = false;
	m_iValue = 0;
}

CScrollbar::~CScrollbar()
{
	m_bDrawState = false;
}

CScrollbar* pScrollbar = nullptr;