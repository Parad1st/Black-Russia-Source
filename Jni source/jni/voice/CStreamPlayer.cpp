#include "CStreamPlayer.h"
#include <iostream>

CSoundOutput* CStreamPlayer::soundOutput = nullptr;
ALfloat CStreamPlayer::zeroFloatVector[3] = { 0.f, 0.f, 0.f };
void Log(const char*, ...);

CStreamPlayer::CStreamPlayer()
{
	alGenBuffers(NUM_BUFFERS, buffers);
	for (uint16_t i = 0; i < NUM_BUFFERS; ++i)
		freeBuffers.push(buffers[i]);

	if (alGetError() != AL_NO_ERROR)
	{
		Log("error CStreamPlayer");
		return;
	}
	hasSource = false;
	int opusErr;
	dec = opus_decoder_create(soundOutput->_sampleRate, 1, &opusErr);
	if (opusErr != OPUS_OK || dec == NULL)
	{
		Log("Cannot create opus decoder");
		return;
	}
}


CStreamPlayer::~CStreamPlayer()
{
	if(hasSource)
		soundOutput->FreeSource(source);
		
	alDeleteBuffers(NUM_BUFFERS, buffers);
	if (dec)
	{
		opus_decoder_destroy(dec);
	}
}

bool CStreamPlayer::PushOpusBuffer(const void * data, int count)
{
	if (!dec) return true;
	Sample out[OPUS_BUFFER_SIZE];
	int frame_size = opus_decode(dec, (const unsigned char*)data, count, out, OPUS_BUFFER_SIZE, 0);
	if (frame_size < 0)
		return false;

	ALfloat finalGain = extraGain + soundOutput->extraGain;
	if (finalGain != 1.f)
	{
		for (int i = 0; i < frame_size; ++i)
			out[i] *= finalGain;
	}
	

	ringBuffer.Write(out, frame_size);
	return true;
}

void CStreamPlayer::SetPosition(float x, float y, float z)
{
	currentPos[0] = x;
	currentPos[1] = y;
	currentPos[2] = z;

	if(hasSource && !disableSpatial)
		alSourcefv(source, AL_POSITION, currentPos);
}

void CStreamPlayer::SetVelocity(float x, float y, float z)
{
	currentVel[0] = x;
	currentVel[1] = y;
	currentVel[2] = z;

	if (hasSource && !disableSpatial)
		alSourcefv(source, AL_VELOCITY, currentVel);
}

void CStreamPlayer::SetDirection(float x, float y, float z)
{
	currentDir[0] = x;
	currentDir[1] = y;
	currentDir[2] = z;

	if (hasSource && !disableSpatial)
		alSourcefv(source, AL_DIRECTION, currentDir);
}

void CStreamPlayer::SetMaxDistance(float distance)
{
	maxDistance = distance;
	if(hasSource && !disableSpatial)
		alSourcef(source, AL_MAX_DISTANCE, distance);
}

void CStreamPlayer::SetMinDistance(float distance)
{
	minDistance = distance;
	if(hasSource && !disableSpatial)
		alSourcef(source, AL_REFERENCE_DISTANCE, distance);
}

void CStreamPlayer::SetRolloffFactor(float rolloff)
{
	rolloffFactor = rolloff;
	if(hasSource && !disableSpatial)
		alSourcef(source, AL_ROLLOFF_FACTOR, rolloff);
}

void CStreamPlayer::SetSpatialSoundState(bool state)
{
	if (disableSpatial == !state)
		return;

	disableSpatial = !state;
	if (disableSpatial && hasSource)
	{
		alSourcefv(source, AL_POSITION, zeroFloatVector);
		alSourcefv(source, AL_VELOCITY, zeroFloatVector);
		alSourcefv(source, AL_DIRECTION, zeroFloatVector);
		alSourcef(source, AL_MAX_DISTANCE, 100.f);
		alSourcef(source, AL_REFERENCE_DISTANCE, 100.f);
		alSourcef(source, AL_ROLLOFF_FACTOR, 1.f);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	}
	else if(hasSource)
	{
		alSourcefv(source, AL_POSITION, currentPos);
		alSourcefv(source, AL_VELOCITY, currentVel);
		alSourcefv(source, AL_DIRECTION, currentDir);
		alSourcef(source, AL_MAX_DISTANCE, maxDistance);
		alSourcef(source, AL_REFERENCE_DISTANCE, minDistance);
		alSourcef(source, AL_ROLLOFF_FACTOR, rolloffFactor);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
	}
}

void CStreamPlayer::SetExtraGain(float gain)
{
	extraGain = gain;
}

bool CStreamPlayer::IsPlaying()
{
	return isPlaying;
}

