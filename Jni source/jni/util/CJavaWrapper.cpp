#include "CJavaWrapper.h"
#include "../main.h"

extern "C" JavaVM* javaVM;

#include "..//net/CUDPSocket.h"
#include "..//clientlogic/CNetwork.h"
extern int g_iServer;

#include "..//keyboard.h"
#include "..//chatwindow.h"
#include "..//settings.h"
#include "..//net/netgame.h"
#include "../game/game.h"
#include "../str_obfuscator_no_template.hpp"

extern CKeyBoard* pKeyBoard;
extern CChatWindow* pChatWindow;
extern CSettings* pSettings;
extern CNetGame* pNetGame;
extern CGame* pGame;

JNIEnv* CJavaWrapper::GetEnv()
{
	JNIEnv* env = nullptr;
	int getEnvStat = javaVM->GetEnv((void**)& env, JNI_VERSION_1_4);

	if (getEnvStat == JNI_EDETACHED)
	{
		Log("GetEnv: not attached");
		if (javaVM->AttachCurrentThread(&env, NULL) != 0)
		{
			Log("Failed to attach");
			return nullptr;
		}
	}
	if (getEnvStat == JNI_EVERSION)
	{
		Log("GetEnv: version not supported");
		return nullptr;
	}

	if (getEnvStat == JNI_ERR)
	{
		Log("GetEnv: JNI_ERR");
		return nullptr;
	}

	return env;
}

std::string CJavaWrapper::GetClipboardString()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return std::string("");
	}

	jbyteArray retn = (jbyteArray)env->CallObjectMethod(activity, s_GetClipboardText);

	if ((env)->ExceptionCheck())
	{
		(env)->ExceptionDescribe();
		(env)->ExceptionClear();
		return std::string("");
	}

	if (!retn)
	{
		return std::string("");
	}

	jboolean isCopy = true;

	jbyte* pText = env->GetByteArrayElements(retn, &isCopy);
	jsize length = env->GetArrayLength(retn);

	std::string str((char*)pText, length);

	env->ReleaseByteArrayElements(retn, pText, JNI_ABORT);
	
	return str;
}


void CJavaWrapper::SetRadar()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_RadarBR);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::CallLauncherActivity(int type)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_CallLauncherActivity, type);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::showHelper(int type)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(activity, s_showHelper, type);
}
void CJavaWrapper::hideHelper()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(activity, s_hideHelper);
}

void CJavaWrapper::AddChatMessage(const char msg[], uint32_t textcolor)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    char msg_utf[1024];
    cp1251_to_utf8(msg_utf, msg);
    //CChatWindow::FilterInvalidChars(msg_utf);
    //
    jstring jmsg = env->NewStringUTF(msg_utf);

    env->CallVoidMethod(activity, addChatMessage, jmsg, textcolor);
    env->DeleteLocalRef(jmsg);
}

