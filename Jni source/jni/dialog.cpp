#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "dialog.h"
#include "scoreboard.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboard.h"
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>

extern CGUI* pGUI;
extern CGame* pGame;
extern CNetGame* pNetGame;
extern CKeyBoard* pKeyBoard;
extern CScoreBoard* pScoreBoard;

char szDialogInputBuffer[100];
char utf8DialogInputBuffer[100 * 3];

CDialogWindow::CDialogWindow()
{
	m_bIsActive = false;
	m_putf8Info = nullptr;
	m_pszInfo = nullptr;
}
ImVec2 CalcTextSizeWithoutTags(char* szStr)
{
	if (!szStr) return ImVec2(0, 0);

	char szNonColored[2048 + 1];
	int iNonColoredMsgLen = 0;

	for (int pos = 0; pos < strlen(szStr) && szStr[pos] != '\0'; pos++)
	{
		if (pos + 7 < strlen(szStr))
		{
			if (szStr[pos] == '{' && szStr[pos + 7] == '}')
			{
				pos += 7;
				continue;
			}
		}

		szNonColored[iNonColoredMsgLen] = szStr[pos];
		iNonColoredMsgLen++;
	}

	szNonColored[iNonColoredMsgLen] = 0;

	return ImGui::CalcTextSize(szNonColored);
}
CDialogWindow::~CDialogWindow()
{

}

void CDialogWindow::Show(bool bShow)
{
	if (pNetGame)
	{
		if (pGame && pGame->FindPlayerPed()
			&& !pNetGame->GetTextDrawPool()->m_bSelectState)
			//if(!pGame->FindPlayerPed()->GetVehicleSeatID()) 
			pGame->FindPlayerPed()->TogglePlayerControllable(!bShow);
	}

	m_bIsActive = bShow;
}

void CDialogWindow::Clear()
{
	if (m_putf8Info)
	{
		free(m_putf8Info);
		m_putf8Info = nullptr;
	}

	if (m_pszInfo)
	{
		free(m_pszInfo);
		m_pszInfo = nullptr;
	}

	m_iSelectedItem = 0;
	m_bIsActive = false;
	m_byteDialogStyle = 0;
	m_wDialogID = -1;
	m_utf8Title[0] = 0;
	m_utf8Button1[0] = 0;
	m_utf8Button2[0] = 0;

	m_fSizeX = -1.0f;
	m_fSizeY = -1.0f;

	memset(szDialogInputBuffer, 0, 100);
	memset(utf8DialogInputBuffer, 0, 100 * 3);
}

void CDialogWindow::SetInfo(char* szInfo, int length)
{
	if (!szInfo || !length) return;

	if (m_putf8Info)
	{
		free(m_putf8Info);
		m_putf8Info = nullptr;
	}

	if (m_pszInfo)
	{
		free(m_pszInfo);
		m_pszInfo = nullptr;
	}

	m_putf8Info = (char*)malloc((length * 3) + 1);
	if (!m_putf8Info)
	{
		Log("CDialog::SetInfo: can't allocate memory");
		return;
	}

	m_pszInfo = (char*)malloc((length * 3) + 1);
	if (!m_pszInfo)
	{
		Log("CDialog::SetInfo: can't allocate memory");
		return;
	}

	cp1251_to_utf8(m_putf8Info, szInfo);

	// =========
	char szNonColorEmbeddedMsg[4200];
	int iNonColorEmbeddedMsgLen = 0;

	for (size_t pos = 0; pos < strlen(szInfo) && szInfo[pos] != '\0'; pos++)
	{
		if (pos + 7 < strlen(szInfo))
		{
			if (szInfo[pos] == '{' && szInfo[pos + 7] == '}')
			{
				pos += 7;
				continue;
			}
		}

		szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = szInfo[pos];
		iNonColorEmbeddedMsgLen++;
	}

	szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = 0;
	// ========

	cp1251_to_utf8(m_pszInfo, szNonColorEmbeddedMsg);
}

bool ProcessInlineHexColor(const char* start, const char* end, ImVec4& color);

