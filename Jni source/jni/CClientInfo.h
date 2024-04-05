#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "main.h"
#include "game/common.h"
#include "game/game.h"
#include "net/netgame.h"

#include <jni.h>


class CClientInfo
{
public:
	static uint16_t usSAMPMajorVersion;
	static uint16_t usSAMPMinorVersion;
	static bool		bSAMPModified;
	static bool		bJoinedFromLauncher;

	// filled from game.info
	static char szSerial[0x7F];
	static uint16_t usLauncherVersion;
	static uint16_t usModpackVersion;

	static char szIP[0xFF];
	static uint16_t usPort;


	static void WriteClientInfoToBitStream(RakNet::BitStream& bs);

	static void SetClientMajorVersion(JNIEnv* pEnv, jobject thiz, jint version);
	static void SetClientMinorVersion(JNIEnv* pEnv, jobject thiz, jint version);

	static void SetClientIP(JNIEnv* pEnv, jobject thiz, jbyteArray ip);
	static void SetClientPort(JNIEnv* pEnv, jobject thiz, jint port);
	static void SetClientUUID(JNIEnv* pEnv, jobject thiz, jbyteArray uuid);
	static void SetClientLauncherVersion(JNIEnv* pEnv, jobject thiz, jint version);
	static void SetClientModPackVersion(JNIEnv* pEnv, jobject thiz, jint modpack);
	static bool IsAPKAndGameEditionSame(JNIEnv* pEnv, jobject thiz, jint gameEdition);
};

