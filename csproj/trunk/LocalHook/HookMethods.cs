using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

class HookMethods {
	// フックプロシージャのためのデリゲート
	public delegate IntPtr HookProcedureDelegete(int nCode, IntPtr wParam, IntPtr lParam);

	// フックプロシージャ 'lpfn' をフックチェーン内にインストールする
	// 返り値はフックプロシージャのハンドル
	[DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
	public static extern IntPtr SetWindowsHookEx(int idHook, HookProcedureDelegete lpfn, IntPtr hInstance, int threadId);

	// フックプロシージャを取り除く
	[DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
	public static extern bool UnhookWindowsHookEx(IntPtr idHook);

	// 次のフックプロシージャに情報を渡す
	[DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
	public static extern IntPtr CallNextHookEx(IntPtr idHook, int nCode, IntPtr wParam, IntPtr lParam);
}
