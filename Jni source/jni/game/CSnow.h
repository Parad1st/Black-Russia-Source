#pragma once

class CSnow
{
	static uint32_t m_dwParticeHandle1;
	static uint32_t m_dwParticeHandle2;
	static uint32_t m_dwLastTickCreated;
	static int m_iCurrentSnow;
public:
	static void Initialise();
	static void Process(CPlayerPed* pPed, int iInterior);
	static void SetCurrentSnow(int iSnow);
	static int GetCurrentSnow();
};