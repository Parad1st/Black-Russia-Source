#include <stdint.h>
#include "..//game/common.h"

#include "CVoiceChatClient.h"
#include <thread>

#include "../main.h"
#include "../gui/gui.h"
#include "../chatwindow.h"
#include "../keyboard.h"
#include "../settings.h"
#include "../game/game.h"
#include "../net/netgame.h"

extern CNetGame* pNetGame;
void Log(const char*, ...);
void CrashLog(const char*, ...);
extern CChatWindow* pChatWindow;
extern CVoiceChatClient* pVoice;
void CVoiceChatClient::OnVoiceInput(CVoiceChatClient* thisptr, void* pData, int iSize)
{
	if (!iSize)
	{
		return;
	}
	int i, j;
	short* inbuffer = (short*)pData;
	if (thisptr->IsDisconnected())
	{
		return;
	}
	thisptr->m_RingBufferRecord.Write(inbuffer, iSize);
	while (thisptr->m_RingBufferRecord.BytesToRead() >= FRAME_SIZE_OPUS)
	{
		Sample opusFrameBuffer[FRAME_SIZE_OPUS];
		unsigned char packet[MAX_PACKET_SIZE];
		thisptr->m_RingBufferRecord.Read((Sample*)opusFrameBuffer, FRAME_SIZE_OPUS);
		int len = opus_encode(thisptr->m_pEncoder, opusFrameBuffer, FRAME_SIZE_OPUS, packet, MAX_PACKET_SIZE);

		if (len < 0 || len > MAX_PACKET_SIZE)
		{
			return;
		}

		CRawData** pData = thisptr->bufferedVoice.WriteLock();

		*pData = new CRawData(sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint16_t) + len);
		(*pData)->Write((uint8_t)244);
		(*pData)->Write((uint16_t)pNetGame->GetPlayerPool()->GetLocalPlayerID());
		(*pData)->Write((uint16_t)len);
		(*pData)->Write((const char*)packet, len);
		
		thisptr->bufferedVoice.WriteUnlock();

	}
}


float posOnCircle = 0;

void CVoiceChatClient::OnClientDisconnected(ENetEvent& event)
{
	if (event.data == 228)
	{
		pVoice->SetNetworkState(VOICECHAT_NONE);
		return;
	}
	pVoice->SetNetworkState(VOICECHAT_WAIT_CONNECT);
}
void CVoiceChatClient::OnClientConnected(ENetEvent& event)
{
}
#include "..//clientlogic/ChatMessenger.h"
void CVoiceChatClient::OnPacketIncoming(ENetEvent& event)
{
	CRawData data(event.packet->data, event.packet->dataLength, true);
	uint8_t id;
	uint16_t playerid;
	uint16_t len;
	data.Read(id);
	switch (id)
	{
		case 244:
		{
			data.Read(playerid);
			if (playerid == pNetGame->GetPlayerPool()->GetLocalPlayerID())
			{
				return;
			}
			data.Read(len);
			const char* voice = new char[len];
			data.Read(voice, len);
			pVoice->PushOpusBufferByPlayerID(playerid, (const void*)voice, len);
			break;
		}
		case 245:
		{
			uint8_t value;
			data.Read(playerid);
			data.Read(value);
			pVoice->SetVolume(value);
			break;
		}
		case 246:
		{
			static uint32_t lastMuted = GetTickCount();
			if (GetTickCount() - lastMuted <= 250)
			{
				return;
			}
			lastMuted = GetTickCount();
			pChatWindow->AddDebugMessageNonFormatted(CLocalisation::GetMessage(E_MSG::VOICE_MUTED));
			pVoice->m_bRecording = false;
			OPENSL_STREAM* m_pPlayer;
			m_pPlayer = pVoice->m_pPlayer;
			if (pVoice->m_bCouldRecord)
			{
				(*m_pPlayer->recorderRecord)->SetRecordState(m_pPlayer->recorderRecord, SL_RECORDSTATE_STOPPED);
			}
			break;
		}
	}

}
CVoiceChatClient::CVoiceChatClient()
{
	m_bCouldRecord = true;
	m_iActiveStreams = 0;
	if (enet_initialize() != 0) 
	{
		Log("Could not initialize network");
		return;
	}
	m_pClient = new CENetClient();
	m_pClient->RegisterCallback(CVoiceChatClient::OnClientDisconnected, ENET_EVENT_TYPE_DISCONNECT);
	m_pClient->RegisterCallback(CVoiceChatClient::OnPacketIncoming, ENET_EVENT_TYPE_RECEIVE);
	m_pOutput = new CSoundOutput("", SR, 255);
	for (int i = 0; i < MAX_PLAYERS_VOICE; i++)
	{
		m_pStreamPlayers[i] = nullptr;
		m_bMuted[i] = false;
		m_pPlayersPos[i].pos.X = m_pPlayersPos[i].pos.Y = m_pPlayersPos[i].pos.Z = 0.0f;
	}
	m_pOutput->SetMyPosition(0.f, 0.f, 0.f);
	m_pOutput->SetMyVelocity(0.f, 0.f, 0.f);

	m_pOutput->SetMyOrientationFront(0.f, 0.f, 1.f);
	m_pOutput->SetMyOrientationUp(1.f, 0.f, 0.f);
	m_pOutput->UpdateMe();

	SetNetworkState(VOICECHAT_NONE);

	m_pPlayer = nullptr;
	m_pPlayer = android_OpenAudioDevice(SR, 1, 2, BUFFERFRAMES, (RecordCallback)CVoiceChatClient::OnVoiceInput, (void*)this);
	if (!m_pPlayer)
	{
		Log("Cannot open microphone, allow this in permissions");
		m_bCouldRecord = false;
		return;
	}
	int opusErr = OPUS_OK;
	m_pEncoder = opus_encoder_create(SR, 1, OPUS_APPLICATION_VOIP, &opusErr);
	if (opusErr != OPUS_OK || m_pEncoder == NULL)
	{
		return;
	}
	if (opus_encoder_ctl(m_pEncoder, OPUS_SET_BITRATE(16000)) != OPUS_OK)
	{
		return;
	}
	m_bRecording = false;
	(*m_pPlayer->recorderRecord)->SetRecordState(m_pPlayer->recorderRecord, SL_RECORDSTATE_STOPPED);
}

