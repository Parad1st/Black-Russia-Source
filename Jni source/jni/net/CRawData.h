#pragma once
#include <stdint.h>
#include <cassert>
#include <string.h>

class CRawData
{
	uint8_t* m_pBuffer;
	uint32_t m_iSize;
	uint64_t m_iWriteOffset;
	uint64_t m_iReadOffset;
public:
	CRawData(uint32_t iSize);
	CRawData();
	~CRawData();

	CRawData(uint8_t* pSource, uint32_t iSize, bool bCopy);

	template<typename T>
	void Write(T value);

	template<typename T>
	void Write(T* value, uint32_t iSize);

	template<typename T>
	void Read(T& dest);

	template<typename T>
	void Read(T* dest, uint32_t iSize);

	void SetWriteOffset(uint64_t iOffset);
	void SetReadOffset(uint64_t iOffset);

	uint32_t GetWriteOffset() const;

	const uint8_t* GetRawData() const;
	uint32_t GetSize() const;

	void Resize(uint32_t iNewSize);
};

template<typename T>
inline void CRawData::Write(T value)
{
	assert(m_iSize != 0);
	assert(m_iWriteOffset < m_iSize);
	memcpy((void*)(m_pBuffer + m_iWriteOffset), &value, sizeof(T));
	m_iWriteOffset += sizeof(T);
}

template<typename T>
inline void CRawData::Write(T* value, uint32_t iSize)
{
	assert(m_iSize != 0);
	assert(m_iWriteOffset < m_iSize);
	memcpy((void*)(m_pBuffer + m_iWriteOffset), value, iSize);
	m_iWriteOffset += iSize;
}

template<typename T>
inline void CRawData::Read(T & dest)
{
	assert(m_iSize != 0);
	assert(m_iReadOffset < m_iSize);
	memcpy((void*)&dest, (void*)(m_pBuffer + m_iReadOffset), sizeof(T));
	m_iReadOffset += sizeof(T);
}

template<typename T>
inline void CRawData::Read(T * dest, uint32_t iSize)
{
	assert(m_iSize != 0);
	assert(m_iReadOffset < m_iSize);
	memcpy((void*)dest, (void*)(m_pBuffer + m_iReadOffset), iSize);
	m_iReadOffset += iSize;
}
