#pragma once

#include <vector>
#include <thread>
#include <mutex>

class CFPSFix
{
private:
	void Routine();

	std::mutex m_Mutex;
	std::vector<pid_t> m_Threads;
public:
	CFPSFix();
	~CFPSFix();

	void PushThread(pid_t tid);
};

