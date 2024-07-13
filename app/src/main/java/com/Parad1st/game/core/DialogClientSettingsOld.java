package com.byparad1st.game.core;

import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.RelativeLayout;
import android.widget.SeekBar;

import androidx.annotation.NonNull;
import androidx.appcompat.widget.AppCompatButton;
import androidx.appcompat.widget.SwitchCompat;

import com.byparad1st.game.R;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.HashMap;
import java.util.Iterator;

public class DialogClientSettingsOld extends Dialog {

    private SwitchCompat mSwitchKeyboard;
    private SwitchCompat mSwitchCutout;
    private SwitchCompat mSwitchFPSCounter;
    private SwitchCompat mSwitchOutfit;
    private SwitchCompat mSwitchHpArmour;
    private SwitchCompat mSwitchRadarrect;
    private SwitchCompat mSwitchPCMoney;
    private SwitchCompat mSwitchSkyBox;

    private AppCompatButton mButtonHPColor;
    private AppCompatButton mButtonArmorColor;
    private AppCompatButton mButtonMoneyColor;
    private AppCompatButton mButtonWantedColor;
    private AppCompatButton mButtonHpTextColor;
    private AppCompatButton mButtonArmorTextColor;
    private AppCompatButton mButtonRadarColor;
    private AppCompatButton mButtonAmmoColor;

    static final int mSettingsHudCount = 12;

    private SeekBar.OnSeekBarChangeListener mListenerSeekBars;

    private NvEventQueueActivity mContext;

    private boolean mChangingAllowed = false;

    private HashMap<ViewGroup, Drawable> mOldDrawables;

    public DialogClientSettingsOld(@NonNull Context context) {
        super(context);

        mContext = (NvEventQueueActivity)context;
    }

    private void makeAllElementsInvisible(ViewGroup parent, View notVisible, boolean first)
    {
        if(first)
        {
            mOldDrawables = new HashMap<ViewGroup, Drawable>();
            mOldDrawables.put((ViewGroup)parent, ((ViewGroup)parent).getBackground());
            parent.setBackground(new ColorDrawable(android.graphics.Color.TRANSPARENT));
        }

        if(parent == null) {
            return;
        }


        for(int i = 0; i < parent.getChildCount(); i++)
        {
            View view = parent.getChildAt(i);

            if(view instanceof ViewGroup)
            {
                makeAllElementsInvisible((ViewGroup)view, notVisible, false);
                mOldDrawables.put((ViewGroup)view, ((ViewGroup)view).getBackground());
                view.setBackground(new ColorDrawable(android.graphics.Color.TRANSPARENT));
            }
            else
            {
                if(view != notVisible)
                {
                    view.setAlpha(0.0f);
                }
            }
        }
    }

    private void makeAllElementsVisible(ViewGroup parent, View notVisible, boolean first)
    {
        if(first)
        {
            Iterator myVeryOwnIterator = mOldDrawables.keySet().iterator();
            while(myVeryOwnIterator.hasNext())
            {
                ViewGroup key=(ViewGroup)myVeryOwnIterator.next();
                Drawable value=(Drawable)mOldDrawables.get(key);

                key.setBackground(value);
            }
        }

        if(parent == null)
        {
            return;
        }
        for(int i = 0; i < parent.getChildCount(); i++)
        {
            View view = parent.getChildAt(i);

            if(view instanceof ViewGroup)
            {
                makeAllElementsVisible((ViewGroup)view, notVisible, false);
            }
            else
            {
                if(view != notVisible)
                {
                    view.setAlpha(1.0f);
                }
            }
        }
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        mChangingAllowed = false;
        super.onCreate(savedInstanceState);

        setContentView(R.layout.dialog_settings_old);

        findViewById(R.id.settings_root_layout).getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                mChangingAllowed = true;
            }
        });

        getWindow().setDimAmount(.0f);
        getWindow().setGravity(Gravity.CENTER);
        getWindow().setLayout(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
        getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.P)
        {
            findViewById(R.id.switch_cutout).setVisibility(View.GONE);
        }




        //setSeekBarListeners();

        setCancelable(false);

    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        passValuesToNative();
        mContext.onSettingsWindowSave();
        DialogClientSettingsOld.this.dismiss();
    }

    public void passValuesToNative()
    {

    }

    private void getValues()
    {
        mChangingAllowed = false;



        getColors();

        mChangingAllowed = true;
    }

    private void getColors()
    {

    }
}
