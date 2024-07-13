package com.byparad1st.launcher.fragment;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;

import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.widget.*;

import androidx.appcompat.widget.AppCompatButton;

import androidx.fragment.app.Fragment;

import android.view.View.OnClickListener;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.view.animation.Animation;
import android.view.LayoutInflater;
import android.view.ViewGroup;

import java.io.File;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import com.byparad1st.game.R;

import org.ini4j.Wini;

public class MainFragment extends Fragment {
	
	EditText nickname;
	ImageButton ib_info;
	
	@Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View inflate = inflater.inflate(R.layout.fragment_main, container, false);
    	Animation animation = AnimationUtils.loadAnimation(getActivity(), R.anim.button_click);
        
        InitLogic();
        
        nickname = inflate.findViewById(R.id.edit_text_name);
        ib_info = inflate.findViewById(R.id.ib_info);
		
	    ((AppCompatButton) inflate.findViewById(R.id.button_play)).setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(animation);
                Timer t = new Timer();
                t.schedule(new TimerTask(){
                   @Override
                   public void run() {
                       onClickPlay();
                   }
                }, 200L);
            }
        });
        
        ((ImageButton) ib_info).setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(animation);
                TextView info_nick = inflate.findViewById(R.id.text_view_info_about_nickname);
                if(info_nick.getVisibility() == View.INVISIBLE)
                    info_nick.setVisibility(View.VISIBLE);
                else 
                    info_nick.setVisibility(View.INVISIBLE);
            }
        });
        
        ((ImageButton) inflate.findViewById(R.id.button_vk)).setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
            	v.startAnimation(animation);
        	    Timer t = new Timer();
                t.schedule(new TimerTask(){
                   @Override
                   public void run() {
                       startActivity(new Intent("android.intent.action.VIEW", Uri.parse("https://vk.com/luxrussiacrmp")));
                   }
                }, 200L);
            }
        });
        
        ((ImageButton) inflate.findViewById(R.id.button_discord)).setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
            	v.startAnimation(animation);
        	    Timer t = new Timer();
                t.schedule(new TimerTask(){
                   @Override
                   public void run() {
                       startActivity(new Intent("android.intent.action.VIEW", Uri.parse("https://discrord.com/")));
                   }
                }, 200L);
            }
        });
        
        ((ImageButton) inflate.findViewById(R.id.button_telegram)).setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
            	v.startAnimation(animation);
        	    Timer t = new Timer();
                t.schedule(new TimerTask(){
                   @Override
                   public void run() {
                       startActivity(new Intent("android.intent.action.VIEW", Uri.parse("https://t.me/LUXRUSSIACRIMINAL")));
                   }
                }, 200L);
            }
        });
        
        ((AppCompatButton) inflate.findViewById(R.id.button_clean_game)).setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
            	v.startAnimation(animation);
        	    Timer t = new Timer();
                t.schedule(new TimerTask(){
                   @Override
                   public void run() {
                       /*MainActivity activity = getActivity();
		               activity.toLoaderFragment();*/
                   }
                }, 200L);
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
                                                                + "/LUXRUSSIA/SAMP/settings.ini");
                                        if (!f.exists()) {
                                            f.createNewFile();
                                            f.mkdirs();
                                        }
                                        Wini w =
                                                new Wini(
                                                        new File(
                                                                Environment.getExternalStorageDirectory()
                                 + "/LUXRUSSIA/SAMP/settings.ini"));
								 if(checkValidNick()){
									 w.put("client", "name", nickname.getText().toString());
                                        Toast.makeText(
                                                getActivity(),
                                                "Ваш новый никнейм успешно сохранен!",
                                                Toast.LENGTH_SHORT).show();
								 } else {
									 checkValidNick();
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
        
        nickname.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
        
            }

             @Override
             public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                 try {
                     File f = new File(Environment.getExternalStorageDirectory() + "/LUXRUSSIA/SAMP/settings.ini");
                     if (!f.exists()) {
                         f.createNewFile();
                         f.mkdirs();
                     }
                     Wini w = new Wini(new File(Environment.getExternalStorageDirectory() + "/LUXRUSSIA/SAMP/settings.ini"));
					 if(checkValidNick()){
						w.put("client", "name", nickname.getText().toString());        
					  } else {
					      checkValidNick();
					   }
                       w.store();
                       } catch (IOException e) {
                           e.printStackTrace();
                       }
                 }

             @Override
             public void afterTextChanged(Editable editable) {
        
             }
        });
		
        return inflate;
    }
    
    public void onClickPlay() {
        if(IsGameInstalled()) {
            startActivity(new Intent(getActivity(), com.byparad1st.game.core.GTASA.class));
		} else {
		   /* MainActivity activity = getActivity();
		    activity.toLoaderFragment();*/
		}
    }
    
    private boolean IsGameInstalled()
    {
        String CheckFile = Environment.getExternalStorageDirectory() + "/LUXRUSSIA/texdb/gta3.img";
        File file = new File(CheckFile);
        return file.exists();
    }
	
	private void InitLogic() {
        try {
            Wini w = new Wini(new File(Environment.getExternalStorageDirectory() + "/LUXRUSSIA/SAMP/settings.ini"));
            nickname.setText(w.get("client", "name"));
            w.store();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
	
	public boolean checkValidNick(){
		EditText nick = (EditText)  getActivity().findViewById(R.id.editText);
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