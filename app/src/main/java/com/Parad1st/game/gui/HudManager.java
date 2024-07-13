package com.blackrussia.game.gui;

import android.app.Activity;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.nvidia.devtech.NvEventQueueActivity;
import com.blackrussia.game.R;
import com.blackrussia.game.gui.util.Utils;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.ArrayList;
import java.util.Formatter;

public class HudManager {
    public Activity activity;
    public ConstraintLayout hud_layout;

    public TextView hud_money;

    public ImageView hud_weapon;

    public ImageView hud_menu;

    public ArrayList<ImageView> hud_wanted;

    public ProgressBar progressHP;
    public ProgressBar progressArmor;

    public HudManager(Activity aactivity) {
        activity = aactivity;
        hud_layout = aactivity.findViewById(R.id.hud_main);
        hud_layout.setVisibility(View.GONE);

        progressArmor = aactivity.findViewById(R.id.hud_armor_pb);
        progressHP = aactivity.findViewById(R.id.hud_health_pb);

        hud_money = aactivity.findViewById(R.id.hud_money);
        hud_weapon = aactivity.findViewById(R.id.hud_weapon);
        hud_menu = aactivity.findViewById(R.id.hud_menu);

        hud_wanted = new ArrayList<>();
        hud_wanted.add(activity.findViewById(R.id.hud_star_1));
        hud_wanted.add(activity.findViewById(R.id.hud_star_2));
        hud_wanted.add(activity.findViewById(R.id.hud_star_3));
        hud_wanted.add(activity.findViewById(R.id.hud_star_4));
        hud_wanted.add(activity.findViewById(R.id.hud_star_5));
        hud_wanted.add(activity.findViewById(R.id.hud_star_6));
        hud_menu.setOnClickListener( view -> {
            NvEventQueueActivity.getInstance().showMenu();
            NvEventQueueActivity.getInstance().togglePlayer(1);
        });
    }

    public void UpdateHudInfo(int health, int armour, int hunger, int weaponid, int ammo, int playerid, int money, int wanted)
    {
        progressHP.setProgress(health);
        progressArmor.setProgress(armour);

        DecimalFormat formatter=new DecimalFormat();
        DecimalFormatSymbols symbols= DecimalFormatSymbols.getInstance();
        symbols.setGroupingSeparator('.');
        formatter.setDecimalFormatSymbols(symbols);
        String s=formatter.format(money).toString();
        hud_money.setText(String.valueOf(s));

        int id = activity.getResources().getIdentifier(new Formatter().format("weapon_%d", Integer.valueOf(weaponid)).toString(), "drawable", activity.getPackageName());
        hud_weapon.setImageResource(id);

        hud_weapon.setOnClickListener(v -> NvEventQueueActivity.getInstance().onWeaponChanged());
        if(wanted > 6) wanted = 6;
        for (int i2 = 0; i2 < wanted; i2++) {
            hud_wanted.get(i2).setBackgroundResource(R.drawable.ic_y_star);
        }

    }

    public void ShowHud()
    {
        Utils.ShowLayout(hud_layout, false);
    }

    public void HideHud()
    {
        Utils.HideLayout(hud_layout, false);
    }

}
