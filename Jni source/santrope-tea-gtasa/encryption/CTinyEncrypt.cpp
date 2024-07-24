#include "CTinyEncrypt.h"

void CTinyEncrypt::EncryptBlock(unsigned int num_rounds, uint32_t v[2])
{
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = OBFUSCATE_DATA(ENCRYPTION_DELTA);
	for (i = 0; i < num_rounds; i++)
	{
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_aKey[sum & 3]);
		sum += UNOBFUSCATE_DATA(delta);
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_aKey[(sum >> 11) & 3]);
	}
	v[0] = v0; v[1] = v1;
}

void CTinyEncrypt::DecryptBlock(unsigned int num_rounds, uint32_t v[2])
{
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], delta = OBFUSCATE_DATA(ENCRYPTION_DELTA), sum = UNOBFUSCATE_DATA(delta) * num_rounds;
	for (i = 0; i < num_rounds; i++)
	{
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_aKey[(sum >> 11) & 3]);
		sum -= UNOBFUSCATE_DATA(delta);
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_aKey[sum & 3]);
	}
	v[0] = v0;
	v[1] = v1;
}

void CTinyEncrypt::SetKey(const uint32_t* pKey)
{
	for (int i = 0; i < 4; i++)
	{
		m_aKey[i] = UNOBFUSCATE_DATA(pKey[i]);
	}
}

void CTinyEncrypt::EncryptData(void* pData, size_t iSize, size_t iNumRounds)
{
	uint32_t dwBlocks = iSize / 4;
	uint32_t* pdwData = reinterpret_cast<uint32_t*>(pData);
	for (size_t i = 0; i < dwBlocks; i += 2)
	{
		EncryptBlock(iNumRounds, &pdwData[i + 0]);
	}
}

void CTinyEncrypt::DecryptData(void* pData, size_t iSize, size_t iNumRounds)
{
	uint32_t dwBlocks = iSize / 4;
	uint32_t* pdwData = reinterpret_cast<uint32_t*>(pData);
	for (size_t i = 0; i < dwBlocks; i += 2)
	{
		DecryptBlock(iNumRounds, &pdwData[i + 0]);
	}
}