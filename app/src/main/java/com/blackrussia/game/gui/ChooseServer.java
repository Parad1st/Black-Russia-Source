package com.blackrussia.game.gui;

import android.animation.Animator;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.text.Html;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.core.content.res.ResourcesCompat;

import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;
import com.blackrussia.game.R;
import com.blackrussia.game.gui.util.Utils;
import com.blackrussia.launcher.model.Servers;
import com.blackrussia.launcher.other.Lists;
import com.nvidia.devtech.NvEventQueueActivity;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Formatter;

public class ChooseServer {

    FrameLayout serverLayout;
    LinearLayout chooseServerLayout;
    LinearLayout loadingLayout;
    ArrayList<Servers> mServers;
    TextView percentText;
    Activity aactivity;
    int type, size;
    TextView mProgress;
    LinearLayout linearLayout2;
    View findViewById, findViewById2;
    Button myButton, allButton;

    public ChooseServer(Activity activity){
        aactivity = activity;
        mServers = Lists    .slist;
        serverLayout = activity.findViewById(R.id.br_serverselect_layout);
        percentText = activity.findViewById(R.id.br_ls_progress);
        chooseServerLayout = activity.findViewById(R.id.choose_server_root_choose);
        loadingLayout = activity.findViewById(R.id.choose_server_root_loading);
        type = NvEventQueueActivity.getInstance().getLastServer();
        findViewById = aactivity.findViewById(R.id.main_servers_choose);
        findViewById2 = aactivity.findViewById(R.id.list_servers_choose);
        myButton = aactivity.findViewById(R.id.servers_btn_my_server);
        allButton = aactivity.findViewById(R.id.all_servers_button);
        Utils.HideLayout(serverLayout, false);
    }

    public void Update(int percent) {
        if (percent <= 100)
            percentText.setText(new Formatter().format("%d%s", Integer.valueOf(percent), "%").toString());
        else {
            chooseServerLayout.setVisibility(View.VISIBLE);
            chooseServerLayout.setAlpha(0.0f);
            chooseServerLayout.animate().setDuration(1500).alpha(1.0f).setListener(new Animator.AnimatorListener() {
                @Override
                public void onAnimationCancel(Animator animator) {
                }

                @Override
                public void onAnimationRepeat(Animator animator) {
                }

                @Override
                public void onAnimationStart(Animator animator) {
                    initUi();
                }

                @Override
                public void onAnimationEnd(Animator animator) {
                    loadingLayout.setVisibility(View.GONE);
                }
            }).start();
        }
    }

