package com.byparad1st.launcher.Registration;

public class reg {
    public static int CurrentDialogId = 0;
    public static int ListitemToSend = 0;
    public static int playerID = 0;
    public static String nickname = "";
    public static String password_login = "";
    public static Boolean autologin = false;
    public static void setAll(int iCurrentDialogId, int iListitemToSend){
        setCurrentDialogId(iCurrentDialogId);
        setListitemToSend(iListitemToSend);
    }
    public static int GetCurrentDialogId() { return CurrentDialogId; }
    public static int setCurrentDialogId(int nCurrentDialogId) {
        return CurrentDialogId = nCurrentDialogId;
    }
    public static int GetListitemToSend() { return ListitemToSend; }
    public static int setListitemToSend(int nListitemToSend) {
        return ListitemToSend = nListitemToSend;
    }
    public static String getNick(){ return nickname; }
    public static String setNick(String string){
        return nickname = string;
    }

    public static String getPassword(){ return password_login; }
    public static String setPassword(String string){
        return password_login = string;
    }

    public static int GetID() { return playerID; }
    public static int setUID(int id) {
        return playerID = id;
    }

    public static boolean GetAutoLoginStatus() { return autologin; }
    public static boolean setAL(boolean tf) {
        return autologin = tf;
    }
}
