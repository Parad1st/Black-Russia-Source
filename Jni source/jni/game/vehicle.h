#pragma once

enum E_CUSTOM_COMPONENTS
{
	ccBumperF = 0,
	ccBumperR,
	ccFenderF,
	ccFenderR,
	ccSpoiler,
	ccExhaust,
	ccRoof,
	ccTaillights,
	ccHeadlights,
	ccDiffuser,
	ccSplitter,
	ccExtra,
	ccMax
};

enum E_HANDLING_PARAMS
{
	hpMaxSpeed,
	hpAcceleration,
	hpGear,
	hpEngineInertion,
	hpMass,
	hpMassTurn,

	hpBrakeDeceleration,
	hpTractionMultiplier,
	hpTractionLoss,
	hpTractionBias,
	hpSuspensionLowerLimit,
	hpSuspensionBias,

	hpWheelSize,

	hpMax
};

#define EXTRA_COMPONENT_BOOT			10
#define EXTRA_COMPONENT_BONNET			11
#define EXTRA_COMPONENT_BUMP_REAR		12
#define EXTRA_COMPONENT_DEFAULT_DOOR 	13
#define EXTRA_COMPONENT_WHEEL			14
#define EXTRA_COMPONENT_BUMP_FRONT		15

#define MAX_REPLACED_TEXTURES	32
#define MAX_REPLACED_TEXTURE_NAME	32

struct SReplacedTexture
{
	char szOld[MAX_REPLACED_TEXTURE_NAME];
	RwTexture* pTexture;
};

struct SCustomCarShadow
{
	RwTexture* pTexture;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	float fSizeX;
	float fSizeY;
};

void* GetSuspensionLinesFromModel(int nModelIndex, int& numWheels);
uint8_t* GetCollisionDataFromModel(int nModelIndex);

class CVehicle : public CEntity
{
public:
	CVehicle(int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, bool bSiren = false);
	~CVehicle();

	void LinkToInterior(int iInterior);
	void SetColor(int iColor1, int iColor2);

	void SetHealth(float fHealth);
	float GetHealth();

	// 0.3.7
	bool IsOccupied();

	// 0.3.7
	void SetInvulnerable(bool bInv);
	// 0.3.7
	bool IsDriverLocalPlayer();
	// 0.3.7
	bool HasSunk();

	void ProcessMarkers();

	void RemoveEveryoneFromVehicle();

	void SetWheelPopped(uint8_t bytePopped);
	void SetDoorState(int iState);
	int GetDoorState();

	void UpdateDamageStatus(uint32_t dwPanelDamage, uint32_t dwDoorDamage, uint8_t byteLightDamage);

	void AttachTrailer();
	void DetachTrailer();
	void SetTrailer(CVehicle* pTrailer);
	CVehicle* GetTrailer();

	unsigned int GetVehicleSubtype();

	void SetEngineState(int iState);
	int GetEngineState();
	void SetLightsState(int iState);
	int GetLightsState();
	void SetBootAndBonnetState(int iBoot, int iBonnet);

	void RemoveComponent(uint16_t uiComponent);

	void SetComponentVisible(uint8_t group, uint16_t components);
	void SetHandlingData(std::vector<SHandlingData>& vHandlingData);
	void ResetVehicleHandling();

	void ApplyVinyls(uint8_t bSlot1, uint8_t bSlot2);
	void ApplyToner(uint8_t bSlot, uint8_t bID);

	void ApplyTexture(const char* szTexture, const char* szNew);
	void ApplyTexture(const char* szTexture, RwTexture* pTexture);
	void RemoveTexture(const char* szOldTexture);
	bool IsRetextured(const char* szOldTexture);

	void SetHeadlightsColor(uint8_t r, uint8_t g, uint8_t b);
	void ProcessHeadlightsColor(uint8_t& r, uint8_t& g, uint8_t& b);
	void SetWheelAlignment(int iWheel, float angle);
	void SetWheelOffset(int iWheel, float offset);
	void SetWheelWidth(float fValue);
	void ProcessWheelsOffset();

	void SetCustomShadow(uint8_t r, uint8_t g, uint8_t b, float fSizeX, float fSizeY, const char* szTex);

private:
	void ProcessWheelOffset(RwFrame* pFrame, bool bLeft, float fValue, int iID);
	void SetComponentAngle(bool bUnk, int iID, float angle);

	void SetComponentVisibleInternal(const char* szComponent, bool bVisible);
	std::string GetComponentNameByIDs(uint8_t group, int subgroup);

	void CopyGlobalSuspensionLinesToPrivate();
public:
	VEHICLE_TYPE* m_pVehicle;
	bool 			m_bIsLocked;
	CVehicle* m_pTrailer;
	uint32_t		m_dwMarkerID;
	bool 			m_bIsInvulnerable;
	bool 			m_bDoorsLocked;
	uint8_t			m_byteObjectiveVehicle; // Is this a special objective vehicle? 0/1
	uint8_t			m_bSpecialMarkerEnabled;


	uint8_t			m_byteColor1;
	uint8_t			m_byteColor2;
	bool 			m_bColorChanged;

	tHandlingData* m_pCustomHandling;

	SReplacedTexture m_szReplacedTextures[MAX_REPLACED_TEXTURES];
	bool m_bReplaceTextureStatus[MAX_REPLACED_TEXTURES];
	bool m_bReplacedTexture;

	void* m_pSuspensionLines;
	bool bHasSuspensionLines;

	bool m_bHeadlightsColor;
	uint8_t m_bHeadlightsR;
	uint8_t m_bHeadlightsG;
	uint8_t m_bHeadlightsB;

	bool m_bWheelAlignmentX;
	float m_fWheelAlignmentX;

	bool m_bWheelAlignmentY;
	float m_fWheelAlignmentY;

	bool m_bWheelSize;
	float m_fWheelSize;

	bool m_bWheelWidth;
	float m_fWheelWidth;

	bool m_bWheelOffsetX;
	float m_fWheelOffsetX;

	bool m_bWheelOffsetY;
	float m_fWheelOffsetY;

	float m_fNewOffsetX;
	float m_fNewOffsetY;

	bool m_bWasWheelOffsetProcessedX;
	bool m_bWasWheelOffsetProcessedY;
	uint32_t m_uiLastProcessedWheelOffset;

	MATRIX4X4 m_vInitialWheelMatrix[4];

	SCustomCarShadow m_Shadow;
	bool m_bShadow;
	int bEngine;
	int fDoorState;
	int bLights;
};