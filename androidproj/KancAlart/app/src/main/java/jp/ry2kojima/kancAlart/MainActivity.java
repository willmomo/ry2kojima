package jp.ry2kojima.kancAlart;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Environment;
import android.provider.ContactsContract;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URLDecoder;
import java.nio.channels.FileChannel;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends ActionBarActivity
        implements View.OnLongClickListener, View.OnClickListener, DirSelectDialog.OnDirSelectDialogListener {

    // Switch のリソースID と設定キーを対で持つ Map
    private static Map<Integer, String> m_map = new HashMap<Integer, String>() {
        {put(R.id.swUse, "useAlarm");}
        {put(R.id.swTime00, "time00");}
        {put(R.id.swTime01, "time01");}
        {put(R.id.swTime02, "time02");}
        {put(R.id.swTime03, "time03");}
        {put(R.id.swTime04, "time04");}
        {put(R.id.swTime05, "time05");}
        {put(R.id.swTime06, "time06");}
        {put(R.id.swTime07, "time07");}
        {put(R.id.swTime08, "time08");}
        {put(R.id.swTime09, "time09");}
        {put(R.id.swTime10, "time10");}
        {put(R.id.swTime11, "time11");}
        {put(R.id.swTime12, "time12");}
        {put(R.id.swTime13, "time13");}
        {put(R.id.swTime14, "time14");}
        {put(R.id.swTime15, "time15");}
        {put(R.id.swTime16, "time16");}
        {put(R.id.swTime17, "time17");}
        {put(R.id.swTime18, "time18");}
        {put(R.id.swTime19, "time19");}
        {put(R.id.swTime20, "time20");}
        {put(R.id.swTime21, "time21");}
        {put(R.id.swTime22, "time22");}
        {put(R.id.swTime23, "time23");}
    };

    public void importVoice(String srcPath) {
        for (int i = 30; i <= 53; i++) {
            File file = getFileStreamPath(String.format("%d.mp3", i));
            String src = srcPath + String.format("/%d.mp3", i);

            Log.d("MYTEST", src);
            Log.d("MYTEST", file.getAbsolutePath());

            FileChannel inputChannel = null;
            FileChannel outputChannel = null;
            try {
                inputChannel = new FileInputStream(src).getChannel();
                outputChannel = new FileOutputStream(file.getAbsolutePath()).getChannel();
                outputChannel.transferFrom(inputChannel, 0, inputChannel.size());
                inputChannel.close();
                outputChannel.close();
            } catch (IOException e) {
                Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT).show();
            } finally {
            }
        }
    }

    @Override
    public void onClickDirSelect(File file) {
        if (file != null) {
            Toast.makeText(this, file.getAbsolutePath(), Toast.LENGTH_SHORT).show();
            importVoice(file.getAbsolutePath());
        }
    }

    @Override
    public boolean onLongClick(View v) {
        Pattern pattern = Pattern.compile("^time([0-9][0-9])$");
        Matcher matcher = pattern.matcher(m_map.get(v.getId()));

        if (matcher.find()) {
            String path = getFileStreamPath(String.format("%02d.mp3", Integer.valueOf(matcher.group(1)) + 30)).getAbsolutePath();

            Toast.makeText(this, String.format("再生中 %s", new File(path).getName()), Toast.LENGTH_SHORT).show();

            MediaPlayer mediaPlayer = new MediaPlayer();
            try {
                mediaPlayer.setDataSource(path);
                mediaPlayer.prepare();
                mediaPlayer.start();
            } catch (IOException e) {
                Log.d("MYTEST", "IOException");
            }
            return true;
        }
        return false;
    }

    /**
     * 各種コントロールのクリック処理
     * @param v
     */
    private final static int s_RequestCode = 12345;
    public void onClick(View v) {
        SharedPreferences sharedPreferences = getSharedPreferences("option", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        if (v.getId() == R.id.button) {
            /*
            DirSelectDialog dialog = new DirSelectDialog(this);
            dialog.setOnDirSelectDialogListener(this);
            dialog.show(Environment.getExternalStorageDirectory().getPath());
            */
            // zipファイル選択バージョンを作成
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("file/*");
            startActivityForResult(intent, s_RequestCode);
        } else if (v.getId() == R.id.swUse) {
            Boolean checked = ((Switch) v).isChecked();
            if (checked) {
                setAlarm();
            } else {
                cancelAlarm();
            }
            editor.putBoolean(m_map.get(v.getId()), checked);
            editor.apply();
        } else {
            editor.putBoolean(m_map.get(v.getId()), ((Switch) v).isChecked());
            editor.apply();
        }
    }

    /*
     * ファイルおよびディレクトリを削除する
     */
    private static void delete(File f)
    {
        /*
         * ファイルまたはディレクトリが存在しない場合は何もしない
         */
        if(f.exists() == false) {
            return;
        }

        if(f.isFile()) {
            /*
             * ファイルの場合は削除する
             */
            f.delete();

        } else if(f.isDirectory()){

            /*
             * ディレクトリの場合は、すべてのファイルを削除する
             */

            /*
             * 対象ディレクトリ内のファイルおよびディレクトリの一覧を取得
             */
            File[] files = f.listFiles();

            /*
             * ファイルおよびディレクトリをすべて削除
             */
            for(int i=0; i<files.length; i++) {
                /*
                 * 自身をコールし、再帰的に削除する
                 */
                delete( files[i] );
            }
            /*
             * 自ディレクトリを削除する
             */
            f.delete();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        try {
            if (requestCode == s_RequestCode && resultCode == RESULT_OK) {
                String filePath = data.getDataString();
                filePath = filePath.replace("file://", "");
                filePath = URLDecoder.decode(filePath, "utf-8");
                Toast.makeText(this, filePath, Toast.LENGTH_SHORT).show();

                String zipFile = filePath;
                String unzipLocation = Environment.getExternalStorageDirectory() + "/unzipped";
                File f = new File(unzipLocation);
                delete(f);
                Decompress d = new Decompress(zipFile, unzipLocation + "/");
                d.unzip("^[345][0-9]\\.mp3$");
                importVoice(unzipLocation);
                Toast.makeText(this, unzipLocation, Toast.LENGTH_SHORT).show();
                delete(f);
            }
        } catch (Exception ex) {
            Toast.makeText(this, ex.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 設定に合わせてコントロールの状態を初期化する
     * @param id
     * @param key
     */
    private void initSwitch(int id, String key){
        SharedPreferences sharedPreferences = getSharedPreferences("option", Context.MODE_PRIVATE);
        Switch ctrl = (Switch)findViewById(id);
        ctrl.setChecked(sharedPreferences.getBoolean(key, id != R.id.swUse));
        ctrl.setOnClickListener(this);
        ctrl.setOnLongClickListener(this);
        if (id == R.id.swUse) {
            if (ctrl.isChecked()) {
                setAlarm();
            } else {
                cancelAlarm();
            }
        }
    }

    private int getResID(int id) {
        int ret = 0;
        switch (id) {
            case 30:    ret = R.raw.v30;    break;
            case 31:    ret = R.raw.v31;    break;
            case 32:    ret = R.raw.v32;    break;
            case 33:    ret = R.raw.v33;    break;
            case 34:    ret = R.raw.v34;    break;
            case 35:    ret = R.raw.v35;    break;
            case 36:    ret = R.raw.v36;    break;
            case 37:    ret = R.raw.v37;    break;
            case 38:    ret = R.raw.v38;    break;
            case 39:    ret = R.raw.v39;    break;
            case 40:    ret = R.raw.v40;    break;
            case 41:    ret = R.raw.v41;    break;
            case 42:    ret = R.raw.v42;    break;
            case 43:    ret = R.raw.v43;    break;
            case 44:    ret = R.raw.v44;    break;
            case 45:    ret = R.raw.v45;    break;
            case 46:    ret = R.raw.v46;    break;
            case 47:    ret = R.raw.v47;    break;
            case 48:    ret = R.raw.v48;    break;
            case 49:    ret = R.raw.v49;    break;
            case 50:    ret = R.raw.v50;    break;
            case 51:    ret = R.raw.v51;    break;
            case 52:    ret = R.raw.v52;    break;
            case 53:    ret = R.raw.v53;    break;
        }
        return ret;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        {
            // raw/*.mp3をfilesにコピーする
            for (int i = 30; i <= 53; i++) {
                File file = getFileStreamPath(String.format("%d.mp3", i));

                Log.d("MYTEST", file.getAbsolutePath());
                if (!file.exists()) {
                    Log.d("MYTEST", "ファイルが無い");
                    Resources res = getResources();
                    InputStream ins = getResources().openRawResource(getResID(i));
                    ByteArrayOutputStream outputStream=new ByteArrayOutputStream();
                    int size = 0;
                    byte[] buffer = new byte[1024];

                    try {
                        while ((size = ins.read(buffer, 0, 1024)) >= 0) {
                            outputStream.write(buffer, 0, size);
                        }
                        ins.close();
                        buffer = outputStream.toByteArray();

                        FileOutputStream fos = openFileOutput(String.format("%d.mp3", i), MODE_PRIVATE);
                        fos.write(buffer);
                        fos.close();
                    } catch (IOException e) {
                        Log.d("MYTEST", "異常発生");
                    }
                }
            }
        }

        // 各時間のOFF/ONを設定に合わせて調整する
        for (Map.Entry<Integer, String> e : m_map.entrySet()) {
            initSwitch(e.getKey(), e.getValue());
        }

        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(this);
    }

    /**
     * BroadcastReceiver を AlartManager に登録
     */
    protected void setAlarm() {
        // BroadcastReceiver を AlarmManager に登録
        Intent intent = new Intent(MainActivity.this, KancAlartReceiver.class);
        PendingIntent alarmIntent = PendingIntent.getBroadcast(MainActivity.this, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);
        AlarmManager alarmManager = (AlarmManager)(getBaseContext().getSystemService(ALARM_SERVICE));

        // 現在時刻
        Calendar cal_now = Calendar.getInstance();

        // 未来の00分
        Calendar cal_target = Calendar.getInstance();
        cal_target.add(Calendar.HOUR_OF_DAY, 1);   // １時間進める
        cal_target.set(Calendar.MINUTE, 0);    // 0分
        cal_target.set(Calendar.SECOND, 0);     // ０秒に設定

        // cal_now - (cal_target - cal_now) が、初回 Alarm のタイミング
        alarmManager.setRepeating(AlarmManager.RTC_WAKEUP,
                cal_now.getTimeInMillis() + (cal_target.getTimeInMillis() - cal_now.getTimeInMillis()),
                AlarmManager.INTERVAL_HOUR, alarmIntent);

        Toast.makeText(this, "時報を有効にしました", Toast.LENGTH_SHORT).show();
    }

    /**
     * 登録してある BroadcastReciever を取り除く
     */
    protected void cancelAlarm() {
        //Context context = getBaseContext();
        Intent intent = new Intent(MainActivity.this, KancAlartReceiver.class);
        AlarmManager alarmManager = (AlarmManager)getSystemService(ALARM_SERVICE);
        PendingIntent alarmIntent = PendingIntent.getBroadcast(MainActivity.this, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);
        alarmManager.cancel(alarmIntent);

        Toast.makeText(this, "時報を無効にしました", Toast.LENGTH_SHORT).show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
