package com.byparad1st.game.gui.keyboard;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.view.View;

public class KeyboardButtonBackspaceView extends View {
    int myColor = -7960438;

    public KeyboardButtonBackspaceView(Context context) {
        super(context);
    }

    public KeyboardButtonBackspaceView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
    }

    public KeyboardButtonBackspaceView(Context context, AttributeSet attributeSet, int i) {
        super(context, attributeSet, i);
    }

    public KeyboardButtonBackspaceView(Context context, AttributeSet attributeSet, int i, int i2) {
        super(context, attributeSet, i, i2);
    }

    /* access modifiers changed from: protected */
    public void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Path path = new Path();
        Paint paint = new Paint();
        int width = getWidth();
        int height = getHeight();
        path.reset();
        double d = (double) width;
        double d2 = (double) height;
        path.moveTo((float) (0.35d * d), (float) (0.5d * d2));
        float f = (float) (0.45d * d);
        float f2 = (float) (0.3d * d2);
        path.lineTo(f, f2);
        float f3 = (float) (d2 * 0.7d);
        path.lineTo(f, f3);
        path.moveTo(f, f2);
        float f4 = (float) (d * 0.65d);
        path.lineTo(f4, f2);
        path.lineTo(f4, f3);
        path.lineTo(f, f3);
        path.close();
        paint.setColor(this.myColor);
        canvas.drawPath(path, paint);
    }
}
