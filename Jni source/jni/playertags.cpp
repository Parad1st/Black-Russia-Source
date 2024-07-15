#include "main.h"
#include "game/game.h"
#include "game/RW/RenderWare.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "playertags.h"
#include "settings.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CGUI *pGUI;
extern CSettings *pSettings;

CPlayerTags::CPlayerTags()
{
	Log("Loading afk_icon..");
	m_pAfk_icon = (RwTexture*)LoadTextureFromDB("samp", "afk_icon");
	m_pVoice_icon = (RwTexture*)LoadTextureFromDB("samp", "micro_icon");
#ifdef GAME_EDITION_CR
	m_pKeyboard_icon = (RwTexture*)LoadTextureFromDB("samp", "keyboard_icon");
#else
	m_pKeyboard_icon = nullptr;
#endif
	HealthBarBDRColor = ImColor( 0x00, 0x00, 0x00, 0xFF );

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_bChatBubbleStatus[i] = 0;
		m_pSzText[i] = nullptr;
		m_pSzTextWithoutColors[i] = nullptr;
		m_iVoiceTime[i] = 0;
		m_iLastVoiceTimeUpdated[i] = 0;
	}
}

CPlayerTags::~CPlayerTags() {}
#include "voice/CVoiceChatClient.h"
extern CVoiceChatClient* pVoice;
// допилить
extern bool g_bShowVoiceList;

