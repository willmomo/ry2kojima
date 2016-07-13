//---------------------------------------------------------------------
// ver.0.2014.11.27
//---------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;

namespace Kjm {
	class Util {
		[StructLayout(LayoutKind.Sequential)]
		public struct FLASHWINFO {
			public UInt32 cbSize;		// FLASHWINFO構造体のサイズ
			public IntPtr hwnd;			// 点滅対象のウィンドウ・ハンドル
			public UInt32 dwFlags;		// 以下の「FLASHW_XXX」のいずれか
			public UInt32 uCount;		// 点滅する回数
			public UInt32 dwTimeout;	// 点滅する間隔（ミリ秒単位）
		}

		public const UInt32 FLASHW_STOP = 0;		// 点滅を止める
		public const UInt32 FLASHW_CAPTION = 1;		// タイトルバーを点滅させる
		public const UInt32 FLASHW_TRAY = 2;		// タスクバー・ボタンを点滅させる
		public const UInt32 FLASHW_ALL = 3;			// タスクバー・ボタンとタイトルバーを点滅させる
		public const UInt32 FLASHW_TIMER = 4;		// FLASHW_STOPが指定されるまでずっと点滅させる
		public const UInt32 FLASHW_TIMERNOFG = 12;	// ウィンドウが最前面に来るまでずっと点滅させる

		[DllImport("user32.dll")]
		public static extern Int32 FlashWindowEx(ref FLASHWINFO pfwi);
	}
}
