#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

#include "../util/armhook.h"
#include "../game/scripting.h"

#include "../game/common.h"
#include "..//keyboard.h"
#include "..//chatwindow.h"
#include "../settings.h"
#include "../util/CJavaWrapper.h"
extern CKeyBoard* pKeyBoard;
extern CChatWindow* pChatWindow;

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CSettings *pSettings;

bool bFirstSpawn = true;

extern int iNetModeNormalOnfootSendRate;
extern int iNetModeNormalInCarSendRate;
extern bool bUsedPlayerSlots[];

#define IS_TARGETING(x) (x & 128)
#define IS_FIRING(x) (x & 4)

// SEND RATE TICKS
#define NETMODE_IDLE_ONFOOT_SENDRATE	80
#define NETMODE_NORMAL_ONFOOT_SENDRATE	30
#define NETMODE_IDLE_INCAR_SENDRATE		80
#define NETMODE_NORMAL_INCAR_SENDRATE	30

#define NETMODE_HEADSYNC_SENDRATE		1000
#define NETMODE_AIM_SENDRATE			100
#define NETMODE_FIRING_SENDRATE			30

#define LANMODE_IDLE_ONFOOT_SENDRATE	20
#define LANMODE_NORMAL_ONFOOT_SENDRATE	15
#define LANMODE_IDLE_INCAR_SENDRATE		30
#define LANMODE_NORMAL_INCAR_SENDRATE	15

#define STATS_UPDATE_TICKS				1000

CLocalPlayer::CLocalPlayer()
{
	m_pPlayerPed = pGame->FindPlayerPed();
	m_bIsActive = false;
	m_bIsWasted = false;

	m_iSelectedClass = 0;
	m_bHasSpawnInfo = false;
	m_bWaitingForSpawnRequestReply = false;
	m_bWantsAnotherClass = false;
	m_bInRCMode = false;

	memset(&m_OnFootData, 0, sizeof(ONFOOT_SYNC_DATA));

	m_dwLastSendTick = GetTickCount();
	m_dwLastSendAimTick = GetTickCount();
	m_dwLastSendSpecTick = GetTickCount();
	m_dwLastAimSendTick = m_dwLastSendTick;
	m_dwLastUpdateOnFootData = GetTickCount();
	m_dwLastStatsUpdateTick = GetTickCount();
	m_dwLastUpdateInCarData = GetTickCount();
	m_dwLastUpdatePassengerData = GetTickCount();
	m_dwPassengerEnterExit = GetTickCount();

	m_CurrentVehicle = 0;
	ResetAllSyncAttributes();

	m_bIsSpectating = false;
	m_byteSpectateType = SPECTATE_TYPE_NONE;
	m_SpectateID = 0xFFFFFFFF;

	uint8_t i;
	for (i = 0; i < 13; i++)
	{
		m_byteLastWeapon[i] = 0;
		m_dwLastAmmo[i] = 0;
	}

}

CLocalPlayer::~CLocalPlayer()
{
	
}

void CLocalPlayer::ResetAllSyncAttributes()
{
	m_byteCurInterior = 0;
	m_LastVehicle = INVALID_VEHICLE_ID;
	m_bInRCMode = false;
}

void CLocalPlayer::SendStatsUpdate()
{
	RakNet::BitStream bsStats;
	int iMoney = pGame->GetLocalMoney();
	uint32_t wAmmo = m_pPlayerPed->GetCurrentWeaponSlot()->dwAmmo;

	bsStats.Write((BYTE)ID_STATS_UPDATE);
	bsStats.Write(iMoney);
	bsStats.Write(wAmmo);
	pNetGame->GetRakClient()->Send(&bsStats, HIGH_PRIORITY, UNRELIABLE, 0);
}

