package com.byparad1st.game.gui;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.constraintlayout.widget.ConstraintLayout;
import com.byparad1st.game.R;
import com.nvidia.devtech.NvEventQueueActivity;

public class AdminRecon {
    private static final int EXIT_BUTTON = 0;
    private static final int STATS_BUTTON = 1;
    private static final int SLAP_BUTTON = 2;
    private static final int REFRESH_BUTTON = 3;
    private static final int MUTE_BUTTON = 4;
    private static final int JAIL_BUTTON = 5;
    private static final int KICK_BUTTON = 6;
    private static final int BAN_BUTTON = 7;
    private static final int WARN_BUTTON = 8;
    private static final int NEXT_BUTTON = 9;
    private static final int PREV_BUTTON = 10;
    private static final int AF_BUTTON = 11;

    Activity activity;
    TextView re_nickname_text;
    TextView re_id_text;
    ConstraintLayout re_main_layout;
    ConstraintLayout exit_butt;
    ImageView stats_button;
    ImageView slap_butt;
    ImageView refresh_button;
    ImageView mute_button;
    ImageView jail_butt;
    ImageView kick_butt;
    ImageView ban_butt;
    ImageView warn_butt;
    ImageView af_butt;
    ConstraintLayout re_prev_button;
    ConstraintLayout re_next_button;

    int playerid;

    public AdminRecon(Activity activity){

        this.activity = activity;
        re_main_layout = activity.findViewById(R.id.main_recon_br);
        re_main_layout.setVisibility(View.GONE);

        re_nickname_text = activity.findViewById(R.id.nickname);
        re_id_text = activity.findViewById(R.id.id);

        exit_butt = activity.findViewById(R.id.exit_button);
        exit_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(EXIT_BUTTON, playerid);
            hide();
        });

        stats_button = activity.findViewById(R.id.statssss_button);
        stats_button.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(STATS_BUTTON, playerid);
        });

        slap_butt = activity.findViewById(R.id.slap_button);
        slap_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(SLAP_BUTTON, playerid);
        });

        refresh_button = activity.findViewById(R.id.obnv_button);
        refresh_button.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(REFRESH_BUTTON, playerid);
        });

        mute_button = activity.findViewById(R.id.mute_button);
        mute_button.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(MUTE_BUTTON, playerid);
        });

        jail_butt = activity.findViewById(R.id.jail_button);
        jail_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(JAIL_BUTTON, playerid);
        });

        kick_butt = activity.findViewById(R.id.kick_button);
        kick_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(KICK_BUTTON, playerid);
        });

        ban_butt = activity.findViewById(R.id.ban_button);
        ban_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(BAN_BUTTON, playerid);
        });

        warn_butt = activity.findViewById(R.id.warn_button);
        warn_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(WARN_BUTTON, playerid);
        });
        af_butt = activity.findViewById(R.id.form_button);
        af_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(AF_BUTTON, playerid);
        });

        re_prev_button = activity.findViewById(R.id.nextt_button);
        re_prev_button.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(PREV_BUTTON, playerid);
        });

        re_next_button = activity.findViewById(R.id.prevv_button);
        re_next_button.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().clickButton(NEXT_BUTTON, playerid);
        });
    }

    @SuppressLint("DefaultLocale")
    public void show(String nick, int id){
        activity.runOnUiThread(() -> {
            re_nickname_text.setText(nick);
            re_id_text.setText(String.format("[" + "%d" + "] ", id));
            playerid = id;
            re_main_layout.setVisibility(View.VISIBLE);
        });
    }

    void tempToggle(boolean toggle){
        activity.runOnUiThread(() -> {
            if(toggle) {
                re_main_layout.setVisibility(View.VISIBLE);
            }
            else {
                re_main_layout.setVisibility(View.GONE);
            }
        });
    }

    public void hide(){
        activity.runOnUiThread(() -> {
            re_main_layout.setVisibility(View.GONE);
        });
    }
}
