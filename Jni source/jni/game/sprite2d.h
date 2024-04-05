#pragma once

class CSprite2d
{
public:
	struct RwTexture* m_pRwTexture;
	void Draw(float x, float y, float width, float height, CRGBA& color);

	~CSprite2d();
};