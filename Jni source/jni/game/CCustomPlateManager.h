#pragma once

#include "CRenderTarget.h"
#include "..//gui/CFontInstance.h"
#include "..//gui/CFontRenderer.h"
#include <list>

struct SCustomPlate
{
	uint16_t vehicleId;
	uint32_t dwType;
	char szNumber[30];
	char szRegion[10];
};

#define PLATE_BITMAP_WIDTH	512
#define PLATE_BITMAP_HEIGHT 128

class CCustomPlateManager
{
	static CRenderTarget* m_pRenderTarget;
	static std::list<SCustomPlate> m_Plates;

	static struct CSprite2d* m_pRuSprite;
	static struct CSprite2d* m_pUaSprite;
	static struct CSprite2d* m_pBuSprite;
	static struct CSprite2d* m_pKzSprite;
	static struct CSprite2d* m_pRuPoliceSprite;

	static struct CSprite2d* m_pTaxiSprite;
	static struct CSprite2d* m_pRuGosSprite;
	static struct CSprite2d* m_pNoPlateSprite;

	static CFontInstance* m_pRUFont;
	static CFontInstance* m_pKZFont;
	static CFontInstance* m_pUAFont;

	static uint8_t* m_pBitmap;

	static struct RwTexture* ProcessRuPlate(SCustomPlate& plate);
	static struct RwTexture* ProcessUaPlate(SCustomPlate& plate);
	static struct RwTexture* ProcessBuPlate(SCustomPlate& plate);
	static struct RwTexture* ProcessKzPlate(SCustomPlate& plate);
	static struct RwTexture* ProcessRuPolicePlate(SCustomPlate& plate);

	static struct RwTexture* ProcessTaxiPlate(SCustomPlate& plate);
	static struct RwTexture* ProcessRuGosPlate(SCustomPlate& plate);
	static struct RwTexture* ProcessNoPlatePlate(SCustomPlate& plate);

public:
	static void Initialise();	// call this once in CGame__Process
	static void Shutdown();

	static void PushPlate(uint16_t vehicleId, uint32_t dwType, char* szNumber, char* szRegion);

	static void Process();
};