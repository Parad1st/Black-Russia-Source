#include "CUDPSocket.h"

bool CUDPSocket::Bind(uint16_t usPort)
{
	m_usPort = usPort;
	m_iSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_iSocket <= 0)
	{
		return false;
	}
	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(usPort);
	if (::bind(m_iSocket, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0)
	{
		Cleanup();
		return false;
	}
#ifdef _WIN32
	unsigned long nonBlocking = 1;
	if (ioctlsocket(m_iSocket, FIONBIO, &nonBlocking) != NO_ERROR)
	{
		Cleanup();
		return false;
	}
#else
	int nonBlocking = 1;
	if (fcntl(m_iSocket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	{
		Cleanup();
		return false;
	}
#endif
	return true;
}

bool CUDPSocket::Send(CAddress destination, const CRawData & data)
{
	sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(destination.GetBinaryAddress());;
	addr.sin_port = htons(destination.usPort);
	addr.sin_family = AF_INET;
#ifdef _WIN32
	uint32_t iSentBytes;
#else
	ssize_t iSentBytes;
#endif
	iSentBytes = sendto(m_iSocket, (const char*)data.GetRawData(), data.GetSize(), 0, (sockaddr*)&addr, sizeof(sockaddr_in));

	if (iSentBytes != data.GetSize())
	{
		return false;
	}
	return true;
}

CRawData * CUDPSocket::Recv(CAddress & sender)
{
	sockaddr_in sock_sender;
#ifdef _WIN32
	int size;
#else
	socklen_t size;
#endif
	size = sizeof(sockaddr_in);
	uint8_t pBuffer[256];
	int iReceivedBytes = (int)::recvfrom(m_iSocket, (char*)pBuffer, sizeof(pBuffer), 0, (sockaddr*)&sock_sender, &size);
	if (iReceivedBytes <= 0)
	{
		return nullptr;
	}
	sender.usPort = ntohs(sock_sender.sin_port);
	int addr = ntohl(sock_sender.sin_addr.s_addr);
	sender.a = (unsigned char)(addr >> 24);
	sender.b = (unsigned char)(addr >> 16);
	sender.c = (unsigned char)(addr >> 8);
	sender.d = (unsigned char)(addr);
	
	return new CRawData(&pBuffer[0], iReceivedBytes, true);
}
