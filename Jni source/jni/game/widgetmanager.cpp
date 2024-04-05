#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include <cmath>
#include "../gui/gui.h"
extern CGame *pGame;
extern CNetGame *pNetGame;

CWidgetManager::CWidgetManager()
{
	for (size_t i = 0; i < MAX_WIDGETS; i++)
	{
		m_pWidgets[i] = nullptr;
		m_bWidgetStates[i] = false;
	}
}

CWidgetManager::~CWidgetManager()
{
	for (size_t i = 0; i < MAX_WIDGETS; i++)
	{
		if (m_pWidgets[i] && m_bWidgetStates[i])
		{
			delete m_pWidgets[i];
			m_bWidgetStates[i] = false;
		}
	}
}

void CWidgetManager::New(int iID, float x, float y, float w, float h, const char* szTexture)
{
	if (iID < 0 || iID >= MAX_WIDGETS) return;
	if (m_bWidgetStates[iID])
	{
		Delete(iID);
	}
	m_pWidgets[iID] = new CWidget(szTexture);
	m_pWidgets[iID]->SetPos(x, y);
	m_pWidgets[iID]->SetHeight(h);
	m_pWidgets[iID]->SetWidth(w);
	m_bWidgetStates[iID] = true;
}

void CWidgetManager::Delete(int iID)
{
	if (iID < 0 || iID >= MAX_WIDGETS) return;
	if (m_bWidgetStates[iID])
	{
		m_bWidgetStates[iID] = false;
		delete m_pWidgets[iID];
	}
}

CWidget* CWidgetManager::GetWidget(int iID)
{
	if (iID < 0 || iID > MAX_WIDGETS)
	{
		return nullptr;
	}
	return m_pWidgets[iID];
}

#include "../gui/gui.h"

void CWidgetManager::OnTouchEvent(int type, int num, int x, int y)
{

	int id = 0;

	while (id != MAX_WIDGETS)
	{
		if (m_bWidgetStates[id])
		{
			m_pWidgets[id]->SetState(0);
			switch (type)
			{
			case TOUCH_PUSH:
				if (m_pWidgets[id]->IsXYInRect(x, y))
					m_pWidgets[id]->SetState(1);
				break;

			case TOUCH_POP:
				if (m_pWidgets[id]->IsXYInRect(x, y))
				{
					m_pWidgets[id]->SetState(2);
					return;
				}
				break;

			case TOUCH_MOVE:
				if (m_pWidgets[id]->IsXYInRect(x, y))
					m_pWidgets[id]->SetState(1);
				break;
			}
		}
		id++;
	}
}

void CWidgetManager::OnTouchEventSingle(int iID, int type, int num, int x, int y)
{
	if (m_bWidgetStates[iID])
	{
		m_pWidgets[iID]->SetState(0);
		switch (type)
		{
		case TOUCH_PUSH:
			if (m_pWidgets[iID]->IsXYInRect(x, y))
				m_pWidgets[iID]->SetState(1);
			break;

		case TOUCH_POP:
			if (m_pWidgets[iID]->IsXYInRect(x, y))
			{
				m_pWidgets[iID]->SetState(2);
				return;
			}
			break;

		case TOUCH_MOVE:
			if (m_pWidgets[iID]->IsXYInRect(x, y))
				m_pWidgets[iID]->SetState(1);
			break;
		}
	}
}

void CWidgetManager::Draw()
{
	for (size_t i = 0; i < MAX_WIDGETS; i++)
	{
		if (m_pWidgets[i] && m_bWidgetStates[i])
		{
			m_pWidgets[i]->Draw();
		}
	}
}

bool CWidgetManager::IsTouched(int iID)
{
	if (iID < 0 || iID >= MAX_WIDGETS) return false;
	return (m_pWidgets[iID]->GetState() == 2);
}

bool CWidgetManager::GetSlotState(int iID)
{
	return m_bWidgetStates[iID];
}

CWidgetManager* g_pWidgetManager = nullptr;