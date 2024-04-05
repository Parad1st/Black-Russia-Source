#include <mutex>
#include <thread>

class ITouchListener
{
	std::mutex m_Mutex;
	std::thread::id m_LockedId;
public:
	ITouchListener();
	virtual ~ITouchListener();

	void LockAccess();
	void UnlockAccess();

	virtual bool OnTouchEvent(int type, int numClick, int posX, int posY) = 0;
};

class CTouchListenerLock
{
	ITouchListener* m_pListener;
public:
	CTouchListenerLock(ITouchListener* pClass);
	~CTouchListenerLock();
};