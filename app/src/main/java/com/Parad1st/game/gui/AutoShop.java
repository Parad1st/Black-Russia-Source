package com.byparad1st.game.gui;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.util.Log;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Locale;

public class AutoShop {
    public final int AUTOSHOP_BUTTON_COLOR_LEFT = 1;
    public final int AUTOSHOP_BUTTON_COLOR_RIGHT = 2;
    public final int AUTOSHOP_BUTTON_LEFT = 3;
    public final int AUTOSHOP_BUTTON_TESTDRIVE = 4;
    public final int AUTOSHOP_BUTTON_BUY = 5;
    public final int AUTOSHOP_BUTTON_EXIT = 6;
    public final int AUTOSHOP_BUTTON_RIGHT = 7;
    public final int AUTOSHOP_BUTTON_CAMERA = 8;

    public Button autoshop_buybutt;
    public Button autoshop_colorleft;
    public Button autoshop_colorright;
    public Button autoshop_rightbutt;
    public Button autoshop_leftbutt;
    public Button autoshop_camerabutt;
    public Button testdrive_butt;
    public Button autoshop_exitbutt;
    public ConstraintLayout autoshop_main_layout;
    public TextView autoshop_modelname;
    public TextView autoshop_pricevalue;
    public TextView autoshop_maxspeed_value;
    public TextView autoshop_acceleration_value;
    public TextView autoshop_available_value;
    public TextView autoshop_gear_value;
    public DecimalFormat price_format;


    public AutoShop(Activity activity)
    {
        DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols(Locale.getDefault());
        otherSymbols.setGroupingSeparator('.');
        price_format = new DecimalFormat("###,###.###", otherSymbols);

        autoshop_exitbutt = activity.findViewById(R.id.autoshop_exitbutt);
        autoshop_buybutt = activity.findViewById(R.id.autoshop_buybutt);
        autoshop_colorright = activity.findViewById(R.id.autoshop_colorright);
        autoshop_colorleft = activity.findViewById(R.id.autoshop_colorleft);
        testdrive_butt = activity.findViewById(R.id.testdrive_butt);
        autoshop_leftbutt = activity.findViewById(R.id.autoshop_leftbutt);
        autoshop_camerabutt = activity.findViewById(R.id.autoshop_camerabutt);
        autoshop_rightbutt = activity.findViewById(R.id.autoshop_rightbutt);
        autoshop_main_layout = activity.findViewById(R.id.autoshop_main_layout);
        autoshop_available_value = activity.findViewById(R.id.autoshop_available_value);
        autoshop_acceleration_value = activity.findViewById(R.id.autoshop_acceleration_value);
        autoshop_maxspeed_value = activity.findViewById(R.id.autoshop_maxspeed_value);
        autoshop_modelname = activity.findViewById(R.id.autoshop_modelname);
        autoshop_pricevalue = activity.findViewById(R.id.autoshop_pricevalue);
        autoshop_gear_value = activity.findViewById(R.id.autoshop_gear_value);

        autoshop_colorleft.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_COLOR_LEFT);
        });
        autoshop_colorright.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_COLOR_RIGHT);
        });
        autoshop_leftbutt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_LEFT);
        });
        testdrive_butt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_TESTDRIVE);
        });
        autoshop_buybutt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_BUY);
        });
        autoshop_exitbutt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_EXIT);
            HideShow();
        });
        autoshop_rightbutt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_RIGHT);
        });
        autoshop_camerabutt.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().sendAutoShopButton(AUTOSHOP_BUTTON_CAMERA);
        });
        Utils.HideLayout(autoshop_main_layout, false);
    }
    @SuppressLint({"SetTextI18n", "DefaultLocale"})
    public void Update(String name, int price, int count, float maxspeed, float acceleration, int gear)
    {
        switch (gear){
            case 1: {
                autoshop_gear_value.setText("Задний");
                break;
            }
            case 2: {
                autoshop_gear_value.setText("Передний");
                break;
            }
            case 3: {
                autoshop_gear_value.setText("Полный");
                break;
            }
            case 4: {
                autoshop_gear_value.setText("Цепной");
                break;
            }
            default: {
                autoshop_gear_value.setText("Не указан");
                break;
            }
        }

        autoshop_pricevalue.setText(price_format.format(price)+" руб.");
        autoshop_available_value.setText(String.format("%d", count));
        autoshop_acceleration_value.setText(String.format("%.1f", acceleration)+" с.");
        autoshop_maxspeed_value.setText(String.format("%.0f", maxspeed)+" км/ч");
        autoshop_modelname.setText(String.valueOf(name));
    }

    public void ToggleShow()
    {
        Utils.ShowLayout(autoshop_main_layout, true);
        NvEventQueueActivity.getInstance().hideHud();
    }
    public void HideShow()
    {
        Utils.HideLayout(autoshop_main_layout, true);
        NvEventQueueActivity.getInstance().showHud();
    }
}
