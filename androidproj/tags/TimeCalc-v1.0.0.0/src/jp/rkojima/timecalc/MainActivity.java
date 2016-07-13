package jp.rkojima.timecalc;

import java.util.Calendar;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.os.Bundle;
import android.app.Activity;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.Context;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener, OnKeyListener {
	NumberPicker m_numPicker;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        // ボタンのイベント処理追加
        Button btn = (Button)findViewById(R.id.button1);
        btn.setOnClickListener(this);

        // 最後の時間入力エリアのキー入力イベントを追加」
    	EditText et = (EditText)findViewById(R.id.editText4);
    	et.setOnKeyListener(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
	@Override
    public void onClick(View v) {
		Calendar baseCal = Calendar.getInstance();
		
        InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
    	imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
        
    	Toast.makeText(this, "経過時間計算", Toast.LENGTH_SHORT).show();
    	
    	TextView tv6 = (TextView)findViewById(R.id.textView6);
    	TextView tv8 = (TextView)findViewById(R.id.textView8);
    	TextView tv10 = (TextView)findViewById(R.id.textView10);
    	TextView tv12 = (TextView)findViewById(R.id.textView12);

    	EditText etDate1 = (EditText)findViewById(R.id.editText1);
    	EditText etTime1 = (EditText)findViewById(R.id.editText2);
    	Calendar cal1 = parseDateTime(baseCal, etDate1.getText().toString(), etTime1.getText().toString());

    	EditText etDate2 = (EditText)findViewById(R.id.editText3);
    	EditText etTime2 = (EditText)findViewById(R.id.editText4);
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
		        Button btn = (Button)findViewById(R.id.button1);
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

	    	EditText etDate1 = (EditText)findViewById(R.id.editText1);
	    	EditText etTime1 = (EditText)findViewById(R.id.editText2);
	    	EditText etDate2 = (EditText)findViewById(R.id.editText3);
	    	EditText etTime2 = (EditText)findViewById(R.id.editText4);
	    	TextView tv6 = (TextView)findViewById(R.id.textView6);
	    	TextView tv8 = (TextView)findViewById(R.id.textView8);
	    	TextView tv10 = (TextView)findViewById(R.id.textView10);
	    	TextView tv12 = (TextView)findViewById(R.id.textView12);
	    	
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
		return false;
	}
	
	private Calendar parseDateTime(Calendar baseCal, String sDate, String sTime) {
    	int y, m, d, h, n, s;
    	//Calendar cal = Calendar.getInstance();
    	Calendar cal = (Calendar) baseCal.clone();

		Pattern pat1 = Pattern.compile("([0-9]{4})?[-/\\.]?([0-9]?[0-9])[-/\\.]?([0-9]{2})");
		Matcher mat1 = pat1.matcher(sDate);
		if (mat1.find()) {
			y = (mat1.group(1) != null) ? Integer.parseInt(mat1.group(1)) : cal.get(Calendar.YEAR);
			m = Integer.parseInt(mat1.group(2));
			d = Integer.parseInt(mat1.group(3));
		} else {
			y = cal.get(Calendar.YEAR);
			m = cal.get(Calendar.MONTH) + 1;
			d = cal.get(Calendar.DATE);
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
