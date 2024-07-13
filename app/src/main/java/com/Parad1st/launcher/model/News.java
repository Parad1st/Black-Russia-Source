package com.blackrussia.launcher.model;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class News {

	@SerializedName("imageurl")
	@Expose
	private String imageurl;

	@SerializedName("title")
	@Expose
	private String title;

	@SerializedName("url")
	@Expose
	private String url;

	public News (String imageurl, String title, String url) {
		this.imageurl = imageurl;
		this.title = title;
		this.url = url;
	}

	public String getImageUrl() {
		return imageurl;
	}

	public String getTitle() {
		return title;
	}

	public String getUrl() {
		return url;
	}

}