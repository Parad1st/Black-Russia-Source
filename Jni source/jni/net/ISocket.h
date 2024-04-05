#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <WinSock.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> // close
#include <fcntl.h>
#endif

#include "CAddress.h"
#include "CRawData.h"

class ISocket
{
protected:
	uint16_t m_usPort;
#ifdef _WIN32
	SOCKET m_iSocket;
#else
	uint32_t m_iSocket;
#endif

	void Cleanup();
public:
	ISocket();
	virtual ~ISocket();

	virtual bool Bind(uint16_t usPort) = 0;
	virtual bool Send(CAddress destination, const CRawData& data) = 0;
	virtual CRawData* Recv(CAddress& sender) = 0;
};

