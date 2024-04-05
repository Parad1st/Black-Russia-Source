#include "CSoundOutput.h"
#include "CStreamPlayer.h"
void Log(const char*, ...);
CSoundOutput::CSoundOutput(char* deviceName, int sampleRate, int sourcesCount) : _sampleRate(sampleRate)
{
	device = alcOpenDevice(deviceName);
	if (!device)
	{
		Log("Cannot open device");
	}

	ctx = alcCreateContext(device, NULL);
	if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
	{
		if (ctx != NULL)
			alcDestroyContext(ctx);
		alcCloseDevice(device);
		Log("Cannot create context");
		return;
	}

	ALCint maxMonoSources;
	alcGetIntegerv(device, ALC_MONO_SOURCES, 1, &maxMonoSources);
	if (sourcesCount > maxMonoSources)
		sourcesCount = maxMonoSources;
	Log("Max sources: %d", maxMonoSources);
	sources = new ALuint[sourcesCount];
	alGenSources(sourcesCount, sources);
	if (alGetError() != AL_NO_ERROR)
	{
		Log("Sources error");
		return;
	}
	_sourcesCount = sourcesCount;

	for (uint8_t i = 0; i < sourcesCount; ++i)
		freeSources.push(sources[i]);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	CStreamPlayer::soundOutput = this;
}


CSoundOutput::~CSoundOutput()
{
	for (CStreamPlayer* p : _streamPlayers)
		delete p;
	_streamPlayers.clear();

	alDeleteSources(_sourcesCount, sources);
	delete[] sources;

	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);
	alcCloseDevice(device);
}

void CSoundOutput::SetMyPosition(float x, float y, float z)
{
	listenerPos[0] = x;
	listenerPos[1] = y;
	listenerPos[2] = z;
}

void CSoundOutput::SetMyVelocity(float x, float y, float z)
{
	listenerVel[0] = x;
	listenerVel[1] = y;
	listenerVel[2] = z;
}

void CSoundOutput::SetMyOrientationFront(float x, float y, float z)
{
	listenerOri[0] = x;
	listenerOri[1] = y;
	listenerOri[2] = z;
}

void CSoundOutput::SetMyOrientationUp(float x, float y, float z)
{
	listenerOri[3] = x;
	listenerOri[4] = y;
	listenerOri[5] = z;
}

void CSoundOutput::UpdateMe()
{
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

CStreamPlayer* CSoundOutput::CreateStreamPlayer()
{
	CStreamPlayer* nextStreamPlayer = new CStreamPlayer();
	_streamPlayers.push_back(nextStreamPlayer);
	return nextStreamPlayer;
}

void CSoundOutput::DeleteStreamPlayer(CStreamPlayer* streamPlayer)
{
	_streamPlayers.remove(streamPlayer);
	delete streamPlayer;
}

void CSoundOutput::SetBufferingTime(unsigned int timeMS)
{
	bufferingTime = timeMS;
}

void CSoundOutput::SetExtraGain(float gain)
{
	extraGain = gain;
}

void CSoundOutput::SetVolume(float volume)
{
	alListenerf(AL_GAIN, volume);
}

void CSoundOutput::FreeSource(ALuint source)
{
	freeSources.push(source);
}

bool CSoundOutput::GetSource(ALuint & source)
{
	if(!freeSources.size())
		return false;

	source = freeSources.front();
	freeSources.pop();
	return true;
}
