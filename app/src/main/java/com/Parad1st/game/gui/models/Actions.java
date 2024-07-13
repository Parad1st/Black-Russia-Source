package com.byparad1st.game.gui.models;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Actions {
    @SerializedName("name")
    @Expose
    private String name;

    @SerializedName("name2")
    @Expose
    private String name2;

    @SerializedName("img")
    @Expose
    private String img;

    @SerializedName("id")
    @Expose
    private int id;

    @SerializedName("cost")
    @Expose
    private int cost;

    public Actions(int id, String name, String name2, String img, int cost) {
        this.id = id;
        this.name = name;
        this.name2 = name2;
        this.img = img;
        this.cost = cost;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getName2() {
        return name2;
    }

    public void setName2(String name2) {
        this.name2 = name2;
    }

    public String getImg() {
        return img;
    }

    public void setImg(String img) {
        this.img = img;
    }

    public int getCost() {
        return cost;
    }

    public void setCost(int cost) {
        this.cost = cost;
    }
}
