package jp.rkojima.bjkjmjava;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public static String printHexBinary(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    public static byte[] parseHexBinary(String hexText) {
        String hexText2 = hexText.toUpperCase();
        byte[] bytes = new byte[hexText2.length() / 2];
        for (int i = 0; i < hexText2.length(); i += 2) {
            char c1 = hexText2.charAt(i+0);
            char c2 = hexText2.charAt(i+1);
            int index1 = String.valueOf(hexArray).indexOf(String.valueOf(c1));
            int index2 = String.valueOf(hexArray).indexOf(String.valueOf(c2));
            bytes[i / 2] = (byte)((index1 << 4) | index2);
        }
        return bytes;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn;

        // [復号化]ボタンのイベント
        btn = (Button)findViewById(R.id.btnDecrypt);
        btn.setOnClickListener(this);

        // [符号化]ボタンのイベント
        btn = (Button)findViewById(R.id.btnEncrypt);
        btn.setOnClickListener(this);

        //システムのクリップボードを取得
        ClipboardManager cm = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
        Log.d("hoge", "ok1");

        //クリップボードからClipDataを取得
        ClipData cd = cm.getPrimaryClip();
        Log.d("hoge", "ok2");

        //クリップデータからItemを取得
        if(cd != null){
            Log.d("hoge", "ok3");
            ClipData.Item item = cd.getItemAt(0);
            EditText et = (EditText)findViewById(R.id.edtData);
            et.setText(item.getText());
        }
        Log.d("hoge", "ok4");
    }

    public void onClick(View v) {
        int cryptMode = Cipher.ENCRYPT_MODE;
        if (v.getId() == R.id.btnDecrypt) {
            cryptMode = Cipher.DECRYPT_MODE;
        } else if (v.getId() == R.id.btnEncrypt) {
            cryptMode = Cipher.ENCRYPT_MODE;
        }

        EditText tv;
        tv = (EditText)findViewById(R.id.edtPass);
        byte[] key	= tv.getText().toString().getBytes();
        SecretKeySpec keySpec = new SecretKeySpec(key, "Blowfish");

        tv = (EditText)findViewById(R.id.edtData);
        try {
            Cipher cipher = Cipher.getInstance("Blowfish/ECB/PKCS5Padding");
            if (cryptMode == Cipher.ENCRYPT_MODE) {
                cipher.init(cryptMode, keySpec);
                byte[] encoding = cipher.doFinal(tv.getText().toString().getBytes());

                tv.setText(printHexBinary(encoding));
            } else {
                cipher.init(cryptMode, keySpec);
                byte[] decoding = cipher.doFinal(parseHexBinary(tv.getText().toString()));

                tv.setText(new String(decoding));
            }
        } catch (Exception e) {
            Log.d("bfkjmjava", e.getMessage());
        }
    }
}