static stVoiceSort aVoiceSortTime[MAX_PLAYERS];
#include <algorithm>
void CPlayerTags::Render()
{
	VECTOR VecPos;
	MATRIX4X4 matLocal, matPlayer;
	int dwHitEntity;
	char szNickBuf[50];
	int iVoiceCounter = 0;
	ImVec2 basePos = ImVec2(pGUI->ScaleX(10.0f), pGUI->ScaleY(600.0f));
	if(pNetGame && pNetGame->m_bShowPlayerTags)
	{
		CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
		pGame->FindPlayerPed()->GetMatrix(&matLocal);

		for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
		{
			aVoiceSortTime[playerId].m_iPlayerID = playerId;
			aVoiceSortTime[playerId].m_iVoiceTime = 999999;
			if(pPlayerPool->GetSlotState(playerId) == true)
			{
				CRemotePlayer* pPlayer = pPlayerPool->GetAt(playerId);

				if(pPlayer && pPlayer->IsActive() && pPlayer->m_bShowNameTag)
				{
					CPlayerPed* pPlayerPed = pPlayer->GetPlayerPed();
					
					if (m_bChatBubbleStatus[playerId])
					{
						if (!pPlayerPed)
						{
							ResetChatBubble(playerId);
							continue;
						}
						if (pPlayerPed->GetDistanceFromCamera() <= m_fDistance[playerId])
						{
							if (!pPlayerPed->IsAdded()) continue;
							VecPos.X = 0.0f;
							VecPos.Y = 0.0f;
							VecPos.Z = 0.0f;
							pPlayerPed->GetBonePosition(8, &VecPos);
							DrawChatBubble(playerId, &VecPos, pPlayerPed->GetDistanceFromCamera());
						}
						if (GetTickCount() - m_dwStartTime[playerId] >= m_dwTime[playerId])
						{
							ResetChatBubble(playerId);
						}
					}
					
					if(pPlayerPed->GetDistanceFromCamera() <= pNetGame->m_fNameTagDrawDistance)
					{

						{
							if(!pPlayerPed->IsAdded()) continue;
							VecPos.X = 0.0f;
							VecPos.Y = 0.0f;
							VecPos.Z = 0.0f;
							pPlayerPed->GetBonePosition(8, &VecPos);
						}

						CAMERA_AIM *pCam = GameGetInternalAim();
						dwHitEntity = 0;

						if(pNetGame->m_bNameTagLOS)
						{
							dwHitEntity = ScriptCommand(&get_line_of_sight, 
								VecPos.X, VecPos.Y, VecPos.Z,
								pCam->pos1x, pCam->pos1y, pCam->pos1z,
								1, 0, 0, 1, 0);
						}

						if (!pNetGame->m_bNameTagLOS || dwHitEntity)
						{
							if (pVoice)
							{
								bool bVoice = (GetTickCount() - pVoice->m_aLastPushTime[playerId] <= 500);

								sprintf(szNickBuf, "%s (%d)", pPlayerPool->GetPlayerName(playerId), playerId);
								Draw(&VecPos, szNickBuf,
									pPlayer->GetPlayerColor(),
									pPlayerPed->GetDistanceFromCamera(),
									pPlayer->m_fReportedHealth,
									pPlayer->m_fReportedArmour,
									pPlayer->IsAFK(), bVoice, pPlayer->m_bKeyboardOpened);

								if (bVoice)
								{
									if (m_iVoiceTime[playerId] == 0 || m_iVoiceTime[playerId] == 999999)
									{
										m_iVoiceTime[playerId] = 1000 + (rand() % 20);
										m_iLastVoiceTimeUpdated[playerId] = GetTickCount();
										aVoiceSortTime[playerId].m_iVoiceTime = m_iVoiceTime[playerId];
									}
									else
									{
										m_iVoiceTime[playerId] += GetTickCount() - m_iLastVoiceTimeUpdated[playerId];
										aVoiceSortTime[playerId].m_iVoiceTime = m_iVoiceTime[playerId];
										m_iLastVoiceTimeUpdated[playerId] = GetTickCount();
									}
								}
								else
								{
									m_iVoiceTime[playerId] = 999999;
									aVoiceSortTime[playerId].m_iVoiceTime = m_iVoiceTime[playerId];
									m_iLastVoiceTimeUpdated[playerId] = 0;
								}
							}
							else
							{
								sprintf(szNickBuf, "%s (%d)", pPlayerPool->GetPlayerName(playerId), playerId);
								Draw(&VecPos, szNickBuf,
									pPlayer->GetPlayerColor(),
									pPlayerPed->GetDistanceFromCamera(),
									pPlayer->m_fReportedHealth,
									pPlayer->m_fReportedArmour,
									pPlayer->IsAFK(), 0, pPlayer->m_bKeyboardOpened);

							}

						}
					}
				}
			}
		}
		int n = sizeof(aVoiceSortTime) / sizeof(aVoiceSortTime[0]);

		struct {
			bool operator()(stVoiceSort a, stVoiceSort b)
			{
				return a.m_iVoiceTime < b.m_iVoiceTime;
			}
		} customLess;

		std::sort(aVoiceSortTime, aVoiceSortTime + n, customLess);
		for (int i = 0; i <= 6; i++)
		{
			if (!pGame->IsToggledHUDElement(HUD_ELEMENT_VOICE)) continue;
			if (!g_bShowVoiceList) continue;
			if (aVoiceSortTime[i].m_iVoiceTime >= 999990) break;
			if (pPlayerPool->GetSlotState(aVoiceSortTime[i].m_iPlayerID) == false) continue;
			sprintf(szNickBuf, "%s (%d)", pPlayerPool->GetPlayerName(aVoiceSortTime[i].m_iPlayerID), aVoiceSortTime[i].m_iPlayerID);
			basePos.y += pGUI->GetFontSize() * 1.1f;
			ImVec2 iconPos = basePos;
			iconPos.x += ImGui::CalcTextSize(szNickBuf).x + (pGUI->GetFontSize() * 0.6f);
			if (m_pVoice_icon)
			{
				ImVec2 a = ImVec2(iconPos.x, iconPos.y);
				ImVec2 b = ImVec2(iconPos.x + pGUI->GetFontSize(), iconPos.y + pGUI->GetFontSize());
				ImGui::GetOverlayDrawList()->AddImage((ImTextureID)m_pVoice_icon->raster, a, b);
			}

			pGUI->RenderText(basePos, 0xFFFFFFFF, true, szNickBuf);

			iVoiceCounter++;
		}
	}
}

