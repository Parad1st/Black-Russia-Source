#include "main.h"

#include "gui/gui.h"
#include "game/game.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboardhistory.h"
#include "keyboard.h"

extern CGUI* pGUI;
extern CKeyBoard* pKeyBoard;

CKeyBoardHistory::CKeyBoardHistory()
{
	Log("Initalizing KeyBoardHistory..");
	m_iCounter = 0;
}

CKeyBoardHistory::~CKeyBoardHistory()
{
}


void CKeyBoardHistory::AddStringToHistory(const std::string& msg)
{
	if (msg.size() == 0) return;
	m_Buffer.insert(m_Buffer.begin(), msg);
	while (m_Buffer.size() >= 20)
	{
		m_Buffer.pop_back();
	}
}

void CKeyBoardHistory::PageUp()
{
	if (!m_Buffer.size())
	{
		m_iCounter = 0;
		return;
	}
	m_iCounter++;
	if (m_iCounter >= m_Buffer.size() + 1)
	{
		m_iCounter = m_Buffer.size();
		return;
	}
	pKeyBoard->Flush();
	AddTextToBuffer(m_Buffer[m_iCounter - 1]);
}

void CKeyBoardHistory::PageDown()
{
	m_iCounter--;
	if (m_iCounter <= 0)
	{
		m_iCounter = 0;
		pKeyBoard->Flush();
		return;
	}
	pKeyBoard->Flush();
	AddTextToBuffer(m_Buffer[m_iCounter - 1]);
}

void CKeyBoardHistory::ResetPointer()
{
	m_iCounter = 0;
}

void CKeyBoardHistory::AddTextToBuffer(const std::string& msg)
{
	if (pKeyBoard)
	{
		for (int i = 0; i < msg.size(); i++)
			pKeyBoard->AddCharToInput((char)msg[i]);
	}
}