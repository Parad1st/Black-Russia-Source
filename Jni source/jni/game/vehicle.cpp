#include "../main.h"
#include "game.h"

extern CGame* pGame;
#include "..//debug.h"

#include "..//chatwindow.h"
extern CChatWindow* pChatWindow;

CVehicle::CVehicle(int iType, float fPosX, float fPosY, float fPosZ, float fRotation, bool bSiren)
{
	uint32_t tickStart = GetTickCount();

	CDebugInfo::uiStreamedVehicles++;
	MATRIX4X4 mat;
	uint32_t dwRetID = 0;

	m_pCustomHandling = nullptr;

	m_pVehicle = nullptr;
	m_dwGTAId = 0;
	m_pTrailer = nullptr;

	if ((iType != TRAIN_PASSENGER_LOCO) &&
		(iType != TRAIN_FREIGHT_LOCO) &&
		(iType != TRAIN_PASSENGER) &&
		(iType != TRAIN_FREIGHT) &&
		(iType != TRAIN_TRAM))
	{
		// normal vehicle
		if (!pGame->IsModelLoaded(iType))
		{
			pGame->RequestModel(iType);
			pGame->LoadRequestedModels();
			while (!pGame->IsModelLoaded(iType)) usleep(10);
		}
		if (bSiren)
		{
			// ��������
		}
		else
		{

		}
		ScriptCommand(&create_car, iType, fPosX, fPosY, fPosZ, &dwRetID);
		ScriptCommand(&set_car_z_angle, dwRetID, fRotation);
		ScriptCommand(&car_gas_tank_explosion, dwRetID, 0);
		ScriptCommand(&set_car_hydraulics, dwRetID, 0);
		ScriptCommand(&toggle_car_tires_vulnerable, dwRetID, 0);
		m_pVehicle = (VEHICLE_TYPE*)GamePool_Vehicle_GetAt(dwRetID);
		m_pEntity = (ENTITY_TYPE*)m_pVehicle;
		m_dwGTAId = dwRetID;

		if (m_pVehicle)
		{
			m_pVehicle->dwDoorsLocked = 0;
			m_bIsLocked = false;

			GetMatrix(&mat);
			mat.pos.X = fPosX;
			mat.pos.Y = fPosY;
			mat.pos.Z = fPosZ;

			if (GetVehicleSubtype() != VEHICLE_SUBTYPE_BIKE &&
				GetVehicleSubtype() != VEHICLE_SUBTYPE_PUSHBIKE)
				mat.pos.Z += 0.25f;

			SetMatrix(mat);
		}
	}
	else if ((iType == TRAIN_PASSENGER_LOCO) ||
		(iType == TRAIN_FREIGHT_LOCO) ||
		(iType == TRAIN_TRAM))
	{
		// train locomotives
	}
	else if ((iType == TRAIN_PASSENGER) ||
		iType == TRAIN_FREIGHT)
	{

	}

	m_byteObjectiveVehicle = 0;
	m_bSpecialMarkerEnabled = false;
	m_bDoorsLocked = false;
	m_dwMarkerID = 0;
	m_bIsInvulnerable = false;
	uint8_t defComp = 0;
	BIT_SET(defComp, 0);
	for (int i = 0; i < E_CUSTOM_COMPONENTS::ccMax; i++)
	{
		if (i == E_CUSTOM_COMPONENTS::ccExtra)
		{
			uint16_t defComp_extra = 0;
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BOOT);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BONNET);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_DEFAULT_DOOR);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_WHEEL);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BUMP_REAR);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BUMP_FRONT);
			SetComponentVisible(i, defComp_extra);
		}
		else
		{
			SetComponentVisible(i, (uint16_t)defComp);
		}
	}

	for (size_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
	{
		m_bReplaceTextureStatus[i] = false;
		memset(&(m_szReplacedTextures[i].szOld[0]), 0, MAX_REPLACED_TEXTURE_NAME);
		m_szReplacedTextures[i].pTexture = nullptr;
	}

	m_bReplacedTexture = false;

	bHasSuspensionLines = false;
	m_pSuspensionLines = nullptr;
	if (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		CopyGlobalSuspensionLinesToPrivate();
	}

	m_bHeadlightsColor = false;
	m_bWheelSize = false;
	m_bWheelWidth = false;
	m_bWheelAlignmentX = false;
	m_bWheelAlignmentY = false;

	m_bWheelOffsetX = false;
	m_bWheelOffsetY = false;
	m_fWheelOffsetX = 0.0f;
	m_fWheelOffsetY = 0.0f;
	m_fNewOffsetX = 0.0f;
	m_fNewOffsetY = 0.0f;
	m_bWasWheelOffsetProcessedX = true;
	m_bWasWheelOffsetProcessedY = true;
	m_uiLastProcessedWheelOffset = 0;

	m_bShadow = false;
	m_Shadow.pTexture = nullptr;

	RwFrame* pWheelLF = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_lf_dummy"); // GetFrameFromname
	RwFrame* pWheelRF = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_rf_dummy"); // GetFrameFromname
	RwFrame* pWheelRB = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_rb_dummy"); // GetFrameFromname
	RwFrame* pWheelLB = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_lb_dummy"); // GetFrameFromname

	if (pWheelLF && pWheelRF && pWheelRB && pWheelLB)
	{
		memcpy(&m_vInitialWheelMatrix[0], (const void*)&(pWheelLF->modelling), sizeof(MATRIX4X4));
		memcpy(&m_vInitialWheelMatrix[1], (const void*)&(pWheelRF->modelling), sizeof(MATRIX4X4));
		memcpy(&m_vInitialWheelMatrix[2], (const void*)&(pWheelRB->modelling), sizeof(MATRIX4X4));
		memcpy(&m_vInitialWheelMatrix[3], (const void*)&(pWheelLB->modelling), sizeof(MATRIX4X4));
	}

	//pChatWindow->AddDebugMessage("time for CVehicle::CVehicle %d", GetTickCount() - tickStart);
}

