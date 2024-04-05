#pragma once

#include <stdint.h>

enum E_MSG : uint32_t
{
	CONNECTING,
	CONNECTED,
	SERVER_CLOSED_CONNECTION,
	MODIFIED_FILES,
	NOT_FROM_LAUNCHER,
	BANNED,
	CONNECTION_LOST,
	CONNECTION_ATTEMPT_FAILED,
	FULL_SERVER,
	VOICE_MUTED,

	MSG_COUNT
};

#define MAX_LOCALISATION_LENGTH	256

class CLocalisation
{
	static char m_szMessages[E_MSG::MSG_COUNT][MAX_LOCALISATION_LENGTH];
public:
	static void Initialise(const char* szFile);

	static char* GetMessage(E_MSG msg);
};

