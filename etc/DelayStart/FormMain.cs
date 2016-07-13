using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DelayStart {
    public partial class FormMain : Form {
        // 起動した時間
        private int m_nStartTick = System.Environment.TickCount;

        // 起動待ち時間(ms)
        private int m_nWait = 0;

        public FormMain() {
            InitializeComponent();
        }

        /// <summary>
        /// このアプリケーションが使う設定ファイル名を取得
        /// </summary>
        private string XmlFileName {
            get {
                // 自分のフルパス名を取得。拡張子をxmlに変更。
                // その結果を設定ファイルとして、利用する。
                return System.IO.Path.ChangeExtension(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName, "xml");
            }
        }

        /// <summary>
        /// 設定ファイルで指定された実行ファイルを実行する。
        /// </summary>
        private void ProcessStart() {
            // XmlReaderを使って、設定ファイル(xml)を開く
            System.Xml.XmlReaderSettings rs = new System.Xml.XmlReaderSettings();
            rs.IgnoreWhitespace = true; // タグ間のホワイトスペースを無視する
            System.Xml.XmlReader r = System.Xml.XmlReader.Create(this.XmlFileName, rs);

            // ノードをひとつずつ読み進む
            r.ReadToFollowing("user");
            while (r.IsStartElement("user")) {
                string userName = r.GetAttribute("name");

                if (userName == "All Users" || userName == System.Environment.GetEnvironmentVariable("USERNAME")) {
                    r.Read();
                    while (r.IsStartElement("exefile")) {
                        r.Read();

                        // 起動するプロセスのために、オブジェクトを生成
                        System.Diagnostics.Process p = new System.Diagnostics.Process();

                        while (r.NodeType != System.Xml.XmlNodeType.EndElement) {
                            if (r.Name == "filename") {
                                p.StartInfo.FileName = r.ReadElementString();
                            } else if (r.Name == "WindowStyle") {
                                switch (r.ReadElementString()) {
                                    case "Minimized":
                                        p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
                                        break;
                                    case "Maximized":
                                        p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Maximized;
                                        break;
                                    default:
                                        p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Normal;
                                        break;
                                }
                            } else {
                                break;
                            }
                        }

                        // 指定プロセスを起動する
                        p.Start();
                        p.WaitForInputIdle();

                        r.ReadEndElement(); // </exefile>を読み込む
                    }
                    r.ReadEndElement(); // </user>を読み込む
                } else {
                    r.ReadToNextSibling("user");
                }
            }

            r.Close();
        }

        /// <summary>
        /// Form.Load イベント
        /// 起動待ちタイマーを起動する。
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_Load(object sender, EventArgs e) {
            // XmlReaderを使って、設定ファイル(xml)を開く
            System.Xml.XmlReaderSettings rs = new System.Xml.XmlReaderSettings();
            rs.IgnoreWhitespace = true; // タグ間のホワイトスペースを無視する
            System.Xml.XmlReader r = System.Xml.XmlReader.Create(this.XmlFileName, rs);

            // ノードをひとつずつ読み進む
            while (r.Read()) {
                if (r.NodeType == System.Xml.XmlNodeType.Element && r.Name == "delay") {
                    m_nWait = r.ReadElementContentAsInt() * 1000;
                    break;
                }
            }

            r.Close();

            // 1秒タイマーを起動
            timWait.Interval = 1;
            timWait.Enabled = true;
        }

        /// <summary>
        /// Timer.Tick イベント。
        /// 起動待ちのカウントダウンを行う。
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timWait_Tick(object sender, EventArgs e) {
            // 現在Tick - 起動Tickが、起動待ち時間を過ぎたら、ファイルを実行する
            if ((System.Environment.TickCount - this.m_nStartTick) > this.m_nWait) {

                // タイマーをとめる
                this.timWait.Enabled = false;

                // プロセスの実行
                this.ProcessStart();

                // 自分自身は、終了させる。
                this.Close();
            } else {
                double nLeft = this.m_nWait - (System.Environment.TickCount - this.m_nStartTick);
                nLeft /= 1000;
                label1.Text = nLeft.ToString("00.000");
            }
        }
    }
}