CVehicle::~CVehicle()
{
	CDebugInfo::uiStreamedVehicles--;
	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);

	m_bReplacedTexture = false;

	for (size_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
	{
		if (m_bReplaceTextureStatus[i] && m_szReplacedTextures[i].pTexture)
		{
			m_bReplaceTextureStatus[i] = false;
			RwTextureDestroy(m_szReplacedTextures[i].pTexture);
		}

		m_bReplaceTextureStatus[i] = false;
		memset(&(m_szReplacedTextures[i].szOld[0]), 0, MAX_REPLACED_TEXTURE_NAME);
		m_szReplacedTextures[i].pTexture = nullptr;
	}

	if (bHasSuspensionLines && m_pSuspensionLines)
	{
		delete[] m_pSuspensionLines;
		m_pSuspensionLines = nullptr;
		bHasSuspensionLines = false;
	}

	if (m_bShadow)
	{
		if (m_Shadow.pTexture)
		{
			RwTextureDestroy(m_Shadow.pTexture);
			m_Shadow.pTexture = nullptr;
		}
	}

	if (m_pVehicle)
	{
		if (m_dwMarkerID)
		{
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_dwMarkerID = 0;
		}

		RemoveEveryoneFromVehicle();

		if (m_pTrailer)
		{
			// detach trailer (��������)
		}

		// ��� ��� �����-�� ���������� �����

		if (m_pCustomHandling)
		{
			delete m_pCustomHandling;
			m_pCustomHandling = nullptr;
		}

		if (m_pVehicle->entity.nModelIndex == TRAIN_PASSENGER_LOCO ||
			m_pVehicle->entity.nModelIndex == TRAIN_FREIGHT_LOCO)
		{
			ScriptCommand(&destroy_train, m_dwGTAId);
		}
		else
		{
			int nModelIndex = m_pVehicle->entity.nModelIndex;
			ScriptCommand(&destroy_car, m_dwGTAId);

			if (!GetModelReferenceCount(nModelIndex) &&
				//!m_bKeepModelLoaded &&
				//(pGame->GetVehicleModelsCount() > 80) &&
				pGame->IsModelLoaded(nModelIndex))
			{
				// CStreaming::RemoveModel
				((void (*)(int))(g_libGTASA + 0x290C4C + 1))(nModelIndex);
			}
		}
	}
}

void CVehicle::LinkToInterior(int iInterior)
{
	if (GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		ScriptCommand(&link_vehicle_to_interior, m_dwGTAId, iInterior);
	}
}

void CVehicle::SetColor(int iColor1, int iColor2)
{
	if (iColor1 >= 256 || iColor1 < 0)
	{
		iColor1 = 0;
	}
	if (iColor2 >= 256 || iColor2 < 0)
	{
		iColor2 = 0;
	}
	if (m_pVehicle)
	{
		if (GamePool_Vehicle_GetAt(m_dwGTAId))
		{
			m_pVehicle->byteColor1 = (uint8_t)iColor1;
			m_pVehicle->byteColor2 = (uint8_t)iColor2;
		}
	}

	m_byteColor1 = (uint8_t)iColor1;
	m_byteColor2 = (uint8_t)iColor2;
	m_bColorChanged = true;
}

#include "..//net/netgame.h"
extern CNetGame* pNetGame;
void CVehicle::AttachTrailer()
{
	if (m_pTrailer)
	{
		ScriptCommand(&put_trailer_on_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
	}
}

//-----------------------------------------------------------

void CVehicle::DetachTrailer()
{
	if (m_pTrailer)
	{
		ScriptCommand(&detach_trailer_from_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
	}
}

//-----------------------------------------------------------

void CVehicle::SetTrailer(CVehicle* pTrailer)
{
	m_pTrailer = pTrailer;
}

//-----------------------------------------------------------

CVehicle* CVehicle::GetTrailer()
{
	if (!m_pVehicle) return NULL;

	// Try to find associated trailer
	uint32_t dwTrailerGTAPtr = m_pVehicle->dwTrailer;

	if (pNetGame && dwTrailerGTAPtr) {
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		VEHICLEID TrailerID = (VEHICLEID)pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE*)dwTrailerGTAPtr);
		if (TrailerID < MAX_VEHICLES && pVehiclePool->GetSlotState(TrailerID)) {
			return pVehiclePool->GetAt(TrailerID);
		}
	}

	return NULL;
}

void CVehicle::SetHealth(float fHealth)
{
	if (m_pVehicle)
	{
		m_pVehicle->fHealth = fHealth;
	}
}

float CVehicle::GetHealth()
{
	if (m_pVehicle) return m_pVehicle->fHealth;
	else return 0.0f;
}

