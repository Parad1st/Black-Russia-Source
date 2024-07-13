package com.byparad1st.game.gui.util;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;

public class StrokedTextView extends androidx.appcompat.widget.AppCompatTextView {

	private static final int DEFAULT_STROKE_WIDTH = 0;

    public StrokedTextView(Context context) {
        this(context, null, 0);
    }

	public StrokedTextView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public StrokedTextView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}


		// fields

	// overridden methods
	@Override
	protected void onDraw(Canvas canvas) {
			//set paint to fill mode
			Paint p = getPaint();
			p.setStyle(Paint.Style.FILL);
			//draw the fill part of text
			super.onDraw(canvas);
			//save the text color
		//	int currentTextColor = getCurrentTextColor();
			//set paint to stroke mode and specify
			//stroke color and width
			p.setStyle(Paint.Style.STROKE);
			p.setStrokeWidth(4f);
			setTextColor(Color.rgb(0, 0, 0));
			//draw text stroke
			super.onDraw(canvas);
			//revert the color back to the one
			//initially specified
			//setTextColor(currentTextColor);
	}

	/**
	 * Convenience method to convert density independent pixel(dp) value
	 * into device display specific pixel value.
	 * @param context Context to access device specific display metrics
	 * @param dp density independent pixel value
	 * @return device specific pixel value.
	 */
	public static int dpToPx(Context context, float dp)
	{
		final float scale= context.getResources().getDisplayMetrics().density;
		return (int) (dp * scale + 0.5f);
	}
}