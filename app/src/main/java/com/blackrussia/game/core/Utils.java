package com.blackrussia.game.core;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

import java.io.File;

import static com.blackrussia.game.core.Config.GAME_PATH;

public class Utils extends AppCompatActivity {
    
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
    }

    public static boolean getGameFile(String path)
    {
        File file = new File(GAME_PATH+path);
        return file.exists();
    }

    static int installgametype = 0;

    public static int getType(){ return installgametype;}
    public static int setType(int value){return installgametype = value;}
}