bool ProcessInlineHexColor(const char* start, const char* end, ImVec4& color);

void TextWithColors(ImVec2 pos, ImColor col, const char* szStr, const char* szStrWithoutColors = nullptr);

void FilterColors(char* szStr);

void CPlayerTags::AddChatBubble(PLAYERID playerId, char* szText, uint32_t dwColor, float fDistance, uint32_t dwTime)
{
	if (m_bChatBubbleStatus[playerId])
	{
		ResetChatBubble(playerId);
		m_dwColors[playerId] = dwColor;
		m_fDistance[playerId] = fDistance;
		m_dwTime[playerId] = dwTime;
		m_dwStartTime[playerId] = GetTickCount();
		m_bChatBubbleStatus[playerId] = 1;
		m_fTrueX[playerId] = -1.0f;
		cp1251_to_utf8(m_pSzText[playerId], szText);
		cp1251_to_utf8(m_pSzTextWithoutColors[playerId], szText);
		FilterColors(m_pSzTextWithoutColors[playerId]);
		const char* pText = m_pSzTextWithoutColors[playerId];
		m_iOffset[playerId] = 0;
		while (*pText)
		{
			if (*pText == '\n')
			{
				m_iOffset[playerId]++;
			}
			pText++;
		}
		return;
	}
	m_dwColors[playerId] = dwColor;
	m_fDistance[playerId] = fDistance;
	m_dwTime[playerId] = dwTime;
	m_dwStartTime[playerId] = GetTickCount();
	m_bChatBubbleStatus[playerId] = 1;
	m_fTrueX[playerId] = -1.0f;
	m_pSzText[playerId] = new char[1024];
	m_pSzTextWithoutColors[playerId] = new char[1024];
	cp1251_to_utf8(m_pSzText[playerId], szText);
	cp1251_to_utf8(m_pSzTextWithoutColors[playerId], szText);
	FilterColors(m_pSzTextWithoutColors[playerId]);
	const char* pText = m_pSzTextWithoutColors[playerId];
	m_iOffset[playerId] = 0;
	while (*pText)
	{
		if (*pText == '\n')
		{
			m_iOffset[playerId]++;
		}
		pText++;
	}
}

void CPlayerTags::ResetChatBubble(PLAYERID playerId)
{
	if (m_bChatBubbleStatus[playerId])
	{
		m_dwTime[playerId] = 0;
	}
	m_bChatBubbleStatus[playerId] = 0;
}

void CPlayerTags::DrawChatBubble(PLAYERID playerId, VECTOR* vec, float fDistance)
{
	VECTOR TagPos;

	TagPos.X = vec->X;
	TagPos.Y = vec->Y;
	TagPos.Z = vec->Z;
	TagPos.Z += 0.45f + (fDistance * 0.0675f) + ((float)m_iOffset[playerId] * pGUI->ScaleY(0.35f));

	VECTOR Out;
	// CSprite::CalcScreenCoors
	((void (*)(VECTOR*, VECTOR*, float*, float*, bool, bool))(g_libGTASA + 0x54EEC0 + 1))(&TagPos, &Out, 0, 0, 0, 0);

	if (Out.Z < 1.0f)
		return;

	// name (id)
	ImVec2 pos = ImVec2(Out.X, Out.Y);

	if (m_fTrueX[playerId] < 0)
	{
		char* curBegin = m_pSzTextWithoutColors[playerId];
		char* curPos = m_pSzTextWithoutColors[playerId];
		while (*curPos != '\0')
		{
			if (*curPos == '\n')
			{
				float width = ImGui::CalcTextSize(curBegin, (char*)(curPos - 1)).x;
				if (width > m_fTrueX[playerId])
				{
					m_fTrueX[playerId] = width;
				}

				curBegin = curPos + 1;
			}

			curPos++;
		}

		if (m_fTrueX[playerId] < 0)
		{
			m_fTrueX[playerId] = ImGui::CalcTextSize(m_pSzTextWithoutColors[playerId]).x;
		}

		//Log("m_fTrueX = %f", m_pTextLabels[x]->m_fTrueX);
	}

	pos.x -= (m_fTrueX[playerId] / 2);

	TextWithColors(pos, __builtin_bswap32(m_dwColors[playerId]), m_pSzText[playerId]);
}

