#pragma once
#include <mutex>
#include <queue>
#include <chrono>

#include "..///vendor/openal/AL/al.h"
#include "..///vendor/openal/AL/alc.h"
#include "..///vendor/openal/AL/alext.h"
#include "..//vendor/opus/opus.h"

#include "CRingBuffer.h"
#include "CSoundOutput.h"

#define NUM_BUFFERS 16
#define MIN_BUFFERS_TO_PLAY 8
#define RING_BUFFER_SIZE 262144
#define OPUS_BUFFER_SIZE 8196

using Sample = ALshort;

class CStreamPlayer
{
	friend CSoundOutput;
	static CSoundOutput* soundOutput;
	static ALfloat zeroFloatVector[3];

	RingBuffer<Sample, RING_BUFFER_SIZE> ringBuffer;

	ALuint buffers[NUM_BUFFERS];
	ALuint source;
	ALuint buffersFilled = 0;

	ALenum format = AL_FORMAT_MONO_FLOAT32;

	ALfloat pitch = 1.f;
	ALfloat gain = 1.f;
	ALfloat currentPos[3] = { 0.f, 0.f, 0.f };
	ALfloat currentVel[3] = { 0.f, 0.f, 0.f };
	ALfloat currentDir[3] = { 0.f, 0.f, 0.f };
	ALfloat minDistance = 0.f;
	ALfloat maxDistance = 100.f;
	ALfloat rolloffFactor = 1.f;
	ALfloat extraGain = 0.f;

	OpusDecoder* dec = nullptr;

	std::chrono::time_point<std::chrono::system_clock> lastSourceRequestTime;

	std::queue<ALuint> freeBuffers;
	bool isPlaying = false;
	bool hasSource = false;
	bool sourceUsedOnce = false;

	bool disableSpatial = false;
	int buffersCapacity = 0;
public:
	CStreamPlayer();
	~CStreamPlayer();

	bool PushOpusBuffer(const void * data, int count);

	void SetPosition(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	void SetDirection(float x, float y, float z);

	void SetMaxDistance(float distance);
	void SetMinDistance(float distance);
	void SetRolloffFactor(float rolloff);
	void SetSpatialSoundState(bool state);
	void SetExtraGain(float gain);
	bool IsPlaying();

	void Mute();
	void UnMute();
	void SetVolume(float gain);

	bool Update();

private:
	bool UpdateSource(ALuint source);
	void DropSource();
};

