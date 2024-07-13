package com.byparad1st.game.gui;

import android.app.Activity;
import android.content.Context;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.core.content.res.ResourcesCompat;

import com.nvidia.devtech.CustomEditText;
import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

import java.io.UnsupportedEncodingException;
import java.util.regex.Pattern;
import org.json.JSONObject;

public class BrDialogWindow {
    private NvEventQueueActivity mActivity = NvEventQueueActivity.getInstance();
    private View mSelectedButton = null;
    private TextView mOrigButton = null;
    private int mClickedButton = -1;
    private CustomEditText mEditText = null;
    private int mListitemToSend = -1;
    private boolean mIsTab = false;
    private boolean ifOtherMethodClosed = false;
    String str;
    String str2;
    String str3;
    int i;
    String str4;
    Button button;
    boolean z;
    boolean z2;
    boolean z3;
    String[] strArr;
    String[] strArr2;
    String str5;
    boolean z4;
    TextView textView;
    TextView textView2;
    Activity aactivity;
    int mCurrentDialogId;
    LinearLayout pon;

    public BrDialogWindow (Activity activity) {
        aactivity = activity;
        pon = activity.findViewById(R.id.dw_root);
        Utils.HideLayout(pon, false);
    }

    private String[] getSplittedStrings(String str) {
        return str.split(Pattern.quote("\n"));
    }

    private String[] getSplittedTabs(String str) {
        return str.split(Pattern.quote("\t"));
    }

    public float[] calcuteStringsLength(TextView textView, String[] strArr) {
        float[] fArr = new float[strArr.length];
        TextPaint textPaint = new TextPaint();
        textPaint.setTextSize(textView.getTextSize());
        textPaint.setTypeface(textView.getTypeface());
        for (int i = 0; i < strArr.length; i++) {
            fArr[i] = new StaticLayout(Utils.transfromColors(strArr[i]), textPaint, 10000, Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false).getLineWidth(0) + 5.0f;
        }
        return fArr;
    }

    private TextView createButtonFromOrig(TextView textView, boolean z, boolean z2) {
        TextView textView2 = new TextView(this.mActivity);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(-1, -2);
        if (z) {
            layoutParams.topMargin = NvEventQueueActivity.dpToPx(6.0f, this.mActivity);
        }
        textView2.setLayoutParams(layoutParams);
        textView2.setBackground(textView.getBackground());
        textView2.setTypeface(textView.getTypeface());
        textView2.setGravity(textView.getGravity());
        if (!z2) {
            textView2.setPadding(textView.getPaddingLeft(), textView.getPaddingTop(), textView.getPaddingRight(), textView.getPaddingBottom());
        }
        textView2.setAllCaps(false);
        textView2.setTextColor(textView.getTextColors().getDefaultColor());
        textView2.setTextSize(0, textView.getTextSize());
        return textView2;
    }

    private float[][] getColumnsWidth(TextView textView, String[] strArr) {
        float[][] fArr = new float[strArr.length][];
        for (int i = 0; i < strArr.length; i++) {
            String[] splittedTabs = getSplittedTabs(strArr[i]);
            fArr[i] = new float[splittedTabs.length];
            for (int i2 = 0; i2 < splittedTabs.length; i2++) {
                textView.setText(Utils.transfromColors(splittedTabs[i2]));
                fArr[i][i2] = (float) (Utils.getTextLength(textView) + NvEventQueueActivity.dpToPx(54.0f, this.mActivity));
            }
        }
        return fArr;
    }

    private float[] getMaxWidths(float[][] fArr) {
        float[] fArr2 = new float[5];
        for (int i = 0; i < 5; i++) {
            fArr2[i] = -1.0f;
        }
        for (int i2 = 0; i2 < 5; i2++) {
            for (int i3 = 0; i3 < fArr.length; i3++) {
                if (i2 < fArr[i3].length && fArr[i3][i2] >= fArr2[i2]) {
                    fArr2[i2] = fArr[i3][i2];
                }
            }
        }
        return fArr2;
    }

    public void show(int dialogId, int dialogTypeId, String caption, String content, String leftBtnText, String rightBtnText) {
        mCurrentDialogId = dialogId;
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("i", dialogTypeId);
            jsonObject.put("l", leftBtnText);
            jsonObject.put("r", rightBtnText);
            jsonObject.put("c", caption);
            jsonObject.put("s", content);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        show(jsonObject);
    }

