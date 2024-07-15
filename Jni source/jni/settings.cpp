#include "main.h"
#include "settings.h"
#include "game/game.h"
#include "vendor/ini/config.h"
#include "game/CAdjustableHudColors.h"
#include "game/CAdjustableHudPosition.h"
#include "game/CAdjustableHudScale.h"

static void ClearBackslashN(char* pStr, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (pStr[i] == '\n' || pStr[i] == 13)
		{
			pStr[i] = 0;
		}
	}
}

CSettings::CSettings()
{
	LoadSettings(nullptr);
}

CSettings::~CSettings()
{
}

void CSettings::ToDefaults(int iCategory)
{
	char buff[0x7F];
	sprintf(buff, "%sSAMP/settings.ini", g_pszStorage);

	FILE* pFile = fopen(buff, "w");

	fwrite("[gui]", 1, 6, pFile);

	fclose(pFile);

	Save(iCategory);
	LoadSettings(m_Settings.szNickName, m_Settings.iChatMaxMessages);
}

void CSettings::Save(int iIgnoreCategory)
{
	char buff[0x7F];
	sprintf(buff, "%sSAMP/settings.ini", g_pszStorage);
	remove(buff);

	ini_table_s* config = ini_table_create();
	
	ini_table_create_entry(config, "client", "name", m_Settings.szNickName);
	ini_table_create_entry(config, "client", "password", m_Settings.szPassword);
	ini_table_create_entry_as_int(config, "client", "last_server", m_Settings.last_server);
	ini_table_create_entry(config, "gui", "Font", m_Settings.szFont);

	ini_table_create_entry_as_float(config, "gui", "FontSize", m_Settings.fFontSize);
	ini_table_create_entry_as_int(config, "gui", "FontOutline", m_Settings.iFontOutline);

	if (iIgnoreCategory != 1)
	{
		ini_table_create_entry_as_float(config, "gui", "ChatPosX", m_Settings.fChatPosX);
		ini_table_create_entry_as_float(config, "gui", "ChatPosY", m_Settings.fChatPosY);
	}
	ini_table_create_entry_as_float(config, "gui", "ChatSizeX", m_Settings.fChatSizeX);
	ini_table_create_entry_as_float(config, "gui", "ChatSizeY", m_Settings.fChatSizeY);
	ini_table_create_entry_as_int(config, "gui", "ChatMaxMessages", m_Settings.iChatMaxMessages);

	ini_table_create_entry_as_float(config, "gui", "HealthBarWidth", m_Settings.fHealthBarWidth);
	ini_table_create_entry_as_float(config, "gui", "HealthBarHeight", m_Settings.fHealthBarHeight);

	if (iIgnoreCategory != 2)
	{

		ini_table_create_entry_as_float(config, "gui", "MicrophoneSize", m_Settings.fButtonMicrophoneSize);
		ini_table_create_entry_as_float(config, "gui", "MicrophoneX", m_Settings.fButtonMicrophoneX);
		ini_table_create_entry_as_float(config, "gui", "MicrophoneY", m_Settings.fButtonMicrophoneY);

		ini_table_create_entry_as_float(config, "gui", "CameraCycleSize", m_Settings.fButtonCameraCycleSize);
		ini_table_create_entry_as_float(config, "gui", "CameraCycleX", m_Settings.fButtonCameraCycleX);
		ini_table_create_entry_as_float(config, "gui", "CameraCycleY", m_Settings.fButtonCameraCycleY);
	}

	ini_table_create_entry_as_int(config, "gui", "voicelist", m_Settings.iVoiceList);

	ini_table_create_entry_as_int(config, "gui", "fps", m_Settings.iFPS);

	if (iIgnoreCategory != 1)
	{
		ini_table_create_entry_as_int(config, "gui", "cutout", m_Settings.iCutout);
		ini_table_create_entry_as_int(config, "gui", "androidKeyboard", m_Settings.iAndroidKeyboard);
		ini_table_create_entry_as_int(config, "gui", "fpscounter", m_Settings.iFPSCounter);
		ini_table_create_entry_as_int(config, "gui", "outfit", m_Settings.iOutfitGuns);
		ini_table_create_entry_as_int(config, "gui", "radarrect", m_Settings.iRadarRect);
		ini_table_create_entry_as_int(config, "gui", "hud", m_Settings.iHud);
		//ini_table_create_entry_as_int(config, "gui", "hparmourtext", m_Settings.iHPArmourText);
		//ini_table_create_entry_as_int(config, "gui", "pcmoney", m_Settings.iPCMoney);
		//ini_table_create_entry_as_int(config, "gui", "ctimecyc", m_Settings.iSkyBox);
		//ini_table_create_entry_as_int(config, "gui", "snow", m_Settings.iSnow);
	}

	if (iIgnoreCategory != 2)
	{
		for (int i = 0; i < E_HUD_ELEMENT::HUD_SIZE; i++)
		{
			char buff[30];
			snprintf(buff, sizeof(buff), "hud_color_%d", i);
			if (CAdjustableHudColors::IsUsingHudColor((E_HUD_ELEMENT)i))
			{
				ini_table_create_entry(config, "hud", buff, CAdjustableHudColors::GetHudColorString((E_HUD_ELEMENT)i).c_str());
			}

		}
	}

	if (iIgnoreCategory != 2)
	{
		for (int i = 0; i < E_HUD_ELEMENT::HUD_SIZE-2; i++)
		{
			char buff[30];
			snprintf(buff, sizeof(buff), "hud_position_x_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudPosition::GetElementPosition((E_HUD_ELEMENT)i).X);

			snprintf(buff, sizeof(buff), "hud_position_y_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudPosition::GetElementPosition((E_HUD_ELEMENT)i).Y);

		}

		for (int i = 0; i < E_HUD_ELEMENT::HUD_SIZE-2; i++)
		{
			char buff[30];
			snprintf(buff, sizeof(buff), "hud_scale_x_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudScale::GetElementScale((E_HUD_ELEMENT)i).X);

			snprintf(buff, sizeof(buff), "hud_scale_y_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudScale::GetElementScale((E_HUD_ELEMENT)i).Y);

		}
	}

	if (iIgnoreCategory != 2)
	{
		for (int i = 10; i < E_HUD_ELEMENT::HUD_SIZE; i++)
		{
			char buff[30];
			snprintf(buff, sizeof(buff), "hud_position_x_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudPosition::GetElementPosition((E_HUD_ELEMENT)i).X);

			snprintf(buff, sizeof(buff), "hud_position_y_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudPosition::GetElementPosition((E_HUD_ELEMENT)i).Y);

		}

		for (int i = 10; i < E_HUD_ELEMENT::HUD_SIZE; i++)
		{
			char buff[30];
			snprintf(buff, sizeof(buff), "hud_scale_x_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudScale::GetElementScale((E_HUD_ELEMENT)i).X);

			snprintf(buff, sizeof(buff), "hud_scale_y_%d", i);
			ini_table_create_entry_as_int(config, "hud", buff, CAdjustableHudScale::GetElementScale((E_HUD_ELEMENT)i).Y);

		}
	}

	ini_table_write_to_file(config, buff);
	ini_table_destroy(config);
}

const stSettings& CSettings::GetReadOnly()
{
	return m_Settings;
}

stSettings& CSettings::GetWrite()
{
	return m_Settings;
}

void CSettings::LoadSettings(const char* szNickName, int iChatLines)
{
	char tempNick[40];
	if (szNickName)
	{
		strcpy(tempNick, szNickName);
	}

	Log("Loading settings..");

	char buff[0x7F];
	sprintf(buff, "%sSAMP/settings.ini", g_pszStorage);

	ini_table_s* config = ini_table_create();
	Log("Opening settings: %s", buff);
	if (!ini_table_read_from_file(config, buff))
	{
		Log("Cannot load settings, exiting...");
		std::terminate();
		return;
	}

	snprintf(m_Settings.szNickName, sizeof(m_Settings.szNickName), "__android_%d%d", rand() % 1000, rand() % 1000);
	memset(m_Settings.szPassword, 0, sizeof(m_Settings.szPassword));
	snprintf(m_Settings.szFont, sizeof(m_Settings.szFont), "Arial.ttf");

	const char* szName = ini_table_get_entry(config, "client", "name");
	const char* szPassword = ini_table_get_entry(config, "client", "password");

	const char* szFontName = ini_table_get_entry(config, "gui", "Font");

	if (szName)
	{
		strcpy(m_Settings.szNickName, szName);
	}
	if (szPassword)
	{
		strcpy(m_Settings.szPassword, szPassword);
	}
	if (szFontName)
	{
		strcpy(m_Settings.szFont, szFontName);
	}

	ClearBackslashN(m_Settings.szNickName, sizeof(m_Settings.szNickName));
	ClearBackslashN(m_Settings.szPassword, sizeof(m_Settings.szPassword));
	ClearBackslashN(m_Settings.szFont, sizeof(m_Settings.szFont));

	if (szNickName)
	{
		strcpy(m_Settings.szNickName, tempNick);
	}

	m_Settings.fFontSize = ini_table_get_entry_as_float(config, "gui", "FontSize", 30.0f);
	m_Settings.iFontOutline = ini_table_get_entry_as_int(config, "gui", "FontOutline", 2);

	m_Settings.fChatPosX = ini_table_get_entry_as_float(config, "gui", "ChatPosX", 325.0f);
	m_Settings.fChatPosY = ini_table_get_entry_as_float(config, "gui", "ChatPosY", 25.0f);
	m_Settings.fChatSizeX = ini_table_get_entry_as_float(config, "gui", "ChatSizeX", 1150.0f);
	m_Settings.fChatSizeY = ini_table_get_entry_as_float(config, "gui", "ChatSizeY", 220.0f);
	m_Settings.iChatMaxMessages = ini_table_get_entry_as_int(config, "gui", "ChatMaxMessages", 8);

	m_Settings.fHealthBarWidth = ini_table_get_entry_as_float(config, "gui", "HealthBarWidth", 60.0f);
	m_Settings.fHealthBarHeight = ini_table_get_entry_as_float(config, "gui", "HealthBarHeight", 10.0f);

	m_Settings.fButtonEnterPassengerSize = ini_table_get_entry_as_float(config, "gui", "EnterPassengerSize", 150.0f);
	m_Settings.fButtonEnterPassengerX = ini_table_get_entry_as_float(config, "gui", "EnterPassengerX", 50.0f);
	m_Settings.fButtonEnterPassengerY = ini_table_get_entry_as_float(config, "gui", "EnterPassengerY", 420.0f);

	m_Settings.fButtonMicrophoneSize = ini_table_get_entry_as_float(config, "gui", "MicrophoneSize", 150.0f);
	m_Settings.fButtonMicrophoneX = ini_table_get_entry_as_float(config, "gui", "MicrophoneX", 1450.0f);
	m_Settings.fButtonMicrophoneY = ini_table_get_entry_as_float(config, "gui", "MicrophoneY", 600.0f);

	m_Settings.fButtonCameraCycleX = ini_table_get_entry_as_float(config, "gui", "CameraCycleX", 1810.0f);
	m_Settings.fButtonCameraCycleY = ini_table_get_entry_as_float(config, "gui", "CameraCycleY", 260.0f);
	m_Settings.fButtonCameraCycleSize = ini_table_get_entry_as_float(config, "gui", "CameraCycleSize", 90.0f);

	m_Settings.fScoreBoardSizeX = ini_table_get_entry_as_float(config, "gui", "ScoreBoardSizeX", 1024.0f);
	m_Settings.fScoreBoardSizeY = ini_table_get_entry_as_float(config, "gui", "ScoreBoardSizeY", 768.0f);

	m_Settings.iVoiceList = ini_table_get_entry_as_int(config, "gui", "voicelist", 1);

	m_Settings.iFPS = ini_table_get_entry_as_int(config, "gui", "fps", 60);
	m_Settings.last_server = ini_table_get_entry_as_int(config, "client", "last_server", -1);

	m_Settings.iCutout = ini_table_get_entry_as_int(config, "gui", "cutout", 0);
	m_Settings.iAndroidKeyboard = ini_table_get_entry_as_int(config, "gui", "androidKeyboard", 0);

	m_Settings.iFPSCounter = ini_table_get_entry_as_int(config, "gui", "fpscounter", 1);
	m_Settings.iOutfitGuns = ini_table_get_entry_as_int(config, "gui", "outfit", 1);
	m_Settings.iRadarRect = ini_table_get_entry_as_int(config, "gui", "radarrect", 0);
	m_Settings.iHPArmourText = ini_table_get_entry_as_int(config, "gui", "hparmourtext", 0);
	m_Settings.iPCMoney = ini_table_get_entry_as_int(config, "gui", "pcmoney", 0);
	m_Settings.iSkyBox = ini_table_get_entry_as_int(config, "gui", "ctimecyc", 1);
	m_Settings.iSnow = ini_table_get_entry_as_int(config, "gui", "snow", 1);
	m_Settings.iHud = ini_table_get_entry_as_int(config, "gui", "hud", 1);
	m_Settings.iDialog = ini_table_get_entry_as_int(config, "gui", "dialog", 1);

	for (int i = 0; i < E_HUD_ELEMENT::HUD_SIZE; i++)
	{
		char buff[30];
		snprintf(buff, sizeof(buff), "hud_color_%d", i);
		const char* szInput = ini_table_get_entry(config, "hud", buff);
		if (szInput)
		{
			strcpy(buff, szInput);

			ClearBackslashN(buff, sizeof(buff));

			std::string szTemp(buff + 1);

			CAdjustableHudColors::SetHudColorFromString((E_HUD_ELEMENT)i, szTemp);
		}
		else
		{
			CAdjustableHudColors::SetHudColorFromRGBA((E_HUD_ELEMENT)i, -1, -1, -1, -1);
		}
	}

	for (int i = 0; i < E_HUD_ELEMENT::HUD_SIZE; i++)
	{
		char buff[30];
		snprintf(buff, sizeof(buff), "hud_position_x_%d", i);
		int valueX = ini_table_get_entry_as_int(config, "hud", buff, -1);

		snprintf(buff, sizeof(buff), "hud_position_y_%d", i);
		int valueY = ini_table_get_entry_as_int(config, "hud", buff, -1);

		CAdjustableHudPosition::SetElementPosition((E_HUD_ELEMENT)i, valueX, valueY);

	}

	for (int i = 0; i < E_HUD_ELEMENT::HUD_SIZE; i++)
	{
		char buff[30];
		snprintf(buff, sizeof(buff), "hud_scale_x_%d", i);
		int valueX = ini_table_get_entry_as_int(config, "hud", buff, -1);

		snprintf(buff, sizeof(buff), "hud_scale_y_%d", i);
		int valueY = ini_table_get_entry_as_int(config, "hud", buff, -1);

		CAdjustableHudScale::SetElementScale((E_HUD_ELEMENT)i, valueX, valueY);

	}

	ini_table_destroy(config);
}