void TextWithColors(const char* fmt, ...)
{
	char tempStr[4096];

	va_list argPtr;
	va_start(argPtr, fmt);
	vsnprintf(tempStr, sizeof(tempStr), fmt, argPtr);
	va_end(argPtr);
	tempStr[sizeof(tempStr) - 1] = '\0';

	bool pushedColorStyle = false;
	const char* textStart = tempStr;
	const char* textCur = tempStr;
	while (textCur < (tempStr + sizeof(tempStr)) && *textCur != '\0')
	{
		if (*textCur == '{')
		{
			// Print accumulated text
			if (textCur != textStart)
			{
				ImGui::TextUnformatted(textStart, textCur);
				ImGui::SameLine(0.0f, 0.0f);
			}

			// Process color code
			const char* colorStart = textCur + 1;
			do
			{
				++textCur;
			} while (*textCur != '\0' && *textCur != '}');

			// Change color
			if (pushedColorStyle)
			{
				ImGui::PopStyleColor();
				pushedColorStyle = false;
			}

			ImVec4 textColor;
			if (ProcessInlineHexColor(colorStart, textCur, textColor))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, textColor);
				pushedColorStyle = true;
			}

			textStart = textCur + 1;
		}
		else if (*textCur == '\n')
		{
			// Print accumulated text an go to next line
			ImGui::TextUnformatted(textStart, textCur);
			textStart = textCur + 1;
		}

		++textCur;
	}

	if (textCur != textStart)
		ImGui::TextUnformatted(textStart, textCur);
	else
		ImGui::NewLine();

	if (pushedColorStyle)
		ImGui::PopStyleColor();
}

void DialogWindowInputHandler(const char* str)
{
	if (!str || *str == '\0') return;
	strcpy(szDialogInputBuffer, str);
	cp1251_to_utf8(utf8DialogInputBuffer, str);
}

ImVec2 CalcTextSizeWithoutTags(char*);

