package jp.rkojima.today;

import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

import com.ibm.icu.util.Calendar;

import java.text.SimpleDateFormat;
import java.util.Locale;

public class MainActivity extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        {
            com.ibm.icu.util.Calendar japaneseCalendar = new com.ibm.icu.util.JapaneseCalendar(Locale.JAPANESE);
            //com.ibm.icu.text.SimpleDateFormat japaneseDateFormat = (com.ibm.icu.text.SimpleDateFormat) japaneseCalendar.getDateTimeFormat(com.ibm.icu.text.DateFormat.FULL, -1, Locale.JAPANESE);
            SimpleDateFormat japaneseDateFormat = new SimpleDateFormat("GGGGy年M月d日 EEEE");
            String dateStr = japaneseDateFormat.format(japaneseCalendar.getTime());
            TextView tv = (TextView) findViewById(R.id.textView2);
            tv.setText(dateStr);
        }
        {
            com.ibm.icu.util.Calendar japaneseCalendar = new com.ibm.icu.util.JapaneseCalendar(Locale.JAPANESE);
            com.ibm.icu.text.SimpleDateFormat japaneseDateFormat = (com.ibm.icu.text.SimpleDateFormat) japaneseCalendar.getDateTimeFormat(com.ibm.icu.text.DateFormat.FULL, -1, Locale.JAPANESE);
            japaneseDateFormat.applyLocalizedPattern("GGGGy年");
            String dateStr = "和暦では、" + japaneseDateFormat.format(japaneseCalendar);
            TextView tv = (TextView) findViewById(R.id.textView3);
            tv.setText(dateStr);
        }
        {
            com.ibm.icu.util.Calendar japaneseCalendar = new com.ibm.icu.util.JapaneseCalendar(Locale.JAPANESE);
            com.ibm.icu.text.SimpleDateFormat japaneseDateFormat = (com.ibm.icu.text.SimpleDateFormat) japaneseCalendar.getDateTimeFormat(com.ibm.icu.text.DateFormat.FULL, -1, Locale.JAPANESE);
            japaneseDateFormat.applyLocalizedPattern("M月の異称は、");
            String dateStr = japaneseDateFormat.format(japaneseCalendar);
            String[] imyou = {"睦月", "如月", "弥生", "卯月", "皐月", "水無月", "文月", "葉月", "長月", "神無月", "霜月", "師走"};
            dateStr += imyou[japaneseCalendar.get(Calendar.MONTH)];
            TextView tv = (TextView) findViewById(R.id.textView4);
            tv.setText(dateStr);
        }
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
