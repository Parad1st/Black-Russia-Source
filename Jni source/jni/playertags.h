#pragma once

struct stVoiceSort
{
	uint32_t m_iVoiceTime;
	PLAYERID m_iPlayerID;
};

class CPlayerTags
{
public:
	CPlayerTags();
	~CPlayerTags();

	void Render();
	void AddChatBubble(PLAYERID playerId, char* szText, uint32_t dwColor, float fDistance, uint32_t dwTime);
	void ResetChatBubble(PLAYERID playerId);
	void DrawChatBubble(PLAYERID playerId, VECTOR* vec, float fDistance);

private:
	ImVec2 HealthBarBDR1;
	ImVec2 HealthBarBDR2;
	ImVec2 HealthBarBG1;
	ImVec2 HealthBarBG2;
	ImVec2 HealthBar1;
	ImVec2 HealthBar2;

	ImColor HealthBarColor;
	ImColor HealthBarBGColor;
	ImColor HealthBarBDRColor;

	RwTexture* m_pAfk_icon;
	RwTexture* m_pVoice_icon;
	RwTexture* m_pKeyboard_icon;

	uint32_t m_dwColors[MAX_PLAYERS];
	float m_fDistance[MAX_PLAYERS];
	float m_fTrueX[MAX_PLAYERS];
	uint32_t m_dwTime[MAX_PLAYERS];
	uint32_t m_dwStartTime[MAX_PLAYERS];
	char* m_pSzText[MAX_PLAYERS];
	char* m_pSzTextWithoutColors[MAX_PLAYERS];
	bool m_bChatBubbleStatus[MAX_PLAYERS];
	int m_iOffset[MAX_PLAYERS];

	uint32_t m_iVoiceTime[MAX_PLAYERS];
	uint32_t m_iLastVoiceTimeUpdated[MAX_PLAYERS];
	PLAYERID m_iPlayerIDs[MAX_PLAYERS];

	void Draw(VECTOR* vec, char* szNick, uint32_t dwColor, 
	float fDist, float fHealth, float fArmour, bool bAfk, bool bVoice, bool bKeyboard);
};