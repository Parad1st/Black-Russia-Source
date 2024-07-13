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
import com.byparad1st.game.gui.models.Donatee;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.Formatter;
import java.util.List;

public class DonateAdapter extends RecyclerView.Adapter<DonateAdapter.DonateViewHolder> {

    Context context;
    List<Donatee> donate;


    public DonateAdapter(Context context, List<Donatee> donate) {
        this.context = context;
        this.donate = donate;
    }

    @NonNull
    @Override
    public DonateViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View donate_items = LayoutInflater.from(context).inflate(R.layout.donate_item, parent, false);
        return new DonateAdapter.DonateViewHolder(donate_items);
    }

    @Override
    public void onBindViewHolder(@NonNull DonateViewHolder holder, int position) {

        int imgid = context.getResources().getIdentifier(donate.get(position).getImg(), "drawable", context.getPackageName());
        holder.item_img.setImageResource(imgid);

        int imgid2 = context.getResources().getIdentifier(donate.get(position).getImg2(), "drawable", context.getPackageName());
        holder.item_img2.setImageResource(imgid2);

        holder.item_name.setText(donate.get(position).getName());
        if(donate.get(position).getId() > 7) {
            holder.item_name2.setText(donate.get(position).getName2());
            holder.item_cost.setText(new Formatter().format("%d", Integer.valueOf(donate.get(position).getCost())).toString() + " BC");
        }
        holder.auto_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(context, R.anim.button_click));
                switch (donate.get(position).getId()) {

                    case 1: NvEventQueueActivity.getInstance().show_nizkii(); break;
                    case 2: NvEventQueueActivity.getInstance().show_srednii(); break;
                    case 3: NvEventQueueActivity.getInstance().show_visokii(); break;
                    case 4: NvEventQueueActivity.getInstance().show_motos(); break;
                    case 5: NvEventQueueActivity.getInstance().show_unique();break;
                    case 6: break;
                    case 7: break;

                    default: {
                        NvEventQueueActivity.getInstance().buy_carpodtv(donate.get(position).getCost(), donate.get(position).getId());
                        NvEventQueueActivity.getInstance().getNameOfCar(donate.get(position).getName() + "\n" + donate.get(position).getName2());
                        break;
                    }
                }
            }
        });
    }
    @Override
    public int getItemCount() {
        return donate.size();
    }
    public static final class DonateViewHolder extends RecyclerView.ViewHolder {
        public Button auto_button;
        ImageView  item_img, item_img2;
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
