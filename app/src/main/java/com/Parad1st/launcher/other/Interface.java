package com.byparad1st.launcher.other;

import com.byparad1st.game.gui.models.Actions;
import com.byparad1st.launcher.model.News;
import com.byparad1st.launcher.model.Servers;
import com.byparad1st.game.gui.jbl.JblModels;

import java.util.List;

import retrofit2.Call;
import retrofit2.http.GET;

public interface Interface {

    @GET("https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Json%20files/servers.json")
    Call<List<Servers>> getServers();

    @GET("https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Json%20files/donate.json")
    Call<List<Actions>> getActions();
    @GET("https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Json%20files/music.json")
    Call<List<JblModels>> getMusic();

    @GET("https://raw.githubusercontent.com/Parad1st/Black-Russia-Source/main/Json%20files/stories.json")
    Call<List<News>> getNews();
}