// 0.3.7
void CVehicle::SetInvulnerable(bool bInv)
{
	if (!m_pVehicle) return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;
	if (m_pVehicle->entity.vtable == g_libGTASA + 0x5C7358) return;

	if (bInv)
	{
		ScriptCommand(&set_car_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
		ScriptCommand(&toggle_car_tires_vulnerable, m_dwGTAId, 0);
		m_bIsInvulnerable = true;
	}
	else
	{
		ScriptCommand(&set_car_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
		ScriptCommand(&toggle_car_tires_vulnerable, m_dwGTAId, 1);
		m_bIsInvulnerable = false;
	}
}

// 0.3.7
bool CVehicle::IsDriverLocalPlayer()
{
	if (m_pVehicle)
	{
		if ((PED_TYPE*)m_pVehicle->pDriver == GamePool_FindPlayerPed())
			return true;
	}

	return false;
}

// 0.3.7
bool CVehicle::HasSunk()
{
	if (!m_pVehicle) return false;
	return ScriptCommand(&has_car_sunk, m_dwGTAId);
}

void CVehicle::RemoveEveryoneFromVehicle()
{
	if (!m_pVehicle) return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	float fPosX = m_pVehicle->entity.mat->pos.X;
	float fPosY = m_pVehicle->entity.mat->pos.Y;
	float fPosZ = m_pVehicle->entity.mat->pos.Z;

	int iPlayerID = 0;
	if (m_pVehicle->pDriver)
	{
		iPlayerID = GamePool_Ped_GetIndex(m_pVehicle->pDriver);
		ScriptCommand(&remove_actor_from_car_and_put_at, iPlayerID, fPosX, fPosY, fPosZ + 2.0f);
	}

	for (int i = 0; i < 7; i++)
	{
		if (m_pVehicle->pPassengers[i] != nullptr)
		{
			iPlayerID = GamePool_Ped_GetIndex(m_pVehicle->pPassengers[i]);
			ScriptCommand(&remove_actor_from_car_and_put_at, iPlayerID, fPosX, fPosY, fPosZ + 2.0f);
		}
	}
}

// 0.3.7
bool CVehicle::IsOccupied()
{
	if (m_pVehicle)
	{
		if (m_pVehicle->pDriver) return true;
		if (m_pVehicle->pPassengers[0]) return true;
		if (m_pVehicle->pPassengers[1]) return true;
		if (m_pVehicle->pPassengers[2]) return true;
		if (m_pVehicle->pPassengers[3]) return true;
		if (m_pVehicle->pPassengers[4]) return true;
		if (m_pVehicle->pPassengers[5]) return true;
		if (m_pVehicle->pPassengers[6]) return true;
	}

	return false;
}

void CVehicle::ProcessMarkers()
{
	if (!m_pVehicle) return;

	if (m_byteObjectiveVehicle)
	{
		if (!m_bSpecialMarkerEnabled)
		{
			if (m_dwMarkerID)
			{
				ScriptCommand(&disable_marker, m_dwMarkerID);
				m_dwMarkerID = 0;
			}

			ScriptCommand(&tie_marker_to_car, m_dwGTAId, 1, 3, &m_dwMarkerID);
			ScriptCommand(&set_marker_color, m_dwMarkerID, 1006);
			ScriptCommand(&show_on_radar, m_dwMarkerID, 3);
			m_bSpecialMarkerEnabled = true;
		}

		return;
	}

	if (m_byteObjectiveVehicle && m_bSpecialMarkerEnabled)
	{
		if (m_dwMarkerID)
		{
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_bSpecialMarkerEnabled = false;
			m_dwMarkerID = 0;
		}
	}

	if (GetDistanceFromLocalPlayerPed() < 200.0f && !IsOccupied())
	{
		if (!m_dwMarkerID)
		{
			// show
			ScriptCommand(&tie_marker_to_car, m_dwGTAId, 1, 2, &m_dwMarkerID);
			ScriptCommand(&set_marker_color, m_dwMarkerID, 1004);
		}
	}

	else if (IsOccupied() || GetDistanceFromLocalPlayerPed() >= 200.0f)
	{
		// remove
		if (m_dwMarkerID)
		{
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_dwMarkerID = 0;
		}
	}
}

void CVehicle::SetWheelPopped(uint8_t bytePopped)
{
	

	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	if (m_pVehicle)
	{
		if (!bytePopped)
		{
			if (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
			{
				((void(*)(VEHICLE_TYPE*))(g_libGTASA + 0x004D5CA4 + 1))(m_pVehicle); // CAutomobile::Fix
			}

			if (GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE)
			{
				((void(*)(VEHICLE_TYPE*))(g_libGTASA + 0x004E9234 + 1))(m_pVehicle); // CBike::Fix
			}

			return;
		}
	}
}

void CVehicle::SetDoorState(int iState)
{
	if (!m_pVehicle) return;
	if (iState)
	{
		m_pVehicle->dwDoorsLocked = 2;
		m_bDoorsLocked = true;
		CVehicle::fDoorState = 1;
	}
	else
	{
		m_pVehicle->dwDoorsLocked = 0;
		m_bDoorsLocked = false;
		CVehicle::fDoorState = 0;
	}
}

int CVehicle::GetDoorState(){
	return CVehicle::fDoorState;
}

void CVehicle::SetLightsState(int iState)
{
	if (GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		ScriptCommand(&force_car_lights, m_dwGTAId, iState > 0 ? 2 : 1);
		int bLights = iState > 0 ? 2 : 1;
	}
}

int CVehicle::GetLightsState(){
	return CVehicle::bLights;
}

void CVehicle::SetBootAndBonnetState(int iBoot, int iBonnet)
{
	if (GamePool_Vehicle_GetAt(m_dwGTAId) && m_pVehicle)
	{
		if (iBoot == 1)
		{
			SetComponentAngle(1, 17, 1.0f);
		}
		else
		{
			SetComponentAngle(1, 17, 0.0f);
		}

		if (iBonnet == 1)
		{
			SetComponentAngle(0, 16, 1.0f);
		}
		else
		{
			SetComponentAngle(0, 16, 0.0f);
		}
	}
}

void CVehicle::RemoveComponent(uint16_t uiComponent)
{
	
	int component = (uint16_t)uiComponent;

	if (!m_dwGTAId || !m_pVehicle)
	{
		return;
	}

	if (GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		ScriptCommand(&remove_component, m_dwGTAId, component);
	}
}

#include "..//chatwindow.h"
extern CChatWindow* pChatWindow;

void CVehicle::SetComponentVisible(uint8_t group, uint16_t components)
{
	
	if (group == E_CUSTOM_COMPONENTS::ccExtra)
	{
		for (int i = 0; i < 16; i++)
		{
			std::string szName = GetComponentNameByIDs(group, i);
			SetComponentVisibleInternal(szName.c_str(), false);

			if (BIT_CHECK(components, i))
			{
				SetComponentVisibleInternal(szName.c_str(), true);
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			std::string szName = GetComponentNameByIDs(group, i);
			SetComponentVisibleInternal(szName.c_str(), false);
			if (BIT_CHECK(components, i))
			{
				SetComponentVisibleInternal(szName.c_str(), true);
			}
		}
	}
}

void* GetSuspensionLinesFromModel(int nModelIndex, int& numWheels)
{
	uint8_t* pCollisionData = GetCollisionDataFromModel(nModelIndex);

	if (!pCollisionData)
	{
		return nullptr;
	}

	void* pLines = *(void**)(pCollisionData + 16);

	numWheels = *(uint8_t*)(pCollisionData + 6);

	return pLines;
}

uint8_t* GetCollisionDataFromModel(int nModelIndex)
{
	uintptr_t* dwModelarray = (uintptr_t*)(g_libGTASA + 0x87BF48);
	uint8_t* pModelInfoStart = (uint8_t*)dwModelarray[nModelIndex];

	if (!pModelInfoStart)
	{
		return nullptr;
	}

	uint8_t* pColModel = *(uint8_t * *)(pModelInfoStart + 44);

	if (!pColModel)
	{
		return nullptr;
	}

	uint8_t* pCollisionData = *(uint8_t * *)(pColModel + 44);

	return pCollisionData;
}
void CVehicle::SetHandlingData(std::vector<SHandlingData>& vHandlingData)
{
	

	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}
	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}


	if (!m_pCustomHandling)
	{
		m_pCustomHandling = new tHandlingData;
	}

	uintptr_t* dwModelarray = (uintptr_t*)(g_libGTASA + 0x87BF48);
	uint8_t* pModelInfoStart = (uint8_t*)dwModelarray[m_pVehicle->entity.nModelIndex];
	if (!pModelInfoStart)
	{
		return;
	}

	//pChatWindow->AddDebugMessage("handling id %d", *(uint16_t*)(pModelInfoStart + 98));

	CHandlingDefault::GetDefaultHandling(*(uint16_t*)(pModelInfoStart + 98), m_pCustomHandling);

	/*pChatWindow->AddDebugMessage("mass %f", m_pCustomHandling->m_fMass);
	pChatWindow->AddDebugMessage("turn %f", m_pCustomHandling->m_fTurnMass);
	pChatWindow->AddDebugMessage("m_fEngineAcceleration %f", m_pCustomHandling->m_transmissionData.m_fEngineAcceleration);
	pChatWindow->AddDebugMessage("m_fMaxGearVelocity %f", m_pCustomHandling->m_transmissionData.m_fMaxGearVelocity);
	pChatWindow->AddDebugMessage("flags 0x%x", m_pCustomHandling->m_nHandlingFlags);*/

	bool bNeedRecalculate = false;

	for (auto& i : vHandlingData)
	{
		switch (i.flag)
		{
		case E_HANDLING_PARAMS::hpMaxSpeed:
			m_pCustomHandling->m_transmissionData.m_fMaxGearVelocity = i.fValue;
			break;
		case E_HANDLING_PARAMS::hpAcceleration:
			m_pCustomHandling->m_transmissionData.m_fEngineAcceleration = i.fValue * 0.4f;
			break;
		case E_HANDLING_PARAMS::hpEngineInertion:
			m_pCustomHandling->m_transmissionData.m_fEngineInertia = i.fValue;
			break;
		case E_HANDLING_PARAMS::hpGear:

			if (i.fValue >= 0.0f && i.fValue <= 1.1f)
			{
				m_pCustomHandling->m_transmissionData.m_nDriveType = 'R';
			}

			if (i.fValue >= 1.2f && i.fValue <= 2.1f)
			{
				m_pCustomHandling->m_transmissionData.m_nDriveType = 'F';
			}

			if (i.fValue >= 2.2f && i.fValue <= 3.1f)
			{
				m_pCustomHandling->m_transmissionData.m_nDriveType = '4';
			}

			break;
		case E_HANDLING_PARAMS::hpMass:
			m_pCustomHandling->m_fMass = i.fValue;
			break;
		case E_HANDLING_PARAMS::hpMassTurn:
			m_pCustomHandling->m_fTurnMass = i.fValue;
			break;
		case E_HANDLING_PARAMS::hpBrakeDeceleration:
		{
			m_pCustomHandling->m_fBrakeDeceleration = i.fValue;
			break;
		}
		case E_HANDLING_PARAMS::hpTractionMultiplier:
		{
			m_pCustomHandling->m_fTractionMultiplier = i.fValue;
			break;
		}
		case E_HANDLING_PARAMS::hpTractionLoss:
		{
			m_pCustomHandling->m_fTractionLoss = i.fValue;
			break;
		}
		case E_HANDLING_PARAMS::hpTractionBias:
		{
			m_pCustomHandling->m_fTractionBias = i.fValue;
			break;
		}
		case E_HANDLING_PARAMS::hpSuspensionLowerLimit:
		{
			m_pCustomHandling->m_fSuspensionLowerLimit = i.fValue;
			bNeedRecalculate = true;
			break;
		}
		case E_HANDLING_PARAMS::hpSuspensionBias:
		{
			m_pCustomHandling->m_fSuspensionBiasBetweenFrontAndRear = i.fValue;
			bNeedRecalculate = true;
			break;
		}
		case E_HANDLING_PARAMS::hpWheelSize:
		{
			m_bWheelSize = true;
			m_fWheelSize = i.fValue;
			bNeedRecalculate = true;
			break;
		}
		}
	}
	float fOldFrontWheelSize = 0.0f;
	float fOldRearWheelSize = 0.0f;

	if (m_bWheelSize)
	{
		fOldFrontWheelSize = *(float*)(pModelInfoStart + 88);
		*(float*)(pModelInfoStart + 88) = m_fWheelSize;

		fOldRearWheelSize = *(float*)(pModelInfoStart + 92);
		*(float*)(pModelInfoStart + 92) = m_fWheelSize;
	}

	/*pChatWindow->AddDebugMessage("AFTER");
	pChatWindow->AddDebugMessage("mass %f", m_pCustomHandling->m_fMass);
	pChatWindow->AddDebugMessage("turn %f", m_pCustomHandling->m_fTurnMass);
	pChatWindow->AddDebugMessage("m_fEngineAcceleration %f", m_pCustomHandling->m_transmissionData.m_fEngineAcceleration);
	pChatWindow->AddDebugMessage("m_fMaxGearVelocity %f", m_pCustomHandling->m_transmissionData.m_fMaxGearVelocity);
	pChatWindow->AddDebugMessage("flags 0x%x", m_pCustomHandling->m_nHandlingFlags);*/

	((void (*)(int, tHandlingData*))(g_libGTASA + 0x004FBCF4 + 1))(0, m_pCustomHandling);
	m_pVehicle->pHandling = m_pCustomHandling;

	if (bNeedRecalculate)
	{
		((void (*)(VEHICLE_TYPE*))(g_libGTASA + 0x004D3E2C + 1))(m_pVehicle); // CAutomobile::SetupSuspensionLines

		CopyGlobalSuspensionLinesToPrivate();
	}

	if (m_bWheelSize)
	{
		*(float*)(pModelInfoStart + 88) = fOldFrontWheelSize;
		*(float*)(pModelInfoStart + 92) = fOldRearWheelSize;
	}

	if (bNeedRecalculate)
	{
		((void (*)(VEHICLE_TYPE*))(g_libGTASA + 0x004D6078 + 1))(m_pVehicle); // process suspension
	}
}

void CVehicle::ResetVehicleHandling()
{
	
	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}
	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	if (!m_pCustomHandling)
	{
		m_pCustomHandling = new tHandlingData;
	}
	uintptr_t* dwModelarray = (uintptr_t*)(g_libGTASA + 0x87BF48);
	uint8_t* pModelInfoStart = (uint8_t*)dwModelarray[m_pVehicle->entity.nModelIndex];

	if (!pModelInfoStart)
	{
		return;
	}

	CHandlingDefault::GetDefaultHandling(*(uint16_t*)(pModelInfoStart + 98), m_pCustomHandling);

	((void (*)(int, tHandlingData*))(g_libGTASA + 0x004FBCF4 + 1))(0, m_pCustomHandling);

	m_pVehicle->pHandling = m_pCustomHandling;

	((void (*)(VEHICLE_TYPE*))(g_libGTASA + 0x004D3E2C + 1))(m_pVehicle); // CAutomobile::SetupSuspensionLines
	CopyGlobalSuspensionLinesToPrivate();

	Log("Reseted to defaults");
}

void CVehicle::ApplyVinyls(uint8_t bSlot1, uint8_t bSlot2)
{
	if (bSlot1 == 0)
	{
		RemoveTexture("remap_cbody_0");
		return;
	}
	if (bSlot2 == 0)
	{
		RemoveTexture("remap_cbody_0");
		return;
	}

	char szTex[MAX_REPLACED_TEXTURE_NAME];

	if (bSlot1 != 255)
	{
		sprintf(&szTex[0], "remapbody%d", bSlot1);
		ApplyTexture("remapbody0", &szTex[0]);
	}

	if (bSlot2 != 255)
	{
		sprintf(&szTex[0], "remapbody%d", bSlot2);
		ApplyTexture("remapbody0", &szTex[0]);
	}

}

void CVehicle::ApplyToner(uint8_t bSlot, uint8_t bID)
{
	char szOld[MAX_REPLACED_TEXTURE_NAME];
	char szNew[MAX_REPLACED_TEXTURE_NAME];

	if (bID == 0)
	{
		sprintf(&szOld[0], "remaptoner", bSlot);
		RemoveTexture(&szOld[0]);
		return;
	}
	if (bID == 255)
	{
		return;
	}

	sprintf(&szOld[0], "remaptoner%d", bSlot);
	sprintf(&szNew[0], "remaptoner%d", bID);
	ApplyTexture(&szOld[0], &szNew[0]);
}

RwObject* GetAllAtomicObjectCB(RwObject* object, void* data)
{
	
	std::vector<RwObject*>& result = *((std::vector<RwObject*>*) data);
	result.push_back(object);
	return object;
}

// Get all atomics for this frame (even if they are invisible)
void GetAllAtomicObjects(RwFrame* frame, std::vector<RwObject*>& result)
{
	
	((uintptr_t(*)(RwFrame*, void*, uintptr_t))(g_libGTASA + 0x001AEE2C + 1))(frame, (void*)GetAllAtomicObjectCB, (uintptr_t)& result);
}

void CVehicle::ApplyTexture(const char* szTexture, const char* szNew)
{
	if (IsRetextured(szTexture))
	{
		RemoveTexture(szTexture);
	}

	uint8_t bID = 255;
	for (uint8_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
	{
		if (m_bReplaceTextureStatus[i] == false)
		{
			bID = i;
			break;
		}
	}

	if (bID == 255)
	{
		return;
	}

	m_bReplaceTextureStatus[bID] = true;
	strcpy(&(m_szReplacedTextures[bID].szOld[0]), szTexture);
	m_szReplacedTextures[bID].pTexture = (RwTexture*)LoadTextureFromDB("samp", szNew);

	m_bReplacedTexture = true;
}

void CVehicle::ApplyTexture(const char* szTexture, RwTexture* pTexture)
{
	if (IsRetextured(szTexture))
	{
		RemoveTexture(szTexture);
	}
	//pChatWindow->AddDebugMessage("apply tex %s", szTexture);
	uint8_t bID = 255;
	for (uint8_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
	{
		if (m_bReplaceTextureStatus[i] == false)
		{
			bID = i;
			break;
		}
	}

	if (bID == 255)
	{
		return;
	}

	m_bReplaceTextureStatus[bID] = true;
	strcpy(&(m_szReplacedTextures[bID].szOld[0]), szTexture);
	m_szReplacedTextures[bID].pTexture = pTexture;

	m_bReplacedTexture = true;
}

void CVehicle::RemoveTexture(const char* szOldTexture)
{
	for (size_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
	{
		if (m_bReplaceTextureStatus[i])
		{
			if (!strcmp(m_szReplacedTextures[i].szOld, szOldTexture))
			{
				m_bReplaceTextureStatus[i] = false;

				if (m_szReplacedTextures[i].pTexture)
				{
					RwTextureDestroy(m_szReplacedTextures[i].pTexture);
					m_szReplacedTextures[i].pTexture = nullptr;
				}
				break;
			}
		}
	}
}

bool CVehicle::IsRetextured(const char* szOldTexture)
{
	for (size_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
	{
		if (m_bReplaceTextureStatus[i])
		{
			if (!strcmp(m_szReplacedTextures[i].szOld, szOldTexture))
			{
				return true;
			}
		}
	}
	return false;
}

void CVehicle::SetHeadlightsColor(uint8_t r, uint8_t g, uint8_t b)
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	m_bHeadlightsColor = true;
	m_bHeadlightsR = r;
	m_bHeadlightsG = g;
	m_bHeadlightsB = b;
}

void CVehicle::ProcessHeadlightsColor(uint8_t& r, uint8_t& g, uint8_t& b)
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	if (m_bHeadlightsColor)
	{
		r = m_bHeadlightsR;
		g = m_bHeadlightsG;
		b = m_bHeadlightsB;
	}
}

void CVehicle::SetWheelAlignment(int iWheel, float angle)
{
	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}

	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	if (iWheel == 0)
	{
		m_bWheelAlignmentX = true;
		m_fWheelAlignmentX = (M_PI / 180.0f) * angle;
	}
	else
	{
		m_bWheelAlignmentY = true;
		m_fWheelAlignmentY = (M_PI / 180.0f) * angle;
	}
}

void CVehicle::SetWheelOffset(int iWheel, float offset)
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	//pChatWindow->AddDebugMessage("set for %d wheel %f offset", iWheel, offset);
	if (iWheel == 0)
	{
		m_bWheelOffsetX = true;
		m_fNewOffsetX = offset;
		m_bWasWheelOffsetProcessedX = false;
	}
	else
	{
		m_bWheelOffsetY = true;
		m_fNewOffsetY = offset;
		m_bWasWheelOffsetProcessedY = false;
	}

	m_uiLastProcessedWheelOffset = GetTickCount();
}

void CVehicle::SetWheelWidth(float fValue)
{
	if (fValue == 20.0f)
	{
		m_bWheelWidth = false;
		return;
	}
	m_bWheelWidth = true;
	m_fWheelWidth = fValue;
}

MATRIX4X4* RwMatrixMultiplyByVector(VECTOR* out, MATRIX4X4* a2, VECTOR* in);

void CVehicle::ProcessWheelsOffset()
{
	if (GetTickCount() - m_uiLastProcessedWheelOffset <= 30)
	{
		return;
	}

	if (!m_bWasWheelOffsetProcessedX)
	{
		if (m_bWheelOffsetX)
		{
			//pChatWindow->AddDebugMessage("setting wheel offset X");
			RwFrame* pWheelLF = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_lf_dummy"); // GetFrameFromname
			RwFrame* pWheelRF = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_rf_dummy"); // GetFrameFromname

			/*if (m_fNewOffsetX)
			{
				ProcessWheelOffset(pWheelLF, true, 0.0f - m_fWheelOffsetX);
				ProcessWheelOffset(pWheelRF, false, 0.0f - m_fWheelOffsetX);

				m_fWheelOffsetX = m_fNewOffsetX;
				m_fNewOffsetX = 0.0f;
				//pChatWindow->AddDebugMessage("moved old X");
			}*/
			m_fWheelOffsetX = m_fNewOffsetX;

			ProcessWheelOffset(pWheelLF, true, m_fWheelOffsetX, 0);
			ProcessWheelOffset(pWheelRF, false, m_fWheelOffsetX, 1);

		}
		m_bWasWheelOffsetProcessedX = true;
	}
	if (!m_bWasWheelOffsetProcessedY)
	{
		if (m_bWheelOffsetY)
		{
			//pChatWindow->AddDebugMessage("setting wheel offset Y");
			RwFrame* pWheelRB = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_rb_dummy"); // GetFrameFromname
			RwFrame* pWheelLB = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, "wheel_lb_dummy"); // GetFrameFromname

			/*if (m_fNewOffsetY)
			{
				ProcessWheelOffset(pWheelRB, false, 0.0f - m_fWheelOffsetY);
				ProcessWheelOffset(pWheelLB, true, 0.0f - m_fWheelOffsetY);
				m_fWheelOffsetY = m_fNewOffsetY;
				m_fNewOffsetY = 0.0f;

				//pChatWindow->AddDebugMessage("moved old Y");
			}*/
			m_fWheelOffsetY = m_fNewOffsetY;
			ProcessWheelOffset(pWheelRB, false, m_fWheelOffsetY, 2);
			ProcessWheelOffset(pWheelLB, true, m_fWheelOffsetY, 3);
		}
		m_bWasWheelOffsetProcessedY = true;
	}
}

