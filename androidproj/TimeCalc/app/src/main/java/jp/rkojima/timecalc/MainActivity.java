package jp.rkojima.timecalc;

import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.Context;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Calendar;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends ActionBarActivity implements View.OnClickListener, View.OnKeyListener {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // ボタンのイベント処理追加
        Button btn = (Button) findViewById(R.id.btnCalc);
        btn.setOnClickListener(this);

        // 最後の時間入力エリアのキー入力イベントを追加」
        EditText et = (EditText)findViewById(R.id.etTime2);
        et.setOnKeyListener(this);
    }

    /** アクティビティがデータを一時保存する時に呼ばれます。 */
    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        // Bundleに値を保存する
        TextView tv6 = (TextView)findViewById(R.id.tvResult1);
        TextView tv8 = (TextView)findViewById(R.id.tvResult2);
        TextView tv10 = (TextView)findViewById(R.id.tvResult3);
        TextView tv12 = (TextView)findViewById(R.id.tvResult4);

        outState.putString("tvResult1", tv6.getText().toString());
        outState.putString("tvResult2", tv8.getText().toString());
        outState.putString("tvResult3", tv10.getText().toString());
        outState.putString("tvResult4", tv12.getText().toString());
    }

    /** アクティビティが一時保存されたデータを読み込む時に呼ばれます。 */
    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);

        // Bundleに保存した値を取得する
        TextView tv6 = (TextView)findViewById(R.id.tvResult1);
        TextView tv8 = (TextView)findViewById(R.id.tvResult2);
        TextView tv10 = (TextView)findViewById(R.id.tvResult3);
        TextView tv12 = (TextView)findViewById(R.id.tvResult4);

        tv6.setText(savedInstanceState.getString("tvResult1"));
        tv8.setText(savedInstanceState.getString("tvResult2"));
        tv10.setText(savedInstanceState.getString("tvResult3"));
        tv12.setText(savedInstanceState.getString("tvResult4"));
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public void onClick(View v) {
        Calendar baseCal = Calendar.getInstance();

        InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(v.getWindowToken(), 0);

        Toast.makeText(this, "経過時間計算", Toast.LENGTH_SHORT).show();

        TextView tv6 = (TextView)findViewById(R.id.tvResult1);
        TextView tv8 = (TextView)findViewById(R.id.tvResult2);
        TextView tv10 = (TextView)findViewById(R.id.tvResult3);
        TextView tv12 = (TextView)findViewById(R.id.tvResult4);

        EditText etDate1 = (EditText)findViewById(R.id.etDate1);
        EditText etTime1 = (EditText)findViewById(R.id.etTime1);

        String strDate1 = etDate1.getText().toString();
        String strTime1 = etTime1.getText().toString();

        Calendar cal1 = parseDateTime(baseCal, strDate1, strTime1);

        EditText etDate2 = (EditText)findViewById(R.id.etDate2);
        EditText etTime2 = (EditText)findViewById(R.id.etTime2);
        Calendar cal2 = parseDateTime(baseCal, etDate2.getText().toString(), etTime2.getText().toString());

        etDate1.setText(DateFormat.format("yyyy/MM/dd", cal1));
        etTime1.setText(DateFormat.format("kk:mm:ss", cal1));
        etDate2.setText(DateFormat.format("yyyy/MM/dd", cal2));
        etTime2.setText(DateFormat.format("kk:mm:ss", cal2));

        double timeDiff = (double)cal2.getTimeInMillis() - cal1.getTimeInMillis();

        tv6.setText(String.format("%.5f", timeDiff / 1000.0 / 60.0 / 60.0 / 24.0));
        tv8.setText(String.format("%.5f", timeDiff / 1000.0 / 60.0 / 60.0));
        tv10.setText(String.format("%.5f", timeDiff / 1000.0 / 60.0));
        tv12.setText(String.format("%.5f", timeDiff / 1000.0));

        etDate1.requestFocus();
    }

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (event != null && event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
            if (event.getAction() == KeyEvent.ACTION_UP) {
                Button btn = (Button)findViewById(R.id.btnCalc);
                btn.performClick();
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_copy:
                //Toast.makeText(this, "onOptionsItemSelected action_copy", Toast.LENGTH_SHORT).show();

                EditText etDate1 = (EditText)findViewById(R.id.etDate1);
                EditText etTime1 = (EditText)findViewById(R.id.etTime1);
                EditText etDate2 = (EditText)findViewById(R.id.etDate2);
                EditText etTime2 = (EditText)findViewById(R.id.etTime2);
                TextView tv6 = (TextView)findViewById(R.id.tvResult1);
                TextView tv8 = (TextView)findViewById(R.id.tvResult2);
                TextView tv10 = (TextView)findViewById(R.id.tvResult3);
                TextView tv12 = (TextView)findViewById(R.id.tvResult4);

                String sCopy = "開始日時:" + etDate1.getText().toString() + " " + etTime1.getText().toString() + "\n";
                sCopy += "終了日時:" + etDate2.getText().toString() + " " + etTime2.getText().toString() + "\n";
                sCopy += "経過時間\n";
                sCopy += "日数:" + tv6.getText().toString() + "\n";
                sCopy += "時間:" + tv8.getText().toString() + "\n";
                sCopy += "分:" + tv10.getText().toString() + "\n";
                sCopy += "秒:" + tv12.getText().toString()+ "\n";

                ClipData.Item cdi = new ClipData.Item(sCopy);
                String[] mimeType = new String[1];
                mimeType[0] = ClipDescription.MIMETYPE_TEXT_PLAIN;
                ClipData cd = new ClipData(new ClipDescription("text_data", mimeType), cdi);
                ClipboardManager cm = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
                //cm.setText(sCopy);
                cm.setPrimaryClip(cd);

                return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private Calendar parseDateTime(Calendar baseCal, String sDate, String sTime) {
        int y, m, d, h, n, s;
        //Calendar cal = Calendar.getInstance();
        Calendar cal = (Calendar) baseCal.clone();

        //Pattern pat1 = Pattern.compile("([0-9]{4})?[-/\\.]?([0-9]?[0-9])[-/\\.]?([0-9]{2})");
        Pattern pat1 = Pattern.compile("^(((\\d{4})[-/\\.])?(\\d{1,2})[-/\\.])?(\\d{1,2})$");
        Matcher mat1 = pat1.matcher(sDate);
        if (mat1.find()) {
            //y = (mat1.group(1) != null) ? Integer.parseInt(mat1.group(1)) : cal.get(Calendar.YEAR);
            //m = Integer.parseInt(mat1.group(2));
            //d = Integer.parseInt(mat1.group(3));
            y = (mat1.group(3) != null) ? Integer.parseInt(mat1.group(3)) : cal.get(Calendar.YEAR);
            m = Integer.parseInt(mat1.group(4));
            d = Integer.parseInt(mat1.group(5));
        } else {
            Pattern p = Pattern.compile("^[0-9]+$");
            Matcher m1 = p.matcher(sDate);
            if (m1.find()) {
                int val = Integer.parseInt(sDate);
                d = val % 100;
                val /= 100;
                m = val % 100;
                val /= 100;
                y = val;

                if (y == 0) y = cal.get(Calendar.YEAR);
                if (m == 0) m = cal.get(Calendar.MONTH) + 1;
                if (d == 0) d = cal.get(Calendar.DATE);
            } else {
                y = cal.get(Calendar.YEAR);
                m = cal.get(Calendar.MONTH) + 1;
                d = cal.get(Calendar.DATE);
            }
        }

        if (sTime.length() == 0) {
            sTime = "000000";
        }
        Pattern pat2 = Pattern.compile("([0-2]?[0-9]):?([0-9]{2}):?([0-9]{2})?$");
        Matcher mat2 = pat2.matcher(sTime);
        if (mat2.find()) {
            Log.v("TimeCalc", mat2.group(1) + ", " + mat2.group(2) + ", " + mat2.group(3));
            h = Integer.parseInt(mat2.group(1));
            n = Integer.parseInt(mat2.group(2));
            s = (mat2.group(3) != null) ? Integer.parseInt(mat2.group(3)) : 0;
        } else {
            h = cal.get(Calendar.HOUR);
            n = cal.get(Calendar.MINUTE);
            s = cal.get(Calendar.SECOND);
        }

        cal.set(y, m - 1, d, h, n, s);

        return cal;
    }
}
