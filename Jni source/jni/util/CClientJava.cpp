#include "CClientJava.h"
#include "..//settings.h"
#include "vendor/raknet/SHA1.h"

extern CSettings* pSettings;

uint16_t CClientInfo::usSAMPMajorVersion = SAMP_MAJOR_VERSION;
uint16_t CClientInfo::usSAMPMinorVersion = SAMP_MINOR_VERSION;
bool		CClientInfo::bSAMPModified = false;
bool		CClientInfo::bJoinedFromLauncher = false;

char CClientInfo::szSerial[0x7F];
uint16_t CClientInfo::usLauncherVersion = 0;
uint16_t CClientInfo::usModpackVersion = 0;

char CClientInfo::szIP[0xFF];
uint16_t CClientInfo::usPort = 0;
extern "C"
{
	/*JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientIp(JNIEnv* pEnv, jobject thiz, jbyteArray ip)
	{
		CClientInfo::SetClientIP(pEnv, thiz, ip);
	}

	JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientPort(JNIEnv* pEnv, jobject thiz, jint port)
	{
		CClientInfo::SetClientPort(pEnv, thiz, port);
	}

	JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientUuid(JNIEnv* pEnv, jobject thiz, jbyteArray uuid)
	{
		CClientInfo::SetClientUUID(pEnv, thiz, uuid);
	}

	JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientLauncherVersion(JNIEnv* pEnv, jobject thiz, jint version)
	{
		CClientInfo::SetClientLauncherVersion(pEnv, thiz, version);
	}
	JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientModpackVersion(JNIEnv* pEnv, jobject thiz, jint version)
	{
		CClientInfo::SetClientModPackVersion(pEnv, thiz, version);
	}
	JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientMajorVersion(JNIEnv* pEnv, jobject thiz, jint version)
	{
		CClientInfo::SetClientMajorVersion(pEnv, thiz, version);
	}
	JNIEXPORT void JNICALL Java_com_santrope_core_GTASA_setClientMinorVersion(JNIEnv* pEnv, jobject thiz, jint version)
	{
		CClientInfo::SetClientMinorVersion(pEnv, thiz, version);
	}
	JNIEXPORT jboolean JNICALL Java_com_santrope_core_GTASA_isAPAKAndLibSame(JNIEnv* pEnv, jobject thiz, jint gameEdition)
	{
		return CClientInfo::IsAPKAndGameEditionSame(pEnv, thiz, gameEdition);
	}*/
}

void CClientInfo::WriteClientInfoToBitStream(RakNet::BitStream& bs)
{
	const uint16_t usChecksum = 0x94D5;
	bs.Write(usChecksum);
	bs.Write(usSAMPMajorVersion);
	bs.Write(usSAMPMinorVersion);
	bs.Write(bSAMPModified);

	bs.Write(usLauncherVersion);
	bs.Write(usModpackVersion);

	bs.Write((uint16_t)63);
	bs.Write(szSerial, 63);

	//Log("Send: %hu %hu %hu %d %hu %hu", usChecksum, usSAMPMajorVersion, usSAMPMinorVersion, bSAMPModified, usLauncherVersion, usModpackVersion);
}

void CClientInfo::SetClientMajorVersion(JNIEnv* pEnv, jobject thiz, jint version)
{
	usSAMPMajorVersion = (uint16_t)version;
}

void CClientInfo::SetClientMinorVersion(JNIEnv* pEnv, jobject thiz, jint version)
{
	usSAMPMinorVersion = (uint16_t)version;
}

#include "crashlytics.h"

void CClientInfo::SetClientIP(JNIEnv* pEnv, jobject thiz, jbyteArray ip)
{
	if (ip == 0)
	{
		Log("IP: NULL");
		bJoinedFromLauncher = false;
		return;
	}

	jboolean isCopy = true;

	jbyte* pIP = pEnv->GetByteArrayElements(ip, &isCopy);
	jsize length = pEnv->GetArrayLength(ip);

	memset(szIP, 0, sizeof(szIP) - 1);
	memcpy(szIP, pIP, length);

	firebase::crashlytics::SetCustomKey("IP", szIP);

	pEnv->ReleaseByteArrayElements(ip, pIP, JNI_ABORT);

	bJoinedFromLauncher = true;
}

void CClientInfo::SetClientPort(JNIEnv* pEnv, jobject thiz, jint port)
{
	usPort = (uint16_t)port;
}

void CClientInfo::SetClientUUID(JNIEnv* pEnv, jobject thiz, jbyteArray uuid)
{
	if (uuid == 0)
	{
		bJoinedFromLauncher = false;
		return;
	}

	jboolean isCopy = true;

	jbyte* pUUID = pEnv->GetByteArrayElements(uuid, &isCopy);
	jsize length = pEnv->GetArrayLength(uuid);

	memset(szSerial, 0, sizeof(szSerial) - 1);

	if (length >= sizeof(szSerial) - 1)
	{
		length = sizeof(szSerial) - 2;
	}

	memcpy(szSerial, pUUID, length);

	firebase::crashlytics::SetCustomKey("UUID", szSerial);

	pEnv->ReleaseByteArrayElements(uuid, pUUID, JNI_ABORT);

	bJoinedFromLauncher = true;
}

void CClientInfo::SetClientLauncherVersion(JNIEnv* pEnv, jobject thiz, jint version)
{
	usLauncherVersion = (uint16_t)version;

	Log("Setted launcher version: %d", usLauncherVersion);
}

void CClientInfo::SetClientModPackVersion(JNIEnv* pEnv, jobject thiz, jint modpack)
{
	usModpackVersion = modpack;

	Log("Setted modpack version: %d", usModpackVersion);
}

bool CClientInfo::IsAPKAndGameEditionSame(JNIEnv* pEnv, jobject thiz, jint gameEdition)
{
	return true;
	
#ifdef GAME_EDITION_CR
	if (gameEdition == 2)
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	if (gameEdition == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
#endif
}