void CVehicle::SetCustomShadow(uint8_t r, uint8_t g, uint8_t b, float fSizeX, float fSizeY, const char* szTex)
{
	if (fSizeX == 0.0f || fSizeY == 0.0f)
	{
		m_bShadow = false;

		if (m_Shadow.pTexture)
		{
			RwTextureDestroy(m_Shadow.pTexture);
			m_Shadow.pTexture = nullptr;
		}

		return;
	}

	m_bShadow = true;

	m_Shadow.r = r;
	m_Shadow.g = g;
	m_Shadow.b = b;
	m_Shadow.fSizeX = fSizeX;
	m_Shadow.fSizeY = fSizeY;
	m_Shadow.pTexture = (RwTexture*)LoadTextureFromDB("samp", szTex);
}

void CVehicle::ProcessWheelOffset(RwFrame* pFrame, bool bLeft, float fValue, int iID)
{
	VECTOR vecOffset;
	vecOffset.X = 0.0f - fValue;
	vecOffset.Y = 0.0f;
	vecOffset.Z = 0.0f;
	if (bLeft)
	{
		vecOffset.X *= -1.0f;
	}

	VECTOR vecOut;
	RwMatrixMultiplyByVector(&vecOut, &(m_vInitialWheelMatrix[iID]), &vecOffset);

	pFrame->modelling.pos.X = vecOut.X;
	pFrame->modelling.pos.Y = vecOut.Y;
	pFrame->modelling.pos.Z = vecOut.Z;
}

