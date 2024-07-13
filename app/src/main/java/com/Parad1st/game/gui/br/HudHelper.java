package com.byparad1st.game.gui.br;

import android.app.Activity;

import android.text.Html;
import android.widget.TextView;
import androidx.constraintlayout.widget.ConstraintLayout;
import android.view.View;

import com.byparad1st.game.R;

public class HudHelper {
    public Activity activity;
    public TextView helpertext;
    public ConstraintLayout main_layout;

    public HudHelper(Activity aactivity) {
        activity = aactivity;
        main_layout = aactivity.findViewById(R.id.helper_main_layout);
        main_layout.setVisibility(View.GONE);
        helpertext = aactivity.findViewById(R.id.hint_text_instructor);
        helpertext.setOnClickListener( view -> {
            main_layout.setVisibility(View.GONE);
            helpertext.setText("");
        });
    }

    public void ShowHelper(int type)
    {
        main_layout.setVisibility(View.VISIBLE);
        String text = "";
        switch (type) {
            case 1:
                text = "Вы находитесь в списке очереди на подключение к серверу. Чтобы скоротать время <font color='#56D12A'>исследуйте карту</font> и <font color='#ffffff'>новые места.</font>";
                break;
            case 2:
                text = "Проект <font color='#56D12A'>LUX RUSSIA";
                break;
            default:
                break;
        }
        helpertext.setText(Html.fromHtml(text));
    }

    public void HideHelper()
    {
        main_layout.setVisibility(View.GONE);
        helpertext.setText("");
    }
}
