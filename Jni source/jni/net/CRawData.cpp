#include "CRawData.h"



CRawData::CRawData() : m_pBuffer(nullptr), m_iSize(0), m_iWriteOffset(0), m_iReadOffset(0)
{
}


CRawData::CRawData(uint32_t iSize) : m_iSize(iSize), m_iWriteOffset(0), m_iReadOffset(0)
{
	m_pBuffer = new uint8_t[iSize];
	memset(m_pBuffer, 0, iSize);
}

CRawData::~CRawData()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
	}
}

CRawData::CRawData(uint8_t * pSource, uint32_t iSize, bool bCopy) : m_iSize(iSize), m_iWriteOffset(iSize), m_iReadOffset(0)
{
	if (bCopy)
	{
		m_pBuffer = new uint8_t[iSize];
		memset(m_pBuffer, 0, iSize);
		memcpy(m_pBuffer, pSource, iSize);
	}
	else
	{
		m_pBuffer = pSource;
	}
}

void CRawData::SetWriteOffset(uint64_t iOffset)
{
	m_iWriteOffset = iOffset;
}

void CRawData::SetReadOffset(uint64_t iOffset)
{
	m_iReadOffset = iOffset;
}

uint32_t CRawData::GetWriteOffset() const
{
	return (uint32_t)m_iWriteOffset;
}

const uint8_t * CRawData::GetRawData() const
{
	return const_cast<uint8_t*>(m_pBuffer);
}

uint32_t CRawData::GetSize() const
{
	return m_iSize;
}

void CRawData::Resize(uint32_t iNewSize)
{
	uint8_t* temp = new uint8_t[iNewSize];
	memcpy((void*)temp, m_pBuffer, m_iSize);
	delete[] m_pBuffer;
	m_pBuffer = temp;
	m_iSize = iNewSize;
}