std::string removeColorTags(std::string line)
{
	std::string string;
	for (uint32_t it = 0; it < line.size(); it++)
	{
		if (it + 7 < line.size())
		{
			if (line[it] == '{' && line[it + 7] == '}')
			{
				it += 7;
				continue;
			}
		}
		string.push_back(line[it]);
	}
	return string;
}
#include "chatwindow.h"
extern CChatWindow* pChatWindow;
#include "clientlogic/CNetwork.h"
#include "settings.h"
extern CSettings* pSettings;
void CDialogWindow::RenderTabList(int dStyle)
{
	std::string strUtf8 = m_putf8Info;
	int size = strUtf8.length();
	int iTabsCount = 0;
	std::vector<std::string> vLines;
	std::vector<std::string> firstTabs;
	ImVec2 fTabSize[4];

	int tmplineid = 0;
	std::stringstream ssLine(strUtf8);
	std::string tmpLine;
	while (std::getline(ssLine, tmpLine, '\n'))
	{
		vLines.push_back(tmpLine);
		int tmpTabId = 0;
		if (dStyle != DIALOG_STYLE_LIST)
		{
			std::stringstream ssTabLine(tmpLine);
			std::string tmpTabLine;
			while (std::getline(ssTabLine, tmpTabLine, '\t'))
			{
				if (tmpTabId >= 4) continue;
				if (vLines.size() == 1 && iTabsCount <= 4)
					iTabsCount++;
				ImVec2 tabSize;
				tabSize = CalcTextSizeWithoutTags((char*)tmpTabLine.c_str());

				if (tmpTabId == 0)
					firstTabs.push_back(removeColorTags(tmpTabLine));


				if (tabSize.x > fTabSize[tmpTabId].x)
				{
					fTabSize[tmpTabId].x = tabSize.x;
				}
				tmpTabId++;

			}
		}
		else
		{
			ImVec2 tabSize;
			tabSize = CalcTextSizeWithoutTags((char*)tmpLine.c_str());

			if (tmpTabId == 0)
				firstTabs.push_back(tmpLine);

			if (tabSize.x > fTabSize[tmpTabId].x)
			{
				fTabSize[tmpTabId].x = tabSize.x;
			}
		}
		tmplineid++;
	}
	if (iTabsCount == 1 && dStyle == DIALOG_STYLE_TABLIST)
		dStyle = DIALOG_STYLE_LIST;

	ImGuiIO& io = ImGui::GetIO();
	int x = io.DisplaySize.x, y = io.DisplaySize.y;

	float buttonFactor = pGUI->ScaleX(187.5f) * 2 + pGUI->GetFontSize();

	ImVec2 vecWinSize;
	if (dStyle != DIALOG_STYLE_LIST)
	{
		for (uint8_t i = 0; i < iTabsCount; i++)
		{
			vecWinSize.x += fTabSize[i].x * (i == iTabsCount - 1 ? 1.25 : 1.5);
		}
	}
	else vecWinSize.x += fTabSize[0].x * 1.25;
	if (buttonFactor > vecWinSize.x)
	{
		vecWinSize.x = 0.0f;

		if (dStyle != DIALOG_STYLE_LIST)
		{
			buttonFactor /= iTabsCount;
			for (uint8_t i = 0; i < iTabsCount; i++)
			{
				vecWinSize.x += fTabSize[i].x + buttonFactor;
			}
		}
		else vecWinSize.x += fTabSize[0].x + buttonFactor;
	}
	else
		buttonFactor = 0;

	vecWinSize.y = 720;

	if (vLines.size() > 7)
		vecWinSize.x += ImGui::GetStyle().ScrollbarSize;

	ImGui::Begin(" ", nullptr, ImVec2(pGUI->ScaleX(vecWinSize.x + pGUI->GetFontSize()), pGUI->ScaleY(vecWinSize.y)), 0.9f, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); //  
	m_bRendered = true;
	TextWithColors(m_utf8Title);
	ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2 + 2.75f));

	ImVec2 cursonPosition;
	cursonPosition = ImGui::GetCursorPos();
	if (dStyle == DIALOG_STYLE_TABLIST_HEADERS)
	{
		ImGui::Columns(iTabsCount, "###tablistHeader", false);
		for (uint16_t i = 0; i < iTabsCount; i++)
		{
			ImGui::SetColumnWidth(-1, fTabSize[i].x * (i == iTabsCount - 1 ? 1.25 : 1.5) + buttonFactor);
			ImGui::NextColumn();
		}
		std::stringstream ssTabLine(vLines[0]);
		std::string tmpTabLine;
		int tmpTabId = 0;
		while (std::getline(ssTabLine, tmpTabLine, '\t'))
		{
			if (tmpTabId > iTabsCount) continue;
			tmpTabLine.insert(0, "{a9c4e4}");
			if (tmpTabId == 0)
			{
				cursonPosition = ImGui::GetCursorPos();
				ImGui::SetCursorPosX(cursonPosition.x + pGUI->GetFontSize() / 3);
			}
			TextWithColors(tmpTabLine.c_str());
			ImGui::NextColumn();
			tmpTabId++;

		}
		ImGui::Columns(1);
	}
	ImGui::BeginChild("###tablist", ImVec2(-1, pGUI->ScaleY(vecWinSize.y) - pGUI->ScaleY(95.0f * 2)));

	if (dStyle != DIALOG_STYLE_LIST)
	{
		ImGui::Columns(iTabsCount, "###tablistColumn", false);
		for (uint16_t i = 0; i < iTabsCount; i++)
		{
			ImGui::SetColumnWidth(-1, fTabSize[i].x * (i == iTabsCount - 1 ? 1.25 : 1.5) + buttonFactor);
			ImGui::NextColumn();
		}
	}
	for (uint32_t line = dStyle == DIALOG_STYLE_TABLIST_HEADERS ? 1 : 0; line < vLines.size(); line++)
	{
		std::stringstream ssTabLine(vLines[line]);
		std::string tmpTabLine;
		int tmpTabId = 0;

		ImVec2 differentOffsets;
		if (tmpTabId == 0)
		{
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = (ImVec4)ImColor(119, 4, 4, 255);
			ImGui::GetStyle().Colors[ImGuiCol_Header] = ImGui::GetStyle().Colors[ImGuiCol_HeaderActive];
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImGui::GetStyle().Colors[ImGuiCol_HeaderActive];

			ImGui::PushID(tmpTabId + line);

			std::stringstream ss;
			ss << line + tmpTabId;
			std::string s = ss.str();

			std::string itemid = "##" + s;
			bool is_selected = (m_iSelectedItem == line);

			ImVec2 cursonPosition;
			cursonPosition = ImGui::GetCursorPos();

			if (ImGui::Selectable(itemid.c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
				if (ImGui::IsMouseDoubleClicked(0))
				{
					Show(false);
					if (pNetGame)
						pNetGame->SendDialogResponse(m_wDialogID, 1, m_iSelectedItem, (char*)firstTabs[m_iSelectedItem].c_str());
					else
					{
						pNetGame = new CNetGame(
							g_sEncryptedAddresses[m_iSelectedItem].decrypt(),
							g_sEncryptedAddresses[m_iSelectedItem].getPort(),
							pSettings->GetReadOnly().szNickName,
							pSettings->GetReadOnly().szPassword);
					}

				}
			if (ImGui::IsItemHovered())
			{
				m_iSelectedItem = line;
			}
			ss.clear();
			ss << line + tmpTabId + 1;
			s = ss.str();
			itemid = "##" + s;
			if (ImGui::Selectable(itemid.c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
				if (ImGui::IsMouseDoubleClicked(0))
				{
					Show(false);
					if (pNetGame)
						pNetGame->SendDialogResponse(m_wDialogID, 1, m_iSelectedItem, (char*)firstTabs[m_iSelectedItem].c_str());
					else
					{
						pNetGame = new CNetGame(
							g_sEncryptedAddresses[m_iSelectedItem].decrypt(),
							g_sEncryptedAddresses[m_iSelectedItem].getPort(),
							pSettings->GetReadOnly().szNickName,
							pSettings->GetReadOnly().szPassword);
					}


				}
			if (ImGui::IsItemHovered())
			{
				m_iSelectedItem = line;
			}
			ImVec2 cursonPositionLast;
			cursonPositionLast = ImGui::GetCursorPos();

			differentOffsets.x = cursonPositionLast.x - cursonPosition.x;
			differentOffsets.y = cursonPositionLast.y - cursonPosition.y;

			if (is_selected) ImGui::SetItemDefaultFocus();

			if (dStyle != DIALOG_STYLE_LIST)
				ImGui::SameLine();

		}
		if (dStyle != DIALOG_STYLE_LIST)
		{
			int icolumnsLeft = iTabsCount;
			while (std::getline(ssTabLine, tmpTabLine, '\t'))
			{
				if (tmpTabId > iTabsCount) continue;

				ImVec2 cursonPos;
				cursonPos = ImGui::GetCursorPos();

				if (tmpTabId == 0)
				{
					if (line == m_iSelectedItem)
						m_strSelectedItemText = tmpTabLine;
					ImGui::SetCursorPos(ImVec2(cursonPos.x, cursonPos.y - differentOffsets.y / 4));
				}
				else
					ImGui::SetCursorPos(ImVec2(cursonPos.x, cursonPos.y + differentOffsets.y / 4));

				tmpTabLine.insert(0, "{ffffff}");
				TextWithColors((char*)tmpTabLine.c_str());
				ImGui::SetCursorPos(ImVec2(cursonPos.x, cursonPos.y + differentOffsets.y / 2));

				if (dStyle != DIALOG_STYLE_LIST)
				{
					ImGui::NextColumn();
					icolumnsLeft--;
				}
				tmpTabId++;

			}
			if (dStyle != DIALOG_STYLE_LIST && icolumnsLeft >= 0)
			{
				for (int i = 0; i < icolumnsLeft; i++)
				{
					ImGui::NextColumn();
				}
			}
		}
		else
		{
			ImVec2 cursonPos;
			cursonPos = ImGui::GetCursorPos();

			if (line == m_iSelectedItem)
				m_strSelectedItemText = vLines[line];
			ImGui::SetCursorPos(ImVec2(cursonPos.x, cursonPos.y - differentOffsets.y + pGUI->GetFontSize() / 2));

			vLines[line].insert(0, "{ffffff}");
			TextWithColors((char*)vLines[line].c_str());
			ImGui::SetCursorPos(ImVec2(cursonPos.x, cursonPos.y));

		}
	}

	if (dStyle != DIALOG_STYLE_LIST)
		ImGui::Columns(1);
	ImGui::EndChild();

	if (m_utf8Button1[0] != 0 && m_utf8Button2[0] != 0)
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - pGUI->ScaleX(417.0f) + ImGui::GetStyle().ItemSpacing.x) / 2);
	else
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - pGUI->ScaleX(230.0f) + ImGui::GetStyle().ItemSpacing.x) / 2);

	if (m_utf8Button1[0] != 0)
	{
		if (ImGui::Button(m_utf8Button1, ImVec2(pGUI->ScaleX(193.5f), pGUI->ScaleY(75.0f))))
		{
			Show(false);
			uint8_t TempItem;
			if (dStyle == DIALOG_STYLE_TABLIST_HEADERS && !m_iSelectedItem)
				TempItem = 0;
			else TempItem = m_iSelectedItem - 1;
			if (pNetGame)
				pNetGame->SendDialogResponse(m_wDialogID, 1, dStyle == DIALOG_STYLE_TABLIST_HEADERS ? TempItem : m_iSelectedItem, (char*)firstTabs[m_iSelectedItem].c_str());
			else
			{
				pNetGame = new CNetGame(
					g_sEncryptedAddresses[m_iSelectedItem].decrypt(),
					g_sEncryptedAddresses[m_iSelectedItem].getPort(),
					pSettings->GetReadOnly().szNickName,
					pSettings->GetReadOnly().szPassword);
			}

		}
	}
	ImGui::SameLine(0, pGUI->GetFontSize());
	if (m_utf8Button2[0] != 0)
	{
		if (ImGui::Button(m_utf8Button2, ImVec2(pGUI->ScaleX(193.5f), pGUI->ScaleY(75.0f))))
		{
			Show(false);
			uint8_t TempItem;
			if (dStyle == DIALOG_STYLE_TABLIST_HEADERS && !m_iSelectedItem)
				TempItem = 0;
			else TempItem = m_iSelectedItem - 1;
			if (pNetGame)
				pNetGame->SendDialogResponse(m_wDialogID, 0, dStyle == DIALOG_STYLE_TABLIST_HEADERS ? TempItem : m_iSelectedItem, (char*)firstTabs[m_iSelectedItem].c_str());
			else
			{
				pNetGame = new CNetGame(
					g_sEncryptedAddresses[m_iSelectedItem].decrypt(),
					g_sEncryptedAddresses[m_iSelectedItem].getPort(),
					pSettings->GetReadOnly().szNickName,
					pSettings->GetReadOnly().szPassword);
			}
		}
	}
	ImGui::SetWindowSize(ImVec2(vecWinSize.x, pGUI->ScaleY(vecWinSize.y + dStyle == DIALOG_STYLE_TABLIST_HEADERS ? pGUI->ScaleY(187.5f) / 2 : 0)));
	ImVec2 winsize = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - winsize.x) / 2), ((io.DisplaySize.y - winsize.y) / 2)));
	//

	ImGui::End();

}

