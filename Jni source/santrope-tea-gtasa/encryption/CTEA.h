#pragma once

#include <stdint.h>
#include "common.h"

class CTEA
{
	uint32_t m_aKey[4];
	void EncryptBlock(unsigned int num_rounds, uint32_t v[2]);
	void DecryptBlock(unsigned int num_rounds, uint32_t v[2]);
public:
	void SetKey(const uint32_t* pKey);
	void EncryptData(void* pData, size_t iSize, size_t iNumRounds);
	void DecryptData(void* pData, size_t iSize, size_t iNumRounds);
};

