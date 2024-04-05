#pragma once

#include "..//vendor/enet/include/enet.h"
#include "ENetError.h"

#include <thread>
#include <atomic>
#include "common.h"

class CENetClient
{
	ENetError m_LastError;

	bool m_bConnected;

	ENetPacketCallback m_pCallbacks[MAX_CALLBACKS];
public:
	void ProcessNetwork();
	ENetHost* m_pClient;
	ENetPeer* m_pPeer;
	CENetClient();
	~CENetClient();

	void RegisterCallback(ENetPacketCallback cb, int iID);

	bool Connect(const char* szHost, enet_uint16 port, uint32_t playerId);
	void Disconnect();

	void Send(const void* pData, size_t iSize);
};

