package com.byparad1st.game.gui.jbl;



import static com.byparad1st.game.gui.jbl.Jbl.names;

import android.content.Context;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.byparad1st.game.R;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.Formatter;
import java.util.List;


public class JblAdapter extends RecyclerView.Adapter<JblAdapter.PassViewHolder> {

    Context context;
    List<JblModels> Jbl;


    public JblAdapter(Context context, List<JblModels> jbl) {
        this.context = context;
        this.Jbl = jbl;
    }

    @NonNull
    @Override
    public PassViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View test_items = LayoutInflater.from(context).inflate(R.layout.jbl_item, parent, false);
        return new PassViewHolder(test_items);
    }

    @Override
    public void onBindViewHolder(@NonNull PassViewHolder holder, int position) {
        JblModels passs = this.Jbl.get(position);

        holder.namemusic.setText(Jbl.get(position).getName());
        holder.timemusic.setText(Jbl.get(position).getTime());
        String url = Jbl.get(position).getUrl();

        holder.jbl_click.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                names.setText("Сейчас играет: "+Jbl.get(position).getName());
                NvEventQueueActivity.getInstance().playUrlSound(url);
            }
        });
    }
    @Override
    public int getItemCount() {
        return Jbl.size();
    }
    public List getItems() {
        return Jbl;
    }
    public static final class PassViewHolder extends RecyclerView.ViewHolder {
        LinearLayout jbl_item;
        ConstraintLayout jbl_click;
        TextView namemusic, timemusic;
        public PassViewHolder(@NonNull View itemView) {
            super(itemView);
            jbl_item = itemView.findViewById(R.id.jbl_item);
            namemusic = itemView.findViewById(R.id.namemusic);
            timemusic = itemView.findViewById(R.id.timemusic);
            jbl_click = itemView.findViewById(R.id.jbl_click);


        }
    }
}
