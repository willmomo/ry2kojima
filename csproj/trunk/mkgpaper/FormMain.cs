using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace mkgpaper {
    public partial class FormMain : Form {
        public FormMain() {
            InitializeComponent();
        }

        // e.PageBounds : 単位1/100インチ : 用紙のサイズ
        //
        // e.MarginBounds : 単位1/100インチ : 描画可能領域
        // 
        // PrintDocument.OriginAtMarginsがtrueのとき、
        //
        // 
        //   e.Page
        //   e.PageBoundsからe.PageSettings.Marginsの値を引いた値。
        //   e.PageSettings.Marginsに、ハードマージンより小さい値が設定できるため、考慮が必要。
        // } else {
        // }
        // 
        // e.PageSettings.Margins : ページ設定ダイアログで指定した余白 : 単位1/100インチ
        // ハードマージンより、小さい値が設定できる点に注意。
        //
        private void pd_PrintPage(object sender, System.Drawing.Printing.PrintPageEventArgs e) {
            int mm = (int)this.numericUpDown1.Value;

            int temp = (int)(e.MarginBounds.Width * 25.4f / 100);
            temp = temp / mm * mm;

            int temp2 = (int)(e.MarginBounds.Height * 25.4f / 100);
            temp2 = temp2 / mm * mm;

            Pen outsidePen = new Pen(Color.Gray, 1);
            Pen insidePen = new Pen(Color.LightGray, 0.5f);

            e.Graphics.DrawRectangle(outsidePen, new Rectangle(0, 0, (int)(temp * 10 / 2.54), (int)(temp2 * 10 / 2.54f)));

            if (this.checkBox1.Checked == false) {
                for (int x = mm; x < temp; x += mm) {
                    e.Graphics.DrawLine(insidePen, new PointF((float)(x * 10 / 2.54), 0), new PointF((float)(x * 10 / 2.54), (float)(temp2 * 10 / 2.54f)));
                }
            }

            for (int y = mm; y < temp2; y += mm) {
                e.Graphics.DrawLine(insidePen, new PointF(0, (float)(y * 10 / 2.54)), new PointF((float)(temp * 10 / 2.54), (float)(y * 10 / 2.54f)));
            }

            e.HasMorePages = false;
        }

        private void smiFileSetup_Click(object sender, EventArgs e) {

            PageSetupDialog psDlg = new PageSetupDialog();

            psDlg.EnableMetric = true;
            psDlg.Document = pd;

            if (psDlg.ShowDialog() == DialogResult.OK) {
                pd = psDlg.Document;
            }
        }

        private void button1_Click(object sender, EventArgs e) {

            // 印刷ダイアログオブジェクトの作成
            PrintDialog dlgPrn = new PrintDialog();

			// .NET3.5のバグで、これをtrueにしないと、ShowDialogが、瞬時にCancelで戻ってきてしまう。
			dlgPrn.UseEXDialog = true;

            // PrintDocumentを指定
            dlgPrn.Document = pd;

            // 印刷ダイアログを表示
            if (dlgPrn.ShowDialog() == DialogResult.OK) {

                // 印刷開始
                pd.Print();
            }
        }

        private void 終了XToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void バージョン情報AToolStripMenuItem_Click(object sender, EventArgs e) {
            AboutBox1 aboutBox = new AboutBox1();

            aboutBox.ShowDialog();
        }
    }
}