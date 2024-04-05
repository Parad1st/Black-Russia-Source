package com.blackrussia.launcher.other;

import com.blackrussia.launcher.model.News;
import com.blackrussia.launcher.model.Servers;

import java.util.List;

import retrofit2.Call;
import retrofit2.http.GET;

public interface Interface {

    @GET("https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Json%20files/servers.json")
    Call<List<Servers>> getServers();

    @GET("https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Json%20files/stories.json")
    Call<List<News>> getNews();

}
