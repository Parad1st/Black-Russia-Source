#pragma once
#include "../vendor/imgui/imgui.h"
#include "..//vendor/raknet/SingleProducerConsumer.h"

enum eTouchType
{
	TOUCH_POP = 1,
	TOUCH_PUSH = 2,
	TOUCH_MOVE = 3
};

struct BUFFERED_COMMAND_TEXTDRAW
{
	uint16_t textdrawId;
};

class CGUI
{
public:
	CGUI();
	~CGUI();

	void Render();
                  void RenderGUI();
                  void RenderPosition();
                  void RenderServer();
	void ShowSpeed();
	void SetHealth(float fHealth);
	int GetHealth();
	int bHealth;
	void SetEngine(int engine);
	int bEngine;
	void SetLights(int lights);
	int bLights;
	void SetDoor(int door);
	int bDoor;
	static void CoordsRadar(CRect* rect); // тут мне кажется лучше будет static void чем просто void (сам думай).
	void SetMeliage(float meliage);
                  int bMeliage = 0;
                  void SetEat(float eat);
                  int GetEat();
                  int eat;
                  void SetFuel(float fuel);
	int m_fuel;

	float ScaleX(float x) { return m_vecScale.x * x; }
	float ScaleY(float y) { return m_vecScale.y * y; }
	ImFont* GetFont() { return m_pFont; }
	ImFont* GetSampFont() { return m_pSampFont; }
	float GetFontSize() { return m_fFontSize; }

	bool OnTouchEvent(int type, bool multi, int x, int y);

	void RenderText(ImVec2& pos, ImU32 col, bool bOutline, const char* text_begin, const char* text_end = nullptr);
	void RenderTextForChatWindow(ImVec2& pos, ImU32 col, bool bOutline, const char* text_begin, const char* text_end = nullptr);

	void PushToBufferedQueueTextDrawPressed(uint16_t textdrawId);
private:
	void PreProcessInput();
	void PostProcessInput();
	void RenderRakNetStatistics();
	void RenderVersion();
	void ProcessPushedTextdraws();


private:
	DataStructures::SingleProducerConsumer<BUFFERED_COMMAND_TEXTDRAW> m_BufferedCommandTextdraws;

	struct RwTexture* m_pSplashTexture;
	ImFont* 	m_pFont;
	ImFont*		m_pSampFont;

	ImVec2		m_vecScale;
	float 		m_fFontSize;

	bool		m_bNeedClearMousePos;
	bool		m_bMouseDown;
	ImVec2		m_vTouchPos;
	bool		m_bNextClear;

	bool 		m_bKeysStatus;
};