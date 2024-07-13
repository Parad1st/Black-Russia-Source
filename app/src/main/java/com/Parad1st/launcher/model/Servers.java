package com.byparad1st.launcher.model;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Servers {

	@SerializedName("color")
	@Expose
	private String color;

	@SerializedName("dopname")
	@Expose
	private String dopname;

	@SerializedName("name")
	@Expose
	private String name;

	@SerializedName("online")
	@Expose
	private int online;

	@SerializedName("maxonline")
	@Expose
	private int maxonline;

	public Servers(String color, String dopname, String name, int online, int maxonline) {
		this.color = color;
		this.dopname = dopname;
		this.name = name;
		this.online = online;
		this.maxonline = maxonline;
	}
	 
	public String getname() {
		return name;
	}

    public String getDopname() {
		 return dopname;
	}

    public String getColor() {
		return color;
	}
	
	public int getOnline(){
		return online;
	}

	public int getmaxOnline(){
		return maxonline;
	}
}