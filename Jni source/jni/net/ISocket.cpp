#include "ISocket.h"

#include "ISocket.h"

void ISocket::Cleanup()
{
#ifdef _WIN32
	WSACleanup();
#endif
	if (m_iSocket)
	{
#ifdef _WIN32
		closesocket(m_iSocket);
#else
		close(m_iSocket);
#endif
		m_iSocket = 0;
	}
}

ISocket::ISocket()
{
#ifdef _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

ISocket::~ISocket()
{
	Cleanup();
}
