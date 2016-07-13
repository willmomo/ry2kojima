using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DelayStart {
    public partial class FormMain : Form {
        // �N����������
        private int m_nStartTick = System.Environment.TickCount;

        // �N���҂�����(ms)
        private int m_nWait = 0;

        public FormMain() {
            InitializeComponent();
        }

        /// <summary>
        /// ���̃A�v���P�[�V�������g���ݒ�t�@�C�������擾
        /// </summary>
        private string XmlFileName {
            get {
                // �����̃t���p�X�����擾�B�g���q��xml�ɕύX�B
                // ���̌��ʂ�ݒ�t�@�C���Ƃ��āA���p����B
                return System.IO.Path.ChangeExtension(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName, "xml");
            }
        }

        /// <summary>
        /// �ݒ�t�@�C���Ŏw�肳�ꂽ���s�t�@�C�������s����B
        /// </summary>
        private void ProcessStart() {
            // XmlReader���g���āA�ݒ�t�@�C��(xml)���J��
            System.Xml.XmlReaderSettings rs = new System.Xml.XmlReaderSettings();
            rs.IgnoreWhitespace = true; // �^�O�Ԃ̃z���C�g�X�y�[�X�𖳎�����
            System.Xml.XmlReader r = System.Xml.XmlReader.Create(this.XmlFileName, rs);

            // �m�[�h���ЂƂ��ǂݐi��
            r.ReadToFollowing("user");
            while (r.IsStartElement("user")) {
                string userName = r.GetAttribute("name");

                if (userName == "All Users" || userName == System.Environment.GetEnvironmentVariable("USERNAME")) {
                    r.Read();
                    while (r.IsStartElement("exefile")) {
                        r.Read();

                        // �N������v���Z�X�̂��߂ɁA�I�u�W�F�N�g�𐶐�
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

                        // �w��v���Z�X���N������
                        p.Start();
                        p.WaitForInputIdle();

                        r.ReadEndElement(); // </exefile>��ǂݍ���
                    }
                    r.ReadEndElement(); // </user>��ǂݍ���
                } else {
                    r.ReadToNextSibling("user");
                }
            }

            r.Close();
        }

        /// <summary>
        /// Form.Load �C�x���g
        /// �N���҂��^�C�}�[���N������B
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_Load(object sender, EventArgs e) {
            // XmlReader���g���āA�ݒ�t�@�C��(xml)���J��
            System.Xml.XmlReaderSettings rs = new System.Xml.XmlReaderSettings();
            rs.IgnoreWhitespace = true; // �^�O�Ԃ̃z���C�g�X�y�[�X�𖳎�����
            System.Xml.XmlReader r = System.Xml.XmlReader.Create(this.XmlFileName, rs);

            // �m�[�h���ЂƂ��ǂݐi��
            while (r.Read()) {
                if (r.NodeType == System.Xml.XmlNodeType.Element && r.Name == "delay") {
                    m_nWait = r.ReadElementContentAsInt() * 1000;
                    break;
                }
            }

            r.Close();

            // 1�b�^�C�}�[���N��
            timWait.Interval = 1;
            timWait.Enabled = true;
        }

        /// <summary>
        /// Timer.Tick �C�x���g�B
        /// �N���҂��̃J�E���g�_�E�����s���B
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timWait_Tick(object sender, EventArgs e) {
            // ����Tick - �N��Tick���A�N���҂����Ԃ��߂�����A�t�@�C�������s����
            if ((System.Environment.TickCount - this.m_nStartTick) > this.m_nWait) {

                // �^�C�}�[���Ƃ߂�
                this.timWait.Enabled = false;

                // �v���Z�X�̎��s
                this.ProcessStart();

                // �������g�́A�I��������B
                this.Close();
            } else {
                double nLeft = this.m_nWait - (System.Environment.TickCount - this.m_nStartTick);
                nLeft /= 1000;
                label1.Text = nLeft.ToString("00.000");
            }
        }
    }
}