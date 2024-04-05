#pragma once
#include <cstdint>

// 0 state - not pressed
// 1 state - touched(not released)
// 2 state - touched(released)

class CWidget
{
	CSprite2d* m_pSprite;
	float x, y, w, h;
	int m_iState;
	uint32_t m_dwLastTickClicked;
	bool m_bShouldBeDrawn;
	bool m_bColoured;
	CRGBA color;
public:
	CWidget(const char* szTexture);
	~CWidget();

	void SetDrawState(bool bState);

	void SetWidth(float w);
	void SetHeight(float h);
	void SetPos(float x, float y);
	void SetPosWithoutScale(float x, float y);
	void SetTexture(const char* szDB, const char* szTexture);
	void Draw();
	bool IsXYInRect(float x, float y);

	void SetColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
	void ResetColor();

	void SetState(int iState);
	int GetState() const;
};