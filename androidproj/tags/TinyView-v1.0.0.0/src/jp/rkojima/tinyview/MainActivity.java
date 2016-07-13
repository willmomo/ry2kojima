/*
 * 超簡単な画像表示プログラム。
 * /mnt/sdcard 上の 001.jpg からファイルが見つかる限り、タップで画像を切り替える。
 * 余計なアンドロイドの動作を無くして、でも画像を表示するためだけに作成。
 * 
 * コマンドラインサンプル：
 * インストール: pm install TinyView.apk
 * アンインストール: pm uninstall jp.rkojima.tinyview
 * 実行: am start -a android.intent.action.MAIN -n jp.rkojima.tinyview/jp.rkojima.tinyview.MainActivity
 */
package jp.rkojima.tinyview;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;

import android.support.v7.app.ActionBarActivity;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;

//public class MainActivity extends ActionBarActivity {
public class MainActivity extends Activity implements View.OnClickListener {

	private double stime = 0.0;
	private int m_fileNo = 0;
	
	public void onClick(View view) {
	}
	
	public File getNextFile() {
		++m_fileNo;
		File f = new File(String.format("/mnt/sdcard/%1$03d.jpg", m_fileNo));
		if (!f.exists()) {
			m_fileNo = 1;
			f = new File(String.format("/mnt/sdcard/%1$03d.jpg", m_fileNo));
		}
		return f;
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

		setContentView(R.layout.activity_main);
		
		ImageView v = (ImageView) findViewById(R.id.imageView1);
		String fname = String.format("/mnt/sdcard/%1$03d.jpg", m_fileNo);
		
		File srcFile = getNextFile();
		try {
			InputStream is = new FileInputStream(srcFile);
			Bitmap bm = BitmapFactory.decodeStream(is);
			v.setImageBitmap(bm);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		v.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				double stime2 = System.currentTimeMillis(); 
				//Toast.makeText(getApplicationContext(), "連続タップを無視", Toast.LENGTH_SHORT).show();
				if ((stime2 - stime) < 500.0) {
					//Toast.makeText(getApplicationContext(), "連続タップを無視", Toast.LENGTH_SHORT).show();
					return;
				}
				stime = stime2;
				ImageView iv = (ImageView) findViewById(R.id.imageView1);
				File srcFile = getNextFile();
				try {
					InputStream is = new FileInputStream(srcFile);
					Bitmap bm = BitmapFactory.decodeStream(is);
					iv.setImageBitmap(bm);
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				}
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		// getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