void CJavaWrapper::ShowInputLayout()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_ShowInputLayout);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::ShowRadar()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_showradar);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::HideRadar()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_hideradar);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::HideInputLayout()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_HideInputLayout);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::ShowClientSettings()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_ShowClientSettings);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::MakeDialog(int dialogId, int dialogTypeId, char* caption, char* content, char* leftBtnText, char* rightBtnText)
{
    JNIEnv* env = GetEnv();
    if (!env)
    {
	Log("No env");
	return;
    }
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");
    jbyteArray bytes = env->NewByteArray(strlen(caption));
    env->SetByteArrayRegion(bytes, 0, strlen(caption), (jbyte*)caption);
    jstring str1 = (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
    //
    jclass strClass1 = env->FindClass("java/lang/String");
    jmethodID ctorID1 = env->GetMethodID(strClass1, "<init>", "([BLjava/lang/String;)V");
    jstring encoding1 = env->NewStringUTF("UTF-8");
    jbyteArray bytes1 = env->NewByteArray(strlen(content));
    env->SetByteArrayRegion(bytes1, 0, strlen(content), (jbyte*)content);
    jstring str2 = (jstring)env->NewObject(strClass1, ctorID1, bytes1, encoding1);
    //
    jclass strClass2 = env->FindClass("java/lang/String");
    jmethodID ctorID2 = env->GetMethodID(strClass2, "<init>", "([BLjava/lang/String;)V");
    jstring encoding2 = env->NewStringUTF("UTF-8");
    jbyteArray bytes2 = env->NewByteArray(strlen(leftBtnText));
    env->SetByteArrayRegion(bytes2, 0, strlen(leftBtnText), (jbyte*)leftBtnText);
    jstring str3 = (jstring)env->NewObject(strClass2, ctorID2, bytes2, encoding2);
    //
    jclass strClass3 = env->FindClass("java/lang/String");
    jmethodID ctorID3 = env->GetMethodID(strClass3, "<init>", "([BLjava/lang/String;)V");
    jstring encoding3 = env->NewStringUTF("UTF-8");
    jbyteArray bytes3 = env->NewByteArray(strlen(rightBtnText));
    env->SetByteArrayRegion(bytes3, 0, strlen(rightBtnText), (jbyte*)rightBtnText);
    jstring str4 = (jstring)env->NewObject(strClass3, ctorID3, bytes3, encoding3);

    env->CallVoidMethod(activity, s_MakeDialog, dialogId, dialogTypeId, str1, str2, str3, str4);

    EXCEPTION_CHECK(env);
}

void CJavaWrapper::SetUseFullScreen(int b)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(activity, s_SetUseFullScreen, b);

	EXCEPTION_CHECK(env);
}
extern int g_iStatusDriftChanged;
#include "..//debug.h"
extern "C"
{
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onInputEnd(JNIEnv* pEnv, jobject thiz, jbyteArray str)
	{
		if (pKeyBoard)
		{
			pKeyBoard->OnNewKeyboardInput(pEnv, thiz, str);
		}
	}

JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_buycar(JNIEnv* pEnv, jobject thiz, jint id, jint cost, jint action)
{
	switch(action) {
			case 1: 
				pNetGame->SendDonateCar(id, cost); break;
			}
}


JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendRPC(JNIEnv* pEnv, jobject thiz, jint type, jbyteArray str, jint action)
{
	switch(type) {
		case 1:
			switch(action) {
				case 398:
					pNetGame->SendChatCommand("/gps");
					break;
				case 1:
					pNetGame->SendChatCommand("/kalcor");
					break;
				case 2:
					pNetGame->SendChatCommand("/mn");
					break;
				case 4: 
					pNetGame->SendChatCommand("/inv");
					break;
				case 5: {
					pNetGame->SendChatCommand("/anim");
					pNetGame->SendChatCommand("/anim");	
					break;
				              }
				case 6:
					pNetGame->SendChatCommand("/donate");
					break;
				case 7:
					pNetGame->SendChatCommand("/car");
					break;
				case 8:
					pNetGame->SendChatCommand("/");		
					break;		
				}
			case 2:
				switch(action) {
					case 0:     
		                                                                       pNetGame = new CNetGame(
			                                                                g_sEncryptedAddresses[g_iServer].decrypt(),
			                                                                g_sEncryptedAddresses[g_iServer].getPort(),
			                                                                pSettings->GetReadOnly().szNickName,
			                                                                pSettings->GetReadOnly().szPassword);
		                                                                        pSettings->GetWrite().last_server = 0;

                                                                                                            g_pJavaWrapper->HideGPS();
                                                                                                            g_pJavaWrapper->ShowRadar();
                                                                                                            g_pJavaWrapper->HideZona();
                                                                                                            g_pJavaWrapper->HideX2();
						break;
				}
		}

		
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendDialogResponse(JNIEnv* pEnv, jobject thiz, jint i3, jint i, jint i2, jbyteArray str)
	{
		jboolean isCopy = true;

		jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
		jsize length = pEnv->GetArrayLength(str);

		std::string szStr((char*)pMsg, length);

		if(pNetGame) {
			pNetGame->SendDialogResponse(i, i3, i2, (char*)szStr.c_str());
			pGame->FindPlayerPed()->TogglePlayerControllable(true);
		}

		pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
	}
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendMessage(JNIEnv *env, jobject thiz, jbyteArray msg)
	{
	    jbyte* pMsg = env->GetByteArrayElements(msg, nullptr);
	    jsize length = env->GetArrayLength(msg);

	    std::string szStr((char*)pMsg, length);

	    pNetGame->SendChatMessage(szStr.c_str());

	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendCommand(JNIEnv* pEnv, jobject thiz, jbyteArray str)
	{
		jboolean isCopy = true;

		jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
		jsize length = pEnv->GetArrayLength(str);

		std::string szStr((char*)pMsg, length);

		if(pNetGame) {
			pNetGame->SendChatCommand((char*)szStr.c_str());
		}

		pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_togglePlayer(JNIEnv* pEnv, jobject thiz, jint toggle) {
		if(toggle)
			pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllable(false);
		else
			pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllable(true);
	}
	JNIEXPORT jint JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getLastServer(JNIEnv* pEnv, jobject thiz)
	{
		return (jint)pSettings->GetReadOnly().last_server;
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onEventBackPressed(JNIEnv* pEnv, jobject thiz)
	{
		if (pKeyBoard)
		{
			if (pKeyBoard->IsOpen())
			{
				Log("Closing keyboard");
				pKeyBoard->Close();
			}
		}
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onNativeHeightChanged(JNIEnv* pEnv, jobject thiz, jint orientation, jint height)
	{
		if (pChatWindow)
		{
			pChatWindow->SetLowerBound(height);
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeCutoutSettings(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iCutout = b;
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeKeyboardSettings(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iAndroidKeyboard = b;
		}

		if (pKeyBoard && b)
		{
			pKeyBoard->EnableNewKeyboard();
		}
		else if(pKeyBoard)
		{
			pKeyBoard->EnableOldKeyboard();
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeFpsCounterSettings(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iFPSCounter = b;
		}

		CDebugInfo::SetDrawFPS(b);
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeHud(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iHud = b;
			if(!b)
			{
				*(uint8_t*)(g_libGTASA+0x7165E8) = 1;
				g_pJavaWrapper->HideHud();
			}
			else
			{
				*(uint8_t*)(g_libGTASA+0x7165E8) = 0;
				g_pJavaWrapper->ShowHud();
			}
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeDialog(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iDialog = b;
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeHpArmourText(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			if (!pSettings->GetWrite().iHPArmourText && b)
			{
				if (CAdjustableHudColors::IsUsingHudColor(HUD_HP_TEXT) == false)
				{
					CAdjustableHudColors::SetHudColorFromRGBA(HUD_HP_TEXT, 255, 0, 0, 255);
				}
				if (CAdjustableHudPosition::GetElementPosition(HUD_HP_TEXT).X == -1 || CAdjustableHudPosition::GetElementPosition(HUD_HP_TEXT).Y == -1)
				{
					CAdjustableHudPosition::SetElementPosition(HUD_HP_TEXT, 500, 500);
				}
				if (CAdjustableHudScale::GetElementScale(HUD_HP_TEXT).X == -1 || CAdjustableHudScale::GetElementScale(HUD_HP_TEXT).Y == -1)
				{
					CAdjustableHudScale::SetElementScale(HUD_HP_TEXT, 400, 400);
				}

				if (CAdjustableHudColors::IsUsingHudColor(HUD_ARMOR_TEXT) == false)
				{
					CAdjustableHudColors::SetHudColorFromRGBA(HUD_ARMOR_TEXT, 255, 0, 0, 255);
				}
				if (CAdjustableHudPosition::GetElementPosition(HUD_ARMOR_TEXT).X == -1 || CAdjustableHudPosition::GetElementPosition(HUD_ARMOR_TEXT).Y == -1)
				{
					CAdjustableHudPosition::SetElementPosition(HUD_ARMOR_TEXT, 300, 500);
				}
				if (CAdjustableHudScale::GetElementScale(HUD_ARMOR_TEXT).X == -1 || CAdjustableHudScale::GetElementScale(HUD_ARMOR_TEXT).Y == -1)
				{
					CAdjustableHudScale::SetElementScale(HUD_ARMOR_TEXT, 400, 400);
				}
			}

			pSettings->GetWrite().iHPArmourText = b;
		}

		CInfoBarText::SetEnabled(b);
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeOutfitGunsSettings(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iOutfitGuns = b;

			CWeaponsOutFit::SetEnabled(b);
		}
	}
	
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_showMenuu(JNIEnv* pEnv, jobject thiz)
	{
		if (pGame->FindPlayerPed()->IsInVehicle() && !pGame->FindPlayerPed()->IsAPassenger())
            pNetGame->SendChatCommand("/oxwjdovke");
        else
            g_pJavaWrapper->ShowMenu();
	}
	
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendRadialClick(JNIEnv* pEnv, jobject thiz, jint id)
	{
		if (!pGame->FindPlayerPed()->IsInVehicle() && !pGame->FindPlayerPed()->IsAPassenger())
            return;
        switch (id) {
        	case 0: pNetGame->SendChatCommand("/key"); break;
            case 1: pNetGame->SendChatCommand("/park"); break;
            case 2: pNetGame->SendChatCommand("/turbo"); break;
            case 3: pNetGame->SendChatCommand("/en"); break;
            case 4: pNetGame->SendChatCommand("/lock"); break;
            case 5: pNetGame->SendChatCommand("/lights"); break;
            case 6: pNetGame->SendChatCommand("/suspension"); break;
            case 7: pNetGame->SendChatCommand("/launch_control"); break;
        }
	}
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendHuddeClick(JNIEnv* pEnv, jobject thiz, jint id)
	{
        switch (id) {
        	case 0: pNetGame->SendChatCommand("/donate"); break;
            case 1: pNetGame->SendChatCommand("/hud"); break;
            case 2: pNetGame->SendChatCommand("/quest"); break;
        }
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendDonateClick(JNIEnv* pEnv, jobject thiz, jint id)
	{
        switch (id) {
        	case 0: pNetGame->SendChatCommand("/openbc"); break;
            	case 1: pNetGame->SendChatCommand("/openrub"); break;
		case 2: pNetGame->SendChatCommand("/keys"); break;
		case 3: pNetGame->SendChatCommand("/bp"); break;
		case 4: pNetGame->SendChatCommand("/nabori"); break;
		case 5: pNetGame->SendChatCommand("/akss"); break;
		case 6: pNetGame->SendChatCommand("/changename"); break;
		case 7: pNetGame->SendChatCommand("/unnwarn"); break;
		case 8: pNetGame->SendChatCommand("/alllic"); break;
		case 9: pNetGame->SendChatCommand("/powerbuy"); break;
		case 10: pNetGame->SendChatCommand("/bomboxbuy"); break;
		case 11: pNetGame->SendChatCommand("/buyslot"); break;
		case 12: pNetGame->SendChatCommand("/buynomber"); break;
		case 13: pNetGame->SendChatCommand("/buyphonenum"); break;
		case 14: pNetGame->SendChatCommand("/vipsilv"); break;
		case 15: pNetGame->SendChatCommand("/vipgld"); break;
		case 16: pNetGame->SendChatCommand("/vippltnum"); break;
	}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativePcMoney(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iPCMoney = b;
		}

		CGame::SetEnabledPCMoney(b);
	}

        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendAutoShopButton(JNIEnv *env, jobject thiz, jint id) 
        {
	        pNetGame->sendAutoShopClick(id);
        }
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onShopStoreClick(JNIEnv* pEnv, jobject thiz, jint buttonid)
	{
		pNetGame->SendShopStoreClick(buttonid);
	}
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_clickButton(JNIEnv* pEnv, jobject thiz, jint id, jint splayerid)
	{
		pNetGame->SendReconClick(id, splayerid); 
	}
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onFuelStationClick(JNIEnv *env, jobject thiz, jint fueltype, jint fuelliters) 
        {
	        pNetGame->SendCustomPacketFuelData(fuelliters, fueltype);
        }
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendSpawnClick(JNIEnv* pEnv, jobject thiz, jint id)
	{
        switch (id) {
        	case 1: pNetGame->SendChatCommand("/spawnorg"); break;
            case 2: pNetGame->SendChatCommand("/spawnvokzal"); break;
            case 3: pNetGame->SendChatCommand("/spawnlast"); break;
            case 4: pNetGame->SendChatCommand("/spawngarage"); break;
            case 5: pNetGame->SendChatCommand("/spawnhouse"); break;
        }
	}
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendHospital(JNIEnv* pEnv, jobject thiz, jint id) 
	{
        switch(id)
		{
			case 0:
			{
				pNetGame->SendChatCommand("/hospital");
				break;
			}
		}
	}
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendInvRequest(JNIEnv* pEnv, jobject thiz) 
	{
                pNetGame->SendChatCommand("/slot");
	}
        /*JNIEXPORT void JNICALL Java_com_primer_game_gui_HudManager_ShowKK(JNIEnv *env, jobject thiz)
        {
                pChatWindow->Opencc();
        }
        JNIEXPORT void JNICALL Java_com_primer_game_gui_HudManager_HideKK(JNIEnv *env, jobject thiz)
        {
                pKeyBoard->Close();
        }*/
        JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_personclick(JNIEnv *env, jobject thiz, jint id) 
        {
	        pNetGame->SendPerson(id);
        }
        
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeRadarrect(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iRadarRect = b;

			CRadarRect::SetEnabled(b);
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeSkyBox(JNIEnv* pEnv, jobject thiz, jboolean b)
	{
		if (pSettings)
		{
			pSettings->GetWrite().iSkyBox = b;
			g_iStatusDriftChanged = 1;
		}
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeCutoutSettings(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iCutout;
		}
		return 0;
	}
	
	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeHud(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iHud;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeDialog(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iDialog;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeKeyboardSettings(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iAndroidKeyboard;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeFpsCounterSettings(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iFPSCounter;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeHpArmourText(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iHPArmourText;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeOutfitGunsSettings(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iOutfitGuns;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativePcMoney(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iPCMoney;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeRadarrect(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iRadarRect;
		}
		return 0;
	}

	JNIEXPORT jboolean JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeSkyBox(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			return pSettings->GetReadOnly().iSkyBox;
		}
		return 0;
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onSettingsWindowSave(JNIEnv* pEnv, jobject thiz)
	{
		if (pSettings)
		{
			pSettings->Save();
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onSettingsWindowDefaults(JNIEnv* pEnv, jobject thiz, jint category)
	{
		if (pSettings)
		{
			pSettings->ToDefaults(category);
			if (pChatWindow)
			{
				pChatWindow->m_bPendingReInit = true;
			}
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeHudElementColor(JNIEnv* pEnv, jobject thiz, jint id, jint a, jint r, jint g, jint b)
	{
		CAdjustableHudColors::SetHudColorFromRGBA((E_HUD_ELEMENT)id, r, g, b, a);
	}

	JNIEXPORT jbyteArray JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeHudElementColor(JNIEnv* pEnv, jobject thiz, jint id)
	{
		char pTemp[9];
		jbyteArray color = pEnv->NewByteArray(sizeof(pTemp));

		if (!color)
		{
			return nullptr;
		}

		pEnv->SetByteArrayRegion(color, 0, sizeof(pTemp), (const jbyte*)CAdjustableHudColors::GetHudColorString((E_HUD_ELEMENT)id).c_str());

		return color;
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeHudElementPosition(JNIEnv* pEnv, jobject thiz, jint id, jint x, jint y)
	{
		if (id == 7)
		{
			if (pSettings)
			{
				pSettings->GetWrite().fChatPosX = x;
				pSettings->GetWrite().fChatPosY = y;
				if (pChatWindow)
				{
					pChatWindow->m_bPendingReInit = true;
				}
				return;
			}
			return;
		}
		if (id == HUD_SNOW)
		{
			if (pSettings)
			{
				pSettings->GetWrite().iSnow = x;
			}
			CSnow::SetCurrentSnow(pSettings->GetReadOnly().iSnow);
			return;
		}
		CAdjustableHudPosition::SetElementPosition((E_HUD_ELEMENT)id, x, y);

		if (id >= HUD_WEAPONSPOS && id <= HUD_WEAPONSROT)
		{
			CWeaponsOutFit::OnUpdateOffsets();
		}
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeHudElementScale(JNIEnv* pEnv, jobject thiz, jint id, jint x, jint y)
	{
		CAdjustableHudScale::SetElementScale((E_HUD_ELEMENT)id, x, y);

		if (id >= HUD_WEAPONSPOS && id <= HUD_WEAPONSROT)
		{
			CWeaponsOutFit::OnUpdateOffsets();
		}
	}

	JNIEXPORT jintArray JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeHudElementScale(JNIEnv* pEnv, jobject thiz, jint id)
	{
		jintArray color = pEnv->NewIntArray(2);

		if (!color)
		{
			return nullptr;
		}
		int arr[2];
		arr[0] = CAdjustableHudScale::GetElementScale((E_HUD_ELEMENT)id).X;
		arr[1] = CAdjustableHudScale::GetElementScale((E_HUD_ELEMENT)id).Y;
		pEnv->SetIntArrayRegion(color, 0, 2, (const jint*)& arr[0]);

		return color;
	}

	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_setNativeWidgetPositionAndScale(JNIEnv* pEnv, jobject thiz, jint id, jint x, jint y, jint scale)
	{
		if (id == 0)
		{
			if (pSettings)
			{
				pSettings->GetWrite().fButtonMicrophoneX = x;
				pSettings->GetWrite().fButtonMicrophoneY = y;
				pSettings->GetWrite().fButtonMicrophoneSize = scale;
			}

			if (g_pWidgetManager)
			{
				if (g_pWidgetManager->GetSlotState(WIDGET_MICROPHONE))
				{
					g_pWidgetManager->GetWidget(WIDGET_MICROPHONE)->SetPos(x, y);
					g_pWidgetManager->GetWidget(WIDGET_MICROPHONE)->SetHeight(scale);
					g_pWidgetManager->GetWidget(WIDGET_MICROPHONE)->SetWidth(scale);
				}
			}
		}
		

		if (id == 2)
		{
			if (pSettings)
			{
				pSettings->GetWrite().fButtonCameraCycleX = x;
				pSettings->GetWrite().fButtonCameraCycleY = y;
				pSettings->GetWrite().fButtonCameraCycleSize = scale;
			}

			if (g_pWidgetManager)
			{
				if (g_pWidgetManager->GetSlotState(WIDGET_CAMERA_CYCLE))
				{
					g_pWidgetManager->GetWidget(WIDGET_CAMERA_CYCLE)->SetPos(x, y);
					g_pWidgetManager->GetWidget(WIDGET_CAMERA_CYCLE)->SetHeight(scale);
					g_pWidgetManager->GetWidget(WIDGET_CAMERA_CYCLE)->SetWidth(scale);
				}
			}
		}
	}

	JNIEXPORT jintArray JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeHudElementPosition(JNIEnv* pEnv, jobject thiz, jint id)
	{
		jintArray color = pEnv->NewIntArray(2);

		if (!color)
		{
			return nullptr;
		}
		int arr[2];

		if (id == 7 && pSettings)
		{
			arr[0] = pSettings->GetReadOnly().fChatPosX;
			arr[1] = pSettings->GetReadOnly().fChatPosY;
		}
		else if (id == HUD_SNOW && pSettings)
		{
			arr[0] = CSnow::GetCurrentSnow();
			arr[1] = CSnow::GetCurrentSnow();
		}
		else
		{
			arr[0] = CAdjustableHudPosition::GetElementPosition((E_HUD_ELEMENT)id).X;
			arr[1] = CAdjustableHudPosition::GetElementPosition((E_HUD_ELEMENT)id).Y;
		}

		pEnv->SetIntArrayRegion(color, 0, 2, (const jint*)&arr[0]);

		return color;
	}

	JNIEXPORT jintArray JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_getNativeWidgetPositionAndScale(JNIEnv* pEnv, jobject thiz, jint id)
	{
		jintArray color = pEnv->NewIntArray(3);

		if (!color)
		{
			return nullptr;
		}
		int arr[3] = { -1, -1, -1 };
		

		if (pSettings)
		{
			if (id == 0)
			{
				arr[0] = pSettings->GetWrite().fButtonMicrophoneX;
				arr[1] = pSettings->GetWrite().fButtonMicrophoneY;
				arr[2] = pSettings->GetWrite().fButtonMicrophoneSize;
			}
			if (id == 1)
			{
				arr[0] = pSettings->GetWrite().fButtonEnterPassengerX;
				arr[1] = pSettings->GetWrite().fButtonEnterPassengerY;
				arr[2] = pSettings->GetWrite().fButtonEnterPassengerSize;
			}
			if (id == 2)
			{
				arr[0] = pSettings->GetWrite().fButtonCameraCycleX;
				arr[1] = pSettings->GetWrite().fButtonCameraCycleY;
				arr[2] = pSettings->GetWrite().fButtonCameraCycleSize;
			}
		}
		

		pEnv->SetIntArrayRegion(color, 0, 3, (const jint*)& arr[0]);

		return color;
	}
}

void CJavaWrapper::ShowHud()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    //g_pJavaWrapper->ShowNotification(4, "Худ успешно подключен", 5, "", ">>"); 
    env->CallVoidMethod(this->activity, this->s_showHud);
}

void CJavaWrapper::HideHud()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    //g_pJavaWrapper->ShowNotification(4, "Худ успешно скрыт", 5, "", ">>"); 
    env->CallVoidMethod(this->activity, this->s_hideHud);
}

void CJavaWrapper::ShowGPS()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
   // g_pJavaWrapper->ShowNotification(4, "Метка установлена", 5, "", ">>"); 
    env->CallVoidMethod(this->activity, this->s_showGps);
}

void CJavaWrapper::HideGPS()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_hideGps);
}

void CJavaWrapper::ShowZona()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_showZona);
}

void CJavaWrapper::HideZona()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_hideZona);
}

void CJavaWrapper::ShowX2()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    //g_pJavaWrapper->ShowNotification(4, "x2", 5, "", ">>"); 
    env->CallVoidMethod(this->activity, this->s_showx2);
}

void CJavaWrapper::HideX2()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_hidex2);
}

void CJavaWrapper::UpdateHudInfo(int health, int armour, int hunger, int weaponidweik, int ammo, int ammoinclip, int money, int wanted)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_updateHudInfo, health, armour, hunger, weaponidweik, ammo, ammoinclip, money, wanted);
}

void CJavaWrapper::SetPauseState(bool a1)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_setPauseState, a1);
}

void CJavaWrapper::ShowSplash() {

	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showSplash);
}

void CJavaWrapper::UpdateSplash(int percent) {
	
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_updateSplash, percent);
}

void CJavaWrapper::ShowMenu() 
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
                  //g_pJavaWrapper->ShowNotification(4, "Test game version", 5, "", ">>"); 
	env->CallVoidMethod(this->activity, this->s_showMenu);
}

void CJavaWrapper::ShowSpeed()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_showSpeed);
}

void CJavaWrapper::HideSpeed()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_hideSpeed);
}

void CJavaWrapper::ShowRadial(bool park, bool key, bool doors, bool lights, bool suspension, bool launch_control, bool engine, bool turbo)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showRadial, park, key, doors, lights, suspension, launch_control, engine, turbo);
}

