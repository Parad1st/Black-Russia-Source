package com.byparad1st.game.gui;


import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.app.Activity;
import android.os.Build;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AnimationUtils;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.byparad1st.game.R;
import com.google.firebase.crashlytics.buildtools.reloc.org.apache.commons.cli.HelpFormatter;
import com.nvidia.devtech.CustomEditText;
import com.nvidia.devtech.NvEventQueueActivity;
import com.pedromassango.doubleclick.DoubleClick;
import com.pedromassango.doubleclick.DoubleClickListener;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

public class Keyboard {

    List<ArrayList<FrameLayout>> keys = new ArrayList<>();
    List<ArrayList<FrameLayout>> keys1 = new ArrayList<>();
    List<ArrayList<FrameLayout>> hint = new ArrayList<>();
    public Activity mActivity;

    private TextView hint_params;
    private ImageView clear;
    String[] HistoryTexts = new String[30];
    String[] LastTexts = new String[2];
    int idSelectedHistoryTexts = -1;
    private int cursorPos = 0;

    private String[] strArr = {"й", "ц", "у", "к", "е", "н", "г", "ш", "щ", "з", "х", "ф", "ы", "в", "а", "п", "р", "о", "л", "д", "ж", "э", "null", "none", "я", "ч", "с", "м", "и", "т", "ь", "б", "ю"};
    private String[] strArr1 = {"Й", "Ц", "У", "К", "Е", "Н", "Г", "Ш", "Щ", "З", "Х", "Ф", "Ы", "В", "А", "П", "Р", "О", "Л", "Д", "Ж", "Э", "null", "none", "Я", "Ч", "С", "М", "И", "Т", "Ь", "Б", "Ю"};
    private String[] strArr2 = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "nope", "a", "s", "d", "f", "g", "h", "j", "k", "l", "nope", "nope", "nope", "null", "none", "z", "x", "c", "v", "b", "n", "m", "none"};
    private String[] strArr3 = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "nope", "A", "S", "D", "F", "G", "H", "J", "K", "L", "nope", "nope", "nope", "null", "none", "Z", "X", "C", "V", "B", "N", "M", "none"};
    private String[] strArr4 = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "nope", "@", "#", "$", "%", "\"", "*", "(", ")", "-", "_", "nope", "null", "none", ".", ":", ";", "+", "=", "<", ">", "[", "]"};

    private LinearLayout layout_main, layout_history, layout_input;
    private FrameLayout keyboard_main;
    private FrameLayout history_up, history_down, des_bth, krik_bth, rp_bth, nrp_bth;
    private boolean isEn = false, isSimbol = false, isOneShift = true, isDobleShift = false;
    public static boolean isDialog = false;
    public CustomEditText keyboardTextInput;
    public static String text;

    private final int UP_BUTTON = 0;
    private final int DOWN_BUTTON = 1;
    public Activity inflate;
    public ConstraintLayout mInputLayout;

    public Keyboard(Activity activity) {
        inflate = activity;
        mActivity = activity;

        keyboard_main = inflate.findViewById(R.id.keyboard_main);
        keyboard_main.setVisibility(View.GONE);
        layout_main = inflate.findViewById(R.id.layout_main);
        layout_history = inflate.findViewById(R.id.layout_history);
        layout_input = inflate.findViewById(R.id.layout_input);
        keyboardTextInput = inflate.findViewById(R.id.input);
        hint_params = inflate.findViewById(R.id.hint_params);
        clear = inflate.findViewById(R.id.close_key_parad1st);

        //кнопки тип нрп чат и т.д
        des_bth = inflate.findViewById(R.id.des_bth);
        krik_bth = inflate.findViewById(R.id.krik_bth);
        rp_bth = inflate.findViewById(R.id.rp_chat);
        nrp_bth = inflate.findViewById(R.id.nrp_chat);

        des_bth.setOnClickListener(view -> {
            keyboardTextInput.setText("/me ");
            des_bth.setBackgroundResource(R.drawable.keyboard_red_buttons);
            rp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            krik_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            nrp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            hint_params.setVisibility(View.VISIBLE);
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) items.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                isDobleShift = false;
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }
        });
        krik_bth.setOnClickListener(view -> {
            keyboardTextInput.setText("/s ");
            krik_bth.setBackgroundResource(R.drawable.keyboard_red_buttons);
            rp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            nrp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            des_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            hint_params.setVisibility(View.VISIBLE);
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) items.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                isDobleShift = false;
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }
        });
        rp_bth.setOnClickListener(view -> {
            keyboardTextInput.setText("");
            rp_bth.setBackgroundResource(R.drawable.keyboard_red_buttons);
            nrp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            krik_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            des_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            hint_params.setVisibility(View.VISIBLE);
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) items.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                isDobleShift = false;
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }
        });
        nrp_bth.setOnClickListener(view -> {
            keyboardTextInput.setText("/n ");
            nrp_bth.setBackgroundResource(R.drawable.keyboard_red_buttons);
            rp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            krik_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            des_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
            hint_params.setVisibility(View.VISIBLE);
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) items.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                isDobleShift = false;
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }
        });
        //----------------------------------------------


        history_up = inflate.findViewById(R.id.history_up);
        history_up.setOnClickListener(view -> {
            view.startAnimation(AnimationUtils.loadAnimation(mActivity, R.anim.button_click));
            int i = this.idSelectedHistoryTexts + 1;
            if (i < this.HistoryTexts.length && this.HistoryTexts[i].length() != 0) {
                this.idSelectedHistoryTexts = i;
                this.keyboardTextInput.setText(this.HistoryTexts[i]);
                this.keyboardTextInput.setSelection(this.keyboardTextInput.getText().toString().length());
                this.keyboardTextInput.requestFocus();
            }
            if (!keyboardTextInput.getText().toString().isEmpty()) {
                hint_params.setVisibility(View.GONE);
            }
        });

        history_down = inflate.findViewById(R.id.history_down);
        history_down.setOnClickListener(view -> {
            view.startAnimation(AnimationUtils.loadAnimation(mActivity, R.anim.button_click));
            int i = this.idSelectedHistoryTexts - 1;
            if (i >= -1) {
                if (i == -1) {
                    this.keyboardTextInput.setText("");
                    this.idSelectedHistoryTexts = i;
                } else if (this.HistoryTexts[i].length() != 0) {
                    this.idSelectedHistoryTexts = i;
                    this.keyboardTextInput.setText(this.HistoryTexts[i]);
                    this.keyboardTextInput.setSelection(this.keyboardTextInput.getText().toString().length());
                }
            }
            if (keyboardTextInput.getText().toString().isEmpty()) {
                hint_params.setVisibility(View.VISIBLE);
            }
        });

        int i3 = 0;
        while (true) {
            String[] strArr = this.LastTexts;
            if (i3 >= strArr.length) {
                break;
            }
            strArr[i3] = "";
            i3++;
        }
        int i4 = 0;
        while (true) {
            String[] strArr2 = this.HistoryTexts;
            if (i4 >= strArr2.length) {
                break;
            }
            strArr2[i4] = "";
            i4++;
        }


        ArrayList<FrameLayout> hintkey = new ArrayList<>();
        hintkey.add(inflate.findViewById(R.id.key_hint));
        hint.add(hintkey);


        ArrayList<FrameLayout> key = new ArrayList<>();
        key.add(inflate.findViewById(R.id.key_0));
        key.add(inflate.findViewById(R.id.key_1));
        key.add(inflate.findViewById(R.id.key_2));
        key.add(inflate.findViewById(R.id.key_3));
        key.add(inflate.findViewById(R.id.key_4));
        key.add(inflate.findViewById(R.id.key_5));
        key.add(inflate.findViewById(R.id.key_6));
        key.add(inflate.findViewById(R.id.key_7));
        key.add(inflate.findViewById(R.id.key_8));
        key.add(inflate.findViewById(R.id.key_9));
        key.add(inflate.findViewById(R.id.key_10));
        key.add(inflate.findViewById(R.id.key_11));
        key.add(inflate.findViewById(R.id.key_12));
        key.add(inflate.findViewById(R.id.key_13));
        key.add(inflate.findViewById(R.id.key_14));
        key.add(inflate.findViewById(R.id.key_15));
        key.add(inflate.findViewById(R.id.key_16));
        key.add(inflate.findViewById(R.id.key_17));
        key.add(inflate.findViewById(R.id.key_18));
        key.add(inflate.findViewById(R.id.key_19));
        key.add(inflate.findViewById(R.id.key_20));
        key.add(inflate.findViewById(R.id.key_21));
        key.add(inflate.findViewById(R.id.key_23));
        key.add(inflate.findViewById(R.id.key_24));
        key.add(inflate.findViewById(R.id.key_25));
        key.add(inflate.findViewById(R.id.key_26));
        key.add(inflate.findViewById(R.id.key_27));
        key.add(inflate.findViewById(R.id.key_28));
        key.add(inflate.findViewById(R.id.key_29));
        key.add(inflate.findViewById(R.id.key_30));
        key.add(inflate.findViewById(R.id.key_31));
        key.add(inflate.findViewById(R.id.key_32));
        key.add(inflate.findViewById(R.id.key_33));

        key.add(inflate.findViewById(R.id.key_40));
        key.add(inflate.findViewById(R.id.key_41));
        key.add(inflate.findViewById(R.id.key_42));
        keys.add(key);

        ArrayList<FrameLayout> key1 = new ArrayList<>();
        key1.add(inflate.findViewById(R.id.key_22));
        key1.add(inflate.findViewById(R.id.key_35));
        key1.add(inflate.findViewById(R.id.key_38));
        key1.add(inflate.findViewById(R.id.key_39));
        key1.add(inflate.findViewById(R.id.key_34));
        key1.add(inflate.findViewById(R.id.key_43));
        keys1.add(key1);
        keys1.get(0).get(0).setOnClickListener(new DoubleClick(new DoubleClickListener() {
            @Override
            public void onSingleClick(View view) {
                if (isOneShift == false) {
                    FrameLayout item = (FrameLayout) keys1.get(0).get(0);
                    ImageView image = (ImageView) item.getChildAt(1);
                    int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                    image.setImageResource(id);
                    isDobleShift = false;
                    isOneShift = true;
                } else {
                    FrameLayout item = (FrameLayout) keys1.get(0).get(0);
                    ImageView image = (ImageView) item.getChildAt(1);
                    int id = inflate.getResources().getIdentifier("ic_shift_off", "drawable", mActivity.getPackageName());
                    image.setImageResource(id);
                    isDobleShift = false;
                    isOneShift = false;
                    if (isEn == true) {
                        noShiftEn();
                    } else {
                        noShiftRU();
                    }
                }
            }

            @Override
            public void onDoubleClick(View view) {
                FrameLayout item = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) item.getChildAt(1);
                isOneShift = false;
                isDobleShift = true;
                int id = inflate.getResources().getIdentifier("ic_shift_always", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
            }
        }));

        keys1.get(0).get(1).setOnClickListener(view -> {
            if (isSimbol == false) {
                Simbols();
                FrameLayout item = (FrameLayout) keys1.get(0).get(0);
                item.setVisibility(View.GONE);
            } else {
                if (isEn == true) {
                    if (keyboardTextInput.getText().toString().isEmpty() || isOneShift || isDobleShift) {
                        ShiftEn();
                    } else {
                        noShiftEn();
                    }
                } else {
                    if (keyboardTextInput.getText().toString().isEmpty() || isOneShift || isDobleShift) {
                        ShiftRU();
                    } else {
                        noShiftRU();
                    }
                }
                isSimbol = false;
                FrameLayout shift = (FrameLayout) keys1.get(0).get(0);
                shift.setVisibility(View.VISIBLE);
            }
            System.out.println("simbols");
        });
        keys1.get(0).get(2).setOnClickListener(v -> {
            FrameLayout item = (FrameLayout) keys1.get(0).get(2);
            TextView tw = (TextView) item.getChildAt(1);
            FrameLayout shift = (FrameLayout) keys1.get(0).get(0);
            shift.setVisibility(View.VISIBLE);
            if (isEn == false) {
                if (keyboardTextInput.getText().toString().isEmpty() || isOneShift || isDobleShift) {
                    ShiftEn();
                } else {
                    noShiftEn();
                }
                tw.setText("рус");
            } else {
                if (keyboardTextInput.getText().toString().isEmpty() || isOneShift || isDobleShift) {
                    ShiftRU();
                } else {
                    noShiftRU();
                }
                tw.setText("eng");
            }
            isSimbol = false;
        });

        keys1.get(0).get(3).setOnClickListener(view -> {
            String obj2 = this.keyboardTextInput.getText().toString();
            int selectionStart2 = this.keyboardTextInput.getSelectionStart();
            if (selectionStart2 - this.keyboardTextInput.getSelectionEnd() != 0) {
                String substring5 = obj2.substring(0, selectionStart2);
                String substring6 = obj2.substring(this.keyboardTextInput.getSelectionEnd(), obj2.length());
                this.keyboardTextInput.setText(substring5 + HelpFormatter.DEFAULT_LONG_OPT_SEPARATOR + substring6);
                this.keyboardTextInput.setSelection(selectionStart2 + 1);
                return;
            }
            if (!this.keyboardTextInput.isFocused()) {
                selectionStart2 = this.keyboardTextInput.getText().toString().length();
            }
            String substring7 = obj2.substring(0, selectionStart2);
            String substring8 = obj2.substring(selectionStart2);
            this.keyboardTextInput.setText(substring7 + HelpFormatter.DEFAULT_LONG_OPT_SEPARATOR + substring8);
            this.keyboardTextInput.setSelection(selectionStart2 + 1);
        });

        keys1.get(0).get(4).setOnClickListener(view -> {
            String obj = this.keyboardTextInput.getText().toString();
            int selectionStart = this.keyboardTextInput.getSelectionStart();
            if (selectionStart - this.keyboardTextInput.getSelectionEnd() != 0) {
                String substring = obj.substring(0, selectionStart);
                String substring2 = obj.substring(this.keyboardTextInput.getSelectionEnd(), obj.length());
                this.keyboardTextInput.setText(substring + substring2);
                text = keyboardTextInput.getText().toString();
                this.keyboardTextInput.setSelection(selectionStart);
                //return;
            }
            if (!this.keyboardTextInput.isFocused()) {
                selectionStart = this.keyboardTextInput.getText().toString().length();
            }
            if (selectionStart != 0) {
                int i2 = selectionStart - 1;
                String substring3 = obj.substring(0, i2);
                String substring4 = obj.substring(selectionStart);
                this.keyboardTextInput.setText(substring3 + substring4);
                text = keyboardTextInput.getText().toString();
                this.keyboardTextInput.setSelection(i2);
            }
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout item = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) item.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                hint_params.setVisibility(View.VISIBLE);
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }
        });

        keys1.get(0).get(5).setOnClickListener(view -> {
            //ShiftEn();
            //UpdKey();
            if (keyboardTextInput.getText().toString().isEmpty() || isDialog == true) {
                keyboard_main.setVisibility(View.GONE);
                text = keyboardTextInput.getText().toString();
                isDialog = false;
                return;
            }
            HideKey();
            NvEventQueueActivity.getInstance().closekey();
            try {
                NvEventQueueActivity.getInstance().Send(keyboardTextInput.getText().toString().getBytes("windows-1251"));
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            if (this.keyboardTextInput.getText().toString().length() > 0) {
                addHistoryText(this.keyboardTextInput.getText().toString());
            }

            this.idSelectedHistoryTexts = -1;
            keyboardTextInput.setText("");
            text = keyboardTextInput.getText().toString();
            hint_params.setVisibility(View.VISIBLE);
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) items.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                isDobleShift = false;
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }


        });

        clear.setOnClickListener(view -> {
            keyboardTextInput.setText("");
            hint_params.setVisibility(View.VISIBLE);
            if (keyboardTextInput.getText().toString().isEmpty()) {
                FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                ImageView image = (ImageView) items.getChildAt(1);
                int id = inflate.getResources().getIdentifier("ic_shift_single", "drawable", mActivity.getPackageName());
                image.setImageResource(id);
                isDobleShift = false;
                keys1.get(0).get(0).setVisibility(View.VISIBLE);
                isSimbol = false;
            }
        });

        for (int i = 0; i < keys.size(); i++) {
            for (int j = 0; j < keys.get(i).size(); j++) {
                keys.get(i).get(j).setOnClickListener(v -> {

                    FrameLayout item = (FrameLayout) keys.get(0).get(getKeyPos(v));
                    TextView tw = (TextView) item.getChildAt(1);
                    ImageView images = (ImageView) item.getChildAt(0);
                    String obj = keyboardTextInput.getText().toString();
                    hint_params.setVisibility(View.GONE);
                    int selectionStart = keyboardTextInput.getSelectionStart();
                    if (selectionStart - keyboardTextInput.getSelectionEnd() != 0) {
                        String substring = obj.substring(0, selectionStart);
                        String substring2 = obj.substring(keyboardTextInput.getSelectionEnd(), obj.length());
                        CustomEditText keyboardInputTextView = keyboardTextInput;
                        keyboardInputTextView.setText(substring + ((TextView) tw).getText() + substring2);
                        text = keyboardInputTextView.getText().toString();
                        keyboardTextInput.setSelection(selectionStart + 1);
                        return;
                    }
                    if (!keyboardTextInput.isFocused()) {
                        selectionStart = keyboardTextInput.getText().toString().length();
                    }
                    String substring3 = obj.substring(0, selectionStart);
                    String substring4 = obj.substring(selectionStart);
                    CustomEditText keyboardInputTextView2 = keyboardTextInput;
                    keyboardInputTextView2.setText(substring3 + ((TextView) tw).getText() + substring4);
                    text = keyboardInputTextView2.getText().toString();
                    keyboardTextInput.setSelection(selectionStart + 1);
                    if (isOneShift == true && isDobleShift == false && isSimbol == false) {
                        if (isEn) {
                            noShiftEn();
                        } else {
                            noShiftRU();
                        }
                        FrameLayout items = (FrameLayout) keys1.get(0).get(0);
                        ImageView image = (ImageView) items.getChildAt(1);
                        int id = inflate.getResources().getIdentifier("ic_shift_off", "drawable", mActivity.getPackageName());
                        image.setImageResource(id);
                    }
                    keyboardTextInput.requestFocus();
                    cursorPos = keyboardTextInput.getText().length();
                });
            }
        }

        for (int i = 0; i < keys.size(); i++) {
            for (int j = 0; j < keys.get(i).size(); j++) {
                keys.get(i).get(j).setOnTouchListener(new View.OnTouchListener() {
                    @Override
                    public boolean onTouch(View view, MotionEvent motionEvent) {
                        FrameLayout item = (FrameLayout) keys.get(0).get(getKeyPos(view));
                        TextView tw = (TextView) item.getChildAt(1);
                        ImageView img = (ImageView) item.getChildAt(0);
                        FrameLayout hints = (FrameLayout) hint.get(0).get(0);
                        FrameLayout bghint = (FrameLayout) hints.getChildAt(1);
                        TextView texthint = (TextView) bghint.getChildAt(1);
                        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(item.getLayoutParams());

                        if (motionEvent.getAction() == 0) {
                            int Height = inflate.getResources().getDimensionPixelSize(R.dimen._64sdp);
                            int Width = item.getWidth();
                            int x = (int) (item.getLeft());
                            int[] location = new int[2];
                            item.getLocationInWindow(location);
                            int xw = location[0];
                            int yw = location[1];
                            hints.setVisibility(View.VISIBLE);
                            layoutParams.height = Height;
                            layoutParams.width = Width;
                            if (getKeyPos(view) >= 33 && getKeyPos(view) <= 35) {
                                layoutParams.leftMargin = xw;
                            } else {
                                layoutParams.leftMargin = x;
                            }
                            layoutParams.topMargin = yw - 110;
                            hints.setLayoutParams(new FrameLayout.LayoutParams(layoutParams));
                            texthint.setText(tw.getText().toString());
                        }
                        if (motionEvent.getAction() == 1) {
                            hints.setVisibility(View.GONE);
                        }
                        return false;
                    }
                });
            }
        }


    }

    public void ShiftRU() {
        for (int j = 0; j < strArr1.length; j++) {
            FrameLayout item = (FrameLayout) keys.get(0).get(j);
            TextView tw = (TextView) item.getChildAt(1);
            tw.setText(strArr1[j]);
            if (tw.getText().toString() == "nope" || tw.getText().toString() == "null" || tw.getText().toString() == "none") {
                item.setVisibility(View.GONE);
            } else {
                item.setVisibility(View.VISIBLE);
            }
        }
        isEn = false;
        isOneShift = true;
    }

    public void noShiftRU() {
        for (int j = 0; j < strArr.length; j++) {
            FrameLayout item = (FrameLayout) keys.get(0).get(j);
            TextView tw = (TextView) item.getChildAt(1);
            tw.setText(strArr[j]);
            if (tw.getText().toString() == "nope" || tw.getText().toString() == "null" || tw.getText().toString() == "none") {
                item.setVisibility(View.GONE);
            } else {
                item.setVisibility(View.VISIBLE);
            }
        }
        isEn = false;
        isOneShift = false;
    }

    public void noShiftEn() {
        for (int j = 0; j < strArr2.length; j++) {
            FrameLayout item = (FrameLayout) keys.get(0).get(j);
            TextView tw = (TextView) item.getChildAt(1);
            tw.setText(strArr2[j]);
            if (tw.getText().toString() == "nope" || tw.getText().toString() == "null" || tw.getText().toString() == "none") {
                item.setVisibility(View.GONE);
            } else {
                item.setVisibility(View.VISIBLE);
            }
        }
        isEn = true;
        isOneShift = false;
    }

    public void ShiftEn() {
        for (int j = 0; j < strArr3.length; j++) {
            FrameLayout item = (FrameLayout) keys.get(0).get(j);
            TextView tw = (TextView) item.getChildAt(1);
            tw.setText(strArr3[j]);
            if (tw.getText().toString() == "nope" || tw.getText().toString() == "null" || tw.getText().toString() == "none") {
                item.setVisibility(View.GONE);
            } else {
                item.setVisibility(View.VISIBLE);
            }
            isEn = true;
            isOneShift = true;
        }
    }

    public void Simbols() {
        for (int j = 0; j < strArr4.length; j++) {
            FrameLayout item = (FrameLayout) keys.get(0).get(j);
            TextView tw = (TextView) item.getChildAt(1);
            tw.setText(strArr4[j]);
            if (tw.getText().toString() == "nope" || tw.getText().toString() == "null" || tw.getText().toString() == "none") {
                item.setVisibility(View.GONE);
            } else {
                item.setVisibility(View.VISIBLE);
            }
            isSimbol = true;
        }

    }

    public void UpdKey() {
        for (int j = 0; j < strArr3.length; j++) {
            FrameLayout item = (FrameLayout) keys.get(0).get(j);
            TextView tw = (TextView) item.getChildAt(1);
            tw.setText(strArr3[j]);
            if (tw.getText().toString() == "nope" || tw.getText().toString() == "null" || tw.getText().toString() == "none") {
                item.setVisibility(View.GONE);
            } else {
                item.setVisibility(View.VISIBLE);
            }
        }
    }

    public void ShowKey() {
        NvEventQueueActivity.getInstance().togglePlayer(1);
        rp_bth.setBackgroundResource(R.drawable.keyboard_red_buttons);
        nrp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
        krik_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
        des_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
        mActivity.runOnUiThread(() -> {
            layout_history.setVisibility(View.VISIBLE);
            layout_input.setVisibility(View.VISIBLE);
            keyboard_main.setVisibility(View.VISIBLE);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                keyboardTextInput.setShowSoftInputOnFocus(false);
            }
            cursorPos = keyboardTextInput.getText().length();
            keyboard_main.animate().setDuration(90).setListener(new AnimatorListenerAdapter() {

                @Override
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                }


                @Override
                public void onAnimationStart(Animator animation) {
                    super.onAnimationStart(animation);
                    keyboard_main.setVisibility(View.VISIBLE);
                }
            }).alpha(1.0f);
        });
    }

    public void ShowKeyDialog() {
        if (isDialog == true) {
            return;
        }
        isDialog = true;
        layout_history.setVisibility(View.GONE);
        layout_input.setVisibility(View.GONE);
        keyboard_main.setVisibility(View.VISIBLE);
        cursorPos = keyboardTextInput.getText().length();
        keyboard_main.animate().setDuration(0).translationYBy(700.0f).start();
        keyboard_main.animate().translationY(0f).setDuration(40).setListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
            }

            @Override
            public void onAnimationStart(Animator animation) {
                super.onAnimationStart(animation);
                keyboard_main.setVisibility(View.VISIBLE);
            }
        });
    }

    public void HideKey() {
        NvEventQueueActivity.getInstance().closekey();
        NvEventQueueActivity.getInstance().togglePlayer(0);
        rp_bth.setBackgroundResource(R.drawable.keyboard_red_buttons);
        nrp_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
        krik_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
        des_bth.setBackgroundResource(R.drawable.keyboard_gray_buttons);
        mActivity.runOnUiThread(() -> {
            keyboard_main.animate().setDuration(90).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                    keyboard_main.setVisibility(View.GONE);

                }
            }).alpha(0.0f);
        });
    }

    public int getKeyPos(View view) {
        for (int i = 0; i < keys.size(); i++) {
            for (int j = 0; j < keys.get(i).size(); j++) {
                if (keys.get(i).get(j) == view) {
                    return j;
                }
            }
        }
        return -1;
    }

    public void addHistoryText(String str) {
        offsetSLotsInHistory();
        int freeSlotOfHistory = getFreeSlotOfHistory();
        if (freeSlotOfHistory == -1) {
            deleteLastSlotInHistory();
            freeSlotOfHistory = getFreeSlotOfHistory();
        }
        this.HistoryTexts[freeSlotOfHistory] = str;
    }

    public void offsetSLotsInHistory() {
        int length = this.HistoryTexts.length;
        String[] strArr = new String[length];
        int i = 0;
        for (int i2 = 0; i2 < length; i2++) {
            strArr[i2] = "";
        }
        while (true) {
            String[] strArr2 = this.HistoryTexts;
            if (i < strArr2.length) {
                if (i != strArr2.length - 1) {
                    strArr[i + 1] = strArr2[i];
                }
                i++;
            } else {
                this.HistoryTexts = strArr;
                return;
            }
        }
    }

    public void deleteLastSlotInHistory() {
        int i = 1;
        while (true) {
            String[] strArr = this.HistoryTexts;
            if (i < strArr.length) {
                strArr[i - 1] = strArr[i];
                i++;
            } else {
                strArr[strArr.length - 1] = "";
                return;
            }
        }
    }

    public int getFreeSlotOfHistory() {
        int i = 0;
        while (true) {
            String[] strArr = this.HistoryTexts;
            if (i >= strArr.length) {
                return -1;
            }
            if (strArr[i].length() == 0) {
                return i;
            }
            i++;
        }
    }
}
