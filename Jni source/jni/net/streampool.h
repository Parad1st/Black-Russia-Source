#include "..//vendor/bass/bass.h"
#include <thread>
struct BUFFERED_COMMAND_STREAMPOOL
{
	int iID;
	int iPlayerID;
	int iType;
	const void* pData;
	enum { BC_DELETSTREAM, BC_ADDSTREAM, BC_ATTACH, BC_VOLUME, BC_ACTIVATE } command;
};
#include "..//vendor/raknet/SingleProducerConsumer.h"

class CStreamPool
{
	CStream* m_pStreams[MAX_STREAMS];
	bool	m_bSlotState[MAX_STREAMS];
	HSTREAM m_hIndividualStream;
	MATRIX4X4 m_matListener;
	bool bShutdownThread;
	std::thread* pThread;
	DataStructures::SingleProducerConsumer<BUFFERED_COMMAND_STREAMPOOL> bufferedCommands;

	bool m_bIsDeactivated;

	bool m_bWasPaused;
	char m_szIndividualLastLink[256];
public:
	CStreamPool();
	~CStreamPool();

	void Deactivate();
	void Activate();

	CStream* GetStream(int iID);
	CStream* AddStream(int iID, VECTOR* pPos, int iVirtualWorld, int iInterior, float fDistance, const char* szUrl);
	void DeleteStreamByID(int iID);

	void PlayIndividualStream(const char* szUrl);
	void StopIndividualStream();

	void PostListenerMatrix(MATRIX4X4* pMat);

	void SetStreamVolume(int iID, float fVolume);

	void Process();
};