CVoiceChatClient::~CVoiceChatClient()
{
	for (int i = 0; i < MAX_PLAYERS_VOICE; i++)
	{
		
	}
	if (m_pPlayer)
	{
		android_CloseAudioDevice(m_pPlayer);
	}

}

void CVoiceChatClient::SetNetworkState(int iState)
{
	m_iNetworkState = iState;
}

int CVoiceChatClient::GetNetworkState() const
{
	return m_iNetworkState;
}


void CVoiceChatClient::PushOpusBufferByPlayerID(int iPlayerID, const void* data, int length)
{
	int iID = GetInternalIDByPlayerID(iPlayerID);
	if (iID < 0 || iID >= MAX_PLAYERS_VOICE) return;
	PushOpusBuffer(iID, data, length);
}

void CVoiceChatClient::MutePlayer(int iPlayerID)
{
	if (iPlayerID < 0 || iPlayerID >= MAX_PLAYERS_VOICE) return;
	BUFFERED_COMMAND_LOCAL* bc;
	bc = bufferedLocal.WriteLock();
	bc->iPlayerID = iPlayerID;
	bc->command = BUFFERED_COMMAND_LOCAL::BC_MUTE;

	bufferedLocal.WriteUnlock();
}

void CVoiceChatClient::UnMutePlayer(int iPlayerID)
{
	if (iPlayerID < 0 || iPlayerID >= MAX_PLAYERS_VOICE) return;
	BUFFERED_COMMAND_LOCAL* bc;
	bc = bufferedLocal.WriteLock();
	bc->iPlayerID = iPlayerID;
	bc->command = BUFFERED_COMMAND_LOCAL::BC_UNMUTE;

	bufferedLocal.WriteUnlock();

}

void CVoiceChatClient::SetVolumePlayer(int iPlayerID, int iVolume)
{
	if (iPlayerID < 0 || iPlayerID >= MAX_PLAYERS_VOICE) return;
	BUFFERED_COMMAND_LOCAL* bc;
	bc = bufferedLocal.WriteLock();
	bc->iPlayerID = iPlayerID;
	float volume = (float)iVolume / 100.0f;
	float* pData = new float;
	*pData = volume;
	bc->pData = (void*)pData;
	bc->command = BUFFERED_COMMAND_LOCAL::BC_VOLUMEPLAYER;

	bufferedLocal.WriteUnlock();
}

