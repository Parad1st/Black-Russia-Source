#pragma once

class CWeaponsOutFit
{
	struct SWeaponOutFitSettings
	{
	public:
		int iSlot;
		int iBone;
		VECTOR vOffset;
		VECTOR vRotation;

		bool bUseSecondary;
		int iBoneSecondary;
		VECTOR vOffsetSecondary;
		VECTOR vRotationSecondary;

		SWeaponOutFitSettings()
		{
			bUseSecondary = false;
			iBone = -1;
			iBoneSecondary = -1;

			vOffset.X = 0.0f;
			vOffset.Y = 0.0f;
			vOffset.Z = 0.0f;

			vRotation.X = 0.0f;
			vRotation.Y = 0.0f;
			vRotation.Z = 0.0f;

			vOffsetSecondary.X = 0.0f;
			vOffsetSecondary.Y = 0.0f;
			vOffsetSecondary.Z = 0.0f;

			vRotationSecondary.X = 0.0f;
			vRotationSecondary.Y = 0.0f;
			vRotationSecondary.Z = 0.0f;
		}
	};

	static uint32_t m_bLastTickUpdate;
	static bool m_bUpdated;
	static SWeaponOutFitSettings m_Settings[MAX_WEAPON_MODELS];
	static bool m_bUsedWeapon[MAX_WEAPON_MODELS];
	static int m_iSlots[MAX_WEAPON_MODELS];
	static bool m_bUsedSlots[10];
	static bool m_bEnabled;
	static bool m_bParsed;

	static int GetWeaponModelIDFromSlot(int iSlot);

	static void OnWeaponAdded(CPlayerPed* pPed, int iWeaponID);
	static void OnWeaponRemoved(CPlayerPed* pPed, int iWeaponID);
	static void OnPedDie(CPlayerPed* pPed);
public:
	static void ParseDatFile();
	static void ProcessLocalPlayer(CPlayerPed* pPed);

	static void SetEnabled(bool bEnabled);

	static void OnUpdateOffsets();
};