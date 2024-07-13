package com.nvidia.devtech;

import android.app.Activity;
import android.content.Context;
import android.text.Editable;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.byparad1st.game.R;

import java.util.ArrayList;

public class InputManager {

    public interface InputListener
    {
        public void OnInputEnd(String str);
    }

    private LinearLayout mInputLayout = null;
    private EditText mInputEt = null;

    private Button mButtonSlash = null;
    private Button mButtonHistoryNext = null;
    private Button mButtonHistoryPrev = null;

    private Activity mContext = null;
    private boolean mIsShowing = false;

    private String mSavedInput = null;

    private final int mMaxHistory = 20;
    private ArrayList<String> mInputHistory = null;
    private int mCurrentHistoryMessage = 0;

    private HeightProvider mHeightProvider = null;

    private Runnable mAnimTask = null;

    public boolean IsShowing()
    {
        return mIsShowing;
    }

    public InputManager(Activity act)
    {
        mContext = act;

        mInputLayout = mContext.findViewById(R.id.main_input);
        mInputEt = mContext.findViewById(R.id.main_et_input);
        mButtonSlash = mContext.findViewById(R.id.main_but_input_slash);
        mButtonHistoryPrev = mContext.findViewById(R.id.main_but_input_prev);
        mButtonHistoryNext = mContext.findViewById(R.id.main_but_input_next);

        mInputHistory = new ArrayList<String>();

        mButtonSlash.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mInputEt.getText().insert(mInputEt.getSelectionStart(), "/");
            }
        });

        mButtonHistoryPrev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mCurrentHistoryMessage--;
                if(mCurrentHistoryMessage < 0)
                {
                    mCurrentHistoryMessage = 0;
                }
                if(mCurrentHistoryMessage <= 0)
                {
                    mInputEt.setText("");
                    return;
                }

                mInputEt.setText(mInputHistory.get(mCurrentHistoryMessage - 1));
                mInputEt.setSelection(mInputEt.getText().length());
            }
        });

        mButtonHistoryNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mCurrentHistoryMessage++;
                if((mCurrentHistoryMessage - 1) >= mInputHistory.size())
                {
                    mCurrentHistoryMessage--;
                }
                if(mCurrentHistoryMessage <= 0)
                {
                    return;
                }

                mInputEt.setText(mInputHistory.get(mCurrentHistoryMessage - 1));
                mInputEt.setSelection(mInputEt.getText().length());
            }
        });

        mInputEt.setOnEditorActionListener(new EditText.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent)
            {
                if(i== EditorInfo.IME_ACTION_DONE || i == EditorInfo.IME_ACTION_NEXT)
                {
                    Editable editableText = mInputEt.getText();
                    if(editableText != null)
                    {
                        String str = editableText.toString();

                        mInputEt.setText("");

                        OnInputEnd(str);
                    }

                }
                return false;
            }
        });

        HideInputLayout();
    }

    public void onHeightChanged(int height)
    {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) mInputLayout.getLayoutParams();
        params.setMargins(0, 0 , 0, height);
        mInputLayout.setLayoutParams(params);
    }

    public void ShowInputLayout()
    {
        mIsShowing = true;

        mInputLayout.setVisibility(View.INVISIBLE);

        mInputEt.requestFocus();
        InputMethodManager imm = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(mInputEt, InputMethodManager.SHOW_IMPLICIT);

        if(mAnimTask != null)
        {
            mInputLayout.removeCallbacks(mAnimTask);
            mAnimTask = null;
        }

        mAnimTask = new Runnable() {
            @Override
            public void run() {
                mInputLayout.setVisibility(View.VISIBLE);
                mInputEt.requestFocus();
            }
        };

        mInputLayout.postDelayed(mAnimTask, 60);

        mCurrentHistoryMessage = 0;

        if(mSavedInput != null)
        {
            mInputEt.setText(mSavedInput);
            mInputEt.setSelection(mInputEt.getText().length());
        }
    }

    public void HideInputLayout()
    {

        mCurrentHistoryMessage = 0;

        if(mInputEt.getEditableText() != null)
        {
            mSavedInput = mInputEt.getEditableText().toString();
        }

        if(mAnimTask != null)
        {
            mInputLayout.removeCallbacks(mAnimTask);
            mAnimTask = null;
        }

        if(mContext.getCurrentFocus() != null)
        {
            InputMethodManager inputMethodManager = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
            inputMethodManager.hideSoftInputFromWindow(mContext.getCurrentFocus().getWindowToken(), 0);
        }
        mInputLayout.setVisibility(View.GONE);

        mIsShowing = false;
    }

    private void OnInputEnd(String str)
    {
        if(mInputHistory.size() >= mMaxHistory)
        {
            mInputHistory.remove(mInputHistory.size() - 1);
        }
        mInputHistory.add(0, str);

        InputListener listener = (InputListener)mContext;

        listener.OnInputEnd(str);
    }
}
