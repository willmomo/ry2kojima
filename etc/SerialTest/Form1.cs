using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SerialTest {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e) {
            if (serialPort1.IsOpen == false) {
                serialPort1.PortName = "COM4";
                serialPort1.Open();
            } else {
                MessageBox.Show("すでに開いています");
            }
        }

        private void button2_Click(object sender, EventArgs e) {
            serialPort1.Close();
        }

        delegate void ListBox1_ItemsAddCallback(string text);

        private void ListBox1_ItemsAdd(string text) {
            if (this.listBox1.InvokeRequired) {
                ListBox1_ItemsAddCallback d = new ListBox1_ItemsAddCallback(ListBox1_ItemsAdd);
                this.Invoke(d, new object[] { text });
            } else {
                this.listBox1.Items.Add(text);
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            System.Diagnostics.Debug.WriteLine(DateTime.Now.ToString() + " : " + "DataRecived");
            ListBox1_ItemsAdd(DateTime.Now.ToString() + " : " + "DataRecived");
        }

        private void serialPort1_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e) {
            System.Diagnostics.Debug.WriteLine(DateTime.Now.ToString() + " : " + "ErrorRecived");
            ListBox1_ItemsAdd(DateTime.Now.ToString() + " : " + "ErrorRecived");
        }

        private void serialPort1_PinChanged(object sender, System.IO.Ports.SerialPinChangedEventArgs e) {
            System.Diagnostics.Debug.WriteLine(DateTime.Now.ToString() + " : " + "PinChanged");
            ListBox1_ItemsAdd(DateTime.Now.ToString() + " : " + "PinChanged");
        }

        private void button3_Click(object sender, EventArgs e) {
            if (serialPort1.IsOpen == false) {
                serialPort1.Open();
            }

            serialPort1.WriteLine(DateTime.Now.ToString());
        }

        private void timer1_Tick(object sender, EventArgs e) {

        }
    }
}
