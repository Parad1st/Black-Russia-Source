#define BUFFERFRAMES 1024
#define VECSAMPS_MONO 64
#define VECSAMPS_STEREO 128
#define SR 24000

#define FRAME_SIZE_OPUS 480
#define MAX_PACKET_SIZE 32768
#define OPUS_BUFFER_SIZE 8196
#define RING_BUFFER_SIZE 262144
#define MAIN_PLAYER	0

#define VOICECHAT_WAIT_CONNECT	9
#define VOICECHAT_CONNECTING	13
#define VOICECHAT_CONNECTED 	14
#define VOICECHAT_NONE 			0
#define VOICECHAT_FULLDISCONNECTED	20
#define VOICECHAT_DISCONNECTED	4

#include <android/log.h>
#include "opensl_io2.h"
#include "CRingBuffer.h"
#include "..//vendor/opus/opus.h"
#include "..//vendor/raknet/SingleProducerConsumer.h"

#include "..///vendor/openal/AL/al.h"
#include "..///vendor/openal/AL/alc.h"
#include <queue>
#include <atomic>

#include "CSoundOutput.h"
#include "CStreamPlayer.h"
#include "CENetClient.h"
#include "..//net/CRawData.h"
using Sample = short;

#define NUM_BUFFERS 16
#define MIN_BUFFERS_TO_PLAY 8
#define RING_BUFFER_SIZE 262144
#define OPUS_BUFFER_SIZE 8196
#define MAX_PLAYERS_VOICE	1004

struct PLAYER_VOICE_POS
{
	VECTOR pos;
};

struct BUFFERED_COMMAND_NETWORK
{
	int iPlayerID;
	const void* pData;
	int iLength;
	enum { BC_VOICE } command;
};

struct BUFFERED_COMMAND_LOCAL
{
	int iPlayerID;
	const void* pData;
	int iLength;
	enum { BC_MUTE, BC_UNMUTE, BC_VOLUMEPLAYER, BC_ENABLEMICRO, BC_DISABLEMICRO } command;
};

class CVoiceChatClient
{
	OPENSL_STREAM* m_pPlayer;
	OpusEncoder* m_pEncoder;
public:

	std::atomic<uint32_t> m_aLastPushTime[MAX_PLAYERS_VOICE];
private:

	bool m_bCouldRecord;
	static void OnVoiceInput(CVoiceChatClient* thisptr, void* pData, int iSize);

	void PushOpusBuffer(int iID, const void* data, int length);
	int GetInternalIDByPlayerID(int iPlayerID);

	bool m_bRecording;
	bool m_bClearInputBuffer;
	int m_iNetworkState;
	char m_szHostOrIp[0x7F];
	int m_iPort;
	uint32_t			m_dwLastConnectAttempt;
	CENetClient* m_pClient;

	void ProcessNetwork();

	std::mutex m_InfoMutex;

	CSoundOutput* m_pOutput;
	CStreamPlayer* m_pStreamPlayers[MAX_PLAYERS_VOICE];
	PLAYER_VOICE_POS m_pPlayersPos[MAX_PLAYERS_VOICE];
	int m_iActiveStreams;

	RingBuffer<Sample, 100000> m_RingBufferRecord;

	bool m_bMuted[MAX_PLAYERS_VOICE];
	DataStructures::SingleProducerConsumer<CRawData*> bufferedVoice;
	DataStructures::SingleProducerConsumer<BUFFERED_COMMAND_NETWORK> bufferedNetwork;
	DataStructures::SingleProducerConsumer<BUFFERED_COMMAND_LOCAL> bufferedLocal;

	static void OnClientDisconnected(ENetEvent& event);
	static void OnClientConnected(ENetEvent& event);
	static void OnPacketIncoming(ENetEvent& event);
public:
	CVoiceChatClient();
	~CVoiceChatClient();

	void SetNetworkState(int iState);
	int GetNetworkState() const;

	void PushOpusBufferByPlayerID(int iPlayerID, const void* data, int length);
	
	void MutePlayer(int iPlayerID);
	void UnMutePlayer(int iPlayerID);
	void SetVolumePlayer(int iPlayerID, int iVolume);
	void SetVolume(int iVolume);

	void TurnRecording();
	bool IsRecording();
	void DisableInput();
	void EnableInput();

	void Connect(const char* m_szHostOrIP, unsigned short port);
	void Disconnect();
	void FullDisconnect();
	bool IsDisconnected();

	void StartProcessing();

	void Process();

	void PostNewPlayerInfo(int iPlayerID, PLAYER_VOICE_POS* pData);
	void PostNewLocalPlayerInfo(PLAYER_VOICE_POS* pData, VECTOR* pUP, VECTOR* pFront);
};