void CLocalPlayer::CheckWeapons()
{
	if (m_pPlayerPed->IsInVehicle()) return;
	uint8_t i;
	uint8_t byteCurWep = m_pPlayerPed->GetCurrentWeapon();
	bool bMSend = false;

	RakNet::BitStream bsWeapons;
	bsWeapons.Write((BYTE)ID_WEAPONS_UPDATE);
	bsWeapons.Write((uint16_t)INVALID_PLAYER_ID);
	bsWeapons.Write((uint16_t)INVALID_PLAYER_ID);
	for (i = 0; i < 13; i++)
	{
		if (m_byteLastWeapon[i] != byteCurWep)
		{
			//bsWeapons.Write(i);
			bool bSend = false;
			if (m_byteLastWeapon[i] != m_pPlayerPed->m_pPed->WeaponSlots[i].dwType)
			{
				// non-current weapon has changed
				m_byteLastWeapon[i] = (BYTE)m_pPlayerPed->m_pPed->WeaponSlots[i].dwType;
				bSend = true;
			}
			//bsWeapons.Write(m_byteLastWeapon[i]);
			if (m_dwLastAmmo[i] != m_pPlayerPed->m_pPed->WeaponSlots[i].dwAmmo)
			{
				// non-current ammo has changed
				m_dwLastAmmo[i] = m_pPlayerPed->m_pPed->WeaponSlots[i].dwAmmo;
				bSend = true;
			}
			//bsWeapons.Write(m_dwLastAmmo[i]);
			if (bSend)
			{
				//pChatWindow->AddDebugMessage("Id: %u, Weapon: %u, Ammo: %d\n", i, m_byteLastWeapon[i], m_dwLastAmmo[i]);
				bsWeapons.Write((uint8_t)i);
				bsWeapons.Write((uint8_t)m_byteLastWeapon[i]);
				bsWeapons.Write((uint16_t)m_dwLastAmmo[i]);
				bMSend = true;
			}
		}
	}
	if (bMSend)
	{
		pNetGame->GetRakClient()->Send(&bsWeapons, HIGH_PRIORITY, UNRELIABLE, 0);
	}
}
uint32_t CLocalPlayer::GetCurrentAnimationIndexFlag()
{
	uint32_t dwAnim = 0;

	float fBlendData = 4.0f;

	int iAnimIdx = m_pPlayerPed->GetCurrentAnimationIndex(fBlendData);

	uint32_t hardcodedBlend = 0b00000100;	// 4
	hardcodedBlend <<= 16;

	uint32_t hardcodedFlags = 0;

	if (iAnimIdx)
	{
		hardcodedFlags = 0b00010001;
	}
	else
	{
		hardcodedFlags = 0b10000000;
		iAnimIdx = 1189;
	}

	hardcodedFlags <<= 24;

	uint16_t usAnimidx = (uint16_t)iAnimIdx;

	dwAnim = (uint32_t)usAnimidx;
	dwAnim |= hardcodedBlend;
	dwAnim |= hardcodedFlags;

	return dwAnim;
}
extern uint32_t g_uiHeadMoveEnabled;
#include "..//game/CWeaponsOutFit.h"
bool CLocalPlayer::Process()
{
	uint32_t dwThisTick = GetTickCount();

	if(m_bIsActive && m_pPlayerPed)
	{
		// handle dead
		if(!m_bIsWasted && m_pPlayerPed->GetActionTrigger() == ACTION_DEATH || m_pPlayerPed->IsDead())
		{
			ToggleSpectating(false);
			m_pPlayerPed->FlushAttach();
			// reset tasks/anims
			m_pPlayerPed->TogglePlayerControllable(true);

			if(m_pPlayerPed->IsInVehicle() && !m_pPlayerPed->IsAPassenger())
			{
				SendInCarFullSyncData();
				m_LastVehicle = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
			}

			m_pPlayerPed->SetHealth(0.0f);
			m_pPlayerPed->SetDead();
			SendWastedNotification();

			m_bIsActive = false;
			m_bIsWasted = true;

			return true;
		}

		if ((dwThisTick - m_dwLastStatsUpdateTick) > STATS_UPDATE_TICKS) 
		{
			SendStatsUpdate();
			m_dwLastStatsUpdateTick = dwThisTick;
		}

		CheckWeapons();
		CWeaponsOutFit::ProcessLocalPlayer(m_pPlayerPed);

		if (m_pPlayerPed)
		{
			m_pPlayerPed->ProcessSpecialAction();
		}

		// handle interior changing
		uint8_t byteInterior = pGame->GetActiveInterior();
		if(byteInterior != m_byteCurInterior)
			UpdateRemoteInterior(byteInterior);
		
		// The new regime for adjusting sendrates is based on the number
		// of players that will be effected by this update. The more players
		// there are within a small radius, the more we must scale back
		// the number of sends.
		int iNumberOfPlayersInLocalRange=0;
		iNumberOfPlayersInLocalRange = DetermineNumberOfPlayersInLocalRange();
		if(!iNumberOfPlayersInLocalRange) iNumberOfPlayersInLocalRange = 10;

		// SPECTATING
		if(m_bIsSpectating)
		{
			ProcessSpectating();
		}
		// DRIVER
		else if(m_pPlayerPed->IsInVehicle() && !m_pPlayerPed->IsAPassenger())
		{
			CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
			CVehicle *pVehicle;
			if(pVehiclePool)
				m_CurrentVehicle = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
			pVehicle = pVehiclePool->GetAt(m_CurrentVehicle);

			if((dwThisTick - m_dwLastSendTick) > (unsigned int)GetOptimumInCarSendRate())
			{
				m_dwLastSendTick = GetTickCount();
				SendInCarFullSyncData();
			}
		}
		// ONFOOT
		else if(m_pPlayerPed->GetActionTrigger() == ACTION_NORMAL || m_pPlayerPed->GetActionTrigger() == ACTION_SCOPE)
		{
			UpdateSurfing();

			if ((dwThisTick - m_dwLastHeadUpdate) > 1000 && g_uiHeadMoveEnabled) {
				VECTOR LookAt;
				CAMERA_AIM* Aim = GameGetInternalAim();
				LookAt.X = Aim->pos1x + (Aim->f1x * 20.0f);
				LookAt.Y = Aim->pos1y + (Aim->f1y * 20.0f);
				LookAt.Z = Aim->pos1z + (Aim->f1z * 20.0f);
				pGame->FindPlayerPed()->ApplyCommandTask("FollowPedSA", 0, 2000, -1, &LookAt, 0, 0.1f, 500, 3, 0);
				m_dwLastHeadUpdate = dwThisTick;
			}

			if(m_CurrentVehicle != INVALID_VEHICLE_ID)
			{
				m_LastVehicle = m_CurrentVehicle;
				m_CurrentVehicle = INVALID_VEHICLE_ID;
			}

			if((dwThisTick - m_dwLastSendTick) > (unsigned int)GetOptimumOnFootSendRate())
			{
				m_dwLastSendTick = GetTickCount();
				Log("[DEBUG] Send Packet SyncData RPC");
				SendOnFootFullSyncData();
			}

			if((dwThisTick - m_dwLastSendTick) > (unsigned int)GetOptimumOnFootSendRate() || LocalPlayerKeys.bKeys[ePadKeys::KEY_WALK] || LocalPlayerKeys.bKeys[ePadKeys::KEY_YES] || LocalPlayerKeys.bKeys[ePadKeys::KEY_NO] || LocalPlayerKeys.bKeys[ePadKeys::KEY_CTRL_BACK])
			{

				m_dwLastSendTick = GetTickCount();
				Log("[DEBUG] Send Packet Key RPC");
				SendOnKeyFullSyncData();

			}
			// TIMING FOR ONFOOT AIM SENDS
			uint16_t lrAnalog, udAnalog;
			uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

			// Not targeting or firing. We need a very slow rate to sync the head.
			if (!IS_TARGETING(wKeys) && !IS_FIRING(wKeys)) 
			{
				if ((dwThisTick - m_dwLastAimSendTick) > NETMODE_HEADSYNC_SENDRATE) 
				{
					m_dwLastAimSendTick = dwThisTick;
					SendAimSyncData();
				}
			}

			// Targeting only. Just synced for show really, so use a slower rate
			else if (IS_TARGETING(wKeys) && !IS_FIRING(wKeys))
			{
				if ((dwThisTick - m_dwLastAimSendTick) > (uint32_t)NETMODE_AIM_SENDRATE + (iNumberOfPlayersInLocalRange))
				{
					m_dwLastAimSendTick = dwThisTick;
					SendAimSyncData();
				}
			}

			// Targeting and Firing. Needs a very accurate send rate.
			else if (IS_TARGETING(wKeys) && IS_FIRING(wKeys)) 
			{
				if ((dwThisTick - m_dwLastAimSendTick) > (uint32_t)NETMODE_FIRING_SENDRATE + (iNumberOfPlayersInLocalRange))
				{
					m_dwLastAimSendTick = dwThisTick;
					SendAimSyncData();
				}
			}

			// Firing without targeting. Needs a normal onfoot sendrate.
			else if (!IS_TARGETING(wKeys) && IS_FIRING(wKeys)) 
			{
				if ((dwThisTick - m_dwLastAimSendTick) > (uint32_t)GetOptimumOnFootSendRate())
				{
					m_dwLastAimSendTick = dwThisTick;
					SendAimSyncData();
				}
			}
		}
		// PASSENGER
		else if(m_pPlayerPed->IsInVehicle() && m_pPlayerPed->IsAPassenger())
		{
			if((dwThisTick - m_dwLastSendTick) > (unsigned int)GetOptimumInCarSendRate())
			{
				m_dwLastSendTick = GetTickCount();
				SendPassengerFullSyncData();
			}
		}
	}

	// handle !IsActive spectating
	if(m_bIsSpectating && !m_bIsActive)
	{
		ProcessSpectating();
		return true;
	}

	// handle needs to respawn
	if(m_bIsWasted && (m_pPlayerPed->GetActionTrigger() != ACTION_WASTED) && 
		(m_pPlayerPed->GetActionTrigger() != ACTION_DEATH) )
	{
		if( m_bClearedToSpawn && !m_bWantsAnotherClass &&
			pNetGame->GetGameState() == GAMESTATE_CONNECTED)
		{
			if(m_pPlayerPed->GetHealth() > 0.0f)
				Spawn();
		}
		else
		{
			m_bIsWasted = false;
			HandleClassSelection();
			m_bWantsAnotherClass = false;
		}

		return true;
	}

	return true;
}

