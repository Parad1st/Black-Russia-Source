package com.byparad1st.game.gui.models;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Donatee {

    int id, cost;
    String name, name2, img, img2;

    public Donatee(int id, String name, String name2, String img, String img2, int cost) {
        this.id = id;
        this.name = name;
        this.name2 = name2;
        this.img = img;
        this.img2 = img2;
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

    public String getImg2() {
        return img2;
    }

    public void setImg2(String img2) {
        this.img2 = img2;
    }

    public int getCost() {
        return cost;
    }

    public void setCost(int cost) {
        this.cost = cost;
    }
}