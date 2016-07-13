using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SerialPortSample {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        delegate void AppendTextCallback(string text);

        private void AppendText(string text) {
            if (textBox1.InvokeRequired) {
                AppendTextCallback d = new AppendTextCallback(AppendText);
                Invoke(d, new object[] { text });
            } else {
                textBox1.Text += text;
                textBox1.SelectionStart = textBox1.TextLength;
                textBox1.ScrollToCaret();
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            AppendText(DateTime.Now.ToLongTimeString() + " DataReceived\r\n");
        }

        private void serialPort1_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e) {
            AppendText(DateTime.Now.ToLongTimeString() + " ErrorReceived\r\n");
        }

        private void serialPort1_PinChanged(object sender, System.IO.Ports.SerialPinChangedEventArgs e) {
            AppendText(DateTime.Now.ToLongTimeString() + " PinChanged\r\n");
        }

        private void Form1_Load(object sender, EventArgs e) {
            comboBox1.SelectedIndex = 0;
            comboBox2.SelectedIndex = 0;
            comboBox3.SelectedIndex = 0;
            comboBox4.SelectedIndex = 0;
            comboBox5.SelectedIndex = 0;

            button2.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e) {
            serialPort1.PortName = comboBox1.Text;
            serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
            serialPort1.DataBits = Convert.ToInt32(comboBox3.Text);

            switch (comboBox4.SelectedIndex) {
                case 0:
                    serialPort1.Parity = System.IO.Ports.Parity.Even;
                    break;
                case 1:
                    serialPort1.Parity = System.IO.Ports.Parity.Odd;
                    break;
                default:
                    serialPort1.Parity = System.IO.Ports.Parity.None;
                    break;
            }

            switch (comboBox5.SelectedIndex) {
                case 0:
                    serialPort1.StopBits = System.IO.Ports.StopBits.One;
                    break;
                default:
                    serialPort1.StopBits = System.IO.Ports.StopBits.Two;
                    break;
            }

            try {
                serialPort1.Open();

                button1.Enabled = false;
                button2.Enabled = true;
            } catch (System.IO.IOException ex) {
                MessageBox.Show(ex.Message);
            }
        }

        private void button2_Click(object sender, EventArgs e) {
            serialPort1.Close();

            button1.Enabled = true;
            button2.Enabled = false;
        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e) {
            if (e.KeyChar == '\x0d') {
                serialPort1.Write(textBox2.Text);
                textBox2.Clear();
            }
        }
    }
}
