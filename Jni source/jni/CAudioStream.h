#pragma once

#include <atomic>

class CAudioStream
{
private:
	std::string					m_strAudioStreamURL;
	VECTOR						m_vecAudioStreamPosition;
	float						m_fAudioStreamRadius;

	bool						m_bUsePosition;
	HSTREAM						m_bassStreamSample;

	bool						m_bPlaying;
	bool						m_bPaused;

public:
	CAudioStream();
	~CAudioStream();

	bool PlayByURL(const char* url, float fX, float fY, float fZ, float fRadius, bool bUsePosition);
	bool Stop();
	void Process();

	void AsyncAudioStreamSampleProcessing();
};