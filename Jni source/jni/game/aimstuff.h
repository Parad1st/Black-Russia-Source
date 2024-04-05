#pragma once

typedef struct _CAMERA_AIM
{
	float f1x, f1y, f1z;
	float pos1x, pos1y, pos1z;
	float pos2x, pos2y, pos2z;
	float f2x, f2y, f2z;
} CAMERA_AIM;

extern uint16_t * pbyteCameraMode;
extern uint16_t* wCameraMode2;

void GameAimSyncInit();
void GameStoreLocalPlayerAim();
void GameSetLocalPlayerAim();
CAMERA_AIM * GameGetInternalAim();
void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim);
void GameSetRemotePlayerAim(int iPlayer);
CAMERA_AIM * GameGetRemotePlayerAim(int iPlayer);

void GameSetPlayerCameraMode(uint16_t byteMode, int bytePlayerID);
uint16_t GameGetPlayerCameraMode(int bytePlayerID);
uint16_t GameGetLocalPlayerCameraMode();

void GameSetLocalPlayerCameraMode(uint16_t mode);
void GameStoreLocalPlayerCameraExtZoom();
void GameSetLocalPlayerCameraExtZoom();
void GameSetPlayerCameraExtZoom(int bytePlayerID, float fZoom);
void GameSetRemotePlayerCameraExtZoom(int bytePlayerID);
float GameGetLocalPlayerCameraExtZoom();