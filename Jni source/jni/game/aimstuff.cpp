#include "../main.h"
#include "game.h"

CAMERA_AIM* GameGetInternalAim()
{
	return (CAMERA_AIM*)(g_libGTASA + 0x008B0808 + 528 * *((unsigned char*)(g_libGTASA + 0x008B085F)) + 728);
}

CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[MAX_PLAYERS];
uint16_t * pbyteCameraMode = nullptr;
uint16_t* wCameraMode2 = nullptr;

CAMERA_AIM * pcaInternalAim = nullptr;
float * pfCameraExtZoom = nullptr;

float fCameraExtZoom[MAX_PLAYERS];
float fLocalCameraExtZoom;

uint16_t byteCameraMode[MAX_PLAYERS];

//----------------------------------------------------------

void GameStoreLocalPlayerCameraExtZoom()
{
	fLocalCameraExtZoom = *pfCameraExtZoom;
}

//----------------------------------------------------------

void GameSetLocalPlayerCameraExtZoom()
{
	*pfCameraExtZoom = fLocalCameraExtZoom;
}

//----------------------------------------------------------

void GameSetPlayerCameraExtZoom(int bytePlayerID, float fZoom)
{
	fCameraExtZoom[bytePlayerID] = fZoom;
}

//----------------------------------------------------------

float GameGetPlayerCameraExtZoom(int bytePlayerID)
{
	return fCameraExtZoom[bytePlayerID];
}

//----------------------------------------------------------

float GameGetLocalPlayerCameraExtZoom()
{
	float value = ((*pfCameraExtZoom) - 35.0f) / 35.0f;	// normalize for 35.0 to 70.0
	return value;
}

//----------------------------------------------------------

void GameSetRemotePlayerCameraExtZoom(int bytePlayerID)
{
	float fZoom = fCameraExtZoom[bytePlayerID];
	float fValue = fZoom * 35.0f + 35.0f; // unnormalize for 35.0 to 70.0
	*pfCameraExtZoom = fValue;
}

//----------------------------------------------------------

void GameSetPlayerCameraMode(uint16_t byteMode, int bytePlayerID)
{
	byteCameraMode[bytePlayerID] = byteMode;
}

//----------------------------------------------------------

uint16_t GameGetPlayerCameraMode(int bytePlayerID)
{
	return byteCameraMode[bytePlayerID];
}

//----------------------------------------------------------

uint16_t GameGetLocalPlayerCameraMode()
{
	if (!pbyteCameraMode) return 0;
	return *pbyteCameraMode;
}

void GameSetLocalPlayerCameraMode(uint16_t mode)
{
	if (!pbyteCameraMode) return;
	*pbyteCameraMode = mode;
}

//----------------------------------------------------------

void GameAimSyncInit()
{
	pbyteCameraMode = (uint16_t*)(g_libGTASA + 0x008B0808 + 528 * *((unsigned char*)(g_libGTASA + 0x008B085F)) + 382);
	pcaInternalAim = GameGetInternalAim();
	pfCameraExtZoom = (float*)(g_libGTASA + 0x008B0808 + 528 * *((unsigned char*)(g_libGTASA + 0x008B085F)) + 508);
	wCameraMode2 = (uint16_t*)(g_libGTASA + 0x008B0808 + 0x7B4);

	memset(&caLocalPlayerAim, 0, sizeof(CAMERA_AIM));
	memset(caRemotePlayerAim, 0, sizeof(CAMERA_AIM) * MAX_PLAYERS);
	memset(byteCameraMode, 4, MAX_PLAYERS * sizeof(uint16_t));
	for (int i = 0; i < MAX_PLAYERS; i++)
		fCameraExtZoom[i] = 1.0f;
}

//----------------------------------------------------------

void GameStoreLocalPlayerAim()
{
	if (!pcaInternalAim) return;
	memcpy(&caLocalPlayerAim, pcaInternalAim, sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetLocalPlayerAim()
{
	//if (!pcaInternalAim) return;
	memcpy(pcaInternalAim, &caLocalPlayerAim, sizeof(CAMERA_AIM));
	//memcpy(pInternalCamera,&SavedCam,sizeof(MATRIX4X4));
}

//----------------------------------------------------------

//----------------------------------------------------------

void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer], caAim, sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetRemotePlayerAim(int iPlayer)
{
	if (!pcaInternalAim) return;
	memcpy(pcaInternalAim, &caRemotePlayerAim[iPlayer], sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

CAMERA_AIM * GameGetRemotePlayerAim(int iPlayer)
{
	return &caRemotePlayerAim[iPlayer];
}