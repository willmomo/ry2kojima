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
        /// リストを更新する。
        /// </summary>
        private void UpdateList() {
            // リストを空にする
            listView1.Items.Clear();
        }

        /// <summary>
        /// フォルダ選択ダイアログを表示。
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void フォルダを選択FToolStripMenuItem_Click(object sender, EventArgs e) {
            // フォルダ選択ダイアログを表示
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK) {
            }
        }
    }
}