void CJavaWrapper::ShowShop(int price)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showShop, price);
}
void CJavaWrapper::HideShop()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_hideShop);
}

void CJavaWrapper::ShowPreDeath()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showPreDeath);
}

/*void CJavaWrapper::closekey()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->closekeY);
}*/

void CJavaWrapper::ShowTabWindow()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
	env->CallVoidMethod(this->activity, this->s_showTabWindow);
}

void CJavaWrapper::ShowInventory(int hp, int hunger, int skin_id, int key, int sumka)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showInventory, hp, hunger, skin_id, key, sumka);
}

void CJavaWrapper::SetTabStat(int id, char* name, int score, int ping) {

	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");

    jbyteArray bytes = env->NewByteArray(strlen(name));
    env->SetByteArrayRegion(bytes, 0, strlen(name), (jbyte*)name);
    jstring jname = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	env->CallVoidMethod(this->activity, this->s_setTabStat, id, jname, score, ping);
}

void CJavaWrapper::ShowSpawn()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showSpawn);
}

void CJavaWrapper::UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_updateSpeedInfo, speed, fuel, hp, mileage, engine, light, belt, lock);
}

void CJavaWrapper::ShowFuelStation(int type, int price1, int price2, int price3, int price4, int price5, int maxCount)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
	
	env->CallVoidMethod(this->activity, this->s_showFuelStation, type, price1, price2, price3, price4, price5, maxCount);
}

