#include "CXTEA3.h"

#ifndef _MSC_VER
#include "..//..//jni/cryptors/XTEA3_result.h"
#endif

static uint32_t rol(uint32_t base, uint32_t shift)
{
	uint32_t res;
	/* only 5 bits of shift are significant*/
	shift &= 0x1F;
	res = (base << shift) | (base >> (32 - shift));
	return res;
}

void CXTEA3::EncryptBlock(unsigned int num_rounds, uint32_t* v)
{
	unsigned int i;
	uint32_t a, b, c, d, sum = 0, t, delta = 0x9E3779B9;
	sum = 0;
	a = v[0] + k[0];
	b = v[1] + k[1];
	c = v[2] + k[2];
	d = v[3] + k[3];
	for (i = 0; i < num_rounds; i++) {
		a += (((b << 4) + rol(k[(sum % 4) + 4], b)) ^
			(d + sum) ^ ((b >> 5) + rol(k[sum % 4], b >> 27)));
		sum += delta;
		c += (((d << 4) + rol(k[((sum >> 11) % 4) + 4], d)) ^
			(b + sum) ^ ((d >> 5) + rol(k[(sum >> 11) % 4], d >> 27)));
		t = a; a = b; b = c; c = d; d = t;
	}
	v[0] = a ^ k[4];
	v[1] = b ^ k[5];
	v[2] = c ^ k[6];
	v[3] = d ^ k[7];
}

void CXTEA3::DecryptBlock(unsigned int num_rounds, uint32_t* v)
{
#ifndef _MSC_VER
	PROTECT_CODE_XTEA3;
#endif

	unsigned int i;
	uint32_t a, b, c, d, t, delta = 0x9E3779B9, sum = delta * num_rounds;
	d = v[3] ^ k[7];
	c = v[2] ^ k[6];
	b = v[1] ^ k[5];
	a = v[0] ^ k[4];
	for (i = 0; i < num_rounds; i++) 
	{
		t = d; d = c; c = b; b = a; a = t;
		c -= (((d << 4) + rol(k[((sum >> 11) % 4) + 4], d)) ^
			(b + sum) ^ ((d >> 5) + rol(k[(sum >> 11) % 4], d >> 27)));
		sum -= delta;

		a -= (((b << 4) + rol(k[(sum % 4) + 4], b)) ^
			(d + sum) ^ ((b >> 5) + rol(k[sum % 4], b >> 27)));
	}
	v[3] = d - k[3];
	v[2] = c - k[2];
	v[1] = b - k[1];
	v[0] = a - k[0];
}

void CXTEA3::SetKey(const uint32_t* pKey)
{
	for (int i = 0; i < 8; i++)
	{
		k[i] = UNOBFUSCATE_DATA(pKey[i]);
	}
}

void CXTEA3::EncryptData(void* pData, size_t iSize, size_t iNumRounds)
{
	uint32_t* pdwData = reinterpret_cast<uint32_t*>(pData);
	uint32_t dwBlocks = iSize / 4;

	for (size_t i = 0; i < dwBlocks; i += 4)
	{
		EncryptBlock(iNumRounds, &pdwData[i]);
	}
}

void CXTEA3::DecryptData(void* pData, size_t iSize, size_t iNumRounds)
{
	uint32_t* pdwData = reinterpret_cast<uint32_t*>(pData);
	uint32_t dwBlocks = iSize / 4;

	for (size_t i = 0; i < dwBlocks; i += 4)
	{
		DecryptBlock(iNumRounds, &pdwData[i]);
	}
}