void CDialogWindow::Render()
{
	m_bRendered = false;
	if (!m_bIsActive || !m_putf8Info) return;

	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPosCenter();

	if (m_byteDialogStyle == DIALOG_STYLE_TABLIST_HEADERS || m_byteDialogStyle == DIALOG_STYLE_TABLIST || m_byteDialogStyle == DIALOG_STYLE_LIST)
		return RenderTabList(m_byteDialogStyle);

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(pGUI->GetFontSize(), pGUI->GetFontSize()));

	ImGui::Begin(" ", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	m_bRendered = true;

	TextWithColors(m_utf8Title);
	ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2 + 2.75f));

	switch (m_byteDialogStyle)
	{
	case DIALOG_STYLE_MSGBOX:
		TextWithColors(m_putf8Info);
		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2));
		break;

	case DIALOG_STYLE_INPUT:
		TextWithColors(m_putf8Info);
		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2));

		if (ImGui::Button(utf8DialogInputBuffer, ImVec2(ImGui::CalcTextSize(m_pszInfo).x, pGUI->GetFontSize() * 1.5f)))
		{
			if (!pKeyBoard->IsOpen())
				pKeyBoard->Open(&DialogWindowInputHandler, false);
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2));
		break;

	case DIALOG_STYLE_PASSWORD:
		TextWithColors(m_putf8Info);
		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2 + 10));
		char _utf8DialogInputBuffer[100 * 3 + 1];
		strcpy(_utf8DialogInputBuffer, utf8DialogInputBuffer);

		for (int i = 0; i < strlen(_utf8DialogInputBuffer); i++)
		{
			if (_utf8DialogInputBuffer[i] == '\0')
				break;
			_utf8DialogInputBuffer[i] = '*';
		}

		if (ImGui::Button(_utf8DialogInputBuffer, ImVec2(ImGui::CalcTextSize(m_pszInfo).x + pGUI->GetFontSize(), pGUI->GetFontSize() * 1.5f)))
		{
			if (!pKeyBoard->IsOpen())
				pKeyBoard->Open(&DialogWindowInputHandler, true);
		}
		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize() / 2 + 5));
		break;
	}

	if (m_utf8Button1[0] != 0 && m_utf8Button2[0] != 0)
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - pGUI->ScaleX(417.0f) + ImGui::GetStyle().ItemSpacing.x) / 2);
	else
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - pGUI->ScaleX(230.0f) + ImGui::GetStyle().ItemSpacing.x) / 2);

	if (m_utf8Button1[0] != 0)
	{
		if (ImGui::Button(m_utf8Button1, ImVec2(pGUI->ScaleX(187.5f), pGUI->ScaleY(75.0f))))
		{
			Show(false);
			if (pNetGame)
				pNetGame->SendDialogResponse(m_wDialogID, 1, 0, szDialogInputBuffer);
		}
	}
	ImGui::SameLine(0, pGUI->GetFontSize());
	if (m_utf8Button2[0] != 0)
	{
		if (ImGui::Button(m_utf8Button2, ImVec2(pGUI->ScaleX(187.5f), pGUI->ScaleY(75.0f))))
		{
			Show(false);
			if (pNetGame)
				pNetGame->SendDialogResponse(m_wDialogID, 0, -1, szDialogInputBuffer);
		}
	}

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - size.x) / 2), ((io.DisplaySize.y - size.y) / 2)));
	ImGui::End();

	//ImGui::PopStyleVar();
}