#pragma once
#include <vector>
#include "vendor/raknet/SingleProducerConsumer.h"
#define MAX_MESSAGE_LENGTH	144
#define MAX_LINE_LENGTH		MAX_MESSAGE_LENGTH / 2

#define NEW_MESSAGE_PUSH_TIME	500 // in ms

#define MAX_TIME_CHAT_HIDE	10000	// in ms
#define TIME_CHAT_HIDE_BEGIN	5000	// in ms

enum eChatMessageType
{
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT,
	CHAT_TYPE_INFO,
	CHAT_TYPE_DEBUG
};

typedef struct
{
	eChatMessageType	eType;
	char 				utf8Message[MAX_MESSAGE_LENGTH * 3 + 1];
	char 				szNick[MAX_PLAYER_NAME + 1];
	uint32_t			dwTextColor;
	uint32_t 			dwNickColor;
} CHAT_WINDOW_ENTRY;

struct BUFFERED_COMMAND_CHAT
{
	int type;
	char buff[300];
};

class CChatWindow
{
	friend class CGUI;
public:
	CChatWindow();
	~CChatWindow();

	void AddChatMessage(char* szNick, uint32_t dwNickColor, char* szMessage);
	void AddInfoMessage(char* szFormat, ...);
	void Opencc();
	void AddDebugMessage(char* szFormat, ...);
	void AddDebugMessageNonFormatted(char* szStr);
	void AddClientMessage(uint32_t dwColor, char* szStr);
	void SetLowerBound(int bound);

protected:
	void Render();
	bool OnTouchEvent(int type, bool multi, int x, int y);

public:
	void RenderText(const char* u8Str, float posX, float posY, uint32_t dwColor, float alphaNewMessage);
	void SetChatDissappearTimeout(uint32_t uiTimeoutStart, uint32_t uiTimeoutEnd);

	void ProcessPushedCommands();
private:
	void ReInit();
	void FilterInvalidChars(char* szString);
	void AddToChatWindowBuffer(eChatMessageType eType, char* szString, char* szNick, 
		uint32_t dwTextColor, uint32_t dwNickColor);
	void PushBack(CHAT_WINDOW_ENTRY &entry);

	uint8_t GetAlphaFromLastTimePushedMessage();

private:
	
	uint32_t m_uiMaxTimeChatHide;
	uint32_t m_uiTimeChatHideBegin;

	float m_fChatPosX, m_fChatPosY;
	float m_fChatSizeX, m_fChatSizeY;

	int m_iMaxMessages;
	std::vector<CHAT_WINDOW_ENTRY*> m_vChatWindowEntries;

	uint32_t m_dwTextColor;
	uint32_t m_dwInfoColor;
	uint32_t m_dwDebugColor;

	uint32_t m_uiLastTimePushedMessage;

	bool m_bNewMessage;
	uint32_t m_uiTimePushed;

	float m_fPosBeforeBoundChat;
	float m_fOffsetBefore;
public:
	bool m_bPendingReInit;
	DataStructures::SingleProducerConsumer< BUFFERED_COMMAND_CHAT> bufferedChat;
};