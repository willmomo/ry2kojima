package jp.rkojima.sudokusupport2;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Point;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class MainActivity extends Activity {
	// ゲームの状態を保存するための変数
	public SudokuGame m_sudokuGame = new SudokuGame();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.optionsmenu, menu);
		return true;
	}

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
    	//menu.findItem(R.id.menuItem2).setEnabled(
    	//		this.m_sudokuGame.getBoardCount() == 0 ? false : true);
    	
    	// setEnabledだと、見た目が変化しないうえ、押せたような表示になるため、
    	// メニューから項目そのものを消し去ることにした。
    	//menu.findItem(R.id.menuItem2).setVisible(
    	//		this.m_sudokuGame.getBoardCount() == 0 ? false : true);
    	menu.findItem(R.id.menuItem2).setVisible(this.m_sudokuGame.getBoardCount() != 0);
    	menu.findItem(R.id.menuItem4).setVisible(this.getFileStreamPath("sudoku.dat").exists());
    	return super.onPrepareOptionsMenu(menu);
    }
    
    /** メニュー項目を選択したときの処理 */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	switch (item.getItemId()) {
    	case R.id.menuItem1:
    		this.m_sudokuGame.init();
    		this.findViewById(R.id.view1).invalidate();
    		return true;
    	case R.id.menuItem2:
    		this.m_sudokuGame.popBoard();
    		this.findViewById(R.id.view1).invalidate();
    		return true;
    	case R.id.menuItem3:
    		try {
				this.saveGame();
			} catch (IOException e) {
				e.printStackTrace();
			}
    		return true;
    	case R.id.menuItem4:
			this.m_sudokuGame.pushBoard();
    		try {
    			this.loadGame();
			} catch (IOException e) {
				e.printStackTrace();
			}
    		return true;
    	case R.id.menuItem5:
			this.m_sudokuGame.pushBoard();
    		doAll();
    		return true;
    	}
    	return super.onOptionsItemSelected(item);
    }

    /** [セーブ]処理 */
    private void saveGame() throws IOException {
    	// ストリームを開く
    	FileOutputStream output = this.openFileOutput("sudoku.dat", MODE_PRIVATE);
    	
    	// 数独の状態を書き込む
    	for (int row = 0; row < this.m_sudokuGame.getBoard().blockCellCount(); row++) {
    		for (int col = 0; col < this.m_sudokuGame.getBoard().blockCellCount(); col++) {
    			if (!(row == 0 && col == 0)) {
    				output.write(",".getBytes());
    			}
    			output.write(Integer.toString(this.m_sudokuGame.getBoard().getCell(row, col).getNumber()).getBytes());
    		}
    	}
    	
    	// ストリームを閉じる
    	output.close();
    }
    
    /** [ロード]処理 */
    private void loadGame() throws IOException {
    	// ストリームを開く
    	FileInputStream input = this.openFileInput("sudoku.dat");
    	
    	// 読込む
    	BufferedReader reader = new BufferedReader(new InputStreamReader(input));
    	String line = reader.readLine();
    	reader.close();
    	
    	this.m_sudokuGame.init();
    	String[] nums = line.split(",");
    	for (int i = 0; i < nums.length; i++) {
    		int row = i / 9;
    		int col = i % 9;
    		this.m_sudokuGame.getBoard().fixNumber(row, col, Integer.parseInt(nums[i]));
    	}
		this.findViewById(R.id.view1).invalidate();
    }

    /**
     * ゲーム終了をチェックして表示。
     * @param notOver クリアしていない時もメッセージを表示するときは、trueにする。
     */
    public void gameOver(boolean notOver) {
    	String msg = "";
    	if (this.m_sudokuGame.getBoard().isFinished()) {
			msg = "クリア";
    	} else if (this.m_sudokuGame.getBoard().isFailed()) {
    		msg = "手詰まり";
    	} else if (notOver) {
    		msg = "クリアできません";
    	}
    	if (msg.length() > 0) {
	    	// ダイアログメッセージ表示
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle("数独サポート");
			builder.setMessage(msg);
			builder.setPositiveButton("OK", null);
			builder.show();
    	}
    }
    
    /** [解く]処理 */
    private void doAll() {
    	// [解く]ための処理を全力実行
    	int difficulty = 0;
    	do {
    		++difficulty;
	    	while (m_sudokuGame.parse00() ||
	    		   m_sudokuGame.parse01() ||
	    		   m_sudokuGame.parse02() ||
	    		   m_sudokuGame.parse03() ||
	    		   m_sudokuGame.parse12()) {
	    		;
	    	}
    	} while ((m_sudokuGame.parse30() ||
    			  m_sudokuGame.parse31() ||
    			  m_sudokuGame.parse32()) &&
    			  difficulty < 10);
    	
		this.findViewById(R.id.view1).invalidate();
    	this.gameOver(true);
    }

    /**
     * 数値選択ダイアログを表示
     * @param items
     * @param row
     * @param col
     */
    public void selectDialog(String[] items, int row, int col) {
		// 数値選択リストダイアログを表示
    	
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setIcon(R.mipmap.ic_launcher);
		builder.setTitle("数値選択");
		builder.setItems(items, new DialogInterface.OnClickListener() {
			private MainActivity m_context;
			private String[] m_items;
			private Point m_pt;
			public void onClick(DialogInterface dialog, int which) {
				m_sudokuGame.pushBoard();
				m_sudokuGame.getBoard().fixNumber(
						m_pt.y, m_pt.x, Integer.parseInt(m_items[which]));
				m_context.findViewById(R.id.view1).invalidate();
				m_context.gameOver(false);
			}
			public DialogInterface.OnClickListener factory(MainActivity context, String[] items, int row, int col) {
				this.m_context = context;
				this.m_items = items;
				this.m_pt = new Point(col, row);
				return this;
			}
		}.factory(this, items, row, col));
		builder.show();
    }
}

