package com.byparad1st.game.gui;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.widget.*;
import android.view.View;
import android.os.Handler;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.byparad1st.game.gui.util.SeekArc;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.Formatter;

/*CODE POVOROTNIKI BY PARAD1ST*/

public class Speedometer {
    public Activity activity;
    public TextView mCarHP;
    public ImageView mEngine;
    public TextView mFuel;
    public ConstraintLayout mInputLayout;
    public ImageView mLock;
    public TextView mMileage;
    public TextView mSpeed;
    public SeekArc mSpeedLine;

    //-----------------------------------
    public ImageView blinker, blink;
    public  ImageView speedo_arrow_left;
    public ImageView speedo_arrow;
    private Handler handler;
    private boolean isBlink = false;
    private boolean isLeft = false;
    private boolean isRight = false;
    //-----------------------------------


    public Speedometer(Activity activity){
        this.activity = activity;
        ConstraintLayout relativeLayout = activity.findViewById(R.id.speedometer);
        mInputLayout = relativeLayout;
        mSpeed = activity.findViewById(R.id.speed_text);
        mFuel = activity.findViewById(R.id.speed_fuel_text);
        mCarHP = activity.findViewById(R.id.speed_car_hp_text);
        mMileage = activity.findViewById(R.id.textView2);
        mSpeedLine = activity.findViewById(R.id.speed_line);
        mEngine = activity.findViewById(R.id.speed_engine_ico);
        mLock = activity.findViewById(R.id.speed_lock_ico);

        speedo_arrow_left = activity.findViewById(R.id.povoro_left);
        speedo_arrow = activity.findViewById(R.id.povoro_right);
        blinker = activity.findViewById(R.id.blinker);
        blink = activity.findViewById(R.id.blink);
        handler = new Handler();

        //BY VISKI
        blink.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                NvEventQueueActivity.getInstance().showNotification(2, "Поворотники находяться в разроботке!", 4, "", "");
                if (!isBlink) {
                    startPovoro_Blink();
                    isBlink = true;
                } else {
                    stopPovoro_Blink();
                    isBlink = false;
                }
            }
        });
        blinker.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                NvEventQueueActivity.getInstance().showNotification(2, "Поворотники находяться в разроботке!", 4, "", "");
                if (!isBlink) {
                    startPovoro_Blink();
                    isBlink = true;
                } else {
                    stopPovoro_Blink();
                    isBlink = false;
                }
            }
        });
        speedo_arrow.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                NvEventQueueActivity.getInstance().showNotification(2, "Поворотники находяться в разроботке!", 4, "", "");
                if (!isRight) {
                    startPovoro_Right();
                    isRight = true;
                } else {
                    stopPovoro_Right();
                    isRight = false;
                }
            }
        });
        speedo_arrow_left.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                NvEventQueueActivity.getInstance().showNotification(2, "Поворотники находяться в разроботке!", 4, "", "");
                if (!isLeft) {
                    startPovoro_Left();
                    isLeft = true;
                } else {
                    stopPovoro_Left();
                    isLeft = false;
                }
            }
        });
        Utils.HideLayout(relativeLayout, false);
    }

    //
    //TODO: by parad1st
    //

    private void startPovoro_Blink() {
        if (handler != null) {
            handler.postDelayed(povoro_blink, 0);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_right);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_left);
        }
    }
    private void stopPovoro_Blink() {
        if (handler != null) {
            handler.removeCallbacks(povoro_blink);
            blinker.setImageResource(R.drawable.speedo_blinker);
            blink.setImageResource(R.drawable.speedo_center);
            speedo_arrow.setImageResource(R.drawable.speedo_arrow);
            speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_left);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_right);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_left);
        }
    }
    private void startPovoro_Left() {
        if (handler != null) {
            handler.postDelayed(povoro_left, 0);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_blink);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_right);
        }
        //BY VISKI
    }
    private void stopPovoro_Left() {
        if (handler != null) {
            handler.removeCallbacks(povoro_left);
            blinker.setImageResource(R.drawable.speedo_blinker);
            blink.setImageResource(R.drawable.speedo_center);
            speedo_arrow.setImageResource(R.drawable.speedo_arrow);
            speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_left);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_blink);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_right);
        }
        //BY VISKI
    }
    private void startPovoro_Right() {
        if (handler != null) {
            handler.postDelayed(povoro_right, 0);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_blink);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_left);
        }
    }
    private void stopPovoro_Right() {
        if (handler != null) {
            handler.removeCallbacks(povoro_right);
            blinker.setImageResource(R.drawable.speedo_blinker);
            blink.setImageResource(R.drawable.speedo_center);
            speedo_arrow.setImageResource(R.drawable.speedo_arrow);
            speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_left);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_blink);
        }
        if (handler != null) {
            handler.removeCallbacks(povoro_left);
        }
    }
    private Runnable povoro_blink = new Runnable() {
        @Override
        public void run() {
            if (isBlink) {
                blinker.setImageResource(R.drawable.speedo_ablinker);
                blink.setImageResource(R.drawable.speedo_blink);
                speedo_arrow.setImageResource(R.drawable.speedo_arrowa);
                speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_active);
            } else {
                blinker.setImageResource(R.drawable.speedo_blinker);
                blink.setImageResource(R.drawable.speedo_center);
                speedo_arrow.setImageResource(R.drawable.speedo_arrow);
                speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_left);
            }
            isBlink = !isBlink;
            if (handler != null) {
                handler.postDelayed(this, 400);
            }
            //BY VISKI
        }
    };
    private Runnable povoro_left = new Runnable() {
        @Override
        public void run() {
            if (isLeft) {
                speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_active);
            } else {
                blinker.setImageResource(R.drawable.speedo_blinker);
                blink.setImageResource(R.drawable.speedo_center);
                speedo_arrow.setImageResource(R.drawable.speedo_arrow);
                speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_left);
            }
            isLeft = !isLeft;
            if (handler != null) {
                handler.postDelayed(this, 400);
            }
        }
    };
    private Runnable povoro_right = new Runnable() {
        @Override
        public void run() {
            if (isRight) {
                speedo_arrow.setImageResource(R.drawable.speedo_arrowa);
            } else {
                blinker.setImageResource(R.drawable.speedo_blinker);
                blink.setImageResource(R.drawable.speedo_center);
                speedo_arrow.setImageResource(R.drawable.speedo_arrow);
                speedo_arrow_left.setImageResource(R.drawable.speedo_arrow_left);
            }
            isRight = !isRight;
            if (handler != null) {
                handler.postDelayed(this, 400);
            }
        }
    };

    /**
     * byparad1st
     */

    public void UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock){
        hp= (int) hp/10;
        mFuel.setText(new Formatter().format("%d", Integer.valueOf(fuel)).toString());
        mMileage.setText(new Formatter().format("%06d", Integer.valueOf(mileage)).toString());
        mCarHP.setText(new Formatter().format("%d%s", Integer.valueOf(hp), "%").toString());
        mSpeedLine.setProgress(speed);
        mSpeed.setText(String.valueOf(speed));
        if (speed == 0) {
            this.mSpeed.setAlpha(0.4f);
            this.mSpeed.setText("000");
            this.mSpeed.setTextColor(Color.parseColor("#39FFFFFF"));
        }
        if (fuel == 0) {
            this.mFuel.setText("0");
            this.mFuel.setTextColor(Color.parseColor("#39FFFFFF"));
        }
        if (fuel != 0) {
            String speedFuel;
            //ТУТ ТОЖЕ BY VISKI
            if (speed < 10) {
                speedFuel = String.format("%03d", fuel);
            } else if (speed < 100) {
                speedFuel = String.format("%03d", fuel);
            } else {
                speedFuel = String.valueOf(fuel);
            }
            this.mFuel.setText(speedFuel);
            this.mFuel.setTextColor(Color.parseColor("#FFFFFF"));
        }
        if (speed != 0) {
            this.mSpeed.setAlpha(1.0f);
            String speedText;
            //ТУТ ТОЖЕ BY VISKI
            if (speed < 10) {
                speedText = String.format("%03d", speed);
            } else if (speed < 100) {
                speedText = String.format("%03d", speed);
            } else {
                speedText = String.valueOf(speed);
            }
            this.mSpeed.setText(speedText);
            this.mSpeed.setTextColor(Color.parseColor("#FFFFFF"));
        }
        if(engine == 1)
            mEngine.setColorFilter(Color.parseColor("#00FF00"), PorterDuff.Mode.SRC_IN);
        else
            mEngine.setColorFilter(Color.parseColor("#FF0000"), PorterDuff.Mode.SRC_IN);
        if(lock == 1)
            mLock.setColorFilter(Color.parseColor("#00FF00"), PorterDuff.Mode.SRC_IN);
        else
            mLock.setColorFilter(Color.parseColor("#FF0000"), PorterDuff.Mode.SRC_IN);
    }

    public void ShowSpeed() {
        Utils.ShowLayout(mInputLayout, false);
    }
    public void HideSpeed() {
        Utils.HideLayout(mInputLayout, false);
    }
}