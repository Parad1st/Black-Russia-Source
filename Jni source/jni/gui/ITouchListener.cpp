#include "ITouchListener.h"
void Log(const char* fmt, ...);
ITouchListener::ITouchListener()
{
}

ITouchListener::~ITouchListener()
{
}

void ITouchListener::LockAccess()
{
	m_Mutex.lock();
	m_LockedId = std::this_thread::get_id();
}

void ITouchListener::UnlockAccess()
{
	if (std::this_thread::get_id() != m_LockedId)
	{
		Log("Warning: called UnlockAccess from thread that has no locked it");
	}
	m_Mutex.unlock();
	m_LockedId = std::thread::id();
}

CTouchListenerLock::CTouchListenerLock(ITouchListener* pClass)
{
	if (pClass)
	{
		m_pListener = pClass;
		m_pListener->LockAccess();
	}
	else
	{
		Log("pClass is null");
		std::terminate();
		return;
	}
}

CTouchListenerLock::~CTouchListenerLock()
{
	m_pListener->UnlockAccess();
}
