package com.byparad1st.game.gui.jbl;


import android.app.Activity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.byparad1st.launcher.other.Interface;
import com.byparad1st.launcher.other.Lists;
import com.nvidia.devtech.NvEventQueueActivity;

import java.util.ArrayList;
import java.util.Formatter;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class Jbl extends AppCompatActivity {

    static RecyclerView jblRecycler;
    static JblAdapter jblAdapter;
    public Button close;
    public ConstraintLayout jbl_main;
    public  Activity activity;

    public RecyclerView PassRecyclerr;

    ArrayList<JblModels> jlist;

    public static TextView names;
    public Jbl(Activity aactivity){
        activity = aactivity;
        jblRecycler = activity.findViewById(R.id.JblRecyclerView);
        close = activity.findViewById(R.id.close);
        PassRecyclerr = activity.findViewById(R.id.JblRecyclerView);
        names = activity.findViewById(R.id.namemusic);
        jbl_main = activity.findViewById(R.id.jbl_main);
        jbl_main.setVisibility(View.GONE);

        close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                hidejbl();
            }
        });

        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("http://vbd.fdv.dd/")
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        Interface sInterface = retrofit.create(Interface.class);

        Call<List<JblModels>> jcall = sInterface.getMusic();

        jcall.enqueue(new Callback<List<JblModels>>() {
            @Override
            public void onResponse(Call<List<JblModels>> call, Response<List<JblModels>> response) {
                List<JblModels> jbls = response.body();

                for (JblModels jbl : jbls) {
                    Lists.jlist.add(new JblModels(jbl.getName(), jbl.getUrl(), jbl.getTime()));
                }
            }

            @Override
            public void onFailure(Call<List<JblModels>> call, Throwable t) {
                Toast.makeText(getApplicationContext(), t.toString(), Toast.LENGTH_SHORT).show();
            }

        });

    }
    public void show_jbl() {
        PassRecyclerr.setHasFixedSize(true);
        LinearLayoutManager layoutManager = new LinearLayoutManager(activity, LinearLayoutManager.VERTICAL, false);
        PassRecyclerr.setLayoutManager(layoutManager);


        this.jlist = Lists.jlist;
        jblAdapter = new JblAdapter(activity, this.jlist);
        PassRecyclerr.setAdapter(jblAdapter);
    }
    public void showjbl(){
        jbl_main.setVisibility(View.VISIBLE);
        show_jbl();
    }
    public void hidejbl()
    {
        jbl_main.setVisibility(View.GONE);
    }
}
