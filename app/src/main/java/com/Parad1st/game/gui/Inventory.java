package com.byparad1st.game.gui;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.nvidia.devtech.NvEventQueueActivity;
import com.byparad1st.game.R;
//ну тут все пончик как бы да
import java.util.Formatter;

public class Inventory extends AppCompatActivity {
    public Activity activity;
    public ConstraintLayout inv_main_layout, inv_main_1, textslot1;
    public TextView inv_satiety_text, inv_health_text, key_text_view, text_view_slot1;
    public ImageView active_player_skin, inv_car_key, slot1_img, active_slot_3;
    public View inv_close_butt, inv_use_butt;
    int active_3_slot = 0, save_sumka;

    public Inventory(Activity aactivity) {
        activity = aactivity;
        inv_main_layout = aactivity.findViewById(R.id.inv_main_layout);

        inv_health_text = aactivity.findViewById(R.id.inv_health_text);
        inv_satiety_text = aactivity.findViewById(R.id.inv_satiety_text);

        active_player_skin = aactivity.findViewById(R.id.active_player_skin);

        inv_car_key = aactivity.findViewById(R.id.inv_car_key);
        key_text_view = aactivity.findViewById(R.id.key_text_view);

        textslot1 = aactivity.findViewById(R.id.textslot1);

        slot1_img = aactivity.findViewById(R.id.slot1_img);
        text_view_slot1 = aactivity.findViewById(R.id.text_view_slot1);

        active_slot_3 = aactivity.findViewById(R.id.active_slot_3);

        inv_main_1 = aactivity.findViewById(R.id.inv_main_1);
        inv_main_1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                if (active_3_slot == 0) {
                    inv_main_1.setBackgroundResource(R.drawable.inv_bg_shape_active);
                    active_3_slot = 1;
                }
            }
        });

        inv_use_butt = aactivity.findViewById(R.id.inv_use_butt);
        inv_use_butt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                if (active_3_slot == 1) {
                    view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    inv_main_1.setBackgroundResource(R.drawable.inv_bg_shape);
                    textslot1.setVisibility(View.INVISIBLE);
                    slot1_img.setVisibility(View.INVISIBLE);
                    active_slot_3.setImageResource(R.drawable.inv_sumka8);
                    active_3_slot = 2;
                    NvEventQueueActivity.getInstance().sendInvRequest();
                }
            }
        });

        inv_close_butt = aactivity.findViewById(R.id.inv_close_butt);
        inv_close_butt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                HideInventory();
            }
        });

        inv_main_layout.setVisibility(View.GONE);
    }
    public void ShowInventory(int hp, int hunger, int skin_id, int key, int sumka) {
        inv_satiety_text.setText(String.format("%d", hunger));
        inv_health_text.setText(String.format("%d", hp));

        int id = activity.getResources().getIdentifier(new Formatter().format("skin_%d", Integer.valueOf(skin_id)).toString(), "drawable", activity.getPackageName());
        active_player_skin.setImageResource(id);

        if (key == 0) {
            inv_car_key.setVisibility(View.INVISIBLE);
            key_text_view.setText(String.format("%d", key));
        } else {
            inv_car_key.setVisibility(View.VISIBLE);
            key_text_view.setText(String.format("%d", key));
        }

        if (sumka == 0) {
            slot1_img.setVisibility(View.INVISIBLE);
            text_view_slot1.setVisibility(View.INVISIBLE);
        } else {
            slot1_img.setVisibility(View.VISIBLE);
            text_view_slot1.setText(String.format("%d", sumka));
        }

        inv_main_layout.setVisibility(View.VISIBLE);
    }
    public void HideInventory() {
        inv_main_layout.setVisibility(View.GONE);
    }
}
