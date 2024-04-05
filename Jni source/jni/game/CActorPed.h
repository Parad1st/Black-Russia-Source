#pragma once

class CActorPed : public CEntity
{
public:
	CActorPed(uint16_t usModel, VECTOR vecPosition, float fRotation, float fHealth, bool bInvulnerable);
	~CActorPed();

	void Destroy();
	void SetHealth(float fHealth);
	void SetDead();
	void ForceTargetRotation(float fRotation);
	void ApplyAnimation(char* szAnimName, char* szAnimFile, float fDelta, int bLoop, int bLockX, int bLockY, int bFreeze, int uiTime);
	void PutDirectlyInVehicle(int iVehicleID, int iSeat);
	void RemoveFromVehicle();
	void RemoveFromVehicleAndPutAt(float fX, float fY, float fZ);

	PED_TYPE* m_pPed;
};