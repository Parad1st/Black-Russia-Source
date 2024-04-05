package com.blackrussia.launcher.activity;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;
import com.akexorcist.roundcornerprogressbar.indeterminate.IndeterminateCenteredRoundCornerProgressBar;
import com.blackrussia.game.R;
import com.blackrussia.launcher.other.Utils;

import com.hzy.libp7zip.P7ZipApi;
import com.liulishuo.filedownloader.BaseDownloadTask;
import com.liulishuo.filedownloader.FileDownloadSampleListener;
import com.liulishuo.filedownloader.FileDownloader;

import java.io.File;
import java.util.Formatter;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class LoaderActivity extends AppCompatActivity {
    RoundCornerProgressBar progressbar;
    private ExecutorService mExecutor;
    File folder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loader);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        mExecutor = Executors.newSingleThreadExecutor();
        startDownload();
    }

    public void startDownload() {
        folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
        String url = "https://www.dropbox.com/scl/fi/ebtr8cy7k5h0f1ebqwqx4/black-cache.7z.zip?rlkey=xg0w0w01ti8zjm0m0ozixj4jt&dl=1";
        createDownloadTask(url, folder.getPath()).start();
        /*PRDownloader.download(url, folder.getPath(), "black-cache.7z")
				 .build()
				 .setOnStartOrResumeListener(new OnStartOrResumeListener() {
                            @Override
                            public void onStartOrResume() {

                            }
                        })
                        .setOnPauseListener(new OnPauseListener() {
                            @Override
                            public void onPause() {

                            }
                        })
                        .setOnCancelListener(new OnCancelListener() {
                            @Override
                            public void onCancel() {

                            }
                        })
                        .setOnProgressListener(new OnProgressListener() {
                            @Override
                            public void onProgress(Progress progress) {
                               long progressPercent = progress.currentBytes * 100 / progress.totalBytes;

							   RoundCornerProgressBar progressbar = (RoundCornerProgressBar) findViewById(R.id.progressBar);

							   TextView textprogress = (TextView) findViewById(R.id.loadingPercent);
							   TextView textmb = (TextView) findViewById(R.id.fileName);
							   TextView textloading = (TextView) findViewById(R.id.loadingText);

	                           textloading.setText("Идёт загрузка кеша...");
							   textprogress.setText(new Formatter().format("%.0f%s", new Object[]{Float.valueOf((int)progressPercent), "%"}).toString());
							   textmb.setText(new Formatter().format("%s из %s", new Object[]{Utils.bytesIntoHumanReadable(progress.currentBytes), Utils.bytesIntoHumanReadable(progress.totalBytes)}).toString());
                               progressbar.setProgress((int) progressPercent);
                            }
                        })
                        .start(new OnDownloadListener() {
                            @Override
                            public void onDownloadComplete() {
							   TextView textprogress = (TextView) findViewById(R.id.loadingPercent);
							   TextView textmb = (TextView) findViewById(R.id.fileName);
							   TextView textloading = (TextView) findViewById(R.id.loadingText);


							   textloading.setText("Идёт распаковка файлов игры...");
							   textprogress.setText("1/1");
							   textmb.setText("");
                               UnZipCache();
                            }

                            @Override
                            public void onError(Error error) {
                               Toast.makeText(getApplicationContext(), "Произошла ошибка начните заново установку", Toast.LENGTH_SHORT).show();
							   startActivity(new Intent(LoaderActivity.this, MainActivity.class));
                            }
                        });*/
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

                        RoundCornerProgressBar progressbar = (RoundCornerProgressBar) findViewById(R.id.progressBar);

                        TextView textprogress = (TextView) findViewById(R.id.loadingPercent);
                        TextView textmb = (TextView) findViewById(R.id.fileName);
                        TextView textloading = (TextView) findViewById(R.id.loadingText);

                        textloading.setText("Идёт загрузка кеша...");
                        textprogress.setText(new Formatter().format("%.0f%s", new Object[]{Float.valueOf((int)progressPercent), "%"}).toString());
                        textmb.setText(new Formatter().format("%s из %s", new Object[]{Utils.bytesIntoHumanReadable(soFarBytes), Utils.bytesIntoHumanReadable(totalBytes)}).toString());
                        progressbar.setProgress((int) progressPercent);
                    }

                    @Override
                    protected void error(BaseDownloadTask task, Throwable e) {
                        super.error(task, e);
                        Toast.makeText(getApplicationContext(), "Произошла ошибка начните заново установку", Toast.LENGTH_SHORT).show();
                        startActivity(new Intent(LoaderActivity.this, MainActivity.class));

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
                        TextView textprogress = (TextView) findViewById(R.id.loadingPercent);
                        TextView textmb = (TextView) findViewById(R.id.fileName);
                        TextView textloading = (TextView) findViewById(R.id.loadingText);

                        textloading.setText("Идёт распаковка файлов игры...");
                        textprogress.setText("1/1");
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
        new Un7z().execute(mInputFilePath, mOutputPath);
    }

    public class Un7z extends AsyncTask<String, Void, Void> {
        public String str;
        @Override
        protected void onPreExecute() {

        }

        public Void doInBackground(String... strings) {
            String str = strings[0];
            P7ZipApi.executeCommand(String.format("7z x '%s' '-o%s' -aoa", str, strings[1]));
            return null;
        }

        public void onPostExecute(Void aVoid) {
            OnLoaded();
        }
    }
    public void OnLoaded() {
        Utils.delete(new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/black-cache.7z"));
        Utils.delete(new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/black-cache.7z.temp"));
        Toast.makeText(this, "Игра успешно установлена!", Toast.LENGTH_SHORT).show();
        startActivity(new Intent(this, com.blackrussia.launcher.activity.MainActivity.class));
    }
}