void CVehicle::SetComponentAngle(bool bUnk, int iID, float angle)
{
	if (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		((void(*)(VEHICLE_TYPE*, int a2, int a3, int a4, float a5, uint8_t a6))(g_libGTASA + 0x004DA0E4 + 1))(m_pVehicle, 0, iID, bUnk, angle, 1); // CAutomobile::OpenDoor
	}
}

#include "..//cryptors/COMPONENT_VISIBLE_INTERNAL_result.h"

void CVehicle::SetComponentVisibleInternal(const char* szComponent, bool bVisible)
{
	
	PROTECT_CODE_COMPONENT_VISIBLE_INTERNAL;
	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	if (!m_pVehicle->entity.m_RwObject)
	{
		return;
	}


	RwFrame* pFrame = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->entity.m_RwObject, szComponent); // GetFrameFromname
	if (pFrame != NULL)
	{
		// Get all atomics for this component - Usually one, or two if there is a damaged version
		std::vector<RwObject*> atomicList;
		GetAllAtomicObjects(pFrame, atomicList);

		// Count number currently visible
		uint uiNumAtomicsCurrentlyVisible = 0;
		for (uint i = 0; i < atomicList.size(); i++)
		{
			if (!atomicList[i])
			{
				continue;
			}
			if (atomicList[i]->flags & 0x04)
			{
				uiNumAtomicsCurrentlyVisible++;
			}
		}

		if (bVisible && uiNumAtomicsCurrentlyVisible == 0)
		{
			// Make atomic (undamaged version) visible. TODO - Check if damaged version should be made visible instead
			for (uint i = 0; i < atomicList.size(); i++)
			{
				RwObject* pAtomic = atomicList[i];
				if (!pAtomic)
				{
					continue;
				}
				int       AtomicId = ((int(*)(RwObject*))(g_libGTASA + 0x0055C670 + 1))(pAtomic); // CVisibilityPlugins::GetAtomicId

				if (!(AtomicId & ATOMIC_ID_FLAG_TWO_VERSIONS_DAMAGED))
				{
					// Either only one version, or two versions and this is the undamaged one
					pAtomic->flags |= 0x04;
				}
			}
		}
		else if (!bVisible && uiNumAtomicsCurrentlyVisible > 0)
		{
			// Make all atomics invisible
			for (uint i = 0; i < atomicList.size(); i++)
			{
				if (!atomicList[i])
				{
					continue;
				}
				atomicList[i]->flags &= ~0x05;            // Mimic what GTA seems to do - Not sure what the bottom bit is for
			}
		}
	}
}

