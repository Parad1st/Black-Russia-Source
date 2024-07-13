package com.byparad1st.game.gui.tab;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Filter;
import android.widget.Filterable;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.byparad1st.game.R;
import java.util.ArrayList;
import java.util.List;

public class TabAdapter extends RecyclerView.Adapter implements Filterable {

    private List<PlayerData> mPlayerData;
    private List<PlayerData> mPlayerDataCopy;

    public TabAdapter(List<PlayerData> playerData) {
        this.mPlayerData = playerData;
        this.mPlayerDataCopy = playerData;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.hassle_tab_item, parent, false));
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        onBindViewHolder((ViewHolder) holder, position);
    }

    public void onBindViewHolder(ViewHolder holder, int position) {
        PlayerData data = this.mPlayerData.get(position);
        holder.id.setText(String.valueOf(data.getId()));
        holder.name.setText(data.getName());
        holder.level.setText(String.valueOf(data.getLevel()));
        holder.ping.setText(String.valueOf(data.getPing()));
        holder.View.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
            }
        });
    }

    @Override
    public int getItemCount() {
        return this.mPlayerData.size();
    }

    @Override
    public Filter getFilter() {
        return new Filter() {
            @Override
            public FilterResults performFiltering(CharSequence charSequence) {
                String input = charSequence.toString();
                List<PlayerData> newPlayersDataList = new ArrayList<>();
                if (input.isEmpty()) {
                    newPlayersDataList = mPlayerDataCopy;
                } else {
                    for (PlayerData playersData : mPlayerDataCopy) {
                        if (playersData.getName().toLowerCase().contains(input.toLowerCase())) {
                            newPlayersDataList.add(playersData);
                        }
                    }
                }
                FilterResults filterResults = new FilterResults();
                filterResults.values = newPlayersDataList;
                return filterResults;
            }

            @Override
            public void publishResults(CharSequence charSequence, FilterResults filterResults) {
                mPlayerData = (List) filterResults.values;
                notifyDataSetChanged();
            }
        };
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView id;
        public TextView level;
        public View mView;
        public ConstraintLayout View;
        public TextView name;
        public TextView ping;

        public ViewHolder(View itemView) {
            super(itemView);
            this.mView = itemView;
            this.View = (ConstraintLayout) itemView.findViewById(R.id.hassle_tab_item);
            this.id = (TextView) itemView.findViewById(R.id.player_id);
            this.name = (TextView) itemView.findViewById(R.id.player_name);
            this.level = (TextView) itemView.findViewById(R.id.player_level);
            this.ping = (TextView) itemView.findViewById(R.id.player_ping);
        }

        public View getView() {
            return this.mView;
        }
    }
}
