#include "CAddress.h"


CAddress::CAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t usPort)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->usPort = usPort;
}

CAddress::CAddress(const CAddress & address)
{
	a = address.a;
	b = address.b;
	c = address.c;
	d = address.d;
	usPort = address.usPort;
}

CAddress::~CAddress()
{
}

CAddress & CAddress::operator=(const CAddress & right)
{
	a = right.a;
	b = right.b;
	c = right.c;
	d = right.d;
	usPort = right.usPort;
	return *this;
}

bool CAddress::operator==(const CAddress & right)
{
	if (a == right.a && b == right.b && c == right.c && d == right.d && usPort == right.usPort)
	{
		return true;
	}
	else return false;
}

uint32_t CAddress::GetBinaryAddress() const
{
	return (a << 24) | (b << 16) | (c << 8) | d;
}