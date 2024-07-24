#pragma once
#include "game/RW/RenderWare.h"

class CGButton
{
public:
	CGButton();
	~CGButton();

	void RenderButton();
	void LoadTextureButton();
	
	RwTexture* p_GButtonTexture;
private:
	float ScaleX(float x) { return m_vecScale.x * x; }
	float ScaleY(float y) { return m_vecScale.y * y; }
	ImVec2		m_vecScale;
};

