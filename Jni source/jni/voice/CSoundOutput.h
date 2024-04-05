#pragma once
#include <list>
#include <queue>
#include "..///vendor/openal/AL/al.h"
#include "..///vendor/openal/AL/alc.h"
#include "..///vendor/openal/AL/alext.h"
#include "..//vendor/opus/opus.h"
#include <stdint.h>

class CStreamPlayer;

class CSoundOutput
{
	friend CStreamPlayer;
	std::list<CStreamPlayer *> _streamPlayers;

	uint32_t _sampleRate;

	ALfloat listenerPos[3] = { 0.f, 0.f, 0.f };
	ALfloat listenerVel[3] = { 0.f, 0.f, 0.f };
	ALfloat listenerOri[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	ALfloat extraGain = 1.f;
	ALfloat innerConeAngle = 90.f;
	ALfloat outerConeAngle = 180.f;

	ALuint *sources = nullptr;
	ALuint _sourcesCount;
	std::queue<ALuint> freeSources;
	uint32_t bufferingTime = 0;

	ALCdevice *device;
	ALCcontext *ctx;
public:
	CSoundOutput(char* deviceName, int sampleRate, int sourcesCount);
	~CSoundOutput();

	void SetMyPosition(float x, float y, float z);
	void SetMyVelocity(float x, float y, float z);
	void SetMyOrientationFront(float x, float y, float z);
	void SetMyOrientationUp(float x, float y, float z);
	void UpdateMe();

	CStreamPlayer* CreateStreamPlayer();
	void DeleteStreamPlayer(CStreamPlayer* streamPlayer);
	void SetBufferingTime(unsigned int timeMS);
	void SetExtraGain(float gain);
	void SetVolume(float volume);
private:
	void FreeSource(ALuint source);
	bool GetSource(ALuint& source);
};

