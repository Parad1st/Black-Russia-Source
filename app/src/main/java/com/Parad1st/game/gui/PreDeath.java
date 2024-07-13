package com.byparad1st.game.gui;

import android.app.Activity;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.nvidia.devtech.NvEventQueueActivity;
import com.byparad1st.game.R;

public class PreDeath extends AppCompatActivity {

    public Activity activity;
    public ConstraintLayout pre_death_layout;
    public Button preDeath_to_hospital_button;

    public PreDeath(Activity aactivity) {
        activity = aactivity;
        pre_death_layout = aactivity.findViewById(R.id.pre_death_layout);

        preDeath_to_hospital_button = aactivity.findViewById(R.id.preDeath_to_hospital_button);
        preDeath_to_hospital_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendHospital(0);
                HidePreDeath();
            }
        });

        pre_death_layout.setVisibility(View.GONE);
    }
    public void ShowPreDeath() {
        pre_death_layout.setVisibility(View.VISIBLE);
    }
    public void HidePreDeath() {
        pre_death_layout.setVisibility(View.GONE);
    }
}
