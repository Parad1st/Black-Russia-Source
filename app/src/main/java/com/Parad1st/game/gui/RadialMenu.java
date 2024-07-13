package com.byparad1st.game.gui;

import android.app.Activity;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

import android.view.View;
import android.widget.*;

import androidx.constraintlayout.widget.ConstraintLayout;

public class RadialMenu {
    public Activity activity;
    public ConstraintLayout radial_menu_layout;
    public ImageView button_key, button_park, button_turbo, button_engine, button_door, button_light, button_suspension, button_launch_control, button_close;
    public boolean key, park, turbo, engine, door, light, suspension, launch_control;
    
    public RadialMenu(Activity aactivity) {
        activity = aactivity;
        radial_menu_layout = activity.findViewById(R.id.radial_menu);
        button_key = activity.findViewById(R.id.button_key);
        button_park = activity.findViewById(R.id.button_parking);
        button_turbo = activity.findViewById(R.id.button_turbo);
        button_engine = activity.findViewById(R.id.button_engine);
        button_door = activity.findViewById(R.id.button_door);
        button_light = activity.findViewById(R.id.button_light);
        button_suspension = activity.findViewById(R.id.button_suspension);
        button_launch_control = activity.findViewById(R.id.button_launch_control);
        setListeners();
        button_close = activity.findViewById(R.id.radial_close);
        button_close.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    close();
            }
        });
        Utils.HideLayout(radial_menu_layout, false);
    }
    
    public void setListeners() {
    	button_key.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(0);
                if(!key)
                    button_key.setImageResource(R.drawable.radial_key_checked);
                else 
                    button_key.setImageResource(R.drawable.radial_key_not_checked);
                key = !key;
            }
        });
        button_park.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(1);
                if(!park)
                    button_park.setImageResource(R.drawable.radial_parking_checked);
                else 
                    button_park.setImageResource(R.drawable.radial_parking_not_checked);
                park = !park;
            }
        });
        button_turbo.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(2);
                if(!turbo)
                    button_turbo.setImageResource(R.drawable.radial_turbo_checked);
                else 
                    button_turbo.setImageResource(R.drawable.radial_turbo_not_checked);
                turbo = !turbo;
            }
        });
        button_engine.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(3);
                if(!engine)
                    button_engine.setImageResource(R.drawable.radial_engine_checked);
                else 
                    button_engine.setImageResource(R.drawable.radial_engine_not_checked);
                engine = !engine;
            }
        });
        button_door.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(4);
                if(!door)
                    button_door.setImageResource(R.drawable.radial_door_checked);
                else 
                    button_door.setImageResource(R.drawable.radial_door_not_checked);
                door = !door;
            }
        });
        button_light.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(5);
                if(!light)
                    button_light.setImageResource(R.drawable.radial_light_checked);
                else 
                    button_light.setImageResource(R.drawable.radial_light_not_checked);
                light = !light;
            }
        });
        button_suspension.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(6);
                if(!suspension)
                    button_suspension.setImageResource(R.drawable.radial_suspension_checked);
                else 
                    button_suspension.setImageResource(R.drawable.radial_suspension_not_checked);
                suspension = !suspension;
            }
        });
        button_launch_control.setOnClickListener(new View.OnClickListener() {
            @Override 
            public void onClick(View v) {
        	    NvEventQueueActivity.getInstance().sendRadialClick(7);
                if(!launch_control)
                    button_launch_control.setImageResource(R.drawable.radial_control_checked);
                else 
                    button_launch_control.setImageResource(R.drawable.radial_control_not_checked);
                launch_control = !launch_control;
                close();
            }
        });
    }
    
    public void show(boolean park, boolean key, boolean doors, boolean lights, boolean suspension, boolean launch_control, boolean engine, boolean turbo) {
    	if(park)
            button_park.setImageResource(R.drawable.radial_parking_checked);
        else 
            button_park.setImageResource(R.drawable.radial_parking_not_checked);
        if(key)
            button_key.setImageResource(R.drawable.radial_key_checked);
        else 
            button_key.setImageResource(R.drawable.radial_key_not_checked);
        if(doors)
            button_door.setImageResource(R.drawable.radial_door_checked);
        else
            button_door.setImageResource(R.drawable.radial_door_not_checked);
        if(lights)
            button_light.setImageResource(R.drawable.radial_light_checked);
        else 
            button_light.setImageResource(R.drawable.radial_light_not_checked);
        if(suspension)
            button_suspension.setImageResource(R.drawable.radial_suspension_checked);
        else 
            button_suspension.setImageResource(R.drawable.radial_suspension_not_checked);
        if(launch_control)
            button_launch_control.setImageResource(R.drawable.radial_control_checked);
        else 
            button_launch_control.setImageResource(R.drawable.radial_control_not_checked);
        if(engine)
            button_engine.setImageResource(R.drawable.radial_engine_checked);
        else 
            button_engine.setImageResource(R.drawable.radial_engine_not_checked);
        if(turbo)
            button_turbo.setImageResource(R.drawable.radial_turbo_checked);
        else 
            button_turbo.setImageResource(R.drawable.radial_turbo_not_checked);
        
        this.park = park;
        this.key = key;
        this.door = doors;
        this.light = lights;
        this.suspension = suspension;
        this.launch_control = launch_control;
        this.engine = engine;
        this.turbo = turbo;
    	Utils.ShowLayout(radial_menu_layout, true);
    	NvEventQueueActivity.getInstance().togglePlayer(1);
    }

    public void close() {
        Utils.HideLayout(radial_menu_layout, true);
        NvEventQueueActivity.getInstance().togglePlayer(0);
    }
}