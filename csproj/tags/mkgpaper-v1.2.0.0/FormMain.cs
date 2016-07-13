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

        // e.PageBounds : �P��1/100�C���` : �p���̃T�C�Y
        //
        // e.MarginBounds : �P��1/100�C���` : �`��\�̈�
        // 
        // PrintDocument.OriginAtMargins��true�̂Ƃ��A
        //
        // 
        //   e.Page
        //   e.PageBounds����e.PageSettings.Margins�̒l���������l�B
        //   e.PageSettings.Margins�ɁA�n�[�h�}�[�W����菬�����l���ݒ�ł��邽�߁A�l�����K�v�B
        // } else {
        // }
        // 
        // e.PageSettings.Margins : �y�[�W�ݒ�_�C�A���O�Ŏw�肵���]�� : �P��1/100�C���`
        // �n�[�h�}�[�W�����A�������l���ݒ�ł���_�ɒ��ӁB
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

            // ����_�C�A���O�I�u�W�F�N�g�̍쐬
            PrintDialog dlgPrn = new PrintDialog();

			// .NET3.5�̃o�O�ŁA�����true�ɂ��Ȃ��ƁAShowDialog���A�u����Cancel�Ŗ߂��Ă��Ă��܂��B
			dlgPrn.UseEXDialog = true;

            // PrintDocument���w��
            dlgPrn.Document = pd;

            // ����_�C�A���O��\��
            if (dlgPrn.ShowDialog() == DialogResult.OK) {

                // ����J�n
                pd.Print();
            }
        }

        private void �I��XToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void �o�[�W�������AToolStripMenuItem_Click(object sender, EventArgs e) {
            AboutBox1 aboutBox = new AboutBox1();

            aboutBox.ShowDialog();
        }
    }
}