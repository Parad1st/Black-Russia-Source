#pragma once

#include <stdint.h>

class CGameResourcesDecryptor
{	
	static void DecryptBinaryStreamVersion2(char* pStream);
	static void DecryptBinaryStreamVersion3(char* pStream);
	static void DecryptBinaryStreamVersion4(char* pStream);
	static void DecryptBinaryStreamVersion5(char* pStream);
public:
	static void CStreaming__ConvertBufferToObject_hook(char* pData, int a2, int a3);
};