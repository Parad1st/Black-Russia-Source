#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <filesystem>
#include <Windows.h>
#include "encryption/common.h"
#include "encryption/encrypt.h"
#include "encryption/CTEA.h"

extern "C"
{
#include "encryption/aes.h"
}

std::vector<std::string> get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
			filenames.push_back(iter->path().string());
	}

	return filenames;
}

bool IsEncryptedBinaryFile(uint8_t* pFirstChunk)
{
	SEncryptedHeader* pHeader = (SEncryptedHeader*)(pFirstChunk);
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion5Binary))
	{
		return true;
	}
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion4Binary))
	{
		return true;
	}
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion3))
	{
		return true;
	}
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion2))
	{
		return true;
	}
	if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierVersion1))
	{
		return true;
	}

	return false;
}

inline std::ostream& red(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_INTENSITY);
	return s;
}

inline std::ostream& white(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return s;
}
/*
void InitCTX(AES_ctx& ctx, const uint8_t* pKey)
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
void EncryptBinaryStreamVersion5(std::vector<std::string>& fileNames)
{
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		std::ifstream in(fileNames[i], std::ios::ate | std::ios::binary);

		fileNames[i][0] = 'e';
		fileNames[i][1] = 'n';

		std::ofstream out(fileNames[i], std::ios::binary);

		uint32_t uiFileSize = (uint32_t)in.tellg();
		in.seekg(0, std::ios_base::beg);

		uint8_t arHeader[60];
		in.read((char*)& arHeader[0], 60);
		if (IsEncryptedBinaryFile(&arHeader[0]))
		{
			std::cout << red << "[ERROR] File " << fileNames[i] << " is already encrypted" << white << std::endl;

			continue;
		}
		SEncryptedHeader header;

		header.dwVersion = UNOBFUSCATE_DATA(g_iIdentifierVersion5Binary);
		header.dwFileSize = uiFileSize;
		header.dwChunksEncrypted = (uint32_t)(uiFileSize / PART_SIZE);

		out.write((const char*)& header, sizeof(SEncryptedHeader));

		

		uint8_t pBufferChunk[PART_SIZE];

		CTEA encr;
		encr.SetKey(&g_iEncryptionKeyVersion5Binary[0]);

		in.seekg(0, std::ios_base::beg);
		for (uint32_t i = 0; i < header.dwChunksEncrypted; i++)
		{
			in.read((char*)& pBufferChunk[0], PART_SIZE);
			
			encr.EncryptData((void*)& pBufferChunk[0], PART_SIZE, 8);

			out.write((const char*)& pBufferChunk[0], PART_SIZE);
		}

		uint32_t dwRemainBytes = uiFileSize - (header.dwChunksEncrypted * PART_SIZE);
		if (dwRemainBytes)
		{
			in.read((char*)& pBufferChunk[0], dwRemainBytes);
			out.write((char*)& pBufferChunk[0], dwRemainBytes);
		}

		std::cout << white << "[OK] Encrypted file " << fileNames[i] << " | File size: " << uiFileSize << std::endl;
		std::cout << white << "[OK] Writed " << dwRemainBytes << " as non-encrypted" << std::endl;
	}
}
#include "CGameResourcesDecryptor.h"
void DecryptTest(std::vector<std::string>& fileNames)
{
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		std::ifstream in(fileNames[i], std::ios::binary | std::ios::ate);

		fileNames[i][0] = 'e';
		fileNames[i][1] = 'n';

		std::ofstream out(fileNames[i], std::ios::binary);

		uint32_t uiFileSize = (uint32_t)in.tellg();
		in.seekg(0, std::ios_base::beg);

		char* pBuffer = new char[uiFileSize];

		in.read(pBuffer, uiFileSize);

		CGameResourcesDecryptor::CStreaming__ConvertBufferToObject_hook(pBuffer, 0, 0);

		out.write(pBuffer, uiFileSize);
		delete[] pBuffer;
	}
}

void EncryptTextFileVersion2(std::vector<std::string>& fileNames)
{
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		std::ifstream in(fileNames[i], std::ios::ate);
		fileNames[i][0] = 'e';
		fileNames[i][1] = 'n';
		std::ofstream out(fileNames[i], std::ios::binary);
		in.seekg(0, std::ios_base::beg);

		int lines = 0;
		std::string s;
		while (std::getline(in, s))
		{
			int keyi = UNOBFUSCATE_DATA(g_i64Encrypt);
			std::string key((const char*)& keyi);
			std::string encr = encrypt(s, key);
			int size = encr.length();
			int identifier = UNOBFUSCATE_DATA(g_iIdentifierVersion2);
			out.write((const char*)& identifier, sizeof(int));
			out.write((const char*)& size, sizeof(int));
			out.write((const char*)encr.c_str(), size);
			out.write((const char*)"\n", 1);
		}

		std::cout << "[OK] Encrypted string " << fileNames[i] << std::endl;
	}
}

void EncryptTextureDatabaseVersion1(std::vector<std::string>& fileNames)
{
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		std::ifstream in(fileNames[i], std::ios::ate | std::ios::binary);

		fileNames[i][0] = 'e';
		fileNames[i][1] = 'n';

		std::ofstream out(fileNames[i], std::ios::binary);

		uint32_t uiFileSize = (uint32_t)in.tellg();
		in.seekg(0, std::ios_base::beg);

		uint32_t dwChunksEncrypted = (uint32_t)(uiFileSize / PART_SIZE_TXD);

		AES_ctx ctx;

		//InitCTX(ctx, &g_iEncryptionKeyTXD[0]);

		uint8_t pBufferChunk[PART_SIZE_TXD];

		for (uint32_t i = 0; i < dwChunksEncrypted; i++)
		{
			in.read((char*)& pBufferChunk[0], PART_SIZE_TXD);
			AES_CBC_encrypt_buffer(&ctx, &pBufferChunk[0], PART_SIZE_TXD);
			out.write((const char*)& pBufferChunk[0], PART_SIZE_TXD);
		}

		uint32_t dwRemainBytes = uiFileSize - (dwChunksEncrypted * PART_SIZE_TXD);
		if (dwRemainBytes)
		{
			in.read((char*)& pBufferChunk[0], dwRemainBytes);
			out.write((char*)& pBufferChunk[0], dwRemainBytes);
		}

		std::cout << "[OK] Encrypted TextureDatabase " << fileNames[i] << " | File size: " << uiFileSize << std::endl;
		std::cout << "[OK] Writed " << dwRemainBytes << " as non-encrypted" << std::endl;
	}
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cout << "Specify the mode:" << std::endl;
		std::cout << "1: binary encryption" << std::endl;
		std::cout << "2: .ipl encryption" << std::endl;
		std::cout << "3: .txt encryption(TextureDatabases)" << std::endl;
		return 0;
	}
	auto fileNames = get_filenames("decrypted//");
	if (argv[1][0] == '1')
	{
		EncryptBinaryStreamVersion5(fileNames);
	}
	if (argv[1][0] == '2')
	{
		EncryptTextFileVersion2(fileNames);
	}
	if (argv[1][0] == '3')
	{
		EncryptTextureDatabaseVersion1(fileNames);
	}
	if (argv[1][0] == '4')
	{
		DecryptTest(fileNames);
	}
	system("pause");
	return 0;
	
}