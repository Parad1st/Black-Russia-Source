#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "../util/armhook.h"
#include "common.h"

#include "..//debug.h"
#include "CActorPed.h"

extern CGame* pGame;

CActorPed::CActorPed(uint16_t usModel, VECTOR vecPosition, float fRotation, float fHealth, bool bInvulnerable)
{
	

	m_dwGTAId = 0;
	m_pPed = nullptr;
	m_pEntity = nullptr;

	if (!pGame->IsModelLoaded(usModel)) 
	{
		pGame->RequestModel(usModel);
		pGame->LoadRequestedModels();
	}

	if (!IsPedModel(usModel)) 
	{
		usModel = 0;
	}

	uint32_t actorGTAId = 0;
	ScriptCommand(&create_actor, 22, usModel, vecPosition.X, vecPosition.Y, vecPosition.Z, &actorGTAId);

	m_dwGTAId = actorGTAId;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE*)m_pPed;

	ForceTargetRotation(fRotation);
	TeleportTo(vecPosition.X, vecPosition.Y, vecPosition.Z);

	if (fHealth < 1.0f) 
	{
		SetDead();
	}
	else 
	{
		SetHealth(fHealth);
	}

	ScriptCommand(&lock_actor, m_dwGTAId, 1);

	if (bInvulnerable) 
	{
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
	}
	else 
	{
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
	}
}

CActorPed::~CActorPed()
{
	
	Destroy();
}

bool IsValidGamePed(PED_TYPE* pPed) 
{
	
	//IsPedPointerValid(CPed *) Ч 0x00435614
	if (((bool (*)(PED_TYPE*))(g_libGTASA + 0x00435614 + 1))(pPed)) {
		return true;
	}
	return false;
}

void CActorPed::Destroy()
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (IsValidGamePed(m_pPed)) 
	{
		// CPed::entity.vtable + 0x4 destructor
		((void (*)(PED_TYPE*))(*(void**)(m_pPed->entity.vtable + 0x4)))(m_pPed);
	}

	m_pPed = nullptr;
	m_pEntity = nullptr;
	m_dwGTAId = 0;
}

void CActorPed::SetHealth(float fHealth)
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	m_pPed->fHealth = fHealth;
}

void CActorPed::SetDead()
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	MATRIX4X4 matEntity;
	GetMatrix(&matEntity);
	TeleportTo(matEntity.pos.X, matEntity.pos.Y, matEntity.pos.Z);

	SetHealth(0.0f);
	ScriptCommand(&kill_actor, m_dwGTAId);
}

void CActorPed::ForceTargetRotation(float fRotation)
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	m_pPed->fRotation1 = DegToRad(fRotation);
	m_pPed->fRotation2 = DegToRad(fRotation);

	ScriptCommand(&set_actor_z_angle, m_dwGTAId, fRotation);
}

void CActorPed::ApplyAnimation(char* szAnimName, char* szAnimFile, float fDelta, int bLoop, int bLockX, int bLockY, int bFreeze, int uiTime)
{
	

	int iWaitAnimLoad = 0;

	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!strcasecmp(szAnimFile, "SEX")) return;

	if (!pGame->IsAnimationLoaded(szAnimFile))
	{
		pGame->RequestAnimation(szAnimFile);
		while (!pGame->IsAnimationLoaded(szAnimFile))
		{
			usleep(1000);
			iWaitAnimLoad++;
			if (iWaitAnimLoad > 15) return;
		}
	}

	ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
}

void CActorPed::PutDirectlyInVehicle(int iVehicleID, int iSeat)
{
	if (!m_pPed) return;
	if (!GamePool_Vehicle_GetAt(iVehicleID)) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	VEHICLE_TYPE* pVehicle = GamePool_Vehicle_GetAt(iVehicleID);

	if (pVehicle->fHealth == 0.0f) return;
	if (pVehicle->entity.vtable == g_libGTASA + 0x5C7358) return;
	// check seatid (допилить)

	if (iSeat == 0)
	{
		if (pVehicle->pDriver && IN_VEHICLE(pVehicle->pDriver)) return;
		ScriptCommand(&put_actor_in_car, m_dwGTAId, iVehicleID);
	}
	else
	{
		iSeat--;
		ScriptCommand(&put_actor_in_car2, m_dwGTAId, iVehicleID, iSeat);
	}
}

void CActorPed::RemoveFromVehicle()
{
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	MATRIX4X4 mat;

	if (IN_VEHICLE(m_pPed))
	{
		GetMatrix(&mat);
		RemoveFromVehicleAndPutAt(mat.pos.X, mat.pos.Y, mat.pos.Z);
	}
}

void CActorPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if (m_pPed && IN_VEHICLE(m_pPed))
	{
		ScriptCommand(&remove_actor_from_car_and_put_at, m_dwGTAId, fX, fY, fZ);
	}
}
