#include "main.h"
#include "game/game.h"

#include <pthread.h>
#include <mutex>
#include <chrono>
#include <thread>

#include "vendor/bass/bass.h"
#include "CAudioStream.h"

extern CGame* pGame;

CAudioStream::CAudioStream()
{
	m_strAudioStreamURL.clear();

	m_fAudioStreamRadius = -1;
	m_bassStreamSample = NULL;

	m_bUsePosition = false;
	m_bPlaying = false;
	m_bPaused = false;

	memset(&m_vecAudioStreamPosition, 0, sizeof(m_vecAudioStreamPosition));
}

CAudioStream::~CAudioStream()
{
	if (m_bPlaying) Stop();
	m_strAudioStreamURL.clear();
}

bool CAudioStream::PlayByURL(const char* url, float fX, float fY, float fZ, float fRadius, bool bUsePosition)
{
	if (m_bPlaying && m_bassStreamSample)
	{
		if (!Stop())
			return false;
	}

	m_strAudioStreamURL = url;

	m_vecAudioStreamPosition.X = fX;
	m_vecAudioStreamPosition.Y = fY;
	m_vecAudioStreamPosition.Z = fZ;

	m_fAudioStreamRadius = fRadius;
	m_bUsePosition = bUsePosition;

	m_bPaused = false;

	m_bassStreamSample = BASS_StreamCreateURL(m_strAudioStreamURL.c_str(), 0, 9699328, nullptr, nullptr);
	if (m_bassStreamSample)
	{
		BASS_ChannelPlay(m_bassStreamSample, true);
		m_bPlaying = true;
	}

	return true;
}

bool CAudioStream::Stop()
{
	if (m_bPlaying && m_bassStreamSample)
	{
		BASS_StreamFree(m_bassStreamSample);
		m_bassStreamSample = NULL;
		m_bPlaying = false;
	}
	return true;
}

void CAudioStream::Process()
{
	if (!m_bPlaying) return;

	if (!m_bPaused)
	{
		if (pGame->IsGamePaused())
		{
			if (m_bassStreamSample)
			{
				BASS_ChannelPause(m_bassStreamSample);
				m_bPaused = true;
			}
		}
		else if (m_bUsePosition && (m_fAudioStreamRadius != -1))
		{
			CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
			if (pPlayerPed)
			{
				MATRIX4X4 playerMatrix;
				pPlayerPed->GetMatrix(&playerMatrix);

				if (GetDistanceBetween3DPoints(&m_vecAudioStreamPosition, &playerMatrix.pos) > m_fAudioStreamRadius)
				{
					if (m_bassStreamSample)
					{
						BASS_ChannelPause(m_bassStreamSample);
						m_bPaused = true;
					}
				}
			}
		}
	}
	else
	{
		if (!pGame->IsGamePaused())
		{
			if (m_bassStreamSample)
			{
				BASS_ChannelPlay(m_bassStreamSample, false);
				m_bPaused = false;
			}
		}
		else if (m_bUsePosition && (m_fAudioStreamRadius != -1))
		{
			CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
			if (pPlayerPed)
			{
				MATRIX4X4 playerMatrix;
				pPlayerPed->GetMatrix(&playerMatrix);

				if (GetDistanceBetween3DPoints(&m_vecAudioStreamPosition, &playerMatrix.pos) <= m_fAudioStreamRadius)
				{
					if (m_bassStreamSample)
					{
						BASS_ChannelPlay(m_bassStreamSample, false);
						m_bPaused = false;
					}
				}
			}
		}
	}
}