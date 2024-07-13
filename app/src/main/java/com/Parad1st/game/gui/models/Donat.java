package com.byparad1st.game.gui.models;

public class Donat {

    int id;
    String name, cost, img;

    public Donat(int id, String name, String cost, String img) {
        this.id = id;
        this.name = name;
        this.cost = cost;
        this.img = img;
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

    public String getCost() {
        return cost;
    }

    public void setCost(String cost) {
        this.cost = cost;
    }

    public String getImg() {
        return img;
    }

    public void setImg(String img) {
        this.img = img;
    }
}