void CVoiceChatClient::SetVolume(int iVolume)
{
	float volume = (float)iVolume / 100.0f;
	m_pOutput->SetVolume(volume);
}

void CVoiceChatClient::TurnRecording()
{
	m_bRecording ^= 1;
	if (m_bRecording)
	{
		EnableInput();
	}
	else
	{
		DisableInput();
	}

}

bool CVoiceChatClient::IsRecording()
{
	return m_bRecording;
}

void CVoiceChatClient::DisableInput()
{
	m_bRecording = false;

	BUFFERED_COMMAND_LOCAL* bc;
	bc = bufferedLocal.WriteLock();
	bc->iPlayerID = -1;
	bc->pData = nullptr;
	bc->command = BUFFERED_COMMAND_LOCAL::BC_DISABLEMICRO;

	bufferedLocal.WriteUnlock();
}

void CVoiceChatClient::EnableInput()
{
	m_bRecording = true;
	BUFFERED_COMMAND_LOCAL* bc;
	bc = bufferedLocal.WriteLock();
	bc->iPlayerID = -1;
	bc->pData = nullptr;
	bc->command = BUFFERED_COMMAND_LOCAL::BC_ENABLEMICRO;

	bufferedLocal.WriteUnlock();
}
#define NETGAME_VERSION 4057
#define AUTH_BS "15121F6F18550C00AC4B4F8A167D0379BB0ACA99043"
#include "../vendor/raknet/RakNetworkFactory.h"
#include "../vendor/raknet/PacketEnumerations.h"
#include "../vendor/raknet/StringCompressor.h"
unsigned char GetPacketID(Packet* p);
void CVoiceChatClient::Connect(const char* szHostOrIP, unsigned short port)
{
	strncpy(m_szHostOrIp, szHostOrIP, sizeof(m_szHostOrIp));
	m_iPort = port;
	SetNetworkState(VOICECHAT_WAIT_CONNECT);
}

