#pragma once

#include <string>

class CAddress
{
public:
	uint8_t a, b, c, d;
	uint16_t usPort;

	CAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t usPort = 0);
	CAddress(const CAddress& address);
	CAddress() = default;
	~CAddress();
	CAddress& operator=(const CAddress& right);
	bool operator==(const CAddress& right);

	uint32_t GetBinaryAddress() const;
};

