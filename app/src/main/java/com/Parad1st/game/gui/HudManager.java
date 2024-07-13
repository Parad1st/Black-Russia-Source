package com.byparad1st.game.gui;

import android.app.Activity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import com.triggertrap.seekarc.SeekArc;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.nvidia.devtech.NvEventQueueActivity;
import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.ArrayList;
import java.util.Formatter;

import android.graphics.Color;
import android.text.Spanned;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import org.jetbrains.annotations.NotNull;

import java.util.List;

public class HudManager {
    public Activity activity;
    public ConstraintLayout hud_layout;

    private final NvEventQueueActivity mContext = null;

    public TextView hud_money;

    public ImageView hud_weapon, hud_radar, hud_x2, hud_menu, hud_micro, hud_gps, hud_zona, hud_donate, hud_yved, hud_quest;

    public ArrayList<ImageView> hud_wanted;

    public SeekArc progressHP, progressArmor;
    private RecyclerView chat;
    public int keybo = 0;
    public String colorrr = "ffffff";
    ChatAdapter adapter;
    ArrayList<Spanned> chat_lines = new ArrayList<Spanned>();
    public FrameLayout chat2;

    public HudManager(Activity aactivity) {
        activity = aactivity;
        hud_layout = aactivity.findViewById(R.id.hud_main);
        hud_layout.setVisibility(View.GONE);

        chat = aactivity.findViewById(R.id.chat);
        chat.setVisibility(View.GONE);
        chat2 = aactivity.findViewById(R.id.chat_line);
        chat.smoothScrollToPosition(0);
        starttt();

        progressArmor = aactivity.findViewById(R.id.progress_armor);
        progressHP = aactivity.findViewById(R.id.progress_hp);

        hud_radar = aactivity.findViewById(R.id.radar_hude);

        hud_micro = aactivity.findViewById(R.id.imageView14);

        hud_gps = aactivity.findViewById(R.id.imageView16);
        hud_zona = aactivity.findViewById(R.id.grzona);
        hud_x2 = aactivity.findViewById(R.id.imageView17);

        hud_money = aactivity.findViewById(R.id.hud_money);
        hud_weapon = aactivity.findViewById(R.id.hud_weapon);

        hud_menu = aactivity.findViewById(R.id.hud_menu);
        hud_donate = aactivity.findViewById(R.id.hud_BP);
        hud_yved = aactivity.findViewById(R.id.hud_maga);
        hud_quest = aactivity.findViewById(R.id.hud_yvedomlenia);

        hud_wanted = new ArrayList<>();
        hud_wanted.add(activity.findViewById(R.id.hud_star_1));
        hud_wanted.add(activity.findViewById(R.id.hud_star_2));
        hud_wanted.add(activity.findViewById(R.id.hud_star_3));
        hud_wanted.add(activity.findViewById(R.id.hud_star_4));
        hud_wanted.add(activity.findViewById(R.id.hud_star_5));
        hud_wanted.add(activity.findViewById(R.id.hud_star_6));

        hud_micro.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
        });
        hud_donate.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            NvEventQueueActivity.getInstance().sendHuddeClick(0);
        });
        hud_yved.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            NvEventQueueActivity.getInstance().sendHuddeClick(1);
        });
        hud_quest.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            NvEventQueueActivity.getInstance().sendHuddeClick(2);
        });
        hud_menu.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            NvEventQueueActivity.getInstance().showMenuu();
            NvEventQueueActivity.getInstance().togglePlayer(1);
        });
        chat.setClickable(false);
    }

    public void UpdateHudInfo(int health, int armour, int hunger, int weaponidweik, int ammo, int playerid, int money, int wanted)
    {
        progressHP.setProgress(health);
        progressArmor.setProgress(armour);

        DecimalFormat formatter=new DecimalFormat();
        DecimalFormatSymbols symbols= DecimalFormatSymbols.getInstance();
        symbols.setGroupingSeparator('.');
        formatter.setDecimalFormatSymbols(symbols);
        String s= formatter.format(money);
        hud_money.setText(s);

        int id = activity.getResources().getIdentifier(new Formatter().format("weapon_%d", Integer.valueOf(weaponidweik)).toString(), "drawable", activity.getPackageName());
        hud_weapon.setImageResource(id);

        hud_weapon.setOnClickListener(v -> NvEventQueueActivity.getInstance().onWeaponChanged());
        if(wanted > 6) wanted = 6;
        for (int i2 = 0; i2 < wanted; i2++) {
            hud_wanted.get(i2).setBackgroundResource(R.drawable.ic_y_star);
        }
    }
    public void ShowChatj(){
        if (keybo == 0){
            NvEventQueueActivity.getInstance().showKey();
            hud_micro.setVisibility(View.GONE);
            hud_menu.setVisibility(View.GONE);
            hud_yved.setVisibility(View.GONE);
            hud_quest.setVisibility(View.GONE);
            hud_donate.setVisibility(View.GONE);
            keybo = 1;
        }
        else {
            NvEventQueueActivity.getInstance().hideKey();
            hud_micro.setVisibility(View.VISIBLE);
            hud_menu.setVisibility(View.VISIBLE);
            hud_yved.setVisibility(View.VISIBLE);
            hud_quest.setVisibility(View.VISIBLE);
            hud_donate.setVisibility(View.VISIBLE);
            keybo = 0;
        }
    }
    public void starttt(){
        LinearLayoutManager mLayoutManager = new LinearLayoutManager(activity);
        mLayoutManager.setSmoothScrollbarEnabled(true);

        mLayoutManager.setStackFromEnd(true);
        chat.setLayoutManager(mLayoutManager);

        adapter = new ChatAdapter(activity, chat_lines);
        chat.setAdapter(adapter);
        ShowChat();
    }
    public void AddChatMessage(String msg, int textcolor){
        activity.runOnUiThread(()-> {
            int myColor = textcolor;
            String color = String.format("%X", myColor, msg.hashCode());
            String temp = color.substring(0,6);
            colorrr = temp;

            adapter.addItem("{"+temp+"}"+msg);
        });
    }

    public void ShowGps()
    {
        Utils.ShowLayout(hud_gps, false);
    }

    public void HideGps()
    {
        Utils.HideLayout(hud_gps, false);
    }

    public void ShowX2()
    {
        Utils.ShowLayout(hud_x2, false);
    }

    public void HideX2()
    {
        Utils.HideLayout(hud_x2, false);
    }

    public void ShowZona()
    {
        Utils.ShowLayout(hud_zona, false);
    }

    public void HideZona()
    {
        Utils.HideLayout(hud_zona, false);
    }

    public void ShowRadar()
    {
        Utils.ShowLayout(hud_radar, true);
    }

    public void HideRadar()
    {
        Utils.HideLayout(hud_radar, false);
    }

    public void ShowHud()
    {
        hud_layout.setVisibility(View.VISIBLE);
    }

    public void HideHud()
    {
        hud_layout.setVisibility(View.GONE);
    }

    public void ShowChat()
    {
        chat.setVisibility(View.VISIBLE);
    }

    public void closekey()
    {
        NvEventQueueActivity.getInstance().togglePlayer(0);
        hud_micro.setVisibility(View.VISIBLE);
        hud_menu.setVisibility(View.VISIBLE);
        hud_yved.setVisibility(View.VISIBLE);
        hud_quest.setVisibility(View.VISIBLE);
        hud_donate.setVisibility(View.VISIBLE);
        keybo = 0;
    }

    public class ChatAdapter extends RecyclerView.Adapter<ChatAdapter.ViewHolder>{

        private final LayoutInflater inflater;
        private List<Spanned> chat_lines;

        ChatAdapter(Activity context, List<Spanned> chat_lines) {
            this.chat_lines = chat_lines;
            this.inflater = LayoutInflater.from(context);
        }
        @NotNull
        @Override
        public ViewHolder onCreateViewHolder(@NotNull ViewGroup parent, int viewType) {

            View view = inflater.inflate(R.layout.chatline, parent, false);
            view.setOnClickListener(view1 -> {
                ShowChatj(); //открытие клавы
            });
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, int position) {

            holder.chat_line_text.setText(chat_lines.get(position));
            holder.gggg.setBackgroundColor(Color.parseColor("#" + colorrr));
            holder.chat_line_text.setTextSize(TypedValue.COMPLEX_UNIT_PX, 18);

            if("" == holder.chat_line_text.getText() || " " == holder.chat_line_text.getText())
            {
                holder.chat_line_shadoww.setVisibility(View.INVISIBLE);
            } else
            {
                holder.chat_line_shadoww.setVisibility(View.VISIBLE);
            }
        }

        @Override
        public int getItemCount() {
            return chat_lines.size();
        }

        public List getItems() {
            return chat_lines;
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            final TextView chat_line_text;
            final TextView chat_line_shadoww;
            final ImageView gggg;
            ViewHolder(View view){
                super(view);
                chat_line_text = view.findViewById(R.id.chat_line_text);
                gggg = view.findViewById(R.id.chat__line_text_bg);
                chat_line_shadoww = view.findViewById(R.id.chat_line_bg);
            }
        }
        public void addItem(String item) {
            activity.runOnUiThread(() -> {
                if(this.chat_lines.size() > 40){
                    this.chat_lines.remove(0);
                    notifyItemRemoved(0);
                }
                chat_lines.add(Utils.transfromColors(item));
                notifyItemInserted(this.chat_lines.size()-1);


                if(chat.getScrollState() == RecyclerView.SCROLL_STATE_IDLE) {
                    chat.smoothScrollToPosition(this.chat_lines.size()-1);
                }
            });

        }
    }

}