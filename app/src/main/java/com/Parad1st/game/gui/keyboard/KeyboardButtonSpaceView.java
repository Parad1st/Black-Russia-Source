package com.byparad1st.game.gui.keyboard;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;

import androidx.appcompat.widget.AppCompatButton;

public class KeyboardButtonSpaceView extends AppCompatButton {
    int myColor = -7960438;

    public KeyboardButtonSpaceView(Context context) {
        super(context);
    }

    public KeyboardButtonSpaceView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
    }

    public KeyboardButtonSpaceView(Context context, AttributeSet attributeSet, int i) {
        super(context, attributeSet, i);
    }

    /* access modifiers changed from: protected */
    public void onDraw(Canvas canvas) {
        Path path = new Path();
        Paint paint = new Paint();
        int width = getWidth();
        int height = getHeight();
        path.reset();
        double d = (double) height;
        float f = (float) (0.25d * d);
        path.moveTo(0.0f, f);
        float f2 = (float) width;
        path.lineTo(f2, f);
        float f3 = (float) (d * 0.75d);
        path.lineTo(f2, f3);
        path.lineTo(0.0f, f3);
        path.close();
        paint.setColor(this.myColor);
        canvas.drawPath(path, paint);
        super.onDraw(canvas);
    }
}
