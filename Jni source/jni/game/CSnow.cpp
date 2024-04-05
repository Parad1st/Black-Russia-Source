#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"

#include "CSnow.h"

uint32_t CSnow::m_dwParticeHandle1 = 0;
uint32_t CSnow::m_dwParticeHandle2 = 0;
uint32_t CSnow::m_dwLastTickCreated = 0;
int CSnow::m_iCurrentSnow = 0;

static const char* g_iSnows[] = { "SNOWFXII", "SNOWFXIII", "SNOWFXIV" };

void CSnow::Initialise()
{
	m_dwParticeHandle1 = 0;
	m_dwParticeHandle2 = 0;
	m_dwLastTickCreated = GetTickCount();
}
#include "..//chatwindow.h"
extern CChatWindow* pChatWindow;
void CSnow::Process(CPlayerPed* pPed, int iInterior)
{
	if (!pPed)
	{
		return;
	}
	
	if (iInterior && m_dwParticeHandle1 && m_dwParticeHandle2)
	{
		ScriptCommand(&destroy_particle, m_dwParticeHandle1);
		ScriptCommand(&destroy_particle, m_dwParticeHandle2);

		m_dwParticeHandle1 = 0;
		m_dwParticeHandle2 = 0;
		return;
	}
	if (!m_dwParticeHandle1 && !m_dwParticeHandle2)
	{
		if (m_iCurrentSnow == 0)
		{
			return;
		}
		if (m_iCurrentSnow < 0 || m_iCurrentSnow >= 4)
		{
			m_iCurrentSnow = 1;
		}
		//pChatWindow->AddDebugMessage("govno %d", m_iCurrentSnow);
		uint32_t dwActorhandle = pPed->m_dwGTAId;

		ScriptCommand(&attach_particle_to_actor, g_iSnows[m_iCurrentSnow-1], dwActorhandle, 0.0f, 10.0f, 2.0f, 1, &m_dwParticeHandle1);
		ScriptCommand(&make_particle_visible, m_dwParticeHandle1);

		ScriptCommand(&attach_particle_to_actor, g_iSnows[m_iCurrentSnow-1], dwActorhandle, 0.0f, 1.0f, 2.0f, 1, &m_dwParticeHandle2);
		ScriptCommand(&make_particle_visible, m_dwParticeHandle2);

		m_dwLastTickCreated = GetTickCount();
	}
	if (GetTickCount() - m_dwLastTickCreated >= 2000)
	{
		ScriptCommand(&destroy_particle, m_dwParticeHandle1);
		ScriptCommand(&destroy_particle, m_dwParticeHandle2);

		m_dwParticeHandle1 = 0;
		m_dwParticeHandle2 = 0;
	}
}

void CSnow::SetCurrentSnow(int iSnow)
{
	m_iCurrentSnow = iSnow;
}

int CSnow::GetCurrentSnow()
{
	return m_iCurrentSnow;
}
