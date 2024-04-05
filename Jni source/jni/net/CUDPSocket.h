#pragma once
#include "ISocket.h"

class CUDPSocket :
	public ISocket
{
public:
	CUDPSocket() = default;
	virtual ~CUDPSocket() = default;

	bool Bind(uint16_t usPort) override;
	bool Send(CAddress destination, const CRawData& data) override;
	CRawData* Recv(CAddress& sender) override;
};