    public void initUi() {
        if (type != -1) {
            ((TextView) aactivity.findViewById(R.id.br_server_name)).setText(mServers.get(type).getname());
            View findViewById = aactivity.findViewById(R.id.server_list_back_color);
            RoundCornerProgressBar roundCornerProgressBarr = (RoundCornerProgressBar) aactivity.findViewById(R.id.br_server_progress);
            Drawable backgrounda = findViewById.getBackground();
            backgrounda.setColorFilter(Color.parseColor("#" + mServers.get(type).getColor()), PorterDuff.Mode.SRC_ATOP);
            roundCornerProgressBarr.setProgressColor(Color.parseColor("#" + mServers.get(type).getColor()));
            roundCornerProgressBarr.setProgress((float) ((int) ((Double.parseDouble(String.valueOf(mServers.get(type).getOnline())) / Double.parseDouble(String.valueOf(mServers.get(type).getmaxOnline()))) * 100.0d)));
            ((ImageView) aactivity.findViewById(R.id.br_server_image)).setColorFilter(Color.parseColor("#" + mServers.get(type).getColor()), PorterDuff.Mode.SRC_ATOP);
            ((TextView) aactivity.findViewById(R.id.br_server_online)).setText(Html.fromHtml(mServers.get(type).getOnline() + "<font color='#808080'>/" + mServers.get(type).getmaxOnline()));
        }
        if (type != -1) {
            findViewById2.setAlpha(0.0f);
            findViewById2.setVisibility(View.GONE);
            findViewById.setVisibility(View.VISIBLE);
        } else {
            findViewById.setAlpha(0.0f);
            findViewById.setVisibility(View.GONE);
            findViewById2.setVisibility(View.VISIBLE);
            myButton.setVisibility(View.GONE);
            allButton.setVisibility(View.GONE);
        }
        LinearLayout linearLayout = aactivity.findViewById(R.id.scroll_layout_servers);
        linearLayout.setScrollbarFadingEnabled(false);
        size = (mServers.size() / 4 ) + 1;
        mProgress = aactivity.findViewById(R.id.br_ls_progress);
        String str = "fsdf";
        aactivity.findViewById(R.id.server_main_frame).setOnClickListener(view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
        });
        aactivity.findViewById(R.id.br_servers_play).setOnClickListener(view -> {
            try {
                NvEventQueueActivity.getInstance().sendRPC(2, str.getBytes("windows-1251"), type);
                Utils.HideLayout(serverLayout, true);
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        });
        int size = (mServers.size() / 4) + 1;

        myButton.setOnClickListener(view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            view.setBackground(ResourcesCompat.getDrawable(aactivity.getResources(), R.drawable.button_red_rectangle, null));
            allButton.setBackground(ResourcesCompat.getDrawable(aactivity.getResources(), R.drawable.button_br_red_unfilled_ss, null));
            if (findViewById.getVisibility() != View.VISIBLE) {
                findViewById2.animate().alpha(0.0f).setDuration(100).setListener(new Animator.AnimatorListener() {
                    @Override
                    public void onAnimationStart(Animator animator) {

                    }
                    @Override
                    public void onAnimationEnd(Animator animator) {
                        findViewById2.setAlpha(0.0f);
                        findViewById2.setVisibility(View.GONE);
                        findViewById.setVisibility(View.VISIBLE);
                        findViewById.setAlpha(0.0f);
                        findViewById.animate().alpha(1.0f).setDuration(100).setListener(null).start();
                    }
                    @Override
                    public void onAnimationCancel(Animator animator) {

                    }
                    @Override
                    public void onAnimationRepeat(Animator animator) {

                    }
                });
            }
        });
        allButton.setOnClickListener(view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            view.setBackground(ResourcesCompat.getDrawable(aactivity.getResources(), R.drawable.button_red_rectangle, null));
            myButton.setBackground(ResourcesCompat.getDrawable(aactivity.getResources(), R.drawable.button_br_red_unfilled_ss, null));
            if (findViewById2.getVisibility() != View.VISIBLE) {
                findViewById.animate().alpha(0.0f).setDuration(100).setListener(new Animator.AnimatorListener() {
                    @Override
                    public void onAnimationStart(Animator animator) {

                    }
                    @Override
                    public void onAnimationEnd(Animator animator) {
                        findViewById.setAlpha(0.0f);
                        findViewById.setVisibility(View.GONE);
                        findViewById2.setVisibility(View.VISIBLE);
                        findViewById2.setAlpha(0.0f);
                        findViewById2.animate().alpha(1.0f).setDuration(100).setListener(null).start();
                    }
                    @Override
                    public void onAnimationCancel(Animator animator) {

                    }
                    @Override
                    public void onAnimationRepeat(Animator animator) {

                    }
                });
            }
        });
        int i4 = 0;
        while (i4 < size) {
            linearLayout2 = new LinearLayout(aactivity);
            LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            layoutParams.weight = 1 / size;
            if (i4 < size - 1) {
                layoutParams.setMargins(0 ,0, 0 , NvEventQueueActivity.dpToPx(12.0f, aactivity));
            }
            linearLayout2.setLayoutParams(layoutParams);
            linearLayout2.setOrientation(LinearLayout.HORIZONTAL);
            int i5 = 0;
            while (i5 < 4) {
                int i6 = (i4 * 4) + i5;
                if (i6 >= mServers.size()) {
                    break;
                }
                final int size2 = (mServers.size() - i6) - 1;
                View inflate = ((LayoutInflater) aactivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE)).inflate(R.layout.br_serverselect_server, null, false);
                inflate.setOnClickListener(view -> {
                    view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    try {
                        NvEventQueueActivity.getInstance().sendRPC(2, str.getBytes("windows-1251"), size2);
                        Utils.HideLayout(serverLayout, true);
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                });
                ((TextView) inflate.findViewById(R.id.br_server_name)).setText(mServers.get(size2).getname());
                LinearLayout.LayoutParams layoutParams2 = new LinearLayout.LayoutParams(NvEventQueueActivity.dpToPx(160.0f, aactivity), NvEventQueueActivity.dpToPx(80.0f, aactivity));
                layoutParams2.weight = 0.25f;
                if (i4 < 5) {
                    layoutParams2.setMargins(0, 0, NvEventQueueActivity.dpToPx(20.0f, aactivity), 0);
                }
                inflate.setLayoutParams(layoutParams2);
                View findViewById3 = inflate.findViewById(R.id.server_list_back_color);
                RoundCornerProgressBar roundCornerProgressBar = inflate.findViewById(R.id.br_server_progress);
                ImageView imageView = inflate.findViewById(R.id.br_server_image);
                Drawable background = findViewById3.getBackground();
                background.setColorFilter(Color.parseColor("#" + mServers.get(size2).getColor()), PorterDuff.Mode.SRC_ATOP);
                roundCornerProgressBar.setProgressColor(Color.parseColor("#" + mServers.get(size2).getColor()));
                roundCornerProgressBar.setProgress((float) ((int) ((Double.parseDouble(String.valueOf(mServers.get(size2).getOnline())) / Double.parseDouble(String.valueOf(mServers.get(size2).getmaxOnline()))) * 100.0d)));
                imageView.setColorFilter(Color.parseColor("#" + mServers.get(size2).getColor()), PorterDuff.Mode.SRC_ATOP);
                ((TextView) inflate.findViewById(R.id.br_server_online)).setText(Html.fromHtml(mServers.get(size2).getOnline() + "<font color='#808080'>/" + mServers.get(size2).getmaxOnline()));
                linearLayout2.addView(inflate);
                i5++;
            }
            i4++;
        }
        linearLayout.addView(linearLayout2);
    }

    public void Show() {
        Utils.ShowLayout(serverLayout, false);
    }
}