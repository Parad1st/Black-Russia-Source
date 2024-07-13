package com.byparad1st.launcher.fragment;

import android.os.Bundle;
import android.os.Environment;

import android.widget.*;

import androidx.fragment.app.Fragment;

import android.view.View;
import android.view.LayoutInflater;
import android.view.ViewGroup;

import java.io.File;
import java.util.Formatter;

import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;

import com.byparad1st.game.R;
import com.byparad1st.launcher.other.Utils;
import com.hzy.libp7zip.P7ZipApi;
import com.liulishuo.filedownloader.BaseDownloadTask;
import com.liulishuo.filedownloader.FileDownloadSampleListener;
import com.liulishuo.filedownloader.FileDownloader;

public class LoaderFragment extends Fragment {
	
	TextView textprogress, textmb, textloading;
	RoundCornerProgressBar progressbar;
                        
	@Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View inflate = inflater.inflate(R.layout.fragment_loader, container, false);
        FileDownloader.setup(getActivity());
        RoundCornerProgressBar progressbar = (RoundCornerProgressBar) inflate.findViewById(R.id.progress_bar);
        TextView textprogress = (TextView) inflate.findViewById(R.id.loading_percent);
        TextView textmb = (TextView) inflate.findViewById(R.id.loading_percent);
        TextView textloading = (TextView) inflate.findViewById(R.id.loading_text);
        startDownload();
        return inflate;
    }
    
    public void startDownload() {
        File folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
        String url = "http://wh3606.web3.maze-host.ru//black-cache.7z";
        createDownloadTask(url, folder.getPath()).start();
    }

    private BaseDownloadTask createDownloadTask(String url, String path) {
        return FileDownloader.getImpl().create(url)
                .setPath(path, true)
                .setCallbackProgressTimes(100)
                .setMinIntervalUpdateSpeed(100)
                .setListener(new FileDownloadSampleListener() {

                    @Override
                    protected void pending(BaseDownloadTask task, int soFarBytes, int totalBytes) {
                        super.pending(task, soFarBytes, totalBytes);
                    }

                    @Override
                    protected void progress(BaseDownloadTask task, int soFarBytes, int totalBytes) {
                        super.progress(task, soFarBytes, totalBytes);
                        long progressPercent = soFarBytes * 100L / totalBytes;

                        textloading.setText("Загрузка файлов игры…");
                        textprogress.setText(new Formatter().format("%.0f%s", new Object[]{Float.valueOf((int)progressPercent), "%"}).toString());
                        textmb.setText(new Formatter().format("%s из %s", new Object[]{Utils.bytesIntoHumanReadable(soFarBytes), Utils.bytesIntoHumanReadable(totalBytes)}).toString());
                        progressbar.setProgress((int) progressPercent);
                    }

                    @Override
                    protected void error(BaseDownloadTask task, Throwable e) {
                        super.error(task, e);
                        Toast.makeText(getActivity(), "Произошла ошибка начните заново установку", Toast.LENGTH_SHORT).show();
                        
                    }

                    @Override
                    protected void connected(BaseDownloadTask task, String et, boolean isContinue, int soFarBytes, int totalBytes) {
                        super.connected(task, et, isContinue, soFarBytes, totalBytes);
                    }

                    @Override
                    protected void paused(BaseDownloadTask task, int soFarBytes, int totalBytes) {
                        super.paused(task, soFarBytes, totalBytes);
                    }

                    @Override
                    protected void completed(BaseDownloadTask task) {
                        super.completed(task);
                        
                        textloading.setText("Распаковка...");
                        textprogress.setText("2/2");
                        textmb.setText("");
                        UnZipCache();
                    }

                    @Override
                    protected void warn(BaseDownloadTask task) {
                        super.warn(task);
                    }
                });
    }

    public void UnZipCache(){
        String mInputFilePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/black-cache.7z";
        String mOutputPath = Environment.getExternalStorageDirectory().toString();
        new Thread() {
            @Override
            public void run() {
                P7ZipApi.executeCommand(String.format("7z x '%s' '-o%s' -aoa", mInputFilePath, mOutputPath));
                Utils.delete(new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/black-cache.7z"));
                Utils.delete(new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/black-cache.7z.temp"));
                getActivity().runOnUiThread(() -> {
                    afterDownload();
                }); 
            }
        }.start();
    }
    
    public void afterDownload(){
    	Toast.makeText(getActivity(), "Игра успешно установлена!", Toast.LENGTH_SHORT).show();
         
    }
}