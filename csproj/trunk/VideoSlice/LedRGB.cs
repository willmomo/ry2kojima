using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.IO;

namespace VideoSlice {
	class Log {
		public static void d(string tag, string msg) {
			// ダミー関数
		}
	}

	class Integer {
		public const int SIZE = 32;	// 2 の補数バイナリ形式で int 値を表すのに使用されるビット数です。
	}

	class Byte {
		public const int SIZE = 8;	// 2 の補数バイナリ形式で byte 値を表すのに使用されるビット数です。
	}

	class LedRGB {
		private static String TAG = "LedRGB";

		private static short[] LSB_TABLE_A = { (0x49|0x00),(0x49|0x80),(0x49|0x10),(0x49|0x90),(0x49|0x02),(0x49|0x12),(0x49|0x82),(0x49|0x92)};
		private static short[] LSB_TABLE_B = {(0x92|0x00),(0x92|0x20),(0x92|0x04),(0x92|0x24)};
		private static short[] LSB_TABLE_C = {(0x24|0x00),(0x24|0x40),(0x24|0x08),(0x24|0x48),(0x24|0x01),(0x24|0x41),(0x24|0x09),(0x24|0x49)};

		// 波形信号
		//public byte[][] grb_signal = new byte[3][3];
		public byte[,] grb_signal = new byte[3,3];
		private byte[] grb = new byte[3];

		public LedRGB() {
			set_rgb((byte)0x00, (byte)0x00, (byte)0x00);
		}

		public LedRGB(byte r, byte g, byte b) {
			set_rgb(r, g, b);
		}

		public LedRGB(int rgb) {
			/*
			byte[] tmp = fromInt(rgb);
			// GRB順に並び替える
			set_rgb(tmp[2], tmp[1], tmp[3]);
			 * */
		}

		public void set_rgb(byte r, byte g, byte b){
			Log.d(TAG, "r: " + r + ", g: " + g + ", b: " + b);
			// GRB順に並び替える
			this.grb[0] = g;
			this.grb[1] = r;
			this.grb[2] = b;

		}

		// int -> byte[]
		private static byte[] fromInt(int value) {
			/*
			int arraySize = Integer.SIZE / Byte.SIZE;
			//ByteBuffer buffer = ByteBuffer.allocate(arraySize);
			MemoryStream buffer = new MemoryStream(arraySize);
			return buffer.putInt(value).array();
			 * */
			return null;
		}

		public byte[,] CalcRGB() {
			Log.d(TAG, "CalcRGB: grb.length: " + grb.Length);
			for(int i=0; i<grb.Length; i++){
				Log.d(TAG, "CalcRGB: grb[" + i +  "]: " + grb[i]);
				grb_signal[i,0] = (byte)LSB_TABLE_A[(grb[i] >> 5)];
				grb_signal[i,1] = (byte)LSB_TABLE_B[((grb[i] >> 3 & 0x03))];
				grb_signal[i,2] = (byte)LSB_TABLE_C[(grb[i] & 0x07)];
			}
			return grb_signal;
		}

		public byte[] signal2array(byte[][] signal){
			byte[] si = new byte[3];
			/*
			List<Byte> list = new List<Byte>();
			for(byte[] n1:signal){
				for(byte n2:n1){
					list.add(n2);
				}
			}
			si = new byte[list.size()];
			// Iteratorでの書き方
			int cnt = 0;
			for(Iterator it = list.iterator(); it.hasNext(); ) {
				si[cnt] = (byte)it.next();
				cnt++;
			}
			 * */
			return si;
		}
	}
}
