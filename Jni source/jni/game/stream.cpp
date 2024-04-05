/*

SA:MP Multiplayer Modification
Copyright 2004-2005 SA:MP Team

Version: $Id: textdraw.cpp,v 1.4 2008-04-16 08:54:17 kyecvs Exp $

*/

#include "../main.h"
#include "game.h"
#include "font.h"

#include <vector>
#include <string>
#include <thread>
#include <sstream>
#include "../chatwindow.h"
extern CChatWindow* pChatWindow;
extern CGame* pGame;
#include "..//net/netgame.h"
extern CNetGame* pNetGame;

void CStream::CreateStream() // ready 50%
{
	if (m_hStream)
	{
		DestroyStream();
	}
	m_hStream = BASS_StreamCreateURL(m_szUrl, 0, BASS_SAMPLE_3D | BASS_SAMPLE_MONO | BASS_SAMPLE_LOOP, nullptr, nullptr);
	BASS_ChannelPlay(m_hStream, false);
	BASS_3DVECTOR vec(m_vPos.X, m_vPos.Y, m_vPos.Z);
	BASS_3DVECTOR orient(0.0f, 0.0f, 0.0f);
	BASS_3DVECTOR vel(0.0f, 0.0f, 0.0f);
	BASS_ChannelSet3DPosition(m_hStream, &vec, &orient, &vel);
	BASS_ChannelSet3DAttributes(m_hStream, BASS_3DMODE_NORMAL, 1.0f, m_fDistance, 360, 360, 1.0f);
}

void CStream::DestroyStream() // ready
{
	if (m_hStream)
	{
		BASS_StreamFree(m_hStream);
	}
	m_hStream = NULL;
}

void CStream::ProcessAttached() // todo
{
	if (!pNetGame)
	{
		return;
	}
	if (!pNetGame->GetPlayerPool() || !pNetGame->GetVehiclePool())
	{
		return;
	}
	if (m_iAttachType == 1) // vehicle
	{
		CVehicle* pVeh = pNetGame->GetVehiclePool()->GetAt(m_iAttachedTo);
		if (!pVeh)
		{
			return;
		}
		MATRIX4X4 mat;
		pVeh->GetMatrix(&mat);
		memcpy(&m_vPos, &mat.pos, sizeof(VECTOR));
		//pChatWindow->AddDebugMessage("processed for vehicle %d", m_iAttachedTo);
	}
	if (m_iAttachType == 2) // player
	{
		if (!pNetGame->GetPlayerPool()->GetAt(m_iAttachedTo))
		{
			return;
		}
		CPlayerPed* pPed = pNetGame->GetPlayerPool()->GetAt(m_iAttachedTo)->GetPlayerPed();
		if (!pPed)
		{
			return;
		}
		MATRIX4X4 mat;
		pPed->GetMatrix(&mat);
		memcpy(&m_vPos, &mat.pos, sizeof(VECTOR));
		//pChatWindow->AddDebugMessage("processed for player %d", m_iAttachedTo);
	}

	BASS_3DVECTOR vec(m_vPos.X, m_vPos.Y, m_vPos.Z);
	BASS_3DVECTOR orient(0.0f, 0.0f, 0.0f);
	BASS_3DVECTOR vel(0.0f, 0.0f, 0.0f);

	BASS_ChannelSet3DPosition(m_hStream, &vec, &orient, &vel);
}

CStream::CStream(VECTOR* pPos, int iVirtualWorld, int iInterior, float fDistance, const char* szUrl) // ready
{
	m_bIsDeactivated = false;
	m_bIsAttached = false;
	m_iAttachType = 0;
	m_iAttachedTo = 0;

	m_hStream = NULL;

	strcpy(&m_szUrl[0], szUrl);
	memcpy(&m_vPos, pPos, sizeof(VECTOR));

	m_iVirtualWorld = iVirtualWorld;
	m_iInterior = iInterior;
	m_fDistance = fDistance;
}

CStream::~CStream() // ready
{
	DestroyStream();
}

void CStream::AttachToVehicle(int iVehicleID) // ready
{
	m_bIsAttached = true;
	m_iAttachType = 1;
	m_iAttachedTo = iVehicleID;
}

void CStream::AttachToPlayer(int iPlayerID) // ready
{
	m_bIsAttached = true;
	m_iAttachType = 2;
	m_iAttachedTo = iPlayerID;
}

void CStream::SetVolume(float fValue)
{
	BASS_ChannelSetAttribute(m_hStream, BASS_ATTRIB_VOL, fValue);
}

void CStream::SetIsDeactivated(bool bIsDeactivated)
{
	m_bIsDeactivated = bIsDeactivated;
}

void CStream::Process(MATRIX4X4* pMatListener) // todo
{
	float fDistance = GetDistanceBetween3DPoints(&(pMatListener->pos), &m_vPos);
	if (fDistance <= m_fDistance && !m_hStream && !m_bIsDeactivated)
	{
		//pChatWindow->AddDebugMessage("create stream");
		CreateStream();
	}
	else if (fDistance > m_fDistance && m_hStream)
	{
		//pChatWindow->AddDebugMessage("destroy stream");
		DestroyStream();
	}


	if (m_bIsDeactivated)
	{
		DestroyStream();
	}
}

void CStream::SetPosition(VECTOR vvec)
{
	BASS_3DVECTOR vec(vvec.X, vvec.Y, vvec.Z);
	BASS_3DVECTOR orient(0.0f, 0.0f, 0.0f);
	BASS_3DVECTOR vel(0.0f, 0.0f, 0.0f);

	m_vPos.X = vvec.X;
	m_vPos.Y = vvec.Y;
	m_vPos.Z = vvec.Z;

	BASS_ChannelSet3DPosition(m_hStream, &vec, &orient, &vel);
	BASS_ChannelSet3DAttributes(m_hStream, BASS_3DMODE_OFF, 1.0f, m_fDistance, 360, 360, 1.0f);
}