extern float                    m_fWeaponDamages[43 + 1];

void CLocalPlayer::SendBulletSyncData(PLAYERID byteHitID, uint8_t byteHitType, VECTOR vecHitPos)
{
	if (!m_pPlayerPed) return;
	switch (byteHitType)
	{
	case ENTITY_TYPE_UNKNOWN:
		break;
	case ENTITY_TYPE_PED: //player
		if (!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)byteHitID)) return;
		break;
	default: return; //unknown type
	}
	uint8_t byteCurrWeapon = m_pPlayerPed->GetCurrentWeapon(), byteShotWeapon;

	BULLET_SYNC blSync;

	blSync.hitId = byteHitID;
	blSync.hitType = byteHitType;

	if (byteHitType == ENTITY_TYPE_PED)
	{
		float fDistance = pNetGame->GetPlayerPool()->GetAt((PLAYERID)byteHitID)->GetPlayerPed()->GetDistanceFromLocalPlayerPed();
		if (byteCurrWeapon != 0 && fDistance < 1.0f)
			byteShotWeapon = 0;
		else
			byteShotWeapon = byteCurrWeapon;
	}
	blSync.weapId = byteShotWeapon;

	blSync.hitPos[0] = vecHitPos.X;
	blSync.hitPos[1] = vecHitPos.Y;
	blSync.hitPos[2] = vecHitPos.Z;
	blSync.offsets[0] = 0.0f;
	blSync.offsets[1] = 0.0f;
	blSync.offsets[2] = 0.0f;
	MATRIX4X4 mat;
	m_pPlayerPed->GetMatrix(&mat);
	blSync.origin[0] = mat.pos.X;
	blSync.origin[1] = mat.pos.Y;
	blSync.origin[2] = mat.pos.Z;
	RakNet::BitStream bsBulletSync;
	bsBulletSync.Write((uint8_t)ID_BULLET_SYNC);
	bsBulletSync.Write((const char*)& blSync, sizeof(BULLET_SYNC));
	pNetGame->GetRakClient()->Send(&bsBulletSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

	RakNet::BitStream bsRPC;
	bsRPC.Write((bool)false);
	bsRPC.Write((PLAYERID)blSync.hitId);
	bsRPC.Write((float)m_fWeaponDamages[blSync.weapId]);
	bsRPC.Write((uint32_t)blSync.weapId);
	bsRPC.Write((uint32_t)1);
	pNetGame->GetRakClient()->RPC(&RPC_PlayerGiveTakeDamage, &bsRPC, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
	Log("[BULLET_SYNC] sent.");
}

void CLocalPlayer::SendWastedNotification()
{
	RakNet::BitStream bsPlayerDeath;
	PLAYERID WhoWasResponsible = INVALID_PLAYER_ID;
	uint8_t byteDeathReason = 0;
	
	//uint8_t byteDeathReason = m_pPlayerPed->FindDeathReasonAndResponsiblePlayer(&WhoWasResponsible);

	bsPlayerDeath.Write(byteDeathReason);
	bsPlayerDeath.Write(WhoWasResponsible);
	pNetGame->GetRakClient()->RPC(&RPC_Death, &bsPlayerDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

bool CLocalPlayer::HandlePassengerEntryEx()
{
	if (GetTickCount() - m_dwPassengerEnterExit < 1000)
		return true;

	CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
	// CTouchInterface::IsHoldDown
	//int isHoldDown = (( int (*)(int, int, int))(g_libGTASA+0x270818+1))(0, 1, 1);

	VEHICLEID ClosetVehicleID = pVehiclePool->FindNearestToLocalPlayerPed();
	if (ClosetVehicleID < MAX_VEHICLES && pVehiclePool->GetSlotState(ClosetVehicleID))
	{
		CVehicle* pVehicle = pVehiclePool->GetAt(ClosetVehicleID);
		if (pVehicle->GetDistanceFromLocalPlayerPed() < 4.0f)
		{
			m_pPlayerPed->EnterVehicle(pVehicle->m_dwGTAId, true);
			SendEnterVehicleNotification(ClosetVehicleID, true);
			m_dwPassengerEnterExit = GetTickCount();
			return true;
		}
	}

	return false;
}

void CLocalPlayer::HandleClassSelection()
{
	m_bClearedToSpawn = false;

	if(m_pPlayerPed)
	{
		m_pPlayerPed->SetInitialState();
		m_pPlayerPed->SetHealth(100.0f);
		m_pPlayerPed->TogglePlayerControllable(0);
	}
	
	RequestClass(m_iSelectedClass);

	return;
}

// 0.3.7
void CLocalPlayer::HandleClassSelectionOutcome()
{
	if(m_pPlayerPed)
	{
		m_pPlayerPed->ClearAllWeapons();
		m_pPlayerPed->SetModelIndex(m_SpawnInfo.iSkin);
	}

	m_bClearedToSpawn = true;
}

void CLocalPlayer::SendNextClass()
{
	MATRIX4X4 matPlayer;
	m_pPlayerPed->GetMatrix(&matPlayer);

	if(m_iSelectedClass == (pNetGame->m_iSpawnsAvailable - 1)) m_iSelectedClass = 0;
	else m_iSelectedClass++;

	pGame->PlaySound(1052, matPlayer.pos.X, matPlayer.pos.Y, matPlayer.pos.Z);
	RequestClass(m_iSelectedClass);
}

void CLocalPlayer::SendPrevClass()
{
	MATRIX4X4 matPlayer;
	m_pPlayerPed->GetMatrix(&matPlayer);
	
	if(m_iSelectedClass == 0) m_iSelectedClass = (pNetGame->m_iSpawnsAvailable - 1);
	else m_iSelectedClass--;		

	pGame->PlaySound(1053,matPlayer.pos.X,matPlayer.pos.Y,matPlayer.pos.Z);
	RequestClass(m_iSelectedClass);
}

void CLocalPlayer::SendSpawn()
{
	RequestSpawn();
	m_bWaitingForSpawnRequestReply = true;
}

void CLocalPlayer::RequestClass(int iClass)
{
	RakNet::BitStream bsSpawnRequest;
	bsSpawnRequest.Write(iClass);
	pNetGame->GetRakClient()->RPC(&RPC_RequestClass, &bsSpawnRequest, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, 0);
}

void CLocalPlayer::RequestSpawn()
{
	RakNet::BitStream bsSpawnRequest;
	pNetGame->GetRakClient()->RPC(&RPC_RequestSpawn, &bsSpawnRequest, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, 0);
}

uint32_t CLocalPlayer::GetPlayerColorAsARGB()
{
	return (TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID()) >> 8) | 0xFF000000;
}

bool CLocalPlayer::HandlePassengerEntry()
{
	if(GetTickCount() - m_dwPassengerEnterExit < 1000 )
		return true;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	// CTouchInterface::IsHoldDown
    int isHoldDown = (( int (*)(int, int, int))(g_libGTASA+0x270818+1))(0, 1, 1);

	if (isHoldDown)
	{
		VEHICLEID ClosetVehicleID = pVehiclePool->FindNearestToLocalPlayerPed();
		if(ClosetVehicleID < MAX_VEHICLES && pVehiclePool->GetSlotState(ClosetVehicleID))
		{
			CVehicle* pVehicle = pVehiclePool->GetAt(ClosetVehicleID);
			if(pVehicle->GetDistanceFromLocalPlayerPed() < 4.0f)
			{
				m_pPlayerPed->EnterVehicle(pVehicle->m_dwGTAId, true);
				SendEnterVehicleNotification(ClosetVehicleID, true);
				m_dwPassengerEnterExit = GetTickCount();
				return true;
			}
		}
	}

	return false;
}



void CLocalPlayer::UpdateSurfing() {}
uint32_t CLocalPlayer::GetSpecialAction()
{
	CPlayerPed* pPed = GetPlayerPed();
	if (!pPed) return 0;
	if (pPed->IsCrouching())
	{
		return 1;
	}
	return 0;
}


void CLocalPlayer::SendEnterVehicleNotification(VEHICLEID VehicleID, bool bPassenger)
{
	RakNet::BitStream bsSend;
	uint8_t bytePassenger = 0;

	if(bPassenger)
		bytePassenger = 1;

	bsSend.Write(VehicleID);
	bsSend.Write(bytePassenger);

	pNetGame->GetRakClient()->RPC(&RPC_EnterVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0,false, UNASSIGNED_NETWORK_ID, nullptr);
}

void CLocalPlayer::SendExitVehicleNotification(VEHICLEID VehicleID)
{
	RakNet::BitStream bsSend;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle* pVehicle = pVehiclePool->GetAt(VehicleID);

	if(pVehicle)
	{ 
		if (!m_pPlayerPed->IsAPassenger()) 
			m_LastVehicle = VehicleID;

		bsSend.Write(VehicleID);
		pNetGame->GetRakClient()->RPC(&RPC_ExitVehicle,&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false, UNASSIGNED_NETWORK_ID, NULL);
	}
}

void CLocalPlayer::UpdateRemoteInterior(uint8_t byteInterior)
{
	Log("CLocalPlayer::UpdateRemoteInterior %d", byteInterior);

	m_byteCurInterior = byteInterior;
	RakNet::BitStream bsUpdateInterior;
	bsUpdateInterior.Write(byteInterior);
	pNetGame->GetRakClient()->RPC(&RPC_SetInteriorId, &bsUpdateInterior, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, NULL);
}

void CLocalPlayer::SetSpawnInfo(PLAYER_SPAWN_INFO *pSpawn)
{
	memcpy(&m_SpawnInfo, pSpawn, sizeof(PLAYER_SPAWN_INFO));
	m_bHasSpawnInfo = true;
}

bool CLocalPlayer::Spawn()
{
	if(!m_bHasSpawnInfo) return false;

	if(pSettings && pSettings->GetReadOnly().iHud)
	{
		*(uint8_t*)(g_libGTASA+0x7165E8) = 0;
		g_pJavaWrapper->ShowHud();
	}
	else
	{
		*(uint8_t*)(g_libGTASA+0x7165E8) = 1;
		g_pJavaWrapper->HideHud();
	}
   
    //g_pJavaWrapper->ShowSpeed();
	CCamera *pGameCamera;
	pGameCamera = pGame->GetCamera();
	pGameCamera->Restore();
	pGameCamera->SetBehindPlayer();
	pGame->DisplayWidgets(true);
	pGame->DisplayHUD(true);
	m_pPlayerPed->TogglePlayerControllable(true);
	
	if(!bFirstSpawn)
		m_pPlayerPed->SetInitialState();
	else
		bFirstSpawn = false;

	bFirstSpawn = false;

	pGame->RefreshStreamingAt(m_SpawnInfo.vecPos.X,m_SpawnInfo.vecPos.Y);

	m_pPlayerPed->RestartIfWastedAt(&m_SpawnInfo.vecPos, m_SpawnInfo.fRotation);
	m_pPlayerPed->SetModelIndex(m_SpawnInfo.iSkin);
	m_pPlayerPed->ClearAllWeapons();
	m_pPlayerPed->ResetDamageEntity();

	pGame->DisableTrainTraffic();

	// CCamera::Fade
	WriteMemory(g_libGTASA+0x36EA2C, (uintptr_t)"\x70\x47", 2); // bx lr

	m_pPlayerPed->TeleportTo(m_SpawnInfo.vecPos.X,
		m_SpawnInfo.vecPos.Y, (m_SpawnInfo.vecPos.Z + 0.5f));

	m_pPlayerPed->ForceTargetRotation(m_SpawnInfo.fRotation);

	m_bIsWasted = false;
	m_bIsActive = true;
	m_bWaitingForSpawnRequestReply = false;

	RakNet::BitStream bsSendSpawn;
	pNetGame->GetRakClient()->RPC(&RPC_Spawn, &bsSendSpawn, HIGH_PRIORITY, 
		RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, NULL);

	return true;
}

uint32_t CLocalPlayer::GetPlayerColor()
{
	return TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID());
}

void CLocalPlayer::SetPlayerColor(uint32_t dwColor)
{
	SetRadarColor(pNetGame->GetPlayerPool()->GetLocalPlayerID(), dwColor);
}

void CLocalPlayer::ApplySpecialAction(uint8_t byteSpecialAction)
{
	if (m_pPlayerPed)
	{
		m_pPlayerPed->SetPlayerSpecialAction(byteSpecialAction);
	}

	switch(byteSpecialAction)
	{
		case SPECIAL_ACTION_NONE:
		break;

		case SPECIAL_ACTION_USEJETPACK:
		break;
	}
}

int CLocalPlayer::GetOptimumOnFootSendRate()
{
	if(!m_pPlayerPed) return 1000;

	return (iNetModeNormalOnfootSendRate + DetermineNumberOfPlayersInLocalRange());
}

int CLocalPlayer::GetOptimumInCarSendRate()
{
	if(!m_pPlayerPed) return 1000;

	return (iNetModeNormalInCarSendRate + DetermineNumberOfPlayersInLocalRange());
}

uint8_t CLocalPlayer::DetermineNumberOfPlayersInLocalRange()
{
	int iNumPlayersInRange = 0;
	for(int i=2; i < PLAYER_PED_SLOTS; i++)
		if(bUsedPlayerSlots[i]) iNumPlayersInRange++;

	return iNumPlayersInRange;
}
#include "..//chatwindow.h"
extern CChatWindow* pChatWindow;

void CLocalPlayer::SendOnKeyFullSyncData()
{
	RakNet::BitStream bsPlayerSync;
	//MATRIX4X4 matPlayer;


	uint8_t exKeys = GetPlayerPed()->GetExtendedKeys();
}


void CLocalPlayer::SendOnFootFullSyncData()
{
	RakNet::BitStream bsPlayerSync;
	MATRIX4X4 matPlayer;
	VECTOR vecMoveSpeed;
	uint16_t lrAnalog, udAnalog;
	uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);

	ONFOOT_SYNC_DATA ofSync;

	m_pPlayerPed->GetMatrix(&matPlayer);
	m_pPlayerPed->GetMoveSpeedVector(&vecMoveSpeed);

	ofSync.lrAnalog = lrAnalog;
	ofSync.udAnalog = udAnalog;
	ofSync.wKeys = wKeys;
	ofSync.vecPos.X = matPlayer.pos.X;
	ofSync.vecPos.Y = matPlayer.pos.Y;
	ofSync.vecPos.Z = matPlayer.pos.Z;

	ofSync.quat.SetFromMatrix(matPlayer);
	ofSync.quat.Normalize();

	if( FloatOffset(ofSync.quat.w, m_OnFootData.quat.w) < 0.00001 &&
		FloatOffset(ofSync.quat.x, m_OnFootData.quat.x) < 0.00001 &&
		FloatOffset(ofSync.quat.y, m_OnFootData.quat.y) < 0.00001 &&
		FloatOffset(ofSync.quat.z, m_OnFootData.quat.z) < 0.00001)
	{
		ofSync.quat.Set(m_OnFootData.quat);
	}

	ofSync.byteHealth = (uint8_t)m_pPlayerPed->GetHealth();
	ofSync.byteArmour = (uint8_t)m_pPlayerPed->GetArmour();

	uint8_t exKeys = GetPlayerPed()->GetExtendedKeys();
	ofSync.byteCurrentWeapon = (exKeys << 6) | ofSync.byteCurrentWeapon & 0x3F;
	ofSync.byteCurrentWeapon ^= (ofSync.byteCurrentWeapon ^ GetPlayerPed()->GetCurrentWeapon()) & 0x3F;

	ofSync.byteSpecialAction = (uint8_t)GetSpecialAction();
	ofSync.vecMoveSpeed.X = vecMoveSpeed.X;
	ofSync.vecMoveSpeed.Y = vecMoveSpeed.Y;
	ofSync.vecMoveSpeed.Z = vecMoveSpeed.Z;

	ofSync.vecSurfOffsets.X = 0.0f;
	ofSync.vecSurfOffsets.Y = 0.0f;
	ofSync.vecSurfOffsets.Z = 0.0f;
	ofSync.wSurfInfo = 0;

	ofSync.dwAnimation = GetCurrentAnimationIndexFlag();

	if( (GetTickCount() - m_dwLastUpdateOnFootData) > 500 || memcmp(&m_OnFootData, &ofSync, sizeof(ONFOOT_SYNC_DATA)))
	{
		m_dwLastUpdateOnFootData = GetTickCount();

		bsPlayerSync.Write((uint8_t)ID_PLAYER_SYNC);
		bsPlayerSync.Write((char*)&ofSync, sizeof(ONFOOT_SYNC_DATA));
		pNetGame->GetRakClient()->Send(&bsPlayerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

		memcpy(&m_OnFootData, &ofSync, sizeof(ONFOOT_SYNC_DATA));
	}
}

void CLocalPlayer::SendInCarFullSyncData()
{
	RakNet::BitStream bsVehicleSync;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(!pVehiclePool) return;

	MATRIX4X4 matPlayer;
	VECTOR vecMoveSpeed;

	uint16_t lrAnalog, udAnalog;
	uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);
	CVehicle *pVehicle;

	INCAR_SYNC_DATA icSync;
	memset(&icSync, 0, sizeof(INCAR_SYNC_DATA));

	if(m_pPlayerPed)
	{
		icSync.VehicleID = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());

		if(icSync.VehicleID == INVALID_VEHICLE_ID) return;

		icSync.lrAnalog = lrAnalog;
		icSync.udAnalog = udAnalog;
		icSync.wKeys = wKeys;

		pVehicle = pVehiclePool->GetAt(icSync.VehicleID);
		if(!pVehicle) return;

		pVehicle->GetMatrix(&matPlayer);
		pVehicle->GetMoveSpeedVector(&vecMoveSpeed);

		icSync.quat.SetFromMatrix(matPlayer);
		icSync.quat.Normalize();

		if(	FloatOffset(icSync.quat.w, m_InCarData.quat.w) < 0.00001 &&
			FloatOffset(icSync.quat.x, m_InCarData.quat.x) < 0.00001 &&
			FloatOffset(icSync.quat.y, m_InCarData.quat.y) < 0.00001 &&
			FloatOffset(icSync.quat.z, m_InCarData.quat.z) < 0.00001)
		{
			icSync.quat.Set(m_InCarData.quat);
		}

		// pos
		icSync.vecPos.X = matPlayer.pos.X;
		icSync.vecPos.Y = matPlayer.pos.Y;
		icSync.vecPos.Z = matPlayer.pos.Z;
		// move speed
		icSync.vecMoveSpeed.X = vecMoveSpeed.X;
		icSync.vecMoveSpeed.Y = vecMoveSpeed.Y;
		icSync.vecMoveSpeed.Z = vecMoveSpeed.Z;

		if (pVehicle->GetHealth() <= 300.0f)
		{
			pVehicle->SetHealth(300.0f);
		}

		icSync.fCarHealth = pVehicle->GetHealth();
		icSync.bytePlayerHealth = (uint8_t)m_pPlayerPed->GetHealth();
		icSync.bytePlayerArmour = (uint8_t)m_pPlayerPed->GetArmour();

		//icSync.byteSirenOn = pVehicle->IsSirenOn() != 0;
		//icSync.byteLandingGearState = pVehicle->GetLandingGearState() != 0;
		uint8_t exKeys = GetPlayerPed()->GetExtendedKeys();
		icSync.byteCurrentWeapon = (exKeys << 6) | icSync.byteCurrentWeapon & 0x3F;
		icSync.byteCurrentWeapon ^= (icSync.byteCurrentWeapon ^ GetPlayerPed()->GetCurrentWeapon()) & 0x3F;

		icSync.TrailerID = 0;
		VEHICLE_TYPE* vehTrailer = (VEHICLE_TYPE*)pVehicle->m_pVehicle->dwTrailer;
		if (vehTrailer != NULL)
		{
			uint16_t id = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(vehTrailer);
			if (id == INVALID_OBJECT_ID) return;
			if (ScriptCommand(&is_trailer_on_cab,
				id,
				pVehicle->m_dwGTAId))
			{
				icSync.TrailerID = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(vehTrailer);

			}
			else
			{
				icSync.TrailerID = 0;
			}
		}
		if (icSync.TrailerID && icSync.TrailerID < MAX_VEHICLES)
		{
			MATRIX4X4 matTrailer;
			TRAILER_SYNC_DATA trSync;
			CVehicle* pTrailer = pVehiclePool->GetAt(icSync.TrailerID);
			if (pTrailer)
			{
				pTrailer->GetMatrix(&matTrailer);
				trSync.trailerID = icSync.TrailerID;

				trSync.vecPos.X = matTrailer.pos.X;
				trSync.vecPos.Y = matTrailer.pos.Y;
				trSync.vecPos.Z = matTrailer.pos.Z;

				CQuaternion syncQuat;
				syncQuat.SetFromMatrix(matTrailer);
				trSync.quat = syncQuat;

				pTrailer->GetMoveSpeedVector(&trSync.vecMoveSpeed);
				pTrailer->GetTurnSpeedVector(&trSync.vecTurnSpeed);
				RakNet::BitStream bsTrailerSync;
				bsTrailerSync.Write((BYTE)ID_TRAILER_SYNC);
				bsTrailerSync.Write((char*)& trSync, sizeof(TRAILER_SYNC_DATA));
				pNetGame->GetRakClient()->Send(&bsTrailerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			}
		}

		// send
		if( (GetTickCount() - m_dwLastUpdateInCarData) > 500 || memcmp(&m_InCarData, &icSync, sizeof(INCAR_SYNC_DATA)))
		{
			m_dwLastUpdateInCarData = GetTickCount();

			bsVehicleSync.Write((uint8_t)ID_VEHICLE_SYNC);
			bsVehicleSync.Write((char*)&icSync, sizeof(INCAR_SYNC_DATA));
			pNetGame->GetRakClient()->Send(&bsVehicleSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

			memcpy(&m_InCarData, &icSync, sizeof(INCAR_SYNC_DATA));
		}
	}
}

void CLocalPlayer::SendPassengerFullSyncData()
{
	RakNet::BitStream bsPassengerSync;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	uint16_t lrAnalog, udAnalog;
	uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);
	PASSENGER_SYNC_DATA psSync;
	MATRIX4X4 mat;

	psSync.VehicleID = pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());

	if(psSync.VehicleID == INVALID_VEHICLE_ID) return;

	psSync.lrAnalog = lrAnalog;
	psSync.udAnalog = udAnalog;
	psSync.wKeys = wKeys;
	psSync.bytePlayerHealth = (uint8_t)m_pPlayerPed->GetHealth();
	psSync.bytePlayerArmour = (uint8_t)m_pPlayerPed->GetArmour();

	psSync.byteSeatFlags = m_pPlayerPed->GetVehicleSeatID();
	psSync.byteDriveBy = 0;//m_bPassengerDriveByMode;

	psSync.byteCurrentWeapon = 0;//m_pPlayerPed->GetCurrentWeapon();

	m_pPlayerPed->GetMatrix(&mat);
	psSync.vecPos.X = mat.pos.X;
	psSync.vecPos.Y = mat.pos.Y;
	psSync.vecPos.Z = mat.pos.Z;

	// send
	if((GetTickCount() - m_dwLastUpdatePassengerData) > 500 || memcmp(&m_PassengerData, &psSync, sizeof(PASSENGER_SYNC_DATA)))
	{
		m_dwLastUpdatePassengerData = GetTickCount();

		bsPassengerSync.Write((uint8_t)ID_PASSENGER_SYNC);
		bsPassengerSync.Write((char*)&psSync, sizeof(PASSENGER_SYNC_DATA));
		pNetGame->GetRakClient()->Send(&bsPassengerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

		memcpy(&m_PassengerData, &psSync, sizeof(PASSENGER_SYNC_DATA));
	}
}

