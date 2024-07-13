package com.byparad1st.game.gui.auth;

import android.app.Activity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

public class SpawnSelectorBr extends AppCompatActivity {
    public Activity activity;

    public ConstraintLayout selectbr;
    public ImageView org, vokzal, last, garage, house, orga, vokzala, lasta, tgarage, shouses, svokzala, sorga, slasta;
    public Button voiti;
    public static int type = 0;

    public SpawnSelectorBr(Activity aactivity) {
        activity = aactivity;
        selectbr = aactivity.findViewById(R.id.selectbrspawn);
        sorga = aactivity.findViewById(R.id.sorga);
        slasta = aactivity.findViewById(R.id.slasta);
        tgarage = aactivity.findViewById(R.id.sgarages);
        svokzala = aactivity.findViewById(R.id.svokzala);
        org = aactivity.findViewById(R.id.sorg);
        vokzal = aactivity.findViewById(R.id.svokzal);
        last = aactivity.findViewById(R.id.lastdis);
        garage = aactivity.findViewById(R.id.sgarage);
        house = aactivity.findViewById(R.id.shouse);
        voiti = aactivity.findViewById(R.id.spawnbtnbr);
        orga = aactivity.findViewById(R.id.sorga);
        vokzala = aactivity.findViewById(R.id.svokzala);
        lasta = aactivity.findViewById(R.id.slasta);
        shouses = aactivity.findViewById(R.id.shouses);
        Utils.HideLayout(selectbr, false);
        Utils.HideLayout(slasta, false);
        Utils.HideLayout(sorga, false);
        Utils.HideLayout(tgarage, false);
        Utils.HideLayout(svokzala, false);
        Utils.HideLayout(shouses, false);
        org.setOnClickListener( view -> {
            type = 1;
            Utils.HideLayout(slasta, true);
            Utils.ShowLayout(sorga, true);
            Utils.HideLayout(tgarage, true);
            Utils.HideLayout(svokzala, true);
            Utils.HideLayout(shouses, true);
            vokzal.setVisibility(View.VISIBLE);
            last.setVisibility(View.VISIBLE);
            garage.setVisibility(View.VISIBLE);
        });
        vokzal.setOnClickListener( view -> {
            type = 2;
            Utils.HideLayout(slasta, true);
            Utils.HideLayout(sorga, true);
            Utils.HideLayout(tgarage, true);
            Utils.ShowLayout(svokzala, true);
            Utils.HideLayout(shouses, true);
            org.setVisibility(View.VISIBLE);
            last.setVisibility(View.VISIBLE);
            garage.setVisibility(View.VISIBLE);
        });
        /*last.setOnClickListener( view -> {
            type = 3;
            Utils.ShowLayout(slasta, true);
            Utils.HideLayout(sorga, true);
            Utils.HideLayout(tgarage, true);
            Utils.HideLayout(svokzala, true);
            Utils.HideLayout(shouses, true);
            vokzal.setVisibility(View.VISIBLE);
            org.setVisibility(View.VISIBLE);
            garage.setVisibility(View.VISIBLE);
        });
        garage.setOnClickListener( view -> {
            type = 4;
            Utils.HideLayout(slasta, true);
            Utils.HideLayout(sorga, true);
            Utils.ShowLayout(tgarage, true);
            Utils.HideLayout(svokzala, true);
            Utils.HideLayout(shouses, true);
            vokzal.setVisibility(View.VISIBLE);
            last.setVisibility(View.VISIBLE);
            org.setVisibility(View.VISIBLE);
        });*/
        house.setOnClickListener( view -> {
            type = 5;
            Utils.HideLayout(slasta, true);
            Utils.HideLayout(sorga, true);
            Utils.HideLayout(tgarage, true);
            Utils.HideLayout(svokzala, true);
            Utils.ShowLayout(shouses, true);
            vokzal.setVisibility(View.VISIBLE);
            last.setVisibility(View.VISIBLE);
            garage.setVisibility(View.VISIBLE);
            org.setVisibility(View.VISIBLE);
        });
        voiti.setOnClickListener( view -> {
            if(type == 0){
                view.startAnimation(AnimationUtils.loadAnimation(activity, R.anim.button_click));
                NvEventQueueActivity.getInstance().showNotification(2, "Выберите место", 2, "", "");
            }else {
                NvEventQueueActivity.getInstance().sendSpawnClick(type);
                hide();
            }
        });
        }

    public void showselect() {
        Utils.ShowLayout(selectbr, true);
    }
    public void hide(){
        Utils.HideLayout(selectbr, true);
    }
}
