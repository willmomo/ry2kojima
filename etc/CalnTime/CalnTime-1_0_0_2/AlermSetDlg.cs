using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CalnTime {
    public partial class AlermSetDlg : Form {
        public AlermSetDlg() {
            InitializeComponent();
        }

        public string m_alermName;
        public string m_alermTime;

        // Load イベント処理
        private void AlermSetDlg_Load(object sender, EventArgs e) {
            string path = System.Windows.Forms.Application.ExecutablePath;
            path = System.IO.Path.Combine(
                System.IO.Path.GetDirectoryName(path),
                "Alerts.txt");

        }

        // [OK] ボタンを押したとき、コントロールの内容をメンバ変数に代入する
        private void btnOK_Click(object sender, EventArgs e) {
            m_alermName = comboBox1.Text;
            m_alermTime = textBox1.Text;
        }
    }
}
