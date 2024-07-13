package com.byparad1st.game.gui;

import android.content.Context;
import android.os.Build;
import android.os.CountDownTimer;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.ProgressBar;
import android.widget.TextView;
import androidx.core.content.ContextCompat;
import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

import java.io.UnsupportedEncodingException;
import java.util.LinkedList;
import org.json.JSONObject;

public class BrNotification {
    public static final int MAX_NOTIFICATIONS = 4;
    public static final int TYPE_BUTTON_TEXT_ORANGE = 5;
    public static final int TYPE_BUTTON_VECTOR_ORANGE = 4;
    public static final int TYPE_MONEY_GREEN = 1;
    public static final int TYPE_MONEY_RED = 0;
    public static final int TYPE_NEW_GUI_INTERACTIVE = 6;
    public static final int TYPE_TEXT_GREEN = 3;
    public static final int TYPE_TEXT_RED = 2;
    public static int mActiveNotifications = 0;
    public static boolean mHiddenAll = false;
    public static BrNotification[] mNotifications;
    public static LinkedList<BrNotification> mQueuedNotifications;
    private PopupWindow mWindow = null;
    private NvEventQueueActivity mActivity = NvEventQueueActivity.getInstance();
    private View mView = null;
    private int mId = -1;
    public int mSubid = -1;
    private int mDuration = -1;
    private CountDownTimer mTimer = null;
    private ProgressBar mProgressBar = null;

    public static BrNotification newInstance() {
        return new BrNotification();
    }

