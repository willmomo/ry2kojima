package jp.rkojima.sudokusupport2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import java.util.Map;

/**
 * 数独画面を表示するカスタムビュー
 * @author Ryota
 *
 */
public class SudokuView extends View {
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

        float textSize = 160;
        m_paint.setTextSize(textSize);

		// 盤面の描画
		m_paint.setTextSize(35);

        // 確定している数値を貰う
        Map<Integer, Integer> fixedNum = doc.m_sudokuGame.getBoard().countFixedNum();

		Paint.FontMetrics fontMetrics = m_paint.getFontMetrics();
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
                    if (fixedNum.get(num) == 9) {
                        m_paint.setColor(Color.rgb(0, 100, 0));
                    } else {
                        m_paint.setColor(Color.BLACK);
                    }
					txt = Integer.toString(num);
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
     * 矩形の15%内側にぴったり収まるサイズの文字で描画する。
	 * @param canvas
	 * @param string
	 * @param rect
	 */
	private void drawText(Canvas canvas, String string, RectF rect, Paint.FontMetrics fontMetrics_nouse) {
        rect.inset(rect.width() * 15 / 100, rect.height() * 15 / 100);

        float textSize = 200;
        m_paint.setTextSize(textSize);
        Rect bounds = new Rect();
        m_paint.getTextBounds(string, 0, string.length(), bounds);
        while (bounds.width() > rect.width() || bounds.height() > rect.height()) {

            // Log.i("HOGE", String.format("B(%d,%d)", bounds.width(), bounds.height()));
            // Log.i("HOGE", String.format("A(%f,%f)", rect.width(), rect.height()));

            textSize--;
            m_paint.setTextSize(textSize);
            m_paint.getTextBounds(string, 0, string.length(), bounds);
        }
        Paint.FontMetrics fontMetrics = m_paint.getFontMetrics();
		float textWidth = m_paint.measureText(string);
		float baseX = rect.centerX() - textWidth / 2;
		float baseY = rect.centerY() - (fontMetrics.ascent + fontMetrics.descent) / 2;
		canvas.drawText(string, baseX, baseY, m_paint);
	}
}
