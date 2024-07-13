package com.byparad1st.game.gui.adapters;;

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
import com.byparad1st.game.gui.models.Donat;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.List;

public class DonatAdapter extends RecyclerView.Adapter<DonatAdapter.DonatViewHolder> {

    Context context;
    List<Donat> donat;


    public DonatAdapter(Context context, List<Donat> donat) {
        this.context = context;
        this.donat = donat;
    }

    @NonNull
    @Override
    public DonatViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View donat_items = LayoutInflater.from(context).inflate(R.layout.donat_item, parent, false);
        return new DonatAdapter.DonatViewHolder(donat_items);
    }

    @Override
    public void onBindViewHolder(@NonNull DonatViewHolder holder, int position) {

        /*if(position == 0) {
            int imgid = context.getResources().getIdentifier("button_yellow_gradient", "drawable", context.getPackageName());
            holder.item_name.setBackgroundResource(imgid);
            holder.item_name.setTextColor(Color.parseColor("#000000"));
            holder.item_cost.setBackgroundResource(imgid);
            holder.item_cost.setTextColor(Color.parseColor("#000000"));
        }*/

        holder.item_name.setText(donat.get(position).getName());
        holder.item_cost.setText(donat.get(position).getCost() + " BC");

        holder.item_name.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(context, R.anim.button_click));
                NvEventQueueActivity.getInstance().upd_uslugi(donat.get(position).getName(), donat.get(position).getCost(), donat.get(position).getImg(), donat.get(position).getId());
            }
        });
    }

    @Override
    public int getItemCount() {
        return donat.size();
    }

    public static final class DonatViewHolder extends RecyclerView.ViewHolder {
        Button item_cost, item_name;
        TextView topt, costt;
        ImageView topimg;

        public DonatViewHolder(@NonNull View itemView) {
            super(itemView);

            item_cost = itemView.findViewById(R.id.button_rightt);
            item_name = itemView.findViewById(R.id.button_leftt);
            topt = itemView.findViewById(R.id.toptextuslugi);
            costt = itemView.findViewById(R.id.costusl);
            topimg = itemView.findViewById(R.id.imgusl);

        }
    }

}
