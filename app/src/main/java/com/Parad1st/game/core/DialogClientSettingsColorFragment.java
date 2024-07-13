package com.byparad1st.game.core;

import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.appcompat.widget.AppCompatButton;
import androidx.fragment.app.Fragment;

import com.byparad1st.game.R;
import com.nvidia.devtech.NvEventQueueActivity;
import com.skydoves.colorpickerview.ColorEnvelope;
import com.skydoves.colorpickerview.ColorPickerDialog;
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener;

public class DialogClientSettingsColorFragment extends Fragment implements ISaveableFragment {

    private AppCompatButton mButtonHPColor;
    private AppCompatButton mButtonArmorColor;
    private AppCompatButton mButtonMoneyColor;
    private AppCompatButton mButtonWantedColor;
    private AppCompatButton mButtonHpTextColor;
    private AppCompatButton mButtonArmorTextColor;
    private AppCompatButton mButtonRadarColor;
    private AppCompatButton mButtonAmmoColor;

    private NvEventQueueActivity mContext = null;

    public static DialogClientSettingsColorFragment createInstance(String txt)
    {
        DialogClientSettingsColorFragment fragment = new DialogClientSettingsColorFragment();
        return fragment;
    }
    @Override
    public View onCreateView(LayoutInflater inflater,
                             ViewGroup container,
                             Bundle savedInstanceState) {

        mContext = (NvEventQueueActivity) getActivity();

        View view = inflater.inflate(R.layout.dialog_settings_color,container,false);

        mButtonHPColor = view.findViewById(R.id.button_color_hp);
        mButtonArmorColor = view.findViewById(R.id.button_color_armour);
        mButtonMoneyColor = view.findViewById(R.id.button_color_money);
        mButtonWantedColor = view.findViewById(R.id.button_color_wanted);
        mButtonHpTextColor = view.findViewById(R.id.button_color_hp_text);
        mButtonArmorTextColor = view.findViewById(R.id.button_color_armour_text);
        mButtonRadarColor = view.findViewById(R.id.button_color_radar);
        mButtonAmmoColor = view.findViewById(R.id.button_color_ammos);

        getValues();

        mButtonRadarColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color6")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(6, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;

                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonAmmoColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color9")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(9, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;

                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonHPColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color0")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(0, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;

                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonArmorColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color1")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(1, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;

                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonMoneyColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color2")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(2, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;
                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonWantedColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color3")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(3, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;

                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonHpTextColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color4")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(4, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(2);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;

                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        mButtonArmorTextColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ColorPickerDialog.Builder dialog = new ColorPickerDialog.Builder(mContext)
                        .setPreferenceName("color5")
                        .setPositiveButton("Применить",
                                new ColorEnvelopeListener() {
                                    @Override
                                    public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                        int[] argb = envelope.getArgb();
                                        mContext.setNativeHudElementColor(5, argb[0], argb[1], argb[2], argb[3]);
                                        getValues();
                                    }
                                })
                        .setNegativeButton("Закрыть",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialogInterface, int i) {
                                        dialogInterface.dismiss();
                                    }
                                })
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true)  // the default value is true.
                        .setBottomSpace(0);

                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)dialog.getColorPickerView().getLayoutParams();

                params.height = (int)(params.height * 0.25f);
                params.width = (int)(params.width * 0.25f);
                params.topMargin = 10;
                params.bottomMargin = 0;


                dialog.getColorPickerView().setLayoutParams(params);
                dialog.show();

            }
        });

        return view;
    }

    @Override
    public void save() {

    }

    @Override
    public void getValues() {
        String color = mContext.getHudElementColor(0);
        mButtonHPColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(1);
        mButtonArmorColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(2);
        mButtonMoneyColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(3);
        mButtonWantedColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(4);
        mButtonHpTextColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(5);
        mButtonArmorTextColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(6);
        mButtonRadarColor.setBackgroundColor(Color.parseColor(color));

        color = mContext.getHudElementColor(9);
        mButtonAmmoColor.setBackgroundColor(Color.parseColor(color));

    }
}