std::string CVehicle::GetComponentNameByIDs(uint8_t group, int subgroup)
{
	
	if (group == E_CUSTOM_COMPONENTS::ccExtra && subgroup >= EXTRA_COMPONENT_BOOT)
	{
		switch (subgroup)
		{
		case EXTRA_COMPONENT_BOOT:
			return std::string("boot_dummy");
		case EXTRA_COMPONENT_BONNET:
			return std::string("bonnet_dummy");
		case EXTRA_COMPONENT_BUMP_REAR:
			return std::string("bump_rear_dummy");
		case EXTRA_COMPONENT_DEFAULT_DOOR:
			return std::string("door_lf_dummy");
		case EXTRA_COMPONENT_WHEEL:
			return std::string("wheel_lf_dummy");
		case EXTRA_COMPONENT_BUMP_FRONT:
			return std::string("bump_front_dummy");
		}
	}

	std::string retn;

	switch (group)
	{
	case E_CUSTOM_COMPONENTS::ccBumperF:
		retn += "bumberF_";
		break;
	case E_CUSTOM_COMPONENTS::ccBumperR:
		retn += "bumberR_";
		break;
	case E_CUSTOM_COMPONENTS::ccFenderF:
		retn += "fenderF_";
		break;
	case E_CUSTOM_COMPONENTS::ccFenderR:
		retn += "fenderR_";
		break;
	case E_CUSTOM_COMPONENTS::ccSpoiler:
		retn += "spoiler_";
		break;
	case E_CUSTOM_COMPONENTS::ccExhaust:
		retn += "exhaust_";
		break;
	case E_CUSTOM_COMPONENTS::ccRoof:
		retn += "roof_";
		break;
	case E_CUSTOM_COMPONENTS::ccTaillights:
		retn += "taillights_";
		break;
	case E_CUSTOM_COMPONENTS::ccHeadlights:
		retn += "headlights_";
		break;
	case E_CUSTOM_COMPONENTS::ccDiffuser:
		retn += "diffuser_";
		break;
	case E_CUSTOM_COMPONENTS::ccSplitter:
		retn += "splitter_";
		break;
	case E_CUSTOM_COMPONENTS::ccExtra:
		retn += "ext_";
		break;
	default:
		retn = std::string("err");
		break;
	}

	retn += ('0' + (char)subgroup);

	return retn;
}

