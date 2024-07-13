package com.byparad1st.game.gui;

import static java.lang.String.format;

import android.app.Activity;

import com.byparad1st.game.R;
import com.byparad1st.game.gui.adapters.ActionsAdapter;
import com.byparad1st.game.gui.adapters.DonatAdapter;
import com.byparad1st.game.gui.adapters.DonateAdapter;
import com.byparad1st.game.gui.models.Actions;
import com.byparad1st.game.gui.models.Donat;
import com.byparad1st.game.gui.models.Donatee;
import com.nvidia.devtech.NvEventQueueActivity;

import android.graphics.Color;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.*;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.ArrayList;
import java.util.Formatter;
import java.util.List;

public class Donate extends AppCompatActivity {

    static RecyclerView donateRecycler, donatRecycler, actionsRecycler;
    static DonateAdapter donateAdapter;
    static DonatAdapter donatAdapter;
    static ActionsAdapter actionsAdapter;

    public  Activity activity;
    public ConstraintLayout donate_layout, donate_akijas, donate_item, donate_center, donate_down, donate_uslugi, donate_vip;

    public LinearLayout sc_don, podt_don;
    public ImageView donate_close, plusbc, plusrub, back, imgusl;
    public Button sc_close, sc_close2, auto_button, usl_button1, usl_button2, uslugi_buy, button_okusl, button_nousl, vipsil, vipgld, vipplt;
    public TextView  moneyy, bcc, keys, bp, nabori, aks, tovari, actija, vips, skins, sc1, sc2, auto, maintext,  uslugi, topusl, costuscl, costpodtusl;
    public RecyclerView autoRecycler, uslugiRecycler;
    public int money, bc, autooo, uslugi_status, carid, carcost;
    public String costpodt, CarName;
    public Donate(Activity aactivity) {
        activity = aactivity;
        donate_layout = activity.findViewById(R.id.donate_parad1st);
        donate_item = activity.findViewById(R.id.donate_item);
        donate_center = activity.findViewById(R.id.constraintLayout2);
        donate_akijas = activity.findViewById(R.id.constraintLayout3);
        donate_vip = activity.findViewById(R.id.constraintLayout4);
        donate_uslugi = activity.findViewById(R.id.uslugii);
        sc_don = activity.findViewById(R.id.donate_sc);
        podt_don = activity.findViewById(R.id.donate_podt);
        donate_close = activity.findViewById(R.id.donate_close);
        imgusl = activity.findViewById(R.id.imgusl);
        plusbc = activity.findViewById(R.id.plusbc);
        plusrub = activity.findViewById(R.id.plusrub);
        moneyy = activity.findViewById(R.id.moneydon);
        donate_down = activity.findViewById(R.id.downnn);
        bcc = activity.findViewById(R.id.bcdon);
        keys = activity.findViewById(R.id.textView9);
        bp = activity.findViewById(R.id.textView10);
        nabori = activity.findViewById(R.id.textView13);
        aks = activity.findViewById(R.id.textView11);
        topusl = activity.findViewById(R.id.toptextuslugi);
        costuscl = activity.findViewById(R.id.costusl);
        tovari = activity.findViewById(R.id.textView5);
        actija = activity.findViewById(R.id.textView33);
        auto = activity.findViewById(R.id.textView15);
        vips = activity.findViewById(R.id.textView12);
        skins = activity.findViewById(R.id.textView14);
        costpodtusl = activity.findViewById(R.id.podt3);
        maintext = activity.findViewById(R.id.textView8);
       // autoo = activity.findViewById(R.id.textView15);
        uslugi = activity.findViewById(R.id.textView7);
        sc_close = activity.findViewById(R.id.closesc);
        sc_close2 = activity.findViewById(R.id.butonok);
        button_okusl = activity.findViewById(R.id.butonokk);
        button_nousl = activity.findViewById(R.id.butonnoo);
        vipsil = activity.findViewById(R.id.donate_button1);
        vipgld = activity.findViewById(R.id.donate_button2);
        vipplt = activity.findViewById(R.id.donate_button3);
        auto_button = activity.findViewById(R.id.donate_buton);
        uslugi_buy = activity.findViewById(R.id.donate_butееon);
        back = activity.findViewById(R.id.imageView22);
        sc1 = activity.findViewById(R.id.sc1);
        sc2 = activity.findViewById(R.id.sc2);
        autoRecycler = activity.findViewById(R.id.autoRecycler);
        uslugiRecycler = activity.findViewById(R.id.UslugiRecycler);
        usl_button1 = activity.findViewById(R.id.button_leftt);
        usl_button2 = activity.findViewById(R.id.button_rightt);
        donate_layout.setVisibility(View.GONE);
        sc_don.setVisibility(View.GONE);
        podt_don.setVisibility(View.GONE);
        setListeners(aactivity);

    }
    public void setListeners(Activity aactivity) {
        activity = aactivity;
        donate_close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                close();
            }
        });
        plusbc.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(0);
            }
        });
        plusrub.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(1);
            }
        });
        keys.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(2);
            }
        });
        bp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(3);
            }
        });
        nabori.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(4);
            }
        });
        skins.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                maintext.setText("СКИНЫ");
                auto.setBackgroundColor(Color.parseColor("#00990055"));
                auto.setTextColor(Color.parseColor("#ff666666"));
                vips.setBackgroundColor(Color.parseColor("#00990055"));
                vips.setTextColor(Color.parseColor("#ff666666"));
                actija.setBackgroundColor(Color.parseColor("#00990055"));
                actija.setTextColor(Color.parseColor("#ff666666"));
                skins.setTextColor(Color.parseColor("#ffffff"));
                skins.setBackgroundColor(Color.parseColor("#ffff4500"));
                close_auto();
                close_actions();
                show_skins();
                donate_uslugi.setVisibility(View.GONE);
                back.setVisibility(View.GONE);
                donate_vip.setVisibility(View.GONE);
                donate_akijas.setVisibility(View.GONE);
            }
        });
        aks.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(5);
            }
        });
        actija.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                maintext.setText("АКЦИИ И СПЕЦПРЕДЛОЖЕНИЯ");
                auto.setBackgroundColor(Color.parseColor("#00990055"));
                auto.setTextColor(Color.parseColor("#ff666666"));
                vips.setBackgroundColor(Color.parseColor("#00990055"));
                vips.setTextColor(Color.parseColor("#ff666666"));
                skins.setBackgroundColor(Color.parseColor("#00990055"));
                skins.setTextColor(Color.parseColor("#ff666666"));
                actija.setTextColor(Color.parseColor("#ffffff"));
                actija.setBackgroundColor(Color.parseColor("#ffff4500"));
                close_auto();
                show_actions();
                donate_uslugi.setVisibility(View.GONE);
                back.setVisibility(View.GONE);
                donate_vip.setVisibility(View.GONE);
                donate_akijas.setVisibility(View.VISIBLE);
            }
        });
        vips.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                maintext.setText("VIP ПАКЕТЫ");
                auto.setBackgroundColor(Color.parseColor("#00990055"));
                auto.setTextColor(Color.parseColor("#ff666666"));
                actija.setBackgroundColor(Color.parseColor("#00990055"));
                actija.setTextColor(Color.parseColor("#ff666666"));
                vips.setTextColor(Color.parseColor("#ffffff"));
                vips.setBackgroundColor(Color.parseColor("#ffff4500"));
                skins.setBackgroundColor(Color.parseColor("#00990055"));
                skins.setTextColor(Color.parseColor("#ff666666"));
                close_auto();
                donate_uslugi.setVisibility(View.GONE);
                back.setVisibility(View.GONE);
                donate_akijas.setVisibility(View.GONE);
                donate_vip.setVisibility(View.VISIBLE);
            }
        });
        vipsil.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(14);
            }
        });
        vipgld.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(15);
            }
        });
        vipplt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().sendDonateClick(16);
            }
        });
        sc_close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                close_sc();
            }
        });
        sc_close2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                close_sc();
            }
        });
        auto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                donate_akijas.setVisibility(View.GONE);
                maintext.setText("ТРАНСПОРТ");
                actija.setBackgroundColor(Color.parseColor("#00990055"));
                actija.setTextColor(Color.parseColor("#ff666666"));
                vips.setBackgroundColor(Color.parseColor("#00990055"));
                vips.setTextColor(Color.parseColor("#ff666666"));
                skins.setBackgroundColor(Color.parseColor("#00990055"));
                skins.setTextColor(Color.parseColor("#ff666666"));
                auto.setTextColor(Color.parseColor("#ffffff"));
                auto.setBackgroundColor(Color.parseColor("#ffff4500"));
                back.setVisibility(View.GONE);
                donate_uslugi.setVisibility(View.GONE);
                donate_vip.setVisibility(View.GONE);
                show_auto();
                if(autooo == 1) {
                    back.setVisibility(View.VISIBLE);
                }
            }
        });
        uslugi.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                tovari.setBackgroundColor(Color.parseColor("#00990055"));
                tovari.setTextColor(Color.parseColor("#ff666666"));
                uslugi.setTextColor(Color.parseColor("#ffffff"));
                uslugi.setBackgroundColor(Color.parseColor("#ffff4500"));
                donate_center.setVisibility(View.GONE);
                donate_down.setVisibility(View.GONE);
                maintext.setVisibility(View.GONE);
                back.setVisibility(View.GONE);
                donate_uslugi.setVisibility(View.VISIBLE);
                show_uslugi();
                autooo = 0;
            }
        });
        uslugi_buy.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                podt_don.setVisibility(View.VISIBLE);
                costpodtusl.setText(costpodt + " BC?");
            }
        });
        button_nousl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                podt_don.setVisibility(View.GONE);
            }
        });
        button_okusl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                podt_don.setVisibility(View.GONE);
                if(autooo == 1) {
                        NvEventQueueActivity.getInstance().buycar(carid, carcost,  1);
                }
                else {
                    if(uslugi_status != 0) NvEventQueueActivity.getInstance().sendDonateClick(uslugi_status + 5);
                    else NvEventQueueActivity.getInstance().sendDonateClick(6);
                }

            }
        });
        tovari.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                uslugi.setBackgroundColor(Color.parseColor("#00990055"));
                uslugi.setTextColor(Color.parseColor("#ff666666"));
                tovari.setTextColor(Color.parseColor("#ffffff"));
                tovari.setBackgroundColor(Color.parseColor("#ffff4500"));
                donate_uslugi.setVisibility(View.GONE);
                donate_center.setVisibility(View.VISIBLE);
                donate_down.setVisibility(View.VISIBLE);
                maintext.setVisibility(View.VISIBLE);
                if(autooo == 1) {
                    back.setVisibility(View.VISIBLE);
                }
            }
        });
        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                back.setVisibility(View.GONE);
                close_auto();
                show_auto();
            }
        });
    }
    public void show(int money, int bc) {

        donate_layout.setVisibility(View.VISIBLE);
        this.money = money;
        this.bc = bc;
        DecimalFormat formatter = new DecimalFormat();
        DecimalFormatSymbols symbols = DecimalFormatSymbols.getInstance();
        symbols.setGroupingSeparator(' ');
        formatter.setDecimalFormatSymbols(symbols);
        String mon = formatter.format(money);
        String bcs = formatter.format(bc);
        moneyy.setText(mon);
        bcc.setText(bcs);
        show_actions();
        //moneyy.setText(new Formatter().format("%d%s", Integer.valueOf(money), "").toString());
        //bcc.setText(new Formatter().format("%d%s", Integer.valueOf(bc), "").toString());
        NvEventQueueActivity.getInstance().togglePlayer(1);
    }

    public void close() {
        donate_layout.setVisibility(View.GONE);
        NvEventQueueActivity.getInstance().togglePlayer(0);
    }

    public void update(int money, int bc) {
        this.money = money;
        this.bc = bc;
        DecimalFormat formatter = new DecimalFormat();
        DecimalFormatSymbols symbols = DecimalFormatSymbols.getInstance();
        symbols.setGroupingSeparator(' ');
        formatter.setDecimalFormatSymbols(symbols);
        String mon = formatter.format(money);
        String bcs = formatter.format(bc);
        moneyy.setText(mon);
        bcc.setText(bcs);
        //moneyy.setText(new Formatter().format("%d%s", Integer.valueOf(money), "").toString());
        //bcc.setText(new Formatter().format("%d%s", Integer.valueOf(bc), "").toString());
    }
    public void show_sc(int money, int bc) {
        this.money = money;
        this.bc = bc;
        sc_don.setVisibility(View.VISIBLE);
        if(money == -1) {
            sc1.setText("ВЫ УСПЕШНО ПРИОБРЕЛИ ДАННЫЙ ТОВАР");
            sc2.setText("СПАСИБО ЗА ОПЕРАЦИЮ!");
        }
        else if(money == -777) {
            sc1.setText(format("ВЫ УСПЕШНО ПРИОБРЕЛИ %s", getNameAuto()));
            sc2.setText("ДЛЯ ТОГО ЧТОБЫ ЗАГРУЗИТЬ АВТО,\nИСПОЛЬЗУЙТЕ /CAR");
        }
        else if(money == -666) {
            sc1.setText("ВЫ УСПЕШНО ПРИОБРЕЛИ НОВУЮ ОДЕЖДУ\nОНА НАДЕТА НА ВАС");
            sc2.setText("ТЕПЕРЬ ВЫ ВЫГЛЯДИТЕ КАК\nНАСТОЯЩИЙ МОДНИК!");
        }
        else {
            sc1.setText(new Formatter().format("ВЫ ПЕРЕВЕЛИ %d BC В %d РУБЛЕЙ", Integer.valueOf(money), Integer.valueOf(bc)).toString());
            sc2.setText("СПАСИБО ЗА ОПЕРАЦИЮ!");
        }

    }
    public void close_sc() {
        sc_don.setVisibility(View.GONE);
    }


    private void setDonateRecycler(List<Donatee> donateList) {
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager (this, RecyclerView.HORIZONTAL, false);

        donateRecycler = activity.findViewById(R.id.autoRecycler);
        donateRecycler.setLayoutManager(layoutManager);

        donateAdapter = new DonateAdapter(activity, donateList);
        donateRecycler.setAdapter(donateAdapter);
    }
    private void setActionRecycler(List<Actions> actionsList) {
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager (this, RecyclerView.HORIZONTAL, false);

        actionsRecycler = activity.findViewById(R.id.ActionsRecycler);
        actionsRecycler.setLayoutManager(layoutManager);

        actionsAdapter = new ActionsAdapter(activity, actionsList);
        actionsRecycler.setAdapter(actionsAdapter);


    }

    private void setDonatRecycler(List<Donat> donatList) {
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager (this, RecyclerView.VERTICAL, false);

        donatRecycler = activity.findViewById(R.id.UslugiRecycler);
        donatRecycler.setLayoutManager(layoutManager);

        donatAdapter = new DonatAdapter(activity, donatList);
        donatRecycler.setAdapter(donatAdapter);
    }
    public void show_uslugi() {

        List<Donat> donatList = new ArrayList<>();
        donatList.add(new Donat(1, "ИЗМЕНИТЬ ИМЯ", "50", "change_nickname"));
        donatList.add(new Donat(2, "СНЯТЬ ВАРН", "100", "delete_warn"));
        donatList.add(new Donat(3, "ВСЕ ЛИЦЕНЗИИ", "150", "licenses"));
        donatList.add(new Donat(4, "ПОКУПКА СИЛЫ", "300", "power"));
        donatList.add(new Donat(5, "САБВУФЕР PIONEER", "1000", "subwoofer"));
        donatList.add(new Donat(6, "ДОП. СЛОТ НА АВТО", "100", "add_car_slot"));
        donatList.add(new Donat(7, "НОМЕРА НА АВТО", "85", "buy_car_number"));
        donatList.add(new Donat(8, "НОМЕР ТЕЛЕФОНА", "70", "phone_number"));
        setDonatRecycler(donatList);
        uslugiRecycler.setVisibility(View.VISIBLE);
        topusl.setText("ИЗМЕНИТЬ ИМЯ");
        costuscl.setText("50 BC");
        imgusl.setImageResource(R.drawable.change_nickname);
        costpodt = "50";
        uslugi_status = 1;
    }

    public void upd_usl(String name, String cost, String img, int id) {

        this.uslugi_status = id;
        this.costpodt = cost;
        topusl.setText(name);
        costuscl.setText(cost + " BC");
        int imgid = activity.getResources().getIdentifier(img, "drawable", activity.getPackageName());
        imgusl.setImageResource(imgid);
    }
    public void show_auto() {

        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(1, "ЭКОНОМ", "", "", "auto_econom", 0));
        donateList.add(new Donatee(2, "СРЕДНИЙ", "","", "auto_middle",0 ));
        donateList.add(new Donatee(3, "ПРЕМИУМ", "","", "auto_premium",0 ));
        donateList.add(new Donatee(4, "МОТО", "","","auto_moto", 0));
        donateList.add(new Donatee(5, "УНИКАЛЬНЫЕ","", "","auto_unique",0 ));
        donateList.add(new Donatee(6, "ГРУЗОВОЙ","", "","auto_gruz", 0));
        donateList.add(new Donatee(7, "ЯХТЫ", "","","auto_yacht", 0));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
    }
    public void close_auto() {

        autoRecycler.setVisibility(View.GONE);
        autooo = 0;
    }
    public void close_actions() {

        actionsRecycler.setVisibility(View.GONE);
        autooo = 0;
    }
    public  void show_nizk() {

        close_auto();
        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(8, "ЗАЗ", "968М","auto_zaz", "",10));
        donateList.add(new Donatee(9, "ВАЗ", "1111", "auto_1111","", 15));
        donateList.add(new Donatee(10, "ВАЗ", "2101", "auto_2101", "",30));
        donateList.add(new Donatee(11, "ГАЗ", "Волга 2410", "auto_volga","", 40));
        donateList.add(new Donatee(12, "ВАЗ", "2106", "auto_2106", "",42));
        donateList.add(new Donatee(13, "VOLKSWAGEN", "Golf GTI 2", "auto_golfnizk","", 70));
        donateList.add(new Donatee(14, "ВАЗ", "2107", "auto_2107","", 75));
        donateList.add(new Donatee(15, "ВАЗ", "2108", "auto_2108", "",85));
        donateList.add(new Donatee(16, "ВАЗ", "2109", "auto_2108","", 90));
        donateList.add(new Donatee(17, "ВАЗ", "2110", "auto_2110","", 95));
        donateList.add(new Donatee(18, "ВАЗ", "2114", "auto_2114","",97));
        donateList.add(new Donatee(19, "ВАЗ", "2115", "auto_2115","", 105));
        donateList.add(new Donatee(20, "VOLVO", "242 DL", "auto_volvo242dl", "",125));
        donateList.add(new Donatee(21, "ВАЗ", "2170", "auto_priora", "",150));
        donateList.add(new Donatee(22, "BMW", "M3 E30", "auto_bmwe30","", 150));
        donateList.add(new Donatee(23, "НИВА", "Urban", "auto_niva", "",175));
        donateList.add(new Donatee(24, "MAZDA", "Sedan 3", "auto_mazda", "",190));
        donateList.add(new Donatee(25, "TOYOTA", "Mark II", "auto_mark2", "",210));
        donateList.add(new Donatee(26, "MERSEDES-BENZ", "W124", "auto_mersw124", "",220));
        donateList.add(new Donatee(27, "MERSEDES-BENZ", "E420 W210", "auto_mersw210", "",350));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
        back.setVisibility(View.VISIBLE);
        autooo = 1;
    }

    public  void show_sredn() {
        close_auto();
        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(28, "NISSAN", "Skyline R34","auto_skyline", "",340));
        donateList.add(new Donatee(29, "AUDI", "A4", "auto_audia4", "",375));
        donateList.add(new Donatee(30, "BMW", "M5 E39", "auto_bmwe39","", 400));
        donateList.add(new Donatee(31, "HYUNDAI", "Solaris", "auto_hyndai", "",415));
        donateList.add(new Donatee(32, "VOLKSWAGEN", "Polo", "auto_polo", "",425));
        donateList.add(new Donatee(33, "MERSEDES-BENZ", "S600 W140", "auto_mersw140", "",450));
        donateList.add(new Donatee(35, "BMW", "M3 E46", "auto_bmwe46", "",525));
        donateList.add(new Donatee(36, "ACURA", "TSX", "auto_acura", "",575));
        donateList.add(new Donatee(37, "VOLKSWAGEN", "GOLF GTI", "auto_goldsredn","",700));
        donateList.add(new Donatee(34, "FORD", "Focus RS", "auto_focus", "",900));
        donateList.add(new Donatee(38, "MITSUBISHI", "Lancer EVO X", "auto_lanser", "",1000));
        donateList.add(new Donatee(39, "SUBARU", "WRX STI", "auto_subaru", "",1100));
        donateList.add(new Donatee(40, "TOYOTA", "Camry 2019", "auto_camry","", 1200));
        donateList.add(new Donatee(41, "FORD", "Mustang GT", "auto_mustang", "",1300));
        donateList.add(new Donatee(42, "MERSEDES-BENZ", "A45 AMG", "auto_mersa45", "",1400));
        donateList.add(new Donatee(43, "ALFA ROMEO", "Gullia", "auto_alfa","", 1500));
        donateList.add(new Donatee(44, "INFINITI", "Q60S", "auto_infiniti", "",1600));
        donateList.add(new Donatee(45, "VOLVO", "V60 T6", "auto_v60", "",1750));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
        back.setVisibility(View.VISIBLE);
        autooo = 1;
    }

    public  void show_visok() {
        close_auto();
        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(46, "VOLVO", "XC90", "auto_xc90", "",2100));
        donateList.add(new Donatee(47, "CHEVROLET", "Camaro ZL1", "auto_camaro", "",2300));
        donateList.add(new Donatee(48, "BMW", "Z4 M4Oi", "auto_bmwz4","", 2450 ));
        donateList.add(new Donatee(49, "BMW", "M4 F84", "auto_bmwm4", "",2750));
        donateList.add(new Donatee(50, "FORD", "Raptor F-150", "auto_raptor", "",2880));
        donateList.add(new Donatee(51, "AUDI", "Q8", "auto_audiq7", "",3000));
        donateList.add(new Donatee(52, "DODGE DEMON", "SRT", "auto_dodge", "",3050));
        donateList.add(new Donatee(53, "MERSEDES-BENZ", "GT63s", "auto_mersgt63s","", 3500));
        donateList.add(new Donatee(54, "CADILAC", "Escalade", "auto_kadilac","", 3600));
        donateList.add(new Donatee(55, "NISSAN", "GT-R R35", "auto_nisangtr", "",3950));
        donateList.add(new Donatee(56, "BMW", "X6M F16", "auto_bmwx6m", "",4100));
        donateList.add(new Donatee(57, "PORSCHE", "Panamera S", "auto_porshpanamera", "",4200));
        donateList.add(new Donatee(58, "PORSCHE", "911 Carrera", "auto_porsh911", "",4500));;
        donateList.add(new Donatee(59, "BMW", "M5 F90", "auto_bmwf90", "",4750));
        donateList.add(new Donatee(60, "AUDI", "RS6 C7", "auto_audirc6", "",5000));
        donateList.add(new Donatee(61, "RANGE ROVER", "SVR", "auto_range", "",5000));
        donateList.add(new Donatee(62, "MERSEDES-BENZ", "CLS63 AMG", "auto_merscls63", "",5150));
        donateList.add(new Donatee(63, "AUDI", "R8 V10", "auto_audir8", "",5500));
        donateList.add(new Donatee(64, "BMW", "i8 drive", "auto_bmwi8", "",6300));
        donateList.add(new Donatee(65, "MERSEDES-BENZ", "GT-R", "auto_mersgtr", "",6750));
        donateList.add(new Donatee(66, "MCLAREN", "600 LT", "auto_maclaren", "",7000));
        donateList.add(new Donatee(67, "MERSEDES-BENZ", "G65 AMG", "auto_gelik", "", 8525));
        donateList.add(new Donatee(68, "FERRARI", "488 GTB", "auto_fera", "",8750));
        donateList.add(new Donatee(69, "ASTON MARTIN", "DB11", "auto_aston", "",9250));
        donateList.add(new Donatee(70, "LAMBORGINI ", "Aventador S", "auto_lamba", "",10000));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
        back.setVisibility(View.VISIBLE);
        autooo = 1;
    }

    public  void show_moto() {
// donateList.add(new Donatee(11, "", "", "auto_", " BC"));
        close_auto();
        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(71, "RACER SPORT", "Скутер", "auto_racer", "",10));
        donateList.add(new Donatee(72, "APRILLA", "MXV 450", "auto_aprilla", "",45));
        donateList.add(new Donatee(73, "DUCATI", "SuperSport S", "auto_ducatisupersport","", 915));
        donateList.add(new Donatee(74, "DUCATI", "XDiavel S", "auto_ducatixdiavels", "",1000));
        donateList.add(new Donatee(75, "YAMAHA", "FZ-10", "auto_yamahafz10", "",2250));
        donateList.add(new Donatee(76, "BMW", "S 1000 RR", "auto_bmws1000rr", "",3500));
        donateList.add(new Donatee(77, "SUZUKI", "GSX-R750", "auto_suzukigsxr750", "",4000));
        donateList.add(new Donatee(78, "KAWASAKI", "Ninja H2R", "auto_kawasakininja", "",5000));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
        back.setVisibility(View.VISIBLE);
        autooo = 1;
    }
    public  void show_skins() {
        close_auto();
        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(83, "ФИРМЕННАЯ\nОДЕЖДА", "Скин администратора", "skin_adm", "",30000));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
        back.setVisibility(View.VISIBLE);
        autooo = 1;
    }
    public  void show_uniq() {
        close_auto();
        List<Donatee> donateList = new ArrayList<>();
        donateList.add(new Donatee(79, "ВАЗ", "2170 (ППС)", "auto_priora", "",20000));
        donateList.add(new Donatee(80, "ГАЗ", "ТИГР", "auto_tigr", "",30000));
        donateList.add(new Donatee(81, "VOLKSWAGEN", "Polo (ППС)", "auto_polo","", 35000));
        donateList.add(new Donatee(82, "BMW", "M5 F90 (ППС)", "auto_bmwf90", "",50000));
        setDonateRecycler(donateList);
        autoRecycler.setVisibility(View.VISIBLE);
        back.setVisibility(View.VISIBLE);
        autooo = 1;
    }

    public void buy_carpodt(int cost, int id) {
        this.carcost = cost;
        this.carid = id;
        autooo = 1;
        podt_don.setVisibility(View.VISIBLE);
        costpodtusl.setText(cost + " BC?");
    }

    public void GetCarName(String name) {
        this.CarName = name;
    }
    public String getNameAuto() {
        return CarName;
    }

    public void show_actions() {

        List<Actions> actionList = new ArrayList<>();
        actionList.add(new Actions(28, "ДЛЯ ВАС", "Nissan", "auto_skyline", 327));
        actionList.add(new Actions(54, "АКЦИЯ", "Cadilac", "auto_kadilac", 3300));

        setActionRecycler(actionList);
        actionsRecycler.setVisibility(View.VISIBLE);
    }

}