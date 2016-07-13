package jp.ry2kojima.kancAlart;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.os.Vibrator;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.util.Log;
import android.widget.Toast;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

public class KancAlartReceiver extends BroadcastReceiver {
    /**
     * デフォルトコンストラクタ
     */
    public KancAlartReceiver() {
    }

    /**
     * BoradcastReceiver が受信した時に呼ばれるメソッド
     * @param context
     * @param intent
     */
    @Override
    public void onReceive(Context context, Intent intent) {
        // とりあえず、現在日時を Toast で表示
        Calendar now = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("a h 時です");
        Toast.makeText(context, sdf.format(now.getTime()), Toast.LENGTH_LONG).show();

        // 次に通知エリアに通知
        int notificationId = 1;
        Intent bootIntent =new Intent(context, MainActivity.class);
        PendingIntent contentIntent = PendingIntent.getActivity(context, 0, bootIntent, 0);
        NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(context)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle("時報")
                .setContentText(sdf.format(now.getTime()))
                .setContentIntent(contentIntent);
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
        // 通知機能は今のところOFF
        //notificationManager.notify(notificationId, notificationBuilder.build());

        // バイブでお知らせ
        Vibrator vibrator = (Vibrator)context.getSystemService(Context.VIBRATOR_SERVICE);
        vibrator.vibrate(200);

        // 最後に時報音声を鳴らす（OFFじゃなければ）
        SharedPreferences sharedPreferences = context.getSharedPreferences("option", Context.MODE_PRIVATE);
        boolean b = sharedPreferences.getBoolean(String.format("time%02d", now.get(Calendar.HOUR_OF_DAY)), true);
        if (b) {
            MediaPlayer mp = new MediaPlayer();
            String path = context.getFileStreamPath(String.format("%d.mp3", now.get(Calendar.HOUR_OF_DAY) + 30)).getAbsolutePath();
            try {
                mp.setDataSource(path);
                mp.prepare();
                mp.start();
            } catch (IOException e) {
                Log.d("MYTEST", "IOException");
            }
        }
    }
}