void CPlayerTags::Draw(VECTOR* vec, char* szName, uint32_t dwColor,
	float fDist, float fHealth, float fArmour, bool bAfk, bool bVoice, bool bKeyboard)
{
	if (!pGame->IsToggledHUDElement(HUD_ELEMENT_TAGS)) return;
	VECTOR TagPos;

	TagPos.X = vec->X;
	TagPos.Y = vec->Y;
	TagPos.Z = vec->Z;
	TagPos.Z += 0.25f + (fDist * 0.0475f);

	VECTOR Out;
	// CSprite::CalcScreenCoors
	(( void (*)(VECTOR*, VECTOR*, float*, float*, bool, bool))(g_libGTASA+0x54EEC0+1))(&TagPos, &Out, 0, 0, 0, 0);

	if(Out.Z < 1.0f)
		return;

	char tempBuff[300];
	cp1251_to_utf8(tempBuff, szName);

	// name (id)
	ImVec2 pos = ImVec2(Out.X, Out.Y);
	pos.x -= ImGui::CalcTextSize(tempBuff).x/2;


	pGUI->RenderText(pos, __builtin_bswap32(dwColor | (0x000000FF)), true, tempBuff);

	// Health Bar
	if(fHealth < 0.0f) return;

	// округляем
	Out.X = (float)((int)Out.X);
	Out.Y = (float)((int)Out.Y);

	HealthBarColor = ImColor( 0xB9, 0x22, 0x28, 0xFF );
	HealthBarBGColor = ImColor( 0x4B, 0x0B, 0x14, 0xFF );

	float fWidth = pGUI->ScaleX( pSettings->GetReadOnly().fHealthBarWidth );
	float fHeight = pGUI->ScaleY( pSettings->GetReadOnly().fHealthBarHeight );
	float fOutline = (float)pSettings->GetReadOnly().iFontOutline;

	// top left
	HealthBarBDR1.x = Out.X - ((fWidth/2) + fOutline);
	HealthBarBDR1.y = Out.Y + (pGUI->GetFontSize()*1.2f);//35.0f;
	// bottom right
	HealthBarBDR2.x = Out.X + ((fWidth/2) + fOutline);
	HealthBarBDR2.y = Out.Y + (pGUI->GetFontSize()*1.2f) + fHeight;//48.0f;

	// top left
	HealthBarBG1.x = HealthBarBDR1.x + fOutline;//Out.X - 40.0f;
	HealthBarBG1.y = HealthBarBDR1.y + fOutline;//Out.Y + 37.0f;
	// bottom right
	HealthBarBG2.x = HealthBarBDR2.x - fOutline;//Out.X + 40.0f;
	HealthBarBG2.y = HealthBarBDR2.y - fOutline;//Out.Y + 46.0f;

	// top left
	HealthBar1.x = HealthBarBG1.x;//Out.X - 40.0f;
	HealthBar1.y = HealthBarBG1.y;//Out.Y + 37.0f;
	// bottom right
	HealthBar2.y = HealthBarBG2.y;//Out.Y + 46.0f;

	if (fHealth > 100.0f)
		fHealth = 100.0f;

	fHealth *= fWidth/100.0f;
	fHealth -= (fWidth/2);
	HealthBar2.x = Out.X + fHealth;

	if(fArmour > 0.0f)
	{
		HealthBarBDR1.y += 13.0f;
		HealthBarBDR2.y += 13.0f;
		HealthBarBG1.y += 13.0f;
		HealthBarBG2.y += 13.0f;
		HealthBar1.y += 13.0f;
		HealthBar2.y += 13.0f;
	}

	ImGui::GetOverlayDrawList()->AddRectFilled(HealthBarBDR1, HealthBarBDR2, HealthBarBDRColor);
	ImGui::GetOverlayDrawList()->AddRectFilled(HealthBarBG1, HealthBarBG2, HealthBarBGColor);
	ImGui::GetOverlayDrawList()->AddRectFilled(HealthBar1, HealthBar2, HealthBarColor);

	// Armour Bar
	if(fArmour > 0.0f)
	{
		HealthBarBDR1.y -= 13.0f;
		HealthBarBDR2.y -= 13.0f;
		HealthBarBG1.y -= 13.0f;
		HealthBarBG2.y -= 13.0f;
		HealthBar1.y -= 13.0f;
		HealthBar2.y -= 13.0f;

		HealthBarColor = ImColor(200, 200, 200, 255);
		HealthBarBGColor = ImColor(40, 40, 40, 255);

		if(fArmour > 100.0f)
			fArmour = 100.0f;

		fArmour *= fWidth/100.0f;
		fArmour -= (fWidth/2);
		HealthBar2.x = Out.X + fArmour;
		ImGui::GetOverlayDrawList()->AddRectFilled(HealthBarBDR1, HealthBarBDR2, HealthBarBDRColor);
		ImGui::GetOverlayDrawList()->AddRectFilled(HealthBarBG1, HealthBarBG2, HealthBarBGColor);
		ImGui::GetOverlayDrawList()->AddRectFilled(HealthBar1, HealthBar2, HealthBarColor);
	}

	// AFK Icon
	if(bAfk)
	{
		ImVec2 a = ImVec2(HealthBarBDR1.x - (pGUI->GetFontSize()*1.4f), HealthBarBDR1.y);
		ImVec2 b = ImVec2(a.x + (pGUI->GetFontSize()*1.3f), a.y + (pGUI->GetFontSize()*1.3f));
		ImGui::GetOverlayDrawList()->AddImage((ImTextureID)m_pAfk_icon->raster, a, b);
	}

	VECTOR TagPos_voice;

	TagPos_voice.X = vec->X;
	TagPos_voice.Y = vec->Y;
	TagPos_voice.Z = vec->Z;
	TagPos_voice.Z += 0.65f + (fDist * 0.0475f);

	VECTOR Out_voice;
	// CSprite::CalcScreenCoors
	((void (*)(VECTOR*, VECTOR*, float*, float*, bool, bool))(g_libGTASA + 0x54EEC0 + 1))(&TagPos_voice, &Out_voice, 0, 0, 0, 0);

	if (Out_voice.Z < 1.0f)
		return;

	if (bVoice && m_pVoice_icon)
	{
		ImVec2 a = ImVec2(Out_voice.X - (((pGUI->GetFontSize() * 1.3f) / 2.0f) * 1.5f), Out_voice.Y);
		ImVec2 b = ImVec2(Out_voice.X + (((pGUI->GetFontSize() * 1.3f) / 2.0f) * 1.5f), Out_voice.Y + ((pGUI->GetFontSize() * 1.3f)));
		ImGui::GetOverlayDrawList()->AddImage((ImTextureID)m_pVoice_icon->raster, a, b);
	}

#ifdef GAME_EDITION_CR
	if (bKeyboard && m_pKeyboard_icon)
	{
		ImVec2 a = ImVec2(pos.x + ImGui::CalcTextSize(szName).x + pGUI->GetFontSize() * 0.5f, pos.y);
		ImVec2 b = ImVec2(a.x + (pGUI->GetFontSize() * 1.3f), a.y + (pGUI->GetFontSize() * 1.3f));
		ImGui::GetOverlayDrawList()->AddImage((ImTextureID)m_pKeyboard_icon->raster, a, b);
	}
#endif
}
