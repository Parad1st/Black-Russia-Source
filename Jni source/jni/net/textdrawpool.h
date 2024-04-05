#pragma once

/*

SA:MP Multiplayer Modification
Copyright 2004-2005 SA:MP Team

Version: $Id: textdrawpool.h,v 1.3 2008-02-15 07:20:36 kyecvs Exp $

*/

//----------------------------------------------------

#include "..//gui/ITouchListener.h"

class CTextDrawPool : public ITouchListener
{
private:

	CTextDraw* m_pTextDraw[MAX_TEXT_DRAWS];
	bool			m_bSlotState[MAX_TEXT_DRAWS];

public:
	bool			m_bSelectState;

	CTextDrawPool();
	~CTextDrawPool();
	void SetSelectState(bool bState, uint32_t dwColor);
	CTextDraw* New(uint16_t wText, TEXT_DRAW_TRANSMIT* TextDrawTransmit, char* szText);
	void Delete(uint16_t wText);
	void Draw();
	bool OnTouchEvent(int type, int numClick, int posX, int posY) override;

	void SnapshotProcess();

	CTextDraw* GetAt(uint16_t wText) {
		if (wText >= MAX_TEXT_DRAWS) return NULL;
		if (!m_bSlotState[wText]) return NULL;
		return m_pTextDraw[wText];
	};
};

//----------------------------------------------------
