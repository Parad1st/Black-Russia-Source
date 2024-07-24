#include "CTEA.h"

#ifndef _MSC_VER
#include "..//..//jni/cryptors/TINY_ENCRYPT_result.h"
#include "..//..//jni/cryptors/TEARAK_result.h"
#endif

void CTEA::EncryptBlock(unsigned int num_rounds, uint32_t v[2])
{
#ifndef _MSC_VER
	PROTECT_CODE_TEARAK;
#endif

	/* set up */
	uint32_t v0 = v[0];
	uint32_t v1 = v[1];
	uint32_t sum = 0;
	uint32_t i;

	/* a key schedule constant */
	uint32_t delta = 0x9e3779b9;

	/* cache key */
	uint32_t k0 = m_aKey[0];
	uint32_t k1 = m_aKey[1];
	uint32_t k2 = m_aKey[2];
	uint32_t k3 = m_aKey[3];

	/* basic cycle start */
	for (i = 0; i < num_rounds; i++)
	{
		sum += delta;
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
	}
	/* end cycle */

	v[0] = v0;
	v[1] = v1;
}

void CTEA::DecryptBlock(unsigned int num_rounds, uint32_t v[2])
{
#ifndef _MSC_VER
	PROTECT_CODE_TINY_ENCRYPT;
#endif
	
	/* set up */
	uint32_t v0 = v[0];
	uint32_t v1 = v[1];
	uint32_t i;

	/* a key schedule constant */
	uint32_t delta = 0x9e3779b9;
	uint32_t sum = delta * num_rounds;
	/* cache key */
	uint32_t k0 = m_aKey[0];
	uint32_t k1 = m_aKey[1];
	uint32_t k2 = m_aKey[2];
	uint32_t k3 = m_aKey[3];

	/* basic cycle start */
	for (i = 0; i < num_rounds; i++)
	{
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		sum -= delta;
	}
	/* end cycle */

	v[0] = v0;
	v[1] = v1;
}

void CTEA::SetKey(const uint32_t* pKey)
{
	for (int i = 0; i < 4; i++)
	{
		m_aKey[i] = UNOBFUSCATE_DATA(pKey[i]);
	}
}

void CTEA::EncryptData(void* pData, size_t iSize, size_t iNumRounds)
{
	uint32_t dwBlocks = iSize / 4;
	uint32_t* pdwData = reinterpret_cast<uint32_t*>(pData);
	for (size_t i = 0; i < dwBlocks; i += 2)
	{
		EncryptBlock(iNumRounds, &pdwData[i + 0]);
	}
}

void CTEA::DecryptData(void* pData, size_t iSize, size_t iNumRounds)
{
	uint32_t dwBlocks = iSize / 4;
	uint32_t* pdwData = reinterpret_cast<uint32_t*>(pData);
	for (size_t i = 0; i < dwBlocks; i += 2)
	{
		DecryptBlock(iNumRounds, &pdwData[i + 0]);
	}
}