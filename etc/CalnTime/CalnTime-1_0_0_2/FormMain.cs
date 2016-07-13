using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CalnTime {
    public partial class FormMain : Form {
        public FormMain() {
            InitializeComponent();
        }

        // 1秒ごとにフォームの再描画を促す
        private void timer1_Tick(object sender, EventArgs e) {
            // タスクトレイアイコンのテキストを現在時間に変える
            notifyIcon1.Text = System.DateTime.Now.ToString("yyyy/MM/dd ddd HH:mm:ss");

            // 画面さ描画を指示
            this.Invalidate();
        }

        // 現在日時を描画する
        private void FormMain_Paint(object sender, PaintEventArgs e) {
            // 描画に使用するフォントの作成
            System.Drawing.Font font = new System.Drawing.Font("メイリオ", 12);

            // 描画に使用するブラシの作成
            System.Drawing.SolidBrush brush = new System.Drawing.SolidBrush(System.Drawing.Color.Black);

            // 描画に使用するフォーマット(縦・横中央寄せ
            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;

            ///SizeF sz = e.Graphics.MeasureString(System.DateTime.Now.ToString("yyyy/MM/dd\nddd\nHH:mm:ss"), font);

            // 文字列を描画
            e.Graphics.DrawString(
                System.DateTime.Now.ToString("yyyy/MM/dd ddd HH:mm:ss"),
                font, brush, this.ClientRectangle, stringFormat);

            // 後始末
            font.Dispose();
            brush.Dispose();
        }

        // Load イベント
        private void FormMain_Load(object sender, EventArgs e) {
            // フォームをワークエリアの右下に移動させる
            Rectangle rcWork = SystemInformation.WorkingArea;
            this.Left = rcWork.Right - this.Width;
            this.Top = rcWork.Bottom - this.Height;
        }

        // タスクトレイアイコンをクリックで、アクティブにする
        private void notifyIcon1_Click(object sender, EventArgs e) {
            MouseEventArgs me = (MouseEventArgs)e;
            if (me.Button == MouseButtons.Left) {
                this.Activate();
            }
        }

        // タスクトレイアイコンメニューの[終了]処理
        private void toolStripMenuItem1_Click(object sender, EventArgs e) {
            this.Close();
        }

        // アラーム設定用ダイアログの表示
        private void toolStripMenuItem2_Click(object sender, EventArgs e) {
            AlermSetDlg dlg = new AlermSetDlg();
            DialogResult result = dlg.ShowDialog(this);
            if (result == DialogResult.OK) {
                string msg;

                msg = "設定更新 ";
                msg += dlg.m_alermName + " ";
                msg += dlg.m_alermTime;

                MessageBox.Show(msg);
            }
        }
    }
}
