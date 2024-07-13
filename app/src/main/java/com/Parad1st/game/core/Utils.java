package com.byparad1st.game.core;

import android.os.Bundle;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;

import static com.byparad1st.game.core.Config.GAME_PATH;

public class Utils extends AppCompatActivity {
    
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
    }

    public static boolean getGameFile(String path)
    {
        File file = new File(GAME_PATH+path);
        return file.exists();
    }
    
    public static int getTextLength(TextView textView) {
        TextPaint textPaint = new TextPaint();
        textPaint.setTextSize(textView.getTextSize());
        textPaint.setTypeface(textView.getTypeface());
        return (int) textPaint.measureText(textView.getText().toString());
    }
    
    public static void makeAllViewsVisible(ViewGroup viewGroup) {
        viewGroup.setVisibility(View.VISIBLE);
        for (int i = 0; i < viewGroup.getChildCount(); i++) {
            if (viewGroup.getChildAt(i) instanceof ViewGroup) {
                makeAllViewsVisible((ViewGroup) viewGroup.getChildAt(i));
            } else {
                viewGroup.getChildAt(i).setVisibility(View.VISIBLE);
            }
        }
    }

    public static void changeTextViewWidth(TextView textView) {
        textView.post(new Runnable() {
            @Override
            public void run() {
                TextPaint textPaint = new TextPaint();
                textPaint.setTextSize(textView.getTextSize());
                textPaint.setTypeface(textView.getTypeface());
                textPaint.measureText(textView.getText().toString());
                StaticLayout staticLayout = new StaticLayout(textView.getText(), textPaint, 10000, Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false);
                int lineCount = staticLayout.getLineCount();
                float lineWidth = staticLayout.getLineWidth(0);
                for (int i = 0; i < lineCount; i++) {
                    if (staticLayout.getLineWidth(i) >= lineWidth) {
                        lineWidth = staticLayout.getLineWidth(i) + 5.0f;
                    }
                }
                ViewGroup.LayoutParams layoutParams = textView.getLayoutParams();
                layoutParams.width = (int) lineWidth;
                textView.setLayoutParams(layoutParams);
            }
        });
    }

    static int installgametype = 0;

    public static int getType(){ return installgametype;}
    public static int setType(int value){return installgametype = value;}
}