void CVehicle::CopyGlobalSuspensionLinesToPrivate()
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	if (!bHasSuspensionLines)
	{
		int numWheels;
		void* pOrigSuspension = GetSuspensionLinesFromModel(m_pVehicle->entity.nModelIndex, numWheels);

		if (pOrigSuspension && numWheels)
		{
			bHasSuspensionLines = true;
			m_pSuspensionLines = new uint8_t[0x20 * numWheels];
		}
	}

	int numWheels;
	void* pOrigSuspension = GetSuspensionLinesFromModel(m_pVehicle->entity.nModelIndex, numWheels);

	if (pOrigSuspension && numWheels)
	{
		memcpy(m_pSuspensionLines, pOrigSuspension, 0x20 * numWheels);
	}
}

void CVehicle::SetEngineState(int iState)
{
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) {
		return;
	}

	if (iState)
	{
		m_pVehicle->m_nVehicleFlags.bEngineOn = 1;
		bEngine = 1;
		m_pVehicle->m_nVehicleFlags.bEngineBroken = 0;
	}
	else
	{
		m_pVehicle->m_nVehicleFlags.bEngineOn = 0;
		bEngine = 0;
		m_pVehicle->m_nVehicleFlags.bEngineBroken = 1;
	}
}

int CVehicle::GetEngineState(){
	return bEngine;
}

