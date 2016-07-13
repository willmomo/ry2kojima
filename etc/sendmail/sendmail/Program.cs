using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Mail;

namespace sendmail {
    class SendMail {
        // (-h) smtp host
        public string m_host = "";
        // (-f) From
        public string m_from = "";
        // (-t) To
        public string m_to = "";
        // (-s) Subject
        public string m_subject = "";
        // (-b) Body
        public string m_body = "";

        /// <summary>
        /// メールを送信する
        /// </summary>
        /// <returns></returns>
        public void Send() {
            System.Net.Mail.SmtpClient client = new System.Net.Mail.SmtpClient(m_host);
            System.Net.Mail.MailMessage message = new System.Net.Mail.MailMessage(m_from, m_to, m_subject, m_body);

            client.Timeout = 500;
            client.Send(message);

            System.Console.WriteLine("send end");
        }
    }

    class Program {

        public static void CreateTimeoutTestMessage(string server) {
            string to = "jane@contoso.com";
            string from = "ben@contoso.com";
            string subject = "Using the new SMTP client.";
            string body = @"Using this new feature, you can send an e-mail message from an application very easily.";
            MailMessage message = new MailMessage(from, to, subject, body);
            SmtpClient client = new SmtpClient(server);
            Console.WriteLine("Changing time out from {0} to 100.", client.Timeout);
            client.Timeout = 100;
            // Credentials are necessary if the server requires the client 
            // to authenticate before it will send e-mail on the client's behalf.
            client.Credentials = CredentialCache.DefaultNetworkCredentials;
            client.Send(message);
        }

        static void Main(string[] args) {
            SendMail sm = new SendMail();

            // オプションを調べて、値をセットする。
            for (int i = 0; i < args.Length; i++) {
                string sw = args[i].Substring(0, 2);
                string vl = args[i].Substring(2);

                if (sw == "-h") {
                    sm.m_host = vl;
                } else if (sw == "-f") {
                    sm.m_from = vl;
                } else if (sw == "-t") {
                    sm.m_to = vl;
                } else if (sw == "-s") {
                    sm.m_subject = vl;
                } else if (sw == "-b") {
                    sm.m_body = vl;
                }
            }

            CreateTimeoutTestMessage("smtp.mail.yahoo.co.jp");
            //sm.Send();
        }
    }
}
