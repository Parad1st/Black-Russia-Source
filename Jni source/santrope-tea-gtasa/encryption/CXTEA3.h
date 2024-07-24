#pragma once

#include <stdint.h>
#include "common.h"

class CXTEA3
{
	uint32_t k[8];
	void EncryptBlock(unsigned int num_rounds, uint32_t* v);
	void DecryptBlock(unsigned int num_rounds, uint32_t* v);
public:
	void SetKey(const uint32_t* pKey);
	void EncryptData(void* pData, size_t iSize, size_t iNumRounds);
	void DecryptData(void* pData, size_t iSize, size_t iNumRounds);
};

