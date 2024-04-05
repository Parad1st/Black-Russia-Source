#pragma once

class CObject : public CEntity
{
public:
	MATRIX4X4	m_matTarget;
	MATRIX4X4	m_matCurrent;
	uint8_t		m_byteMoving;
	float		m_fMoveSpeed;
	bool		m_bIsPlayerSurfing;

	MaterialInfo m_pMaterials[MAX_MATERIALS];
	bool		m_bMaterials;

	VECTOR m_vecAttachedOffset;
	VECTOR m_vecAttachedRotation;
	uint16_t m_usAttachedVehicle;
	uint8_t m_bAttachedType;

	CObject(int iModel, float fPosX, float fPosY, float fPosZ, VECTOR vecRot, float fDrawDistance);
	~CObject();

	void Process(float fElapsedTime);
	float DistanceRemaining(MATRIX4X4 *matPos);

	void SetPos(float x, float y, float z);
	void MoveTo(float x, float y, float z, float speed);

	void AttachToVehicle(uint16_t usVehID, VECTOR* pVecOffset, VECTOR* pVecRot);
	void ProcessAttachToVehicle(CVehicle* pVehicle);


	void InstantRotate(float x, float y, float z);
};