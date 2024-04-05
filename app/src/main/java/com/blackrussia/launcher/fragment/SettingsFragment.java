package com.blackrussia.launcher.fragment;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.EditorInfo;
import android.widget.*;
import android.widget.Toast;

import androidx.fragment.app.Fragment;

import com.blackrussia.game.R;
import com.blackrussia.launcher.other.Utils;

import org.ini4j.Wini;

import java.io.File;
import java.io.IOException;

import com.downloader.*;
import com.downloader.Error;

import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SettingsFragment extends Fragment {

    Animation animation;
    public EditText nickname;
    String nickName;

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View inflate = inflater.inflate(R.layout.fragment_settings, container, false);

        Animation animation = AnimationUtils.loadAnimation(getContext(), R.anim.button_click);

        nickname = (EditText) inflate.findViewById(R.id.editText);

        InitLogic();

        ((TextView) inflate.findViewById(R.id.reinstallGame))
                .setOnClickListener(
                        new View.OnClickListener() {
                            public void onClick(View v) {
                                v.startAnimation(animation);
								File gameDirectory = (new File(Environment.getExternalStorageDirectory() + "/Blast"));
								Utils.delete(gameDirectory);
								startActivity(new Intent(getActivity(), com.blackrussia.launcher.activity.LoaderActivity.class));
                            }
        });
        ((TextView) inflate.findViewById(R.id.resetSettings))
                .setOnClickListener(
                        new View.OnClickListener() {
                            public void onClick(View v) {
                                v.startAnimation(animation);
                            }
        });
        ((ImageView) inflate.findViewById(R.id.telegramButton))
                .setOnClickListener(
                        new View.OnClickListener() {
                            public void onClick(View v) {
                                v.startAnimation(animation);
                                startActivity(new Intent("android.intent.action.VIEW", Uri.parse("")));
                            }
        });
        ((ImageView) inflate.findViewById(R.id.instagramButton))
                .setOnClickListener(
                        new View.OnClickListener() {
                            public void onClick(View v) {
                                v.startAnimation(animation);
                                startActivity(new Intent("android.intent.action.VIEW", Uri.parse("")));
                            }
        });
        ((ImageView) inflate.findViewById(R.id.vkButton))
                .setOnClickListener(
                        new View.OnClickListener() {
                            public void onClick(View v) {
                                v.startAnimation(animation);
                                startActivity(
                                        new Intent("android.intent.action.VIEW", Uri.parse("https://vk.com/hyper_bonus")));
                            }
                        });
        ((ImageView) inflate.findViewById(R.id.discordButton))
                .setOnClickListener(
                        new View.OnClickListener() {
                            public void onClick(View v) {
                                v.startAnimation(animation);
                                startActivity(new Intent("android.intent.action.VIEW", Uri.parse("")));
                            }
        });

        ((EditText) nickname)
                .setOnEditorActionListener(
                        new EditText.OnEditorActionListener() {
                            @Override
                            public boolean onEditorAction(
                                    TextView v, int actionId, KeyEvent event) {
                                if (actionId == EditorInfo.IME_ACTION_SEARCH
                                        || actionId == EditorInfo.IME_ACTION_DONE
                                        || event.getAction() == KeyEvent.ACTION_DOWN
                                                && event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                                    try {
                                        File f =
                                                new File(
                                                        Environment.getExternalStorageDirectory()
                                                                + "/BlackRussia/SAMP/settings.ini");
                                        if (!f.exists()) {
                                            f.createNewFile();
                                            f.mkdirs();
                                        }
                                        Wini w =
                                                new Wini(
                                                        new File(
                                                                Environment.getExternalStorageDirectory()
                                 + "/BlackRussia/SAMP/settings.ini"));
								 if(checkValidNick(inflate)){
									 w.put("client", "name", nickname.getText().toString());
                                        Toast.makeText(
                                                getActivity(),
                                                "Ваш новый никнейм успешно сохранен!",
                                                Toast.LENGTH_SHORT).show();
								 } else {
									 checkValidNick(inflate);
								 }
                                        w.store();
                                    } catch (IOException e) {
                                        e.printStackTrace();
										Toast.makeText(getActivity(), "Установите игру!", Toast.LENGTH_SHORT).show();
                                    }
                                }
                                return false;
                            }
        });
        return inflate;
    }

    private void InitLogic() {
        try {
            Wini w = new Wini(new File(Environment.getExternalStorageDirectory() + "/BlackRussia/SAMP/settings.ini"));
            nickname.setText(w.get("client", "name"));
            w.store();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

	public boolean checkValidNick(View inflate){
		EditText nick = (EditText) inflate.findViewById(R.id.editText);
		if(nick.getText().toString().isEmpty()) {
			Toast.makeText(getActivity(), "Введите ник", Toast.LENGTH_SHORT).show();
			return false;
		}
		if(!(nick.getText().toString().contains("_"))){
			Toast.makeText(getActivity(), "Ник должен содержать символ \"_\"", Toast.LENGTH_SHORT).show();
			return false;
		}
		if(nick.getText().toString().length() < 4){
			Toast.makeText(getActivity(), "Длина ника должна быть не менее 4 символов", Toast.LENGTH_SHORT).show();
			return false;
		}
		return true;
	}
}