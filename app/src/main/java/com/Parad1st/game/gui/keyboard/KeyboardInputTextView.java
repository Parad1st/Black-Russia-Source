package com.byparad1st.game.gui.keyboard;

import android.content.Context;
import android.util.AttributeSet;

import androidx.appcompat.widget.AppCompatEditText;

public class KeyboardInputTextView extends AppCompatEditText {
    public boolean canPaste() {
        return false;
    }

    public boolean isSuggestionsEnabled() {
        return false;
    }

    public KeyboardInputTextView(Context context) {
        super(context);
    }

    public KeyboardInputTextView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
    }

    public KeyboardInputTextView(Context context, AttributeSet attributeSet, int i) {
        super(context, attributeSet, i);
    }
}