    public void show(JSONObject jSONObject) {
        if (mNotifications == null) {
            mQueuedNotifications = new LinkedList<>();
            mNotifications = new BrNotification[4];
            for (int i = 0; i < 4; i++) {
                mNotifications[i] = null;
            }
        }
        if (this.mWindow == null) {
            this.mView = ((LayoutInflater) mActivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE)).inflate(R.layout.br_notification, (ViewGroup) null, false);
            PopupWindow popupWindow = new PopupWindow(this.mView, -2, NvEventQueueActivity.dpToPx(50.0f, this.mActivity), true);
            this.mWindow = popupWindow;
            popupWindow.setAnimationStyle(R.style.PopupWindowAnimationNotification);
            this.mWindow.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
        }
        int optInt = jSONObject.optInt("t");
        String optString = jSONObject.optString("i");
        int optInt2 = jSONObject.optInt("d");
        String actionforBtn = jSONObject.optString("a");
        String optString2 = jSONObject.optString("k");
        if (optString2.equalsIgnoreCase("")) {
            optString2 = "Продолжить";
        }
        this.mDuration = optInt2;
        Button button = (Button) this.mView.findViewById(R.id.br_not_button);
        View findViewById = this.mView.findViewById(R.id.br_not_view);
        TextView textView = (TextView) this.mView.findViewById(R.id.br_not_ruble);
        TextView textView2 = (TextView) this.mView.findViewById(R.id.br_not_text);
        final ProgressBar progressBar = (ProgressBar) this.mView.findViewById(R.id.br_not_progress);
        FrameLayout frameLayout = (FrameLayout) this.mView.findViewById(R.id.br_not_fl);
        this.mProgressBar = progressBar;
        int i2 = this.mDuration;
        if (i2 != -1) {
            progressBar.setMax(i2 * 1000);
            progressBar.setProgress(this.mDuration * 1000);
        }
        switch (optInt) {
            case 0:
                button.setVisibility(View.GONE);
                textView.setVisibility(View.VISIBLE);
                findViewById.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.background_br_notification_red));
                break;
            case 1:
                button.setVisibility(View.GONE);
                textView.setVisibility(View.VISIBLE);
                findViewById.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.background_br_notification_green));
                break;
            case 2:
                button.setVisibility(View.GONE);
                textView.setVisibility(View.GONE);
                findViewById.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.background_br_notification_red));
                break;
            case 3:
                button.setVisibility(View.GONE);
                textView.setVisibility(View.GONE);
                findViewById.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.background_br_notification_green));
                break;
            case 4:
                button.setVisibility(View.VISIBLE);
                textView.setVisibility(View.GONE);
                findViewById.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.background_br_notification_orange));
                button.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.br_notify_button));
                break;
            case 5:
                button.setVisibility(View.VISIBLE);
                textView.setVisibility(View.GONE);
                findViewById.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.background_br_notification_orange));
                button.setBackground(ContextCompat.getDrawable(this.mActivity, R.drawable.br_notify_button));
                break;
            case 6:
                close(6);
                return;
        }
        if (optInt == 5 || optInt == 4) {
            button.setText(optString2);
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    view.startAnimation(AnimationUtils.loadAnimation(mActivity, R.anim.button_click));
                    try {
                        NvEventQueueActivity.getInstance().sendCommand((actionforBtn).getBytes("windows-1251"));
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    close(1);
                }
            });
        }
        ((LinearLayout) this.mView.findViewById(R.id.dw_root)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(BrNotification.this.mActivity, R.anim.button_click));
                BrNotification.this.mView.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        BrNotification.this.close(1);
                    }
                }, 100);
            }
        });
        if (optString != null) {
            textView2.setText(Utils.transfromColors(optString));
        }
        this.mWindow.setTouchable(true);
        this.mWindow.setFocusable(false);
        this.mWindow.setOutsideTouchable(false);
        mActiveNotifications++;
        int firstFreeSlot = getFirstFreeSlot();
        if (firstFreeSlot == -1) {
            mQueuedNotifications.push(this);
            return;
        }
        progressBar.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                progressBar.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                startCountdown();
            }
        });
        int yPosForNotification = getYPosForNotification(firstFreeSlot);
        mNotifications[firstFreeSlot] = this;
        this.mWindow.showAtLocation(mActivity.getmRootFrame(), 81, 0, yPosForNotification);
    }

    public void startCountdown() {
        CountDownTimer countDownTimer = this.mTimer;
        if (countDownTimer != null) {
            countDownTimer.cancel();
            this.mTimer = null;
        }
        if (this.mDuration != -1) {
            CountDownTimer r0 = new CountDownTimer((long) this.mProgressBar.getProgress(), 100) {
                @Override
                public void onTick(long j) {
                    if (Build.VERSION.SDK_INT >= 24) {
                        mProgressBar.setProgress((int) j, true);
                    } else {
                        mProgressBar.setProgress((int) j);
                    }
                }

                @Override
                public void onFinish() {
                    mProgressBar.setProgress(0);
                    close(1);
                }
            };
            this.mTimer = r0;
            r0.start();
        }
    }

    public static void closeNotificationById(int i) {
        if (mNotifications != null) {
            for (int i2 = 0; i2 < 4; i2++) {
                BrNotification[] brNotificationArr = mNotifications;
                if (brNotificationArr[i] != null && brNotificationArr[i].mSubid == i) {
                    brNotificationArr[i].close(2);
                    mNotifications[i] = null;
                }
            }
        }
    }

    public static void hideAllNotifications() {
        for (int i = 0; i < 4; i++) {
            BrNotification[] brNotificationArr = mNotifications;
            if (brNotificationArr[i] != null) {
                if (brNotificationArr[i].mTimer != null) {
                    brNotificationArr[i].mTimer.cancel();
                }
                mNotifications[i].mWindow.dismiss();
            }
        }
        mHiddenAll = true;
    }

    public static void resumeNotifications() {
        for (int i = 0; i < 4; i++) {
            BrNotification[] brNotificationArr = mNotifications;
            if (brNotificationArr[i] != null) {
                brNotificationArr[i].mWindow.showAtLocation(brNotificationArr[i].mActivity.getmRootFrame(), 81, 0, mNotifications[i].getYPosForNotification(i));
            }
        }
        for (int i2 = 0; i2 < 4; i2++) {
            BrNotification[] brNotificationArr2 = mNotifications;
            if (brNotificationArr2[i2] != null) {
                brNotificationArr2[i2].startCountdown();
            }
        }
    }

    public void close(int i) {
        CountDownTimer countDownTimer = this.mTimer;
        if (countDownTimer != null) {
            countDownTimer.cancel();
        }
        if (i != 6) {
            close((JSONObject) null);
        }
    }

    int getFirstFreeSlot() {
        int i = 0;
        while (true) {
            BrNotification[] brNotificationArr = mNotifications;
            if (i >= brNotificationArr.length) {
                return -1;
            }
            if (brNotificationArr[i] == null) {
                return i;
            }
            i++;
        }
    }

    int getYPosForNotification(int i) {
        return (NvEventQueueActivity.dpToPx(50.0f, this.mActivity) * i) + ((i + 1) * NvEventQueueActivity.dpToPx(10.0f, this.mActivity));
    }

    public void close(JSONObject jSONObject) {
        BrNotification[] brNotificationArr;
        PopupWindow popupWindow = this.mWindow;
        if (popupWindow != null) {
            popupWindow.dismiss();
        }
        int i = 0;
        while (true) {
            BrNotification[] brNotificationArr2 = mNotifications;
            if (i >= brNotificationArr2.length) {
                i = -1;
                break;
            } else if (brNotificationArr2[i] == this) {
                brNotificationArr2[i] = null;
                break;
            } else {
                i++;
            }
        }
        if (i != -1) {
            while (true) {
                brNotificationArr = mNotifications;
                if (i >= brNotificationArr.length - 1) {
                    break;
                }
                int i2 = i + 1;
                brNotificationArr[i] = brNotificationArr[i2];
                i = i2;
            }
            brNotificationArr[brNotificationArr.length - 1] = null;
            if (mQueuedNotifications.size() > 0) {
                BrNotification[] brNotificationArr3 = mNotifications;
                brNotificationArr3[brNotificationArr3.length - 1] = mQueuedNotifications.getFirst();
                mQueuedNotifications.removeFirst();
                BrNotification[] brNotificationArr4 = mNotifications;
                BrNotification brNotification = brNotificationArr4[brNotificationArr4.length - 1];
                brNotification.mWindow.showAtLocation(brNotification.mActivity.getmRootFrame(), 81, 0, brNotification.getYPosForNotification(mNotifications.length - 1));
                brNotification.startCountdown();
            }
        }
        int i3 = 0;
        while (true) {
            BrNotification[] brNotificationArr5 = mNotifications;
            if (i3 < brNotificationArr5.length) {
                if (brNotificationArr5[i3] != null) {
                    brNotificationArr5[i3].mWindow.update(0, getYPosForNotification(i3), -1, -1);
                }
                i3++;
            } else {
                mActiveNotifications--;
                return;
            }
        }
    }

}