void CJavaWrapper::ShowRecon(char* nick, int id)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");

    jbyteArray bytes = env->NewByteArray(strlen(nick));
    env->SetByteArrayRegion(bytes, 0, strlen(nick), (jbyte*)nick);
    jstring jnick = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	env->CallVoidMethod(this->activity, this->s_showRecon, jnick, id);
}

void CJavaWrapper::HideRecon()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
	
	env->CallVoidMethod(this->activity, this->s_hideRecon);
}

void CJavaWrapper::ShowAuto()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showAuto);
}

void CJavaWrapper::HideAuto()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_hideAuto);
}
void CJavaWrapper::UpdateAuto(char* name, int price, int count, float maxspeed, float acceleration, int gear)
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");

    jbyteArray bytes = env->NewByteArray(strlen(name));
    env->SetByteArrayRegion(bytes, 0, strlen(name), (jbyte*)name);
    jstring jname = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	env->CallVoidMethod(this->activity, this->s_updateAuto, jname, price, count, maxspeed, acceleration, gear);

}
void CJavaWrapper::ShowJbl()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_showJbl);
}
void CJavaWrapper::ShowNotification(int type, char* text, int duration, char* actionforBtn, char* textBtn) 
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
                  jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
                  jstring encoding = env->NewStringUTF("UTF-8");

                  jbyteArray bytes = env->NewByteArray(strlen(text));
                  env->SetByteArrayRegion(bytes, 0, strlen(text), (jbyte*)text);
                  jstring jtext = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	bytes = env->NewByteArray(strlen(actionforBtn));
                  env->SetByteArrayRegion(bytes, 0, strlen(actionforBtn), (jbyte*)actionforBtn);
                  jstring jactionforBtn = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	bytes = env->NewByteArray(strlen(textBtn));
                  env->SetByteArrayRegion(bytes, 0, strlen(textBtn), (jbyte*)textBtn);
                  jstring jtextBtn = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	env->CallVoidMethod(this->activity, this->s_showNotification, type, jtext, duration, jactionforBtn, jtextBtn);
}

