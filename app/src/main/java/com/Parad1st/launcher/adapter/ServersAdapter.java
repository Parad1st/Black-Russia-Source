package com.byparad1st.launcher.adapter;

import android.graphics.PorterDuff;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.animation.AnimationUtils;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;

import androidx.recyclerview.widget.RecyclerView;
import androidx.annotation.NonNull;

import com.byparad1st.game.R;
import com.byparad1st.launcher.model.Servers;
import java.util.ArrayList;

import com.dinuscxj.progressbar.CircleProgressBar;

public class ServersAdapter extends RecyclerView.Adapter<ServersAdapter.ServersViewHolder> {
	Context context;
	
	ArrayList<Servers> slist;
	
	public ServersAdapter(Context context, ArrayList<Servers> slist){
		 this.context = context;
		 this.slist = slist; 
	}
	
	@NonNull
	@Override
    public ServersViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(context).inflate(R.layout.item_server, parent, false);
		return new ServersViewHolder(v); 
    }
  
    @Override
    public void onBindViewHolder(@NonNull ServersViewHolder holder, int position) {
        Servers servers = slist.get(position);
		holder.bearPaw.setColorFilter(Color.parseColor("#" + servers.getColor()),PorterDuff.Mode.SRC_ATOP);
		holder.people.setColorFilter(Color.parseColor("#" + servers.getColor()),PorterDuff.Mode.SRC_ATOP);
		holder.backColor.getBackground().setColorFilter(Color.parseColor("#" + servers.getColor()),PorterDuff.Mode.SRC_ATOP);
		holder.name.setText(servers.getname());
		holder.name.setTextColor(Color.parseColor("#" + servers.getColor()));
		holder.dopname.setText(servers.getDopname());
		holder.textonline.setText(Integer.toString(servers.getOnline()));
		holder.textmaxonline.setText(new String("/" + Integer.toString(servers.getmaxOnline())));
	    holder.progressBar.setProgressStartColor(Color.parseColor("#" + servers.getColor()));
		holder.progressBar.setProgressEndColor(Color.parseColor("#" + servers.getColor()));
		holder.progressBar.setProgress(servers.getOnline());
		holder.progressBar.setMax(servers.getmaxOnline());
		holder.container.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(context, R.anim.button_click));
                Toast.makeText(context, "Для начала игры нажмите красную кнопку", 0).show();
            }
        });
    }

    @Override
    public int getItemCount() {
        return slist.size();
    }

    public static class ServersViewHolder extends RecyclerView.ViewHolder {
        
		FrameLayout container;
		CircleProgressBar progressBar; 
		ImageView x2,bearPaw, people;
		TextView name, dopname, textonline, textmaxonline;
		LinearLayout backColor; 
	    
        public ServersViewHolder(View itemView) {
            super(itemView);
            
		    name = itemView.findViewById(R.id.firstName);
			dopname = itemView.findViewById(R.id.secondName);
			progressBar = itemView.findViewById(R.id.progressBar);
			bearPaw = itemView.findViewById(R.id.bearPaw);
			x2 = itemView.findViewById(R.id.x2);
			people = itemView.findViewById(R.id.people);
			textonline = itemView.findViewById(R.id.online);
			textmaxonline = itemView.findViewById(R.id.onlineTotal);
			backColor = itemView.findViewById(R.id.backColor);
			container = itemView.findViewById(R.id.container);
        }
    }
	
}