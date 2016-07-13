package jp.rkojima.papimopass;

import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends ActionBarActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button button = (Button) findViewById(R.id.button);
//        button.setOnClickListener(this);
/*
        button.setOnClickListener(new View.OnClickListener() {

            // クリック時に呼ばれるメソッド
            @Override
            public void onClick(View view) {
//                Toast.makeText(getApplicationContext(), "クリックされました！", Toast.LENGTH_LONG).show();
//                TextView tv = (TextView)findViewById(R.id.textView2);
//                tv.setText("押しました");
            }
        });
        */
    }

    public void onClick(View view) {
        Toast.makeText(getApplicationContext(), "クリックされました！", Toast.LENGTH_LONG).show();
        TextView tv = (TextView)findViewById(R.id.textView2);
        tv.setText("NEW 押しました");
    }

    // クリック時に呼ばれるメソッド
    public void onButtonClick(View view){
        EditText et = (EditText)findViewById(R.id.editText);
        String fname = et.getText().toString();

        String tenpo = fname.substring(0, 8);
        String date = fname.substring(8, 16);
        String time = fname.substring(16);

        TextView tv = (TextView)findViewById(R.id.textView2);
        String pass = "";
        for (int i = 0; i < 8; i++) {
            Log.d("hogege", "gnv = " + (char)(Character.getNumericValue(date.charAt(i)) + (40 + 10 * i)));
            char c = (char)(Character.getNumericValue(date.charAt(i)) + (40 + 10 * i));
            pass += c;
        }
        tv.setText(pass + tenpo);
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
