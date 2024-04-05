#include "CENetClient.h"
void Log(const char* fmt, ...);
void CENetClient::ProcessNetwork()
{
	ENetEvent event;
	while (enet_host_service(m_pClient, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			if (m_pCallbacks[ENET_EVENT_TYPE_CONNECT])
			{
				m_pCallbacks[ENET_EVENT_TYPE_CONNECT](event);
			}
			break;
		}
		case ENET_EVENT_TYPE_RECEIVE:
		{
			if (m_pCallbacks[ENET_EVENT_TYPE_RECEIVE])
			{
				m_pCallbacks[ENET_EVENT_TYPE_RECEIVE](event);
			}
			enet_packet_destroy(event.packet);
			break;
		}

		case ENET_EVENT_TYPE_DISCONNECT:
		{
			if (m_pCallbacks[ENET_EVENT_TYPE_DISCONNECT])
			{
				m_pCallbacks[ENET_EVENT_TYPE_DISCONNECT](event);
			}
			event.peer->data = NULL;
			break;
		}
		}
	}
}

CENetClient::CENetClient()
{


	for (int i = 0; i < MAX_CALLBACKS; i++)
	{
		m_pCallbacks[i] = nullptr;
	}

	m_bConnected = false;
	m_pPeer = nullptr;
	m_LastError = E_OK;
	m_pClient = nullptr;
	m_pClient = enet_host_create(NULL /* create a client host */,
		10 /* only allow 1 outgoing connection */,
		255 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (m_pClient == nullptr)
	{
		m_LastError = E_COULDNT_START_CLIENT;
		return;
	}
}

CENetClient::~CENetClient()
{
	if (m_pPeer)
	{
		enet_peer_disconnect_now(m_pPeer, 228);
		m_pPeer = nullptr;
	}

	if (m_pClient)
	{
		enet_host_destroy(m_pClient);
		m_pClient = nullptr;
	}
}
void CENetClient::RegisterCallback(ENetPacketCallback cb, int iID)
{
	m_pCallbacks[iID] = cb;
}

void AddDebugGOVNO(char* str, int num);
bool CENetClient::Connect(const char* szHost, enet_uint16 port, uint32_t playerId)
{
	m_bConnected = false;
	m_LastError = E_OK;
	ENetAddress address;
	ENetEvent event;

	enet_address_set_host(&address, szHost);
	address.port = port;

	m_pPeer = enet_host_connect(m_pClient, &address, 255, playerId);
	if (m_pPeer == NULL)
	{
		m_LastError = E_COULDNT_CREATE_PEER;
		return false;
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(m_pClient, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		m_bConnected = true;
		enet_host_flush(m_pClient);
		return true;
	}
	else
	{
		Log("Could not connect to voice-server");
		enet_peer_reset(m_pPeer);
		m_pPeer = nullptr;
		m_bConnected = false;
		return false;
	}
	return true;
}

void CENetClient::Disconnect()
{
	if (m_pPeer)
	{
		enet_peer_disconnect_now(m_pPeer, 228);
		m_pPeer = nullptr;
	}
}

void CENetClient::Send(const void* pData, size_t iSize)
{
	if (m_pPeer)
	{
		ENetPacket* packet = enet_packet_create(pData, iSize, ENET_PACKET_FLAG_UNSEQUENCED);
		enet_peer_send(m_pPeer, 0, packet);
		enet_host_flush(m_pClient);
	}
}
