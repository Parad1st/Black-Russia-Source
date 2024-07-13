package com.byparad1st.game.gui.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.models.Actions;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.Formatter;
import java.util.List;

public class ActionsAdapter extends RecyclerView.Adapter<ActionsAdapter.DonateViewHolder> {

    Context context;
    List<Actions> action;


    public ActionsAdapter(Context context, List<Actions> action) {
        this.context = context;
        this.action = action;
    }

    @NonNull
    @Override
    public DonateViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View donate_items = LayoutInflater.from(context).inflate(R.layout.donate_item, parent, false);
        return new ActionsAdapter.DonateViewHolder(donate_items);
    }

    @Override
    public void onBindViewHolder(@NonNull DonateViewHolder holder, int position) {

        int imgid = context.getResources().getIdentifier(action.get(position).getImg(), "drawable", context.getPackageName());
        holder.item_img.setImageResource(imgid);
        int imgid2 = context.getResources().getIdentifier("", "drawable", context.getPackageName());
        holder.item_img2.setImageResource(imgid2);

        holder.item_name.setText(action.get(position).getName());
            holder.item_name2.setText(action.get(position).getName2());
            holder.item_cost.setText(new Formatter().format("%d", Integer.valueOf(action.get(position).getCost())).toString() + " BC");

        holder.auto_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(context, R.anim.button_click));
                NvEventQueueActivity.getInstance().buy_carpodtv(action.get(position).getCost(), action.get(position).getId());
                NvEventQueueActivity.getInstance().getNameOfCar(action.get(position).getName() + "\n" + action.get(position).getName2());
            }
        });
    }
    @Override
    public int getItemCount() {
        return action.size();
    }
    public static final class DonateViewHolder extends RecyclerView.ViewHolder {
        public Button auto_button;
        ImageView item_img, item_img2;
        TextView item_cost, item_name, item_name2;
        public DonateViewHolder(@NonNull View itemView) {
            super(itemView);

            item_img = itemView.findViewById(R.id.imageView23);
            item_img2 = itemView.findViewById(R.id.imgcars2ffffddf);
            item_cost = itemView.findViewById(R.id.textView22);
            item_name = itemView.findViewById(R.id.textView20);
            item_name2 = itemView.findViewById(R.id.textView21);
            auto_button = itemView.findViewById(R.id.donate_buton);
        }
    }
}
