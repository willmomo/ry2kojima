// /unsafe のとき有効にする
// #define UNSAFE

using System;
using System.Collections.Generic;
using System.Text;

namespace CreateProcess {
    class CreateProcessAPI {
        public const uint NORMAL_PRIORITY_CLASS = 0x0020;

#if UNSAFE
        [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public class SECURITY_ATTRIBUTES {
            public int nLength;
            public unsafe byte* lpSecurityDescriptor;
            public int bInheritHandle;
        }
#else
        [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES {
            public int nLength;
            public IntPtr lpSecurityDescriptor;
            public int bInheritHandle;
        }
#endif

        [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Unicode)]
        public struct STARTUPINFO {
            Int32 cb;
            string lpReserved;
            string lpDesktop;
            string lpTitle;
            Int32 dwX;
            Int32 dwY;
            Int32 dwXSize;
            Int32 dwYSize;
            Int32 dwXCountChars;
            Int32 dwYCountChars;
            Int32 dwFillAttribute;
            Int32 dwFlags;
            Int16 wShowWindow;
            Int16 cbReserved2;
            IntPtr lpReserved2;
            IntPtr hStdInput;
            IntPtr hStdOutput;
            IntPtr hStdError;
        }

        [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct PROCESS_INFORMATION {
            public IntPtr hProcess;
            public IntPtr hThread;
            public int dwProcessId;
            public int dwThreadId;
        }

        [System.Runtime.InteropServices.DllImport("kernel32.dll")]
        public static extern bool CreateProcess(
            string lpApplicationName,
            string lpCommandLine,
            ref SECURITY_ATTRIBUTES lpProcessAttributes,
            ref SECURITY_ATTRIBUTES lpThreadAttributes,
            bool bInheritHandles,
            uint dwCreationFlags,
            IntPtr lpEnvironment,
            string lpCurrentDirectory,
            ref STARTUPINFO lpStartupInfo,
            out PROCESS_INFORMATION lpProcessInformation);
    }

    class Program {
        static void Main(string[] args) {

            // CreateProcess APIを使った、プロセス実行サンプル。
            // System.Diagnostics.Process.Start を使うべきである。

            string strApplication = Environment.GetEnvironmentVariable("windir") + @"\Notepad.exe";
            string strCommandLine = @" c:\boot.ini";
            CreateProcessAPI.PROCESS_INFORMATION pInfo = new CreateProcessAPI.PROCESS_INFORMATION();
            CreateProcessAPI.STARTUPINFO sInfo = new CreateProcessAPI.STARTUPINFO();
            CreateProcessAPI.SECURITY_ATTRIBUTES pSec = new CreateProcessAPI.SECURITY_ATTRIBUTES();
            CreateProcessAPI.SECURITY_ATTRIBUTES tSec = new CreateProcessAPI.SECURITY_ATTRIBUTES();
            pSec.nLength = System.Runtime.InteropServices.Marshal.SizeOf(pSec);
            tSec.nLength = System.Runtime.InteropServices.Marshal.SizeOf(tSec);

            //Open Notepad
            bool retValue = CreateProcessAPI.CreateProcess(
                strApplication,
                strCommandLine,
                ref pSec,
                ref tSec,
                false,
                CreateProcessAPI.NORMAL_PRIORITY_CLASS,
                IntPtr.Zero,
                null,
                ref sInfo,
                out pInfo);

            Console.WriteLine("Process ID (PID): " + pInfo.dwProcessId);
            Console.WriteLine("Process Handle : " + pInfo.hProcess);

            // System.Diagnostics.Process.Start を使った、プロセス実行サンプル。

            System.Diagnostics.Process procNotepad = new System.Diagnostics.Process();

            procNotepad.StartInfo.FileName = Environment.GetEnvironmentVariable("windir") + @"\Notepad.exe";
            procNotepad.StartInfo.Arguments = @"c:\boot.ini";
            procNotepad.Start();

            Console.WriteLine("Process ID (PID): " + procNotepad.Id);
            Console.WriteLine("Process Handle : " + procNotepad.Handle);
        }
    }
}
