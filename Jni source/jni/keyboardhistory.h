#pragma once

#include "gui/gui.h"
#include "game/game.h"


class CKeyBoardHistory
{
	friend class CGUI;
public:

	CKeyBoardHistory();
	~CKeyBoardHistory();

	void AddStringToHistory(const std::string& msg);

	void PageUp();
	void PageDown();
	void ResetPointer();
	int m_iCounter;
private:
	void AddTextToBuffer(const std::string& msg);
	std::vector<std::string> m_Buffer;
};