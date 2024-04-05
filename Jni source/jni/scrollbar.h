#pragma once
class CScrollbar
{
public:
	int m_iMaxValue;

	float m_fX;
	float m_fY;

	float m_fWidthBox;
	float m_fHeightBox;

	float m_fUpY;
	float m_fDownY;

	int m_iValue;
	bool m_bDrawState;

	bool OnTouchEvent(int type, bool multi, int x, int y);
	bool IsClickedOnBar(int x, int y);
	void Draw();

	int GetValue() const;
	void SetOnPos(int iPos);
	CScrollbar();
	~CScrollbar();
};

extern CScrollbar* pScrollbar;
