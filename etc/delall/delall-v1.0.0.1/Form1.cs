using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace delall {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        private bool m_stop;

        private void delall(DirectoryInfo di) {
            foreach (DirectoryInfo obj in di.GetDirectories()) {
                Application.DoEvents();
                if (m_stop) return;

                delall(obj);
            }

            foreach (FileInfo obj in di.GetFiles()) {
                Application.DoEvents();
                if (m_stop) return;

                listBox1.Items.Add("[F]\t" + obj.FullName);
                listBox1.TopIndex = listBox1.Items.Count - listBox1.Height / listBox1.ItemHeight;
                obj.Delete();
            }
            listBox1.Items.Add("[D]\t" + di.FullName);
            listBox1.TopIndex = listBox1.Items.Count - listBox1.Height / listBox1.ItemHeight;
            di.Delete();
        }

        public void delall() {
            delall(new DirectoryInfo(textBox1.Text));
        }

        private void button1_Click(object sender, EventArgs e) {
            button1.Enabled = false;
            button2.Enabled = true;

            m_stop = false;
            listBox1.Items.Clear();

            try {
                delall();
            } catch (IOException ex) {
                listBox1.Items.Add("[ERR]\t" + ex.Message.ToString());
                listBox1.TopIndex = listBox1.Items.Count - listBox1.Height / listBox1.ItemHeight;
            }

            button2.Enabled = false;
            button1.Enabled = true;
        }

        private void textBox1_TextChanged(object sender, EventArgs e) {
            button1.Enabled = (textBox1.TextLength > 0) ? true : false;
        }

        private void button2_Click(object sender, EventArgs e) {
            button2.Enabled = false;
            m_stop = true;
            button1.Enabled = true;
        }

        private void Form1_DragEnter(object sender, DragEventArgs e) {
            e.Effect = DragDropEffects.All;
        }

        private void Form1_DragDrop(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
                foreach (string fileName in (string[])e.Data.GetData(DataFormats.FileDrop)) {
                    textBox1.Text = fileName;
                    break;
                }
            }
        }
    }
}
