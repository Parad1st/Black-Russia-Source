#include "CGameResourcesDecryptor.h"


#include "..//santrope-tea-gtasa/encryption/CTinyEncrypt.h"
#include "..//santrope-tea-gtasa/encryption/CXTEA3.h"
#include "..//santrope-tea-gtasa/encryption/CTEA.h"

extern "C"
{
#include "..//santrope-tea-gtasa/encryption/aes.h"
}
/*
static void InitCTX(AES_ctx& ctx, const uint8_t* pKey)
{
	uint8_t key[16];
	memcpy(&key[0], &pKey[0], 16);
	for (int i = 0; i < 16; i++)
	{
		key[i] = XOR_UNOBFUSCATE(key[i]);
	}
	uint8_t iv[16];
	memcpy(&iv[0], &g_iIV, 16);
	for (int i = 0; i < 16; i++)
	{
		iv[i] = XOR_UNOBFUSCATE(iv[i]);
	}
	AES_init_ctx_iv(&ctx, &key[0], &iv[0]);
}
*/
void CGameResourcesDecryptor::DecryptBinaryStreamVersion2(char* pStream)
{
	CTinyEncrypt tinyEnc;
	tinyEnc.SetKey(&g_iEncryptionKeyVersion2[0]);
	

	pStream += 4;
	int parts = *(int*)pStream;
	pStream += 4;
	int fileSize = *(int*)pStream;
	pStream += 4;
	uintptr_t pointer = (uintptr_t)pStream;


	for (int i = 0; i < parts; i++)
	{
		tinyEnc.DecryptData((void*)pointer, PART_SIZE, 16);
		pointer += PART_SIZE;
	}
	memcpy((void*)(pStream - 12), (const void*)pStream, fileSize);
	pStream -= 12;
}

void CGameResourcesDecryptor::DecryptBinaryStreamVersion3(char* pStream)
{

	SEncryptedHeader* pHeader = (SEncryptedHeader*)pStream;
	pStream += sizeof(SEncryptedHeader);
	AES_ctx ctx;


	//InitCTX(ctx, &g_iEncryptionKey[0]);

	uint8_t* pBufferChunk = (uint8_t*)pStream;
	
	for (uint32_t i = 0; i < pHeader->dwChunksEncrypted; i++)
	{
		AES_CBC_decrypt_buffer(&ctx, &pBufferChunk[0], PART_SIZE);
		
		pBufferChunk += PART_SIZE;
	}
	//pChatWindow->AddDebugMessage("TIME %u", GetTickCount() - tickStart);
	memcpy((void*)(pStream - sizeof(SEncryptedHeader)), (const void*)pStream, pHeader->dwFileSize);
}

void CGameResourcesDecryptor::DecryptBinaryStreamVersion4(char* pStream)
{
	SEncryptedHeader* pHeader = (SEncryptedHeader*)pStream;
	pStream += sizeof(SEncryptedHeader);


	uint8_t* pBufferChunk = (uint8_t*)pStream;

	CXTEA3 encr;
	encr.SetKey(&g_iEncryptionKeyVersion4Binary[0]);

	for (uint32_t i = 0; i < pHeader->dwChunksEncrypted; i++)
	{
		encr.DecryptData((void*)& pBufferChunk[0], PART_SIZE, 32);
		pBufferChunk += PART_SIZE;
	}
	//pChatWindow->AddDebugMessage("TIME binary 444 %u", GetTickCount() - tickStart);
	memcpy((void*)(pStream - sizeof(SEncryptedHeader)), (const void*)pStream, pHeader->dwFileSize);
}

void CGameResourcesDecryptor::DecryptBinaryStreamVersion5(char* pStream)
{

	SEncryptedHeader* pHeader = (SEncryptedHeader*)pStream;
	pStream += sizeof(SEncryptedHeader);


	uint8_t* pBufferChunk = (uint8_t*)pStream;

	CTEA encr;
	encr.SetKey(&g_iEncryptionKeyVersion5Binary[0]);

	for (uint32_t i = 0; i < pHeader->dwChunksEncrypted; i++)
	{
		encr.DecryptData((void*)& pBufferChunk[0], PART_SIZE, 8);
		pBufferChunk += PART_SIZE;
	}
	//pChatWindow->AddDebugMessage("TIME binary 55555 %u", GetTickCount() - tickStart);
	memcpy((void*)(pStream - sizeof(SEncryptedHeader)), (const void*)pStream, pHeader->dwFileSize);
}

void CGameResourcesDecryptor::CStreaming__ConvertBufferToObject_hook(char* pData, int a2, int a3)
{

	SEncryptedHeader* pHeader = (SEncryptedHeader*)pData;
	
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion5Binary))
	{
		DecryptBinaryStreamVersion5(pData);
	}
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion4Binary))
	{
		DecryptBinaryStreamVersion4(pData);
	}
	else if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion3))
	{
		DecryptBinaryStreamVersion3(pData);
	}
	else if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion2))
	{
		DecryptBinaryStreamVersion2(pData);
	}
	else if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion1))
	{
		
	}
}
