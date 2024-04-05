#pragma once

#define MAX_VEHICLE_MODELS (212)

class CHandlingDefault
{
	static tHandlingData m_aDefaultModelHandlings[MAX_VEHICLE_MODELS];
public:
	static void GetDefaultHandling(uint16_t usHandlingID, tHandlingData* tDest);

	static void FillDefaultHandling(uint16_t usHandlingID, tHandlingData* pSrc);
};