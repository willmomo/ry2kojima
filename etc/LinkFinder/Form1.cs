using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace LinkFinder {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        /// <summary>
        /// ���X�g���X�V����B
        /// </summary>
        private void UpdateList() {
            // ���X�g����ɂ���
            listView1.Items.Clear();
        }

        /// <summary>
        /// �t�H���_�I���_�C�A���O��\���B
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void �t�H���_��I��FToolStripMenuItem_Click(object sender, EventArgs e) {
            // �t�H���_�I���_�C�A���O��\��
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK) {
            }
        }
    }
}