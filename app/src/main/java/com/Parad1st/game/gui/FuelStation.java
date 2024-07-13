package com.byparad1st.game.gui;

import android.app.Activity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.byparad1st.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;
import com.byparad1st.game.R;

public class FuelStation extends AppCompatActivity {
    public Activity activity;
    public FrameLayout br_fuelstation_layout;
    public TextView fuelstation_literinfo, fuelstation_buyinfo, fuelstation_1_info, fuelstation_2_info, fuelstation_3_info, fuelstation_4_info, fuelstation_5_info, fuelstation_info;
    public ConstraintLayout fuelstation_1, fuelstation_2, fuelstation_3, fuelstation_4, fuelstation_5;
    int fuelstation_active, fielstation_progress, fuelprice1, fuelprice2, fuelprice3, fuelprice4, fuelprice5, fuelprice;
    public SeekBar fuelstation_bar;
    public ImageView fuelstation_exit;
    public Button fuelstationBuyButt;

    public FuelStation(Activity aactivity) {

        activity = aactivity;
        br_fuelstation_layout = aactivity.findViewById(R.id.br_fuelstation_layout);

        fuelstation_literinfo = aactivity.findViewById(R.id.fuelstation_literinfo);
        fuelstation_literinfo.setText("0 Л");

        fuelstation_buyinfo = aactivity.findViewById(R.id.fuelstation_buyinfo);
        fuelstation_buyinfo.setText("0 P");


        fuelstation_1 = aactivity.findViewById(R.id.fuelstation_1);
        fuelstation_2 = aactivity.findViewById(R.id.fuelstation_2);
        fuelstation_3 = aactivity.findViewById(R.id.fuelstation_3);
        fuelstation_4 = aactivity.findViewById(R.id.fuelstation_4);
        fuelstation_5 = aactivity.findViewById(R.id.fuelstation_5);

        fuelstation_1_info = aactivity.findViewById(R.id.fuelstation_1_info);
        fuelstation_2_info = aactivity.findViewById(R.id.fuelstation_2_info);
        fuelstation_3_info = aactivity.findViewById(R.id.fuelstation_3_info);
        fuelstation_4_info = aactivity.findViewById(R.id.fuelstation_4_info);
        fuelstation_5_info = aactivity.findViewById(R.id.fuelstation_5_info);

        fuelstation_bar = aactivity.findViewById(R.id.fuelstation_bar);

        fuelstation_exit = aactivity.findViewById(R.id.fuelstation_exit);
        fuelstation_exit.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                HideFuelStation(0, 0);
            }
        });

        fuelstationBuyButt = aactivity.findViewById(R.id.fuelstationBuyButt);
        fuelstationBuyButt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                HideFuelStation(fuelprice, fielstation_progress);
            }
        });

        fuelstation_bar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                fielstation_progress = progress;
                if (fuelstation_active == 1) { fuelprice = fuelprice1 * progress; }
                else if (fuelstation_active == 2) { fuelprice = fuelprice2 * progress; }
                else if (fuelstation_active == 3) { fuelprice = fuelprice3 * progress; }
                else if (fuelstation_active == 4) { fuelprice = fuelprice4 * progress; }
                else if (fuelstation_active == 5) { fuelprice = fuelprice5 * progress; }
                String strpriceinfo = String.format("%s р", fuelprice);
                String strliterinfo = String.format("%s л", progress);
                fuelstation_buyinfo.setText(String.valueOf(strpriceinfo));
                fuelstation_literinfo.setText(String.valueOf(strliterinfo));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        fuelstation_info = aactivity.findViewById(R.id.fuelstation_info);

        fuelstation_1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                fuelstation_1.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
                fuelstation_2.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_3.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_4.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_5.setBackgroundResource(R.drawable.fuelstation_item_bg);
            }
        });
        fuelstation_2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                fuelstation_2.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
                fuelstation_1.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_3.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_4.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_5.setBackgroundResource(R.drawable.fuelstation_item_bg);
            }
        });
        fuelstation_3.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                fuelstation_3.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
                fuelstation_1.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_2.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_4.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_5.setBackgroundResource(R.drawable.fuelstation_item_bg);
            }
        });
        fuelstation_4.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                fuelstation_4.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
                fuelstation_1.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_2.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_3.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_5.setBackgroundResource(R.drawable.fuelstation_item_bg);
            }
        });
        fuelstation_5.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                fuelstation_5.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
                fuelstation_1.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_2.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_3.setBackgroundResource(R.drawable.fuelstation_item_bg);
                fuelstation_4.setBackgroundResource(R.drawable.fuelstation_item_bg);
            }
        });

        Utils.HideLayout(br_fuelstation_layout, false);
    }
    public void ShowFuelStation(int type, int price1, int price2, int price3, int price4, int price5, int maxCount) {
        fuelprice1 = price1;
        fuelprice2 = price2;
        fuelprice3 = price3;
        fuelprice4 = price5;
        fuelprice5 = price4;

        String strprice1 = String.format("%sр/литр", price1);
        String strprice2 = String.format("%sр/литр", price2);
        String strprice3 = String.format("%sр/литр", price3);
        String strprice4 = String.format("%sр/литр", price5);
        String strprice5 = String.format("%sр/литр", price4);
        fuelstation_1_info.setText(String.valueOf(strprice1));
        fuelstation_2_info.setText(String.valueOf(strprice2));
        fuelstation_3_info.setText(String.valueOf(strprice3));
        fuelstation_4_info.setText(String.valueOf(strprice5));
        fuelstation_5_info.setText(String.valueOf(strprice4));

        fuelstation_bar.setMax(maxCount);
        fuelstation_bar.setProgress(0);

        fuelstation_active = type;
        if (type == 1) {
            fuelstation_info.setText("Рекомендуемый тип топлива: АИ-92");
            fuelstation_1.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
        }
        else if (type == 2) {
            fuelstation_info.setText("Рекомендуемый тип топлива: АИ-95");
            fuelstation_2.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
        }
        else if (type == 3) {
            fuelstation_info.setText("Рекомендуемый тип топлива: АИ-98");
            fuelstation_3.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
        }
        else if (type == 4) {
            fuelstation_info.setText("Рекомендуемый тип топлива: ДТ");
            fuelstation_4.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
        }
        else if (type == 5) {
            fuelstation_info.setText("Рекомендуемый тип топлива: АИ-100");
            fuelstation_5.setBackgroundResource(R.drawable.fuelstation_item_active_bg);
        }
        
        Utils.ShowLayout(br_fuelstation_layout, true);
    }
    public void HideFuelStation(int typefuel, int literfuel) {
        NvEventQueueActivity.getInstance().onFuelStationClick(typefuel, literfuel);
        Utils.HideLayout(br_fuelstation_layout, true);
    }
}