void CVehicle::UpdateDamageStatus(uint32_t dwPanelDamage, uint32_t dwDoorDamage, uint8_t byteLightDamage)
{
	

	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	// handle only fix, not damaging
	if (m_pVehicle && GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		if (!dwPanelDamage && !dwDoorDamage && !byteLightDamage)
		{
			((void(*)(VEHICLE_TYPE*))(g_libGTASA + 0x004D5CA4 + 1))(m_pVehicle); // CAutomobile::Fix
			return;
		}
	}
}

unsigned int CVehicle::GetVehicleSubtype()
{
	if (m_pVehicle)
	{
		if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CC9F0) // 0x871120
		{
			return VEHICLE_SUBTYPE_CAR;
		}
		else if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CCD48) // 0x8721A0
		{
			return VEHICLE_SUBTYPE_BOAT;
		}
		else if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CCB18) // 0x871360
		{
			return VEHICLE_SUBTYPE_BIKE;
		}
		else if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CD0B0) // 0x871948
		{
			return VEHICLE_SUBTYPE_PLANE;
		}
		else if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CCE60) // 0x871680
		{
			return VEHICLE_SUBTYPE_HELI;
		}
		else if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CCC30) // 0x871528
		{
			return VEHICLE_SUBTYPE_PUSHBIKE;
		}
		else if (m_pVehicle->entity.vtable == g_libGTASA + 0x5CD428) // 0x872370
		{
			return VEHICLE_SUBTYPE_TRAIN;
		}
	}

	return 0;
}