package jp.ne.rkojima.sudokusupport2;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.graphics.Point;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;

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
		builder.setIcon(R.drawable.ic_launcher);
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

/**
 * 数独画面を表示するカスタムビュー
 * @author Ryota
 *
 */
class SudokuView extends View {
	private Paint m_paint = new Paint();
	private Point m_ptDown = new Point(-1, -1);

	/**
	 * コンストラクタ
	 * @param context
	 * @param attr
	 */
	public SudokuView(Context context, AttributeSet attr) {
		super(context, attr);
		// ビュー内部は、自前で描画するので、コンストラクタでは、何もしない。
	}
	
	/**
	 * ここで、ビューの大きさに関する処理をするつもりだった
	 */
	@Override
	protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
		super.onLayout(changed, left, top, right, bottom);
	}

	/**
	 * タップされた時の処理
	 */
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			m_ptDown = new Point((int)(event.getX() / (this.getWidth() / 9)),
							     (int)(event.getY() / (this.getHeight() / 9)));
			// 画面の淵をタッチした時の防御策
			if (m_ptDown.x > 8) m_ptDown.x = 8;
			if (m_ptDown.y > 8) m_ptDown.y = 8;
			break;
		case MotionEvent.ACTION_UP:
			Point pt = new Point((int)(event.getX() / (this.getWidth() / 9)),
				     			 (int)(event.getY() / (this.getHeight() / 9)));
			if (!m_ptDown.equals(pt)) {
				break;	// DOWNとUPの座標不一致は、即終了
			}
			int col = m_ptDown.x;
			int row = m_ptDown.y;
			
			MainActivity doc = (MainActivity)this.getContext();
			SudokuCell cell = doc.m_sudokuGame.getBoard().getCell(row, col);
			
			if (cell.getCandidates().size() == 1) {
				// 直接確定
				doc.m_sudokuGame.pushBoard();
				doc.m_sudokuGame.getBoard().fixNumber(row, col, cell.getCandidates().get(0));
				this.invalidate();
				doc.gameOver(false);
			} else if (cell.getCandidates().size() > 1) {
				// メニューから選択
				doc.selectDialog(
						cell.getCandidates().toString().replaceAll(" |\\[|\\]", "").split(","),
						row, col);
			} else {
				// 何もしない
			}
			break;
		default:
			return super.onTouchEvent(event);
		}
		return true;
	}

	/**
	 * SudokuGameの状態をビュー内部に描画
	 */
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);

		MainActivity doc;
		try {
			doc = (MainActivity) this.getContext();
		} catch (Exception e) {
			canvas.drawText("デザイン時は、描画しません。", 10, 30, m_paint);
			return;
		}

		// 枠線の描画
		m_paint.setColor(Color.BLACK);
		for (int i = 0; i <= 9; i++) {
			float x = (this.getWidth() - 1) * i / 9;
			float y = (this.getHeight() - 1) * i / 9;

			m_paint.setStrokeWidth(((i % 3) == 0) ? 3 : 1);
			canvas.drawLine(x, 0, x, this.getHeight(), m_paint);
			canvas.drawLine(0, y, this.getWidth(), y, m_paint);
		}

		// 盤面の描画
		m_paint.setTextSize(35);
		FontMetrics fontMetrics = m_paint.getFontMetrics();
		for (int row = 0; row < 9; row++) {
			float y1 = this.getHeight() * row / 9;
			float y2 = this.getHeight() * (row + 1) / 9;
			for (int col = 0; col < 9; col++) {
				SudokuCell cell = doc.m_sudokuGame.getBoard().getCell(row, col);
				int num = cell.getNumber();
				String txt;
				m_paint.setTextScaleX(1.0f);
				if (num == 0) {
					switch (cell.getCandidates().size()) {
					case 1:
						txt = Integer.toString(cell.getCandidates().get(0));
						m_paint.setColor(Color.BLUE);
						break;
					case 2:
						txt = Integer.toString(cell.getCandidates().get(0))
							+ Integer.toString(cell.getCandidates().get(1));
						m_paint.setTextScaleX(0.5f);
						m_paint.setColor(Color.GRAY);
						break;
					default:
						txt = "+";
						m_paint.setColor(Color.GRAY);
						break;
					}
					m_paint.setTypeface(Typeface.DEFAULT);
				} else {
					txt = Integer.toString(num);
					m_paint.setColor(Color.BLACK);
					m_paint.setTypeface(Typeface.DEFAULT_BOLD);
				}
						
				float x1 = this.getWidth() * col / 9;
				float x2 = this.getWidth() * (col + 1) / 9;
				this.drawText(canvas, txt, new RectF(x1, y1, x2, y2), fontMetrics);
			}
		}
	}

	/**
	 * 矩形の中央に文字列を描画する関数
	 * @param canvas
	 * @param string
	 * @param rect
	 */
	private void drawText(Canvas canvas, String string, RectF rect, FontMetrics fontMetrics) {
		float textWidth = m_paint.measureText(string);
		float baseX = rect.centerX() - textWidth / 2;
		float baseY = rect.centerY() - (fontMetrics.ascent + fontMetrics.descent) / 2;
		canvas.drawText(string, baseX, baseY, m_paint);
	}
}