void CJavaWrapper::ShowHudFeatures()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_showHudFeatures);
}

void CJavaWrapper::HideHudFeatures()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_hideHudFeatures);
}

void CJavaWrapper::ShowHudAndLogo()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_showHudAndLogo);
}

void CJavaWrapper::HideHudAndLogo()
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_hideHudAndLogo);
}

void CJavaWrapper::ShowDonate(int money, int bc)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_showDonate, money, bc);
}
void CJavaWrapper::UpdateDonate(int money, int bc)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_updateDonate, money, bc);
}
void CJavaWrapper::show_sc(int money, int bc)
{
    JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
    env->CallVoidMethod(this->activity, this->s_show_sc, money, bc);
}
void CJavaWrapper::ShowPerson()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
	env->CallVoidMethod(this->activity, this->s_showPerson);
}

void CJavaWrapper::HidePerson()
{
	JNIEnv* env = GetEnv();

	if (!env)
	{
		Log("No env");
		return;
	}
	env->CallVoidMethod(this->activity, this->s_hidePerson);
}

CJavaWrapper::CJavaWrapper(JNIEnv* env, jobject activity)
{
	this->activity = env->NewGlobalRef(activity);

	jclass nvEventClass = env->GetObjectClass(activity);
	if (!nvEventClass)
	{
		Log("nvEventClass null");
		return;
	}

	//s_CallLauncherActivity = env->GetMethodID(nvEventClass, "callLauncherActivity", "(I)V");
	s_GetClipboardText = env->GetMethodID(nvEventClass, "getClipboardText", "()[B");

	s_ShowInputLayout = env->GetMethodID(nvEventClass, "showInputLayout", "()V");
	s_HideInputLayout = env->GetMethodID(nvEventClass, "hideInputLayout", "()V");

	s_ShowClientSettings = env->GetMethodID(nvEventClass, "showClientSettings", "()V");
	s_SetUseFullScreen = env->GetMethodID(nvEventClass, "setUseFullscreen", "(I)V");
	s_MakeDialog = env->GetMethodID(nvEventClass, "showDialog", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	s_updateHudInfo = env->GetMethodID(nvEventClass, "updateHudInfo", "(IIIIIIII)V");
                  s_showHud = env->GetMethodID(nvEventClass, "showHud", "()V");
                  s_hideHud = env->GetMethodID(nvEventClass, "hideHud", "()V");

                  s_showGps = env->GetMethodID(nvEventClass, "showGps", "()V");
                  s_hideGps = env->GetMethodID(nvEventClass, "hideGps", "()V");

                  s_showradar = env->GetMethodID(nvEventClass, "showradar", "()V");
                  s_hideradar = env->GetMethodID(nvEventClass, "hideradar", "()V");

                  s_showZona = env->GetMethodID(nvEventClass, "showZona", "()V");
                  s_hideZona = env->GetMethodID(nvEventClass, "hideZona", "()V");

                  s_showx2 = env->GetMethodID(nvEventClass, "showx2", "()V");
                  s_hidex2 = env->GetMethodID(nvEventClass, "hidex2", "()V");

	s_updateSpeedInfo = env->GetMethodID(nvEventClass, "updateSpeedInfo", "(IIIIIIII)V");
                  s_showSpeed = env->GetMethodID(nvEventClass, "showSpeed", "()V");
                  s_hideSpeed = env->GetMethodID(nvEventClass, "hideSpeed", "()V");
	
	s_showNotification = env->GetMethodID(nvEventClass, "showNotification","(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)V");
	s_showMenu = env->GetMethodID(nvEventClass, "showMenu", "()V");

	s_RadarBR = env->GetMethodID(nvEventClass, "RadarBR", "()V");

	s_setPauseState = env->GetMethodID(nvEventClass, "setPauseState", "(Z)V");

	s_updateSplash = env->GetMethodID(nvEventClass, "updateSplash", "(I)V");
	s_showSplash = env->GetMethodID(nvEventClass, "showSplash", "()V");
	
	s_showHudFeatures = env->GetMethodID(nvEventClass, "showHudFeatures", "()V");
	s_hideHudFeatures = env->GetMethodID(nvEventClass, "hideHudFeatures", "()V");
	
	s_showHudAndLogo = env->GetMethodID(nvEventClass, "showHudAndLogo", "()V");
	s_hideHudAndLogo = env->GetMethodID(nvEventClass, "hideHudAndLogo", "()V");
	s_showDonate = env->GetMethodID(nvEventClass, "showDonate", "(II)V");
	s_updateDonate = env->GetMethodID(nvEventClass, "updateDonate", "(II)V");
	s_show_sc = env->GetMethodID(nvEventClass, "show_sc", "(II)V");
	
	s_showRadial = env->GetMethodID(nvEventClass, "showRadial", "(ZZZZZZZZ)V");

        s_showShop = env->GetMethodID(nvEventClass, "ShowShop", "(I)V");	
	s_hideShop = env->GetMethodID(nvEventClass, "HideShop", "()V");
        s_showAuto = env->GetMethodID(nvEventClass, "ShowAuto", "()V");
	s_updateAuto = env->GetMethodID(nvEventClass, "UpdateAuto", "(Ljava/lang/String;IIFFI)V");
	s_hideAuto = env->GetMethodID(nvEventClass, "HideAuto", "()V");
        s_showRecon = env->GetMethodID(nvEventClass, "ShowRecon", "(Ljava/lang/String;I)V");
        s_showFuelStation = env->GetMethodID(nvEventClass, "showFuelStation", "(IIIIIII)V");
        s_showTabWindow = env->GetMethodID(nvEventClass, "showTabWindow", "()V");	
	s_setTabStat = env->GetMethodID(nvEventClass, "setTabStat", "(ILjava/lang/String;II)V");
        s_showSpawn = env->GetMethodID(nvEventClass, "ShowSpawn", "()V");
        s_showInventory = env->GetMethodID(nvEventClass, "showInventory", "(IIIII)V");
	s_showJbl = env->GetMethodID(nvEventClass, "ShowJbl", "()V");
        s_showPreDeath = env->GetMethodID(nvEventClass, "showPreDeath", "()V");
        addChatMessage = env->GetMethodID(nvEventClass, "addChatMessage", "(Ljava/lang/String;I)V");
        //closekeY = env->GetMethodID(nvEventClass, "closekey", "()V");
        s_showPerson = env->GetMethodID(nvEventClass, "ShowPerson", "()V");
	s_hidePerson = env->GetMethodID(nvEventClass, "HidePerson", "()V");

s_showHelper = env->GetMethodID(nvEventClass, "ShowHelper", "(I)V");
        s_hideHelper = env->GetMethodID(nvEventClass, "HideHelper", "()V");

	env->DeleteLocalRef(nvEventClass);
}

CJavaWrapper::~CJavaWrapper()
{
	JNIEnv* pEnv = GetEnv();
	if (pEnv)
	{
		pEnv->DeleteGlobalRef(this->activity);
	}
}

CJavaWrapper* g_pJavaWrapper = nullptr;