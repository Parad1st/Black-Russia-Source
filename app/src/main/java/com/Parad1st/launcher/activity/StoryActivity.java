package com.byparad1st.launcher.activity;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;

import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;
import com.byparad1st.game.R;
import com.byparad1st.launcher.adapter.SliderStoriesAdapter;
import com.byparad1st.launcher.other.Lists;
import com.smarteist.autoimageslider.SliderAnimations;
import com.smarteist.autoimageslider.SliderView;

public class StoryActivity extends AppCompatActivity {

    private RoundCornerProgressBar progressStory;
    private SliderStoriesAdapter sliderStoriesAdapter;
    private SliderView sliderView;
    private CountDownTimer countDownTimer;
    private long progress;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_story);
        this.sliderView = (SliderView) findViewById(R.id.sliderView);
        this.progressStory = (RoundCornerProgressBar) findViewById(R.id.progressStory);
        ((ImageView) findViewById(R.id.closeStory)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                StoryActivity.this.lambda$onCreate$0$StoryActivity(view);
            }
        });
        SliderStoriesAdapter sliderStoriesAdapter = new SliderStoriesAdapter(this);
        this.sliderStoriesAdapter = sliderStoriesAdapter;
        this.sliderView.setSliderAdapter(sliderStoriesAdapter);
        this.sliderView.setSliderTransformAnimation(SliderAnimations.SIMPLETRANSFORMATION);
        this.sliderStoriesAdapter.addItems(Lists.nlist);
        this.sliderView.setCurrentPageListener(i -> startTimer());
        int intExtra = getIntent().getIntExtra("position", 0);
        this.sliderView.getSliderPager().setCurrentItem(intExtra, false);
        this.sliderView.getPagerIndicator().setSelection(intExtra);
        this.progressStory.setProgress(0.0f);
        startTimer();
    }

    public void lambda$onCreate$0$StoryActivity(View view) {
        view.startAnimation(AnimationUtils.loadAnimation(this, R.anim.button_click));
        new Handler().postDelayed(() -> closeStory(), 200);
    }

    public void closeStory() {
        countDownTimer.cancel();
        countDownTimer = null;
        progress = 0;
        startActivity(new Intent(getApplicationContext(), MainActivity.class));
    }

    private void startTimer() {
        if (countDownTimer != null) {
            countDownTimer.cancel();
            progress = 0;
            countDownTimer = null;
        }
        countDownTimer = new CountDownTimer(5000, 1) {
            @Override
            public void onTick(long j) {
                progress = 5000-j;
                progressStory.setProgress(progress);
            }
            @Override
            public void onFinish() {
                if(sliderView.getCurrentPagePosition() + 1 == sliderStoriesAdapter.getCount())
                    closeStory();
                else
                sliderView.setCurrentPagePosition(sliderView.getCurrentPagePosition() + 1);
            }
        }.start();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        countDownTimer.cancel();
        progress = 0;
        countDownTimer = null;
    }
}