#pragma once
#include <cstdint>

#define MAX_WIDGETS	32

#define WIDGET_CHATHISTORY_UP	1
#define WIDGET_CHATHISTORY_DOWN	2
#define WIDGET_MICROPHONE		3
#define WIDGET_CAMERA_CYCLE		4

class CWidgetManager
{
	CWidget* m_pWidgets[MAX_WIDGETS];
	bool m_bWidgetStates[MAX_WIDGETS];
public:
	CWidgetManager();
	~CWidgetManager();

	void New(int iID, float x, float y, float w, float h, const char* szTexture);
	void Delete(int iID);

	CWidget* GetWidget(int iID);

	void Draw();
	void OnTouchEvent(int type, int num, int posX, int posY);
	void OnTouchEventSingle(int iID, int type, int num, int posX, int posY);
	bool IsTouched(int iID);

	bool GetSlotState(int iID);
};

extern CWidgetManager* g_pWidgetManager;