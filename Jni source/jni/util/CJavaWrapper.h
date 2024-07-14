#pragma once

#include <jni.h>

#include <string>

#define EXCEPTION_CHECK(env) \
	if ((env)->ExceptionCheck()) \ 
	{ \
		(env)->ExceptionDescribe(); \
		(env)->ExceptionClear(); \
		return; \
	}

class CJavaWrapper
{
	jobject activity;

	jmethodID s_GetClipboardText;
	jmethodID s_CallLauncherActivity;

                  jmethodID s_RadarBR;

	jmethodID s_ShowInputLayout;
	jmethodID s_HideInputLayout;

	jmethodID s_ShowClientSettings;
	jmethodID s_SetUseFullScreen;
	jmethodID s_MakeDialog;

	jmethodID s_showHud;
                  jmethodID s_hideHud;
	jmethodID s_updateHudInfo;

	jmethodID s_showradar;
                  jmethodID s_hideradar;

	jmethodID s_showGps;
                  jmethodID s_hideGps;

	jmethodID s_showZona;
	jmethodID s_hideZona;

                  jmethodID s_showx2;
	jmethodID s_hidex2;

	jmethodID s_showSpeed;
                  jmethodID s_hideSpeed;
	jmethodID s_updateSpeedInfo;

	jmethodID s_showNotification;
	jmethodID s_showMenu;

	jmethodID s_setPauseState;

	jmethodID s_showSplash;
	jmethodID s_updateSplash;
	jmethodID s_showRadial;

        jmethodID s_updateAuto;
	jmethodID s_showAuto;
	jmethodID s_hideAuto;

        jmethodID s_showShop;
	jmethodID s_hideShop;

        jmethodID s_showRecon;
	jmethodID s_hideRecon;

	jmethodID s_showFuelStation;
        jmethodID s_showTabWindow;
	jmethodID s_setTabStat;
        jmethodID s_showSpawn;
        jmethodID s_showInventory;
        jmethodID s_showJbl;
        jmethodID s_showPreDeath;
        jmethodID addChatMessage;
        jmethodID s_addChatMessage;
        //jmethodID closekeY;
        jmethodID s_showPerson;
	jmethodID s_hidePerson;
	
	jmethodID s_showHudFeatures;
	jmethodID s_hideHudFeatures;

	jmethodID s_showDonate;
	jmethodID s_updateDonate;
	jmethodID s_show_sc;
	
	jmethodID s_showHudAndLogo;
	jmethodID s_hideHudAndLogo;
        jmethodID s_showHelper;
        jmethodID s_hideHelper;
public:
	JNIEnv* GetEnv();

	std::string GetClipboardString();
	void CallLauncherActivity(int type);

	void ShowInputLayout();
	void HideInputLayout();

	void ShowRadar();
	void HideRadar();

	void SetRadar();

	void ShowClientSettings();

	void SetUseFullScreen(int b);

	void UpdateHudInfo(int health, int armour, int hunger, int weaponidweik, int ammo, int ammoinclip, int money, int wanted);
	void ShowHud();
                  void HideHud();

	void ShowGPS();
                  void HideGPS();

	void ShowZona();
                  void HideZona();

	void ShowX2();
                  void HideX2();
                  
   void ShowHudFeatures();
   void HideHudFeatures();
   
   void ShowHudAndLogo();
   void HideHudAndLogo();

	void UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock);
	void ShowSpeed();
                  void HideSpeed();

	void MakeDialog(int dialogId, int dialogTypeId, char* caption, char* content, char* leftBtnText, char* rightBtnText); // Диалоги
	void ShowNotification(int type, char* text, int duration, char* actionforBtn, char* textBtn);
	void ShowMenu();

	void SetPauseState(bool a1);

	void ShowSplash();
	void UpdateSplash(int progress);	
	
	void ShowRadial(bool park, bool key, bool doors, bool lights, bool suspension, bool launch_control, bool engine, bool turbo);

        void ShowAuto();
	void UpdateAuto(char* name, int price, int count, float maxspeed, float acceleration, int gear);
	void HideAuto();

        void ShowShop(int price);
	void HideShop();

	void HideRecon();
	void ShowRecon(char* nick, int id);

	void ShowFuelStation(int type, int price1, int price2, int price3, int price4, int price5, int maxCount);
        void ShowSpawn();
        void ShowTabWindow();
	void SetTabStat(int id, char* name, int score, int ping);
        void ShowInventory(int hp, int hunger, int skin_id, int key, int sumka);
        void ShowJbl();
        void ShowPreDeath();
        void AddChatMessage(const char msg[], uint32_t textcolor);
        //void closekey();

	void ShowDonate(int money, int bc);
	void UpdateDonate(int money, int bc);
	void show_sc(int money, int bc);
        void ShowPerson();
	void HidePerson();

        void showHelper(int type);
        void hideHelper();

	CJavaWrapper(JNIEnv* env, jobject activity);
	~CJavaWrapper();
};

extern CJavaWrapper* g_pJavaWrapper;