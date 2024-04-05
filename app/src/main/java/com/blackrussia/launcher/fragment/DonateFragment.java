package com.blackrussia.launcher.fragment;

import android.os.Bundle;          
import android.os.Environment;
import android.os.Build;

import android.app.Dialog;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import android.webkit.ValueCallback;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import com.bumptech.glide.load.Key;
import java.net.URLEncoder;

import android.widget.*;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

import android.view.View.OnClickListener;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AnimationUtils;
import android.view.animation.Animation;
import android.view.LayoutInflater;

import androidx.fragment.app.Fragment;

import com.blackrussia.game.R;

import java.io.UnsupportedEncodingException;

public class DonateFragment extends Fragment {
	
	Animation animation;
	
	public Dialog dialog;
	
	public EditText nik;
    public EditText sum;
	
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View inflate = inflater.inflate(R.layout.fragment_donate, container, false);
		
		TextView textView = (TextView) inflate.findViewById(R.id.deposit);
		
		Animation animation = AnimationUtils.loadAnimation(getContext(), R.anim.button_click);
		
		nik = inflate.findViewById(R.id.nik);
        sum = inflate.findViewById(R.id.sum);
		
		textView.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(animation);
				onClickDeposit();
            }
        });
		
        return inflate;
    }
	
	public void onClickDeposit() {
        if (nik.getText().toString().isEmpty()) {
            Toast.makeText(getActivity(), "Введите ник!", 1).show();
        } else if (!nik.getText().toString().contains("_")) {
            Toast.makeText(getActivity(),"Ник должен иметь \"_\"!", 1).show();
        } else if (nik.getText().toString().length() < 4) {
            Toast.makeText(getActivity(), "Минимальная длина ника 4 символа!", 1).show();
        } else if (this.sum.getText().toString().isEmpty()) {
            Toast.makeText(getActivity(), "Введите сумму!", 1).show();
        } else {

        }
    }
}
