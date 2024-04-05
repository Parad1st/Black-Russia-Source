#include "../main.h"
#include <vector>

uintptr_t FindLibrary(const char* library)
{
	char filename[0xFF] = { 0 },
		buffer[2048] = { 0 };
	FILE* fp = 0;
	uintptr_t address = 0;

	sprintf(filename, "/proc/%d/maps", getpid());

	fp = fopen(filename, "rt");
	if (fp == 0)
	{
		Log("ERROR: can't open file %s", filename);
		goto done;
	}

	while (fgets(buffer, sizeof(buffer), fp))
	{
		if (strstr(buffer, library))
		{
			address = (uintptr_t)strtoul(buffer, 0, 16);
			break;
		}
	}

done:

	if (fp)
		fclose(fp);

	return address;
}
void CrashLog(const char* fmt, ...);
#include "..//chatWindow.h"
extern CChatWindow* pChatWindow;
#include <algorithm>
#include "..//cryptors/DUMPLIBRARIES_result.h"
#include "..//str_obfuscator_no_template.hpp"

#include <unistd.h> // system api
#include <sys/mman.h>
#include <assert.h> // assert()
#include <dlfcn.h> // dlopen

auto libarm = cryptor::create("/lib/arm/", 10);
auto libarmeabi = cryptor::create("/lib/armeabi-v7a", 17);
auto packetst = cryptor::create("com.santrope.game", 18);
auto packetbh = cryptor::create("com.barvikha.game", 18);
auto procmaps = cryptor::create("/proc/%d/maps", 14);
#ifdef GAME_EDITION_CR
auto pmpath1 = cryptor::create("/data/data/com.barvikha", 24);
#else
auto pmpath1 = cryptor::create("/data/data/com.santrope", 24);
#endif
auto pmpath2 = cryptor::create(".game/cache/libutil.so", 23);
#include "..//cryptors/ISPMHERE_result.h"
bool IsPMHere()
{
	PROTECT_CODE_ISPMHERE;

	char path[255];
	memset(path, 0, 255);

	sprintf(path, "%s%s", pmpath1.decrypt(), pmpath2.decrypt());

	if (dlopen(path, 3))
	{
		return true;
	}

	return false;
}

bool DumpLibraries(std::vector<std::string>& buff)
{
	PROTECT_CODE_DUMPLIBRARIES;

	char filename[0xFF] = { 0 },
		buffer[2048] = { 0 };

	sprintf(filename, procmaps.decrypt(), getpid());

	FILE* fp = fopen(filename, "rt");

	if (!fp)
	{
		return false;
	}



	while (fgets(buffer, sizeof(buffer), fp))
	{
		if (strstr(&buffer[0], packetst.decrypt()) || strstr(&buffer[0], packetbh.decrypt()))
		{
			char* pBegin = strstr(&buffer[0], libarm.decrypt());
			if (!pBegin)
			{
				pBegin = strstr(&buffer[0], libarmeabi.decrypt());
			}
			if (!pBegin)
			{
				pBegin = strstr(&buffer[0], pmpath1.decrypt());
				continue;
			}
			if (!pBegin)
			{
				continue;
			}
			char* pEnd = pBegin + strlen(pBegin) - 1;

			if (*pEnd == '\n')
			{
				*pEnd = 0;
				pEnd--;
			}

			while (*pEnd != '/')
			{
				pEnd--;
			}
			pEnd++;

			std::string toPush(pEnd);

			bool bPush = true;

			for (size_t i = 0; i < buff.size(); i++)
			{
				if (buff[i] == toPush)
				{
					bPush = false;
				}
			}

			if (bPush)
			{
				buff.push_back(toPush);
			}
		}
	}

	fclose(fp);
	return true;
}

void cp1251_to_utf8(char* out, const char* in, unsigned int len)
{
	static const int table[128] =
	{
		// 80
		0x82D0,     0x83D0,     0x9A80E2,   0x93D1,     0x9E80E2,   0xA680E2,   0xA080E2,   0xA180E2,
		0xAC82E2,   0xB080E2,   0x89D0,     0xB980E2,   0x8AD0,     0x8CD0,     0x8BD0,     0x8FD0,
		// 90
		0x92D1,     0x9880E2,   0x9980E2,   0x9C80E2,   0x9D80E2,   0xA280E2,   0x9380E2,   0x9480E2,
		0,          0xA284E2,   0x99D1,     0xBA80E2,   0x9AD1,     0x9CD1,     0x9BD1,     0x9FD1,
		// A0
		0xA0C2,     0x8ED0,     0x9ED1,     0x88D0,     0xA4C2,     0x90D2,     0xA6C2,     0xA7C2,
		0x81D0,     0xA9C2,     0x84D0,     0xABC2,     0xACC2,     0xADC2,     0xAEC2,     0x87D0,
		// B0
		0xB0C2,     0xB1C2,     0x86D0,     0x96D1,     0x91D2,     0xB5C2,     0xB6C2,     0xB7C2,
		0x91D1,     0x9684E2,   0x94D1,     0xBBC2,     0x98D1,     0x85D0,     0x95D1,     0x97D1,
		// C0
		0x90D0,     0x91D0,     0x92D0,     0x93D0,     0x94D0,     0x95D0,     0x96D0,     0x97D0,
		0x98D0,     0x99D0,     0x9AD0,     0x9BD0,     0x9CD0,     0x9DD0,     0x9ED0,     0x9FD0,
		// D0
		0xA0D0,     0xA1D0,     0xA2D0,     0xA3D0,     0xA4D0,     0xA5D0,     0xA6D0,     0xA7D0,
		0xA8D0,     0xA9D0,     0xAAD0,     0xABD0,     0xACD0,     0xADD0,     0xAED0,     0xAFD0,
		// E0
		0xB0D0,     0xB1D0,     0xB2D0,     0xB3D0,     0xB4D0,     0xB5D0,     0xB6D0,     0xB7D0,
		0xB8D0,     0xB9D0,     0xBAD0,     0xBBD0,     0xBCD0,     0xBDD0,     0xBED0,     0xBFD0,
		// F0
		0x80D1,     0x81D1,     0x82D1,     0x83D1,     0x84D1,     0x85D1,     0x86D1,     0x87D1,
		0x88D1,     0x89D1,     0x8AD1,     0x8BD1,     0x8CD1,     0x8DD1,     0x8ED1,     0x8FD1
	};

	int count = 0;

	while (*in)
	{
		if (len && (count >= len)) break;

		if (*in & 0x80)
		{
			register int v = table[(int)(0x7f & *in++)];
			if (!v)
				continue;
			*out++ = (char)v;
			*out++ = (char)(v >> 8);
			if (v >>= 16)
				* out++ = (char)v;
		}
		else // ASCII
			*out++ = *in++;

		count++;
	}

	*out = 0;
}

void AND_OpenLink(const char* szLink)
{
	((void (*)(const char*))(g_libGTASA + 0x00242A64 + 1))(szLink);
}