    public void show(JSONObject jSONObject) {
        this.mClickedButton = -1;
        this.mListitemToSend = -1;
        Utils.makeAllViewsVisible(pon);
        try {
            i = jSONObject.getInt("i");
            try {
                str3 = jSONObject.getString("c");
            } catch (Exception ignored) {
            }
        } catch (Exception ignored) {
        }
        try {
            str2 = jSONObject.getString("s");
            try {
                str = jSONObject.getString("l");
                try {
                    str4 = jSONObject.getString("r");
                } catch (Exception ignored) {
                }
            } catch (Exception ignored) {
            }
        } catch (Exception ignored) {
        }
        Button button222 = aactivity.findViewById(R.id.dw_button_ok);
        button = aactivity.findViewById(R.id.dw_button_cancel);
        TextView textView322 = aactivity.findViewById(R.id.dw_caption);
        CustomEditText customEditTextWithBackPressEvent22 = aactivity.findViewById(R.id.dw_edittext);
        this.mEditText = customEditTextWithBackPressEvent22;
        NvEventQueueActivity.fixEditTextForAndroid10Xiaomi(customEditTextWithBackPressEvent22);
        button222.setText(Utils.transfromColors(str));
        button.setText(Utils.transfromColors(str4));
        textView322.setText(Utils.transfromColors(str3));
        if (str4 != null && str4.length() == 0) {
            button.setVisibility(View.GONE);
        }
        if (i == 4) {
            z3 = false;
            z2 = true;
            i = 2;
        }
        if (i == 5) {
            z3 = true;
            i = 2;
            z2 = true;
        }
        button222.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ifOtherMethodClosed = true;
                sendResponse(1);
            }
        });
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ifOtherMethodClosed = true;
                sendResponse(0);
            }
        });
        Utils.changeTextViewWidth(textView322);
        if (i == 0) {
            this.mEditText.setVisibility(View.GONE);
            aactivity.findViewById(R.id.dw_scroll_root).setVisibility(View.GONE);
            TextView textView4 = (TextView) aactivity.findViewById(R.id.dw_info);
            textView4.setText(Utils.transfromColors(str2));
            Utils.changeTextViewWidth(textView4);
        } else if (i == 1) {
            aactivity.findViewById(R.id.dw_scroll_root).setVisibility(View.GONE);
            TextView textView5 = (TextView) aactivity.findViewById(R.id.dw_info);
            textView5.setText(Utils.transfromColors(str2));
            Utils.changeTextViewWidth(textView5);
            this.mEditText.getEditableText().clear();
        } else if (i == 3) {
            aactivity.findViewById(R.id.dw_scroll_root).setVisibility(View.GONE);
            TextView textView5 = aactivity.findViewById(R.id.dw_info);
            textView5.setText(Utils.transfromColors(str2));
            Utils.changeTextViewWidth(textView5);
            this.mEditText.getEditableText().clear();
            //this.mEditText.setTransformationMethod(PasswordTransformationMethod.getInstance());
        } else if (i == 2) {
            LinearLayout linearLayout = aactivity.findViewById(R.id.dw_header);
            aactivity.findViewById(R.id.dw_scroll_info).setVisibility(View.GONE);
            this.mEditText.setVisibility(View.GONE);
            if (!z3) {
                aactivity.findViewById(R.id.dw_divider).setVisibility(View.GONE);
                linearLayout.setVisibility(View.GONE);
            }
            if (this.mOrigButton == null) {
                this.mOrigButton = aactivity.findViewById(R.id.dw_list_button);
            }
            TextView textView6 = this.mOrigButton;
            String[] splittedStrings = getSplittedStrings(str2);
            final float[] maxWidths = z2 ? getMaxWidths(getColumnsWidth(textView6, splittedStrings)) : null;
            if (z3) {
                str5 = splittedStrings[0];
                String[] strArr3 = new String[splittedStrings.length - 1];
                System.arraycopy(splittedStrings, 1, strArr3, 0, splittedStrings.length - 1);
                String[] splittedTabs = getSplittedTabs(str5);
                for (int i3 = 0; i3 < linearLayout.getChildCount(); i3++) {
                    linearLayout.getChildAt(i3).setVisibility(View.GONE);
                }
                for (int i4 = 0; i4 < splittedTabs.length; i4++) {
                    TextView textView7 = (TextView) linearLayout.getChildAt(i4);
                    textView7.setText(Utils.transfromColors(splittedTabs[i4]));
                    textView7.setVisibility(View.VISIBLE);
                }
                strArr = splittedTabs;
                strArr2 = strArr3;
            } else {
                strArr2 = splittedStrings;
                str5 = null;
                strArr = null;
            }
            float[] calcuteStringsLength = calcuteStringsLength(textView6, strArr2);
            if (calcuteStringsLength != null && calcuteStringsLength.length != 0) {
                float f = calcuteStringsLength[0];
                for (float v : calcuteStringsLength) {
                    if (v > f) {
                        f = v;
                    }
                }
                final View findViewById = aactivity.findViewById(R.id.dw_scroll_root);
                LinearLayout.LayoutParams layoutParams = (LinearLayout.LayoutParams) findViewById.getLayoutParams();
                layoutParams.width = ((int) f) + NvEventQueueActivity.dpToPx(54.0f, this.mActivity);
                if (z2) {
                    layoutParams.width = 0;
                    for (float f2 : maxWidths) {
                        layoutParams.width = (int) (((float) layoutParams.width) + f2);
                    }
                }
                int textLength = Utils.getTextLength(textView322);
                if (layoutParams.width < textLength) {
                    layoutParams.width = textLength;
                    z4 = true;
                } else {
                    z4 = false;
                }
                findViewById.setLayoutParams(layoutParams);
                ViewGroup viewGroup = (ViewGroup) aactivity.findViewById(R.id.dw_root);
                LinearLayout linearLayout2 = (LinearLayout) aactivity.findViewById(R.id.dw_scroll_layout);
                TextView textView8 = textView6;
                viewGroup.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
                    @Override
                    public void onGlobalLayout() {
                        viewGroup.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                        if (viewGroup.getWidth() > findViewById.getWidth() && z4) {
                            ViewGroup.LayoutParams layoutParams2 = findViewById.getLayoutParams();
                            layoutParams2.width = -1;
                            findViewById.setLayoutParams(layoutParams2);
                        }
                        if (viewGroup.findViewById(R.id.dw_root_buttons).getWidth() > findViewById.getWidth()) {
                            ViewGroup.LayoutParams layoutParams3 = findViewById.getLayoutParams();
                            layoutParams3.width = -1;
                            findViewById.setLayoutParams(layoutParams3);
                        }
                        float[] fArr = maxWidths;
                        if (fArr != null) {
                            int length = fArr.length;
                            float[] fArr2 = new float[length];
                            for (int i6 = 0; i6 < length; i6++) {
                                fArr2[i6] = maxWidths[i6] / ((float) findViewById.getWidth());
                            }
                            if (strArr != null) {
                                for (int i7 = 0; i7 < strArr.length; i7++) {
                                    TextView textView9 = (TextView) linearLayout.getChildAt(i7);
                                    LinearLayout.LayoutParams layoutParams4 = (LinearLayout.LayoutParams) textView9.getLayoutParams();
                                    layoutParams4.weight = 1.0f - fArr2[i7];
                                    textView9.setLayoutParams(layoutParams4);
                                }
                            }
                            for (int i8 = 0; i8 < linearLayout2.getChildCount(); i8++) {
                                if (linearLayout2.getChildAt(i8) instanceof LinearLayout) {
                                    LinearLayout linearLayout3 = (LinearLayout) linearLayout2.getChildAt(i8);
                                    for (int i9 = 0; i9 < linearLayout3.getChildCount(); i9++) {
                                        LinearLayout.LayoutParams layoutParams5 = (LinearLayout.LayoutParams) linearLayout3.getChildAt(i9).getLayoutParams();
                                        layoutParams5.weight = 1.0f - fArr2[i9];
                                        linearLayout3.getChildAt(i9).setLayoutParams(layoutParams5);
                                    }
                                }
                            }
                        }
                    }
                });
                linearLayout2.removeAllViews();
                int i6 = 0;
                while (i6 < strArr2.length) {
                    if (z2) {
                        LinearLayout linearLayout3 = new LinearLayout(this.mActivity);
                        LinearLayout.LayoutParams layoutParams2 = new LinearLayout.LayoutParams(-1, -2);
                        if (i6 >= 1) {
                            layoutParams2.topMargin = NvEventQueueActivity.dpToPx(6.0f, this.mActivity);
                        }
                        linearLayout3.setLayoutParams(layoutParams2);
                        linearLayout3.setOrientation(LinearLayout.HORIZONTAL);
                        linearLayout3.setPadding(textView8.getPaddingLeft(), textView8.getPaddingTop(), textView8.getPaddingRight(), textView8.getPaddingBottom());
                        linearLayout3.setBackground(ResourcesCompat.getDrawable(this.mActivity.getResources(), R.drawable.br_list_inactive, null));
                        linearLayout2.addView(linearLayout3);
                        for (String str6 : getSplittedTabs(strArr2[i6])) {
                            TextView textView77 = createButtonFromOrig(textView, true, false);
                            TextView textView = new TextView(mActivity);
                            textView.setText(Utils.transfromColors(str6));
                            textView.setTypeface(textView77.getTypeface());
                            textView.setTextSize(13);
                            linearLayout3.addView(textView);
                        }
                        if(i6 == 0) {
                            mSelectedButton = linearLayout3;
                            mClickedButton = 1;
                            mListitemToSend = i6;
                            mSelectedButton.setBackground(ResourcesCompat.getDrawable(BrDialogWindow.this.mActivity.getResources(), R.drawable.br_list_active, null));
                        }
                        int finalI1 = i6;
                        linearLayout3.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View view) {
                                if (mSelectedButton != view) {
                                    if (mSelectedButton != null) {
                                        mSelectedButton.setBackground(ResourcesCompat.getDrawable(BrDialogWindow.this.mActivity.getResources(), R.drawable.br_list_inactive, null));
                                    }
                                    mSelectedButton = view;
                                    mClickedButton = 1;
                                    mListitemToSend = finalI1;
                                    mSelectedButton.setBackground(ResourcesCompat.getDrawable(BrDialogWindow.this.mActivity.getResources(), R.drawable.br_list_active, null));
                                    mSelectedButton.startAnimation(AnimationUtils.loadAnimation(BrDialogWindow.this.mActivity, R.anim.button_click));
                                    return;
                                }
                                mClickedButton = 1;
                                ifOtherMethodClosed = true;
                                sendResponse(1);
                                mSelectedButton = null;
                            }
                        });
                        textView = textView8;
                    } else {
                        textView = textView8;
                        if (i6 >= 1) {
                            textView2 = createButtonFromOrig(textView, true, false);
                        }
                        if (i6 == 0) {
                            textView2 = createButtonFromOrig(textView, false, false);
                        }
                    }
                        int finalI = i6;
                    if(!z2) {
                        textView2.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View view) {
                                if (mSelectedButton != view) {
                                    if (mSelectedButton != null) {
                                        mSelectedButton.setBackground(ResourcesCompat.getDrawable(BrDialogWindow.this.mActivity.getResources(), R.drawable.br_list_inactive, null));
                                    }
                                    mSelectedButton = view;
                                    mClickedButton = 1;
                                    mListitemToSend = finalI;
                                    mSelectedButton.setBackground(ResourcesCompat.getDrawable(BrDialogWindow.this.mActivity.getResources(), R.drawable.br_list_active, null));
                                    mSelectedButton.startAnimation(AnimationUtils.loadAnimation(BrDialogWindow.this.mActivity, R.anim.button_click));
                                    return;
                                }
                                mClickedButton = 1;
                                ifOtherMethodClosed = true;
                                sendResponse(1);
                                mSelectedButton = null;
                            }
                        });
                    }
                    if (!z2) {
                        textView2.setText(Utils.transfromColors(strArr2[i6]));
                        linearLayout2.addView(textView2);
                        if(i6 == 0) {
                            mSelectedButton = textView2;
                            mClickedButton = 1;
                            mListitemToSend = i6;
                            mSelectedButton.setBackground(ResourcesCompat.getDrawable(BrDialogWindow.this.mActivity.getResources(), R.drawable.br_list_active, null));
                        }
                    }
                    i6++;
                    textView8 = textView;
                }
                textView8.setVisibility(View.GONE);
            } else if (str5 != null) {
                return;
            } else {
                return;
            }
        }
        this.mEditText.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View view, int i22, KeyEvent keyEvent) {
                if (i22 != 66) {
                    return false;
                };
                ((InputMethodManager) mActivity.getSystemService(Context.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
                mEditText.setFocusable(false);
                mEditText.setFocusableInTouchMode(true);
                return true;
            }
        });
        z2 = false;
        z3 = false;
        Utils.ShowLayout(pon, true);
    }

    public void sendResponse(int i) {
        try {
            NvEventQueueActivity.getInstance().sendDialogResponse(i, mCurrentDialogId, mListitemToSend, mEditText.getText().toString().getBytes("windows-1251"));
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        this.mClickedButton = -1;
        close();
    }

    public void close() {
        Utils.HideLayout(pon, true);
    }

    public void onHeightChanged(int height) {
    }
}
