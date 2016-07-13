using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Runtime.InteropServices;

namespace comport
{
    public class Win32Interop
    {
        [DllImport("crtdll.dll")]
        public static extern int _kbhit();
    }

    class Program
    {
        /// 
        /// USBタイプのシリアルポートを抜くとアプリケーションが異常終了する現象を
        /// 回避するための SerialPort クラス
        /// 
        public class SafeSerialPort : System.IO.Ports.SerialPort
        {
            bool userOpen = false;
            System.IO.Stream SafeBaseStream = null;

            public new void Open()
            {
                base.Open();
                userOpen = true;
                SafeBaseStream = this.BaseStream;
            }

            protected override void Dispose(bool disposing)
            {
                if (userOpen && base.IsOpen == false)
                {
                    // 予期せず COM ポートが閉じている
                    try
                    {
                        userOpen = false;
                        if (disposing)
                        {
                            SafeBaseStream.Close();
                        }
                    }
                    catch (UnauthorizedAccessException)
                    {
                        // 必ず例外が発生するが 無視する
                    }
                }
                try
                {
                    base.Dispose(disposing);
                }
                catch (UnauthorizedAccessException)
                {
                    int a = 1;
                }
            }
        }

        static bool open_port(SafeSerialPort sp)
        {
            try
            {
                Console.WriteLine("開きます。");
                sp.PortName = "COM8";
                sp.BaudRate = 19200;
                sp.DataBits = 8;
                sp.Parity = Parity.Even;
                sp.StopBits = StopBits.One;

                sp.Open();
            }
            catch (System.IO.IOException e)
            {
                Console.WriteLine(e.Message);
                return false;
            }

            return true;
        }

        static int write_read(SafeSerialPort sp)
        {
            while (true)
            {
                if (Win32Interop._kbhit() != 0) break;

                try
                {
                    byte[] wbuffer = new byte[12];
                    wbuffer[0] = 0xfe;
                    wbuffer[1] = 0x0c;
                    wbuffer[2] = 0x08;
                    wbuffer[3] = 0x45;
                    wbuffer[4] = 0x77;
                    wbuffer[5] = 0x01;
                    wbuffer[6] = 0x31;
                    wbuffer[7] = 0x32;
                    wbuffer[8] = 0x33;
                    wbuffer[9] = 0x34;
                    wbuffer[10] = 0x33;
                    wbuffer[11] = 0xff;
                    sp.Write(wbuffer, 0, 12);

                    byte[] rbuffer = new byte[12];
                    int byteRead = sp.Read(rbuffer, 0, 12);
                    Console.WriteLine("読み込み1 " + byteRead);

                    System.Threading.Thread.Sleep(1000);

                    wbuffer[0] = 0xfe;
                    wbuffer[1] = 0x0c;
                    wbuffer[2] = 0x08;
                    wbuffer[3] = 0x45;
                    wbuffer[4] = 0x77;
                    wbuffer[5] = 0x02;
                    wbuffer[6] = 0x31;
                    wbuffer[7] = 0x32;
                    wbuffer[8] = 0x33;
                    wbuffer[9] = 0x34;
                    wbuffer[10] = 0x30;
                    wbuffer[11] = 0xff;
                    sp.Write(wbuffer, 0, 12);

                    sp.Read(rbuffer, 0, 12);
                    Console.WriteLine("読み込み2 " + byteRead);
                    System.Threading.Thread.Sleep(1000);
                }
                catch (System.IO.IOException e)
                {
                    Console.WriteLine(e.Message);
                    return -1;
                }
                catch (System.InvalidOperationException e)
                {
                    Console.WriteLine(e.Message);
                    return -1;
                }
                catch (System.UnauthorizedAccessException e)
                {
                    Console.WriteLine(e.Message);
                    return -1;
                }
            }

            return 0;
        }

        static void Main(string[] args)
        {
            SafeSerialPort sp = new SafeSerialPort();

            while (true)
            {
                while (open_port(sp) == false)
                {
                    Console.WriteLine("オープン失敗");
                    System.Threading.Thread.Sleep(1000);
                }

                int n = write_read(sp);
                if (n == 0) break;
            }

            Console.WriteLine("ユーザーによる終了");

            sp.Close();
        }
    }
}