void CVoiceChatClient::Disconnect()
{
	SetNetworkState(VOICECHAT_DISCONNECTED);
}
void CVoiceChatClient::FullDisconnect()
{
	if (pChatWindow) pChatWindow->AddDebugMessage("Вы отключены от войс чата");
	SetNetworkState(VOICECHAT_FULLDISCONNECTED);
}
bool CVoiceChatClient::IsDisconnected()
{
	return (GetNetworkState() == VOICECHAT_DISCONNECTED || GetNetworkState() == VOICECHAT_FULLDISCONNECTED || GetNetworkState() == VOICECHAT_NONE);
}
extern uint32_t bProcessedRender2dstuff;
void CVoiceChatClient::StartProcessing()
{
	std::thread([this] {
		while (true)
		{
			if (IsDisconnected())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				continue;
			}

			static uint32_t time = GetTickCount();
			
			{
				BUFFERED_COMMAND_NETWORK* bcs;
				while ((bcs = bufferedNetwork.ReadLock()) != 0)
				{
					if (bcs->command == BUFFERED_COMMAND_NETWORK::BC_VOICE)
					{
						int state = *(int*)(g_libGTASA + 0x008ED86D); // CMenuManager::m_bMenuActive
						int state1 = *(int*)(g_libGTASA + 0x0063E06C); // gMobileMenu
						int state2 = *(int*)(g_libGTASA + 0x0063E074);
						int state3 = 0;

						if (GetTickCount() - bProcessedRender2dstuff >= 500)
						{
							state3 = 1;
						}

						uint8_t isPaused = *(uint8_t*)(g_libGTASA + 0x005DDD38);

						if (bcs->iPlayerID < 0 || bcs->iPlayerID >= MAX_PLAYERS_VOICE)
						{
							bufferedNetwork.ReadUnlock();
							delete[](char*)bcs->pData;
							continue;
						}

						if (state1 || state3 || m_bMuted[bcs->iPlayerID])
						{
							bufferedNetwork.ReadUnlock();
							delete[](char*)bcs->pData;
							continue;
						}
						if (m_pStreamPlayers[bcs->iPlayerID])
						{
							m_aLastPushTime[bcs->iPlayerID] = GetTickCount();

							m_pStreamPlayers[bcs->iPlayerID]->PushOpusBuffer(bcs->pData, bcs->iLength);
							int i = bcs->iPlayerID;
							m_pStreamPlayers[i]->SetPosition(m_pPlayersPos[i].pos.X, m_pPlayersPos[i].pos.Y, m_pPlayersPos[i].pos.Z);
							m_pStreamPlayers[i]->SetMinDistance(5.0f);
							m_pStreamPlayers[i]->SetRolloffFactor(1.0f);
							m_pStreamPlayers[i]->SetMaxDistance(15.0f);
							m_pStreamPlayers[i]->SetVelocity(0.0f, 0.0f, 0.0f);
							m_pStreamPlayers[i]->SetDirection(0.0f, 0.0f, 0.0f);
							if (!m_pStreamPlayers[i]->Update())
							{
								delete[](char*)bcs->pData;
								bufferedNetwork.ReadUnlock();
								continue;
							}
						}
						else
						{
							if (m_iActiveStreams >= 240)
							{
								for (int i = 0; i < MAX_PLAYERS_VOICE; i++)
								{
									if (m_pStreamPlayers[i])
									{
										if (GetTickCount() - m_aLastPushTime[i] >= 5000)
										{
											m_pOutput->DeleteStreamPlayer(m_pStreamPlayers[i]);
											m_pStreamPlayers[i] = nullptr;
											m_iActiveStreams--;
										}
									}
								}
							}
							if (m_iActiveStreams >= 240)
							{
								delete[](char*)bcs->pData;
								bufferedNetwork.ReadUnlock();
								continue;
							}
							m_pStreamPlayers[bcs->iPlayerID] = m_pOutput->CreateStreamPlayer();
							m_iActiveStreams++;
							m_aLastPushTime[bcs->iPlayerID] = GetTickCount();
						}
						delete[](char*)bcs->pData;
					}
	
					bufferedNetwork.ReadUnlock();
				}
			}

			{
				BUFFERED_COMMAND_LOCAL* bcl;
				while ((bcl = bufferedLocal.ReadLock()) != 0)
				{
					if (bcl->command == BUFFERED_COMMAND_LOCAL::BC_MUTE)
					{
						if (bcl->iPlayerID >= 0 && bcl->iPlayerID < MAX_PLAYERS_VOICE)
						{
							m_bMuted[bcl->iPlayerID] = true;
						}
					}
					if (bcl->command == BUFFERED_COMMAND_LOCAL::BC_DISABLEMICRO)
					{
						if (m_bCouldRecord)
						{
							(*m_pPlayer->recorderRecord)->SetRecordState(m_pPlayer->recorderRecord, SL_RECORDSTATE_STOPPED);
						}
					}
					if (bcl->command == BUFFERED_COMMAND_LOCAL::BC_ENABLEMICRO)
					{
						if (m_bCouldRecord)
						{
							(*m_pPlayer->recorderRecord)->SetRecordState(m_pPlayer->recorderRecord, SL_RECORDSTATE_RECORDING);
						}
					}
					if (bcl->command == BUFFERED_COMMAND_LOCAL::BC_VOLUMEPLAYER)
					{
						float* volume = (float*)bcl->pData;

						if (bcl->iPlayerID >= 0 && bcl->iPlayerID < MAX_PLAYERS_VOICE)
						{
							if (m_pStreamPlayers[bcl->iPlayerID])
							{
								if (*volume > 1.0f)
								{
									m_pStreamPlayers[bcl->iPlayerID]->SetExtraGain(*volume);
								}
								else
								{
									m_pStreamPlayers[bcl->iPlayerID]->SetVolume(*volume);
									m_pStreamPlayers[bcl->iPlayerID]->SetExtraGain(0.0f);
								}
							}
						}

						delete volume;
					}

					if (bcl->command == BUFFERED_COMMAND_LOCAL::BC_UNMUTE)
					{
						if (bcl->iPlayerID >= 0 && bcl->iPlayerID < MAX_PLAYERS_VOICE)
						{
							m_bMuted[bcl->iPlayerID] = false;
						}
					}

					bufferedLocal.ReadUnlock();
				}
			}

			time = GetTickCount();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		}).detach();

	std::thread(&CVoiceChatClient::Process, this).detach();
}
void AddDebugGOVNO(char* str, int num)
{
	pChatWindow->AddDebugMessage(str, num);
}
void CVoiceChatClient::Process()
{
	while (true)
	{
		int state = *(int*)(g_libGTASA + 0x008ED86D); // CMenuManager::m_bMenuActive
		int state1 = *(int*)(g_libGTASA + 0x0063E06C); // gMobileMenu
		int state2 = *(int*)(g_libGTASA + 0x0063E074);
		uint8_t isPaused = *(uint8_t*)(g_libGTASA + 0x005DDD38);

		int state3 = 0;

		if (GetTickCount() - bProcessedRender2dstuff >= 500)
		{
			state3 = 1;
		}

		if (IsDisconnected())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		if (state1 || state3)
		{
			/*CRawData** pData;
			while ((pData = bufferedVoice.ReadLock()) != 0)
			{
				delete (*pData);
				bufferedVoice.ReadUnlock();
			}

			bufferedVoice.Clear();*/
			m_bClearInputBuffer = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}
		else
		{
			m_bClearInputBuffer = false;
		}
		if (GetNetworkState() == VOICECHAT_FULLDISCONNECTED)
		{
			delete m_pClient;
			m_pClient = nullptr;
			continue;
		}
		if (GetNetworkState() == VOICECHAT_DISCONNECTED)
		{
			delete m_pClient;
			m_pClient = nullptr;
			SetNetworkState(VOICECHAT_WAIT_CONNECT);
		}
		if (GetNetworkState() == VOICECHAT_WAIT_CONNECT && (pNetGame->GetGameState() == GAMESTATE_CONNECTED))
		{
			m_bRecording = false;
			if (m_pPlayer->recorderRecord && m_bCouldRecord)
			{
				if ((*m_pPlayer->recorderRecord))
				{
					(*m_pPlayer->recorderRecord)->SetRecordState(m_pPlayer->recorderRecord, SL_RECORDSTATE_STOPPED);
				}
			}


			SetNetworkState(VOICECHAT_CONNECTING);
			if (!m_pClient)
			{
				m_pClient = new CENetClient();
				m_pClient->RegisterCallback(CVoiceChatClient::OnClientDisconnected, ENET_EVENT_TYPE_DISCONNECT);
				m_pClient->RegisterCallback(CVoiceChatClient::OnPacketIncoming, ENET_EVENT_TYPE_RECEIVE);
			}
			if (m_pClient)
			{
				if (m_pClient->Connect(m_szHostOrIp, m_iPort, pNetGame->GetPlayerPool()->GetLocalPlayerID()))
				{
					SetNetworkState(VOICECHAT_CONNECTED);
				}
				else
				{
					SetNetworkState(VOICECHAT_WAIT_CONNECT);
				}
			}
		}
		else if(GetNetworkState() == VOICECHAT_CONNECTED)
		{
			ProcessNetwork();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

void CVoiceChatClient::PostNewPlayerInfo(int iPlayerID, PLAYER_VOICE_POS* pData)
{
	//m_InfoMutex.lock();
	memcpy(&m_pPlayersPos[iPlayerID], (const void*)pData, sizeof(PLAYER_VOICE_POS));
	//m_InfoMutex.unlock();
}

void CVoiceChatClient::PostNewLocalPlayerInfo(PLAYER_VOICE_POS* pData, VECTOR* pUP, VECTOR* pFront)
{
	m_pOutput->SetMyPosition(pData->pos.X, pData->pos.Y, pData->pos.Z);
	m_pOutput->SetMyVelocity(0.f, 0.f, 0.f);

	m_pOutput->SetMyOrientationFront(pFront->X, pFront->Y, pFront->Z);
	m_pOutput->SetMyOrientationUp(pUP->X, pUP->Y, pUP->Z);

	m_pOutput->UpdateMe();
}

void CVoiceChatClient::ProcessNetwork()
{
	CRawData** pData;
	while ((pData = bufferedVoice.ReadLock()) != 0)
	{
		if (!m_bClearInputBuffer)
		{
			if (m_pClient) m_pClient->Send((*pData)->GetRawData(), (*pData)->GetSize());
		}
		delete (*pData);
		bufferedVoice.ReadUnlock();
	}

	if (m_pClient) m_pClient->ProcessNetwork();
}

void CVoiceChatClient::PushOpusBuffer(int iID, const void* data, int length)
{
	BUFFERED_COMMAND_NETWORK* bc;
	bc = bufferedNetwork.WriteLock();
	bc->iPlayerID = iID;
	bc->pData = data;
	bc->iLength = length;
	bc->command = BUFFERED_COMMAND_NETWORK::BC_VOICE;
	bufferedNetwork.WriteUnlock();
}

int CVoiceChatClient::GetInternalIDByPlayerID(int iPlayerID)
{
	return iPlayerID;
}