void CLocalPlayer::SendAimSyncData()
{
	RakNet::BitStream bsAimSync;
	AIM_SYNC_DATA aimSync;
	CAMERA_AIM * caAim = m_pPlayerPed->GetCurrentAim();

	aimSync.byteCamMode = (uint8_t)m_pPlayerPed->GetCameraMode();
	aimSync.vecAimf1[0] = caAim->f1x;
	aimSync.vecAimf1[1] = caAim->f1y;
	aimSync.vecAimf1[2] = caAim->f1z;
	aimSync.vecAimPos[0] = caAim->pos1x;
	aimSync.vecAimPos[1] = caAim->pos1y;
	aimSync.vecAimPos[2] = caAim->pos1z;

	aimSync.fAimZ = m_pPlayerPed->GetAimZ();

#ifdef GAME_EDITION_CR
	aimSync.bUnk = pKeyBoard->IsOpen();
#else
	aimSync.bUnk = 0;
#endif

	aimSync.byteCamExtZoom = (uint8_t)(m_pPlayerPed->GetCameraExtendedZoom() * 63.0f);

	WEAPON_SLOT_TYPE* pwstWeapon = m_pPlayerPed->GetCurrentWeaponSlot();
	if (pwstWeapon->dwState == 2)
	{
		aimSync.byteWeaponState = WS_RELOADING;
	}
	else
	{
		aimSync.byteWeaponState = (pwstWeapon->dwAmmoInClip > 1) ? WS_MORE_BULLETS : pwstWeapon->dwAmmoInClip;
	}

	
	bsAimSync.Write((uint8_t)ID_AIM_SYNC);
	bsAimSync.Write((char*)&aimSync, sizeof(AIM_SYNC_DATA));
	pNetGame->GetRakClient()->Send(&bsAimSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void CLocalPlayer::ProcessSpectating()
{
	RakNet::BitStream bsSpectatorSync;
	SPECTATOR_SYNC_DATA spSync;
	MATRIX4X4 matPos;

	uint16_t lrAnalog, udAnalog;
	uint16_t wKeys = m_pPlayerPed->GetKeys(&lrAnalog, &udAnalog);
	pGame->GetCamera()->GetMatrix(&matPos);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(!pPlayerPool || !pVehiclePool) return;

	spSync.vecPos.X = matPos.pos.X;
	spSync.vecPos.Y = matPos.pos.Y;
	spSync.vecPos.Z = matPos.pos.Z;
	spSync.lrAnalog = lrAnalog;
	spSync.udAnalog = udAnalog;
	spSync.wKeys = wKeys;

	if((GetTickCount() - m_dwLastSendSpecTick) > GetOptimumOnFootSendRate())
	{
		m_dwLastSendSpecTick = GetTickCount();
		bsSpectatorSync.Write((uint8_t)ID_SPECTATOR_SYNC);
		bsSpectatorSync.Write((char*)&spSync, sizeof(SPECTATOR_SYNC_DATA));
		pNetGame->GetRakClient()->Send(&bsSpectatorSync, HIGH_PRIORITY, UNRELIABLE, 0);

		if((GetTickCount() - m_dwLastSendAimTick) > (GetOptimumOnFootSendRate() * 2))
		{
			m_dwLastSendAimTick = GetTickCount();
			
		}
	}

	pGame->DisplayHUD(false);

	m_pPlayerPed->SetHealth(100.0f);
	GetPlayerPed()->TeleportTo(spSync.vecPos.X, spSync.vecPos.Y, spSync.vecPos.Z + 20.0f);

	// handle spectate player left the server
	if(m_byteSpectateType == SPECTATE_TYPE_PLAYER &&
		!pPlayerPool->GetSlotState(m_SpectateID))
	{
		m_byteSpectateType = SPECTATE_TYPE_NONE;
		m_bSpectateProcessed = false;
	}

	// handle spectate player is no longer active (ie Died)
	if(m_byteSpectateType == SPECTATE_TYPE_PLAYER &&
		pPlayerPool->GetSlotState(m_SpectateID) &&
		(!pPlayerPool->GetAt(m_SpectateID)->IsActive() ||
		pPlayerPool->GetAt(m_SpectateID)->GetState() == PLAYER_STATE_WASTED))
	{
		m_byteSpectateType = SPECTATE_TYPE_NONE;
		m_bSpectateProcessed = false;
	}

	if(m_bSpectateProcessed) return;

	if(m_byteSpectateType == SPECTATE_TYPE_NONE)
	{
		GetPlayerPed()->RemoveFromVehicleAndPutAt(0.0f, 0.0f, 10.0f);
		pGame->GetCamera()->SetPosition(50.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f);
		pGame->GetCamera()->LookAtPoint(60.0f, 60.0f, 50.0f, 2);
		m_bSpectateProcessed = true;
	}
	else if(m_byteSpectateType == SPECTATE_TYPE_PLAYER)
	{
		uint32_t dwGTAId = 0;
		CPlayerPed *pPlayerPed = 0;

		if(pPlayerPool->GetSlotState(m_SpectateID))
		{
			pPlayerPed = pPlayerPool->GetAt(m_SpectateID)->GetPlayerPed();
			if(pPlayerPed)
			{
				dwGTAId = pPlayerPed->m_dwGTAId;
				ScriptCommand(&camera_on_actor, dwGTAId, m_byteSpectateMode, 2);
				m_bSpectateProcessed = true;
			}
		}
	}
	else if(m_byteSpectateType == SPECTATE_TYPE_VEHICLE)
	{
		CVehicle *pVehicle = nullptr;
		uint32_t dwGTAId = 0;

		if (pVehiclePool->GetSlotState((VEHICLEID)m_SpectateID)) 
		{
			pVehicle = pVehiclePool->GetAt((VEHICLEID)m_SpectateID);
			if(pVehicle) 
			{
				dwGTAId = pVehicle->m_dwGTAId;
				ScriptCommand(&camera_on_vehicle, dwGTAId, m_byteSpectateMode, 2);
				m_bSpectateProcessed = true;
			}
		}
	}	
}

void CLocalPlayer::ToggleSpectating(bool bToggle)
{
	if(m_bIsSpectating && !bToggle)
		Spawn();

	m_bIsSpectating = bToggle;
	m_byteSpectateType = SPECTATE_TYPE_NONE;
	m_SpectateID = 0xFFFFFFFF;
	m_bSpectateProcessed = false;
}

void CLocalPlayer::SpectatePlayer(PLAYERID playerId)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if(pPlayerPool && pPlayerPool->GetSlotState(playerId))
	{
		if(pPlayerPool->GetAt(playerId)->GetState() != PLAYER_STATE_NONE &&
			pPlayerPool->GetAt(playerId)->GetState() != PLAYER_STATE_WASTED)
		{
			m_byteSpectateType = SPECTATE_TYPE_PLAYER;
			m_SpectateID = playerId;
			m_bSpectateProcessed = false;
		}
	}
}

void CLocalPlayer::SpectateVehicle(VEHICLEID VehicleID)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if (pVehiclePool && pVehiclePool->GetSlotState(VehicleID)) 
	{
		m_byteSpectateType = SPECTATE_TYPE_VEHICLE;
		m_SpectateID = VehicleID;
		m_bSpectateProcessed = false;
	}
}