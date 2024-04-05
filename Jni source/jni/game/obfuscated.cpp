#include "../main.h"
#include "../util/armhook.h"
#include "common.h"

#include "..//..//santrope-tea-gtasa/encryption/common.h"

#define UNVERIFIED_FLAG0	0x00328212
#define VERIFIED_FLAG0	OBFUSCATE_DATA(66)

#define UNVERIFIED_FLAG1	0x00872312
#define VERIFIED_FLAG1	OBFUSCATE_DATA(33)

static int* pVerified0 = nullptr;
static int* pVerified1 = nullptr;
void WriteMemory1(uintptr_t dest, uintptr_t src, size_t size);
static int g(int a)
{
	return UNOBFUSCATE_DATA(a);
}

void ObfuscatedForceExit0()
{
	//  CEntryInfoNode::operator new
	//WriteMemory(g_libGTASA + 0x0039463E, (uintptr_t)"\x4f\xf0\x00\x00\x00\x68", 6); // mov r0, 0, ldr r0, [r0] // trap
}

void ObfuscatedForceExit1()
{
	// CPlayerPed::CPlayerPed
	//WriteMemory(g_libGTASA + 0x00458E88, (uintptr_t)"\x4f\xf0\x00\x03", 4); // mov r3, 0 // trap
	//NOP(g_libGTASA + 0x00458E8A, 2);
}

void ObfuscatedForceExit2()
{
	// CTaskSimpleFall::ProcessPed
	//WriteMemory(g_libGTASA + 0x004B2F02, (uintptr_t)"\x4f\xf0\x00\x01", 4); // trap
}

void WriteUnVerified0()
{
	/*pVerified0 = (int*)((char* (*)(int))(g_libGTASA + 0x179B40))(4);
	uintptr_t v = UNVERIFIED_FLAG0;
	WriteMemory1((uintptr_t)pVerified0, (uintptr_t)& v, 4);*/
}

void WriteUnVerified1()
{
	/*pVerified1 = (int*)((char* (*)(int))(g_libGTASA + 0x179B40))(4);
	uintptr_t v = UNVERIFIED_FLAG1;
	WriteMemory1((uintptr_t)pVerified1, (uintptr_t)& v, 4);*/
}

void WriteVerified0()
{
	/*if (!pVerified0)
	{
		ObfuscatedForceExit0();
	}
	*pVerified0 = VERIFIED_FLAG0;*/
}

void WriteVerified1()
{
	/*if (!pVerified1)
	{
		ObfuscatedForceExit2();
	}
	*pVerified1 = VERIFIED_FLAG1;*/
}

void CheckForProtected0()
{
	/*if (!pVerified0)
	{
		//Log("govno1");
		ObfuscatedForceExit1();
	}
	if (UNOBFUSCATE_DATA(*pVerified0) != g(VERIFIED_FLAG0))
	{
		ObfuscatedForceExit2();
	}*/
}

void CheckForProtected1()
{
	/*if (!pVerified1)
	{
		ObfuscatedForceExit0();
	}
	if (UNOBFUSCATE_DATA(*pVerified1) != g(VERIFIED_FLAG1))
	{
		//Log("govno2");
		ObfuscatedForceExit1();
	}*/
}