void CStreamPlayer::Mute()
{
	if (hasSource && !disableSpatial)
		alSourcef(source, AL_GAIN, 0.0f);
}

void CStreamPlayer::UnMute()
{
	if (hasSource && !disableSpatial)
		alSourcef(source, AL_GAIN, 1.0f);
}

void CStreamPlayer::SetVolume(float gain)
{
	if (hasSource && !disableSpatial)
		alSourcef(source, AL_GAIN, gain);
}

bool CStreamPlayer::Update()
{
	if (!hasSource)
	{
		if (!ringBuffer.BytesToRead())
			return true;

		//TODO: Remove sources from far objects if all busy
		if (!soundOutput->GetSource(source))
			return false;
		hasSource = true;

		lastSourceRequestTime = std::chrono::system_clock::now();
		sourceUsedOnce = false;

		if (!UpdateSource(source))
		{
			DropSource();
			return false;
		}
		isPlaying = false;
	}

	ALint processed, state;

	//Unqueue old buffers
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if (alGetError() != AL_NO_ERROR)
		return false;

	isPlaying = state == AL_PLAYING;

	alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		DropSource();
		return false;
	}

	if (!ringBuffer.BytesToRead() && !isPlaying && sourceUsedOnce)
	{
		while (processed > 0)
		{
			ALuint bufid;
			alSourceUnqueueBuffers(source, 1, &bufid);
			freeBuffers.push(bufid);
			processed--;
		}

		DropSource();
		return true;
	}

	if (ringBuffer.IsHalfFull())
		alSourcef(source, AL_PITCH, 1.05f);
	else
		alSourcef(source, AL_PITCH, 1.f);

	if (alGetError() != AL_NO_ERROR)
	{
		DropSource();
		return false;
	}

	while (processed > 0)
	{
		ALuint bufid;
		alSourceUnqueueBuffers(source, 1, &bufid);
		freeBuffers.push(bufid);
		processed--;
	}
	
	while (ringBuffer.BytesToRead() && freeBuffers.size())
	{
		Sample tempBuffer[4096];
		const ALuint bufferId = freeBuffers.front();
		freeBuffers.pop();

		ALsizei readed = 0;
		int bufferIndex = 0;
		readed = (ALsizei)ringBuffer.Read(tempBuffer, 4096);
		if (readed > 0)
		{
			alBufferData(bufferId, AL_FORMAT_MONO16, tempBuffer, readed * sizeof(Sample), soundOutput->_sampleRate);
			if (alGetError() != AL_NO_ERROR)
			{
				DropSource();
				return false;
			}

			alSourceQueueBuffers(source, 1, &bufferId);
			if (alGetError() != AL_NO_ERROR)
			{
				DropSource();
				return false;
			}
			buffersCapacity++;
		}
		else
			break;
	}

	if (!isPlaying && buffersCapacity >= 10)
	{
		auto currentTime = std::chrono::system_clock::now();
		auto timeFromFirstBuffer = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSourceRequestTime).count();
		if (timeFromFirstBuffer > soundOutput->bufferingTime)
		{
			alSourcePlay(source);
			buffersCapacity = 0;
			sourceUsedOnce = true;
			if (alGetError() != AL_NO_ERROR)
			{
				DropSource();
				return false;
			}
		}
	}
	return true;
}

//TODO: Refactor
bool CStreamPlayer::UpdateSource(ALuint source)
{
	alSourceRewind(source);
	alSourcei(source, AL_LOOPING, false);
	alSourcei(source, AL_BUFFER, 0);

	if (!disableSpatial)
	{
		alSourcefv(source, AL_POSITION, currentPos);
		alSourcefv(source, AL_VELOCITY, currentVel);
		alSourcefv(source, AL_DIRECTION, currentDir);
		alSourcef(source, AL_MAX_DISTANCE, maxDistance);
		alSourcef(source, AL_REFERENCE_DISTANCE, minDistance);
		alSourcef(source, AL_ROLLOFF_FACTOR, rolloffFactor);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
	}
	else
	{
		alSourcefv(source, AL_POSITION, zeroFloatVector);
		alSourcefv(source, AL_VELOCITY, zeroFloatVector);
		alSourcefv(source, AL_DIRECTION, zeroFloatVector);
		alSourcef(source, AL_MAX_DISTANCE, 100.f);
		alSourcef(source, AL_REFERENCE_DISTANCE, 100.f);
		alSourcef(source, AL_ROLLOFF_FACTOR, 1.f);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	}

	if (alGetError() != AL_NO_ERROR)
		return false;
	return true;
}

void CStreamPlayer::DropSource()
{
	if (hasSource)
	{
		hasSource = false;
		alSourceStop(source);
		soundOutput->FreeSource(source);
	}
}
