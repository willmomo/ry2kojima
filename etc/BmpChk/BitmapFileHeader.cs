using System;

namespace BmpChk
{
	/// <summary>
	/// BitmapFileHeader の概要の説明です。
	/// </summary>
	public class BitmapFileHeader
	{
		// BMPファイルか否かのフラグ
		private bool m_bBMPFile;
		public bool IsBMPFile
		{
			get
			{
				return m_bBMPFile;
			}
		}

		// BITMAPFILEHEADER構造体のメンバ相当
		private ushort m_bfType;
		private uint m_bfSize;
		private ushort m_bfReserved1;
		private ushort m_bfReserved2;
		private uint m_bfOffBits;

		/// <summary>
		/// コンストラクタ
		/// 指定されたreaderオブジェクトから、データ読み出して、メンバを初期化する。
		/// </summary>
		/// <param name="reader"></param>
		public BitmapFileHeader(System.IO.BinaryReader reader)
		{
			m_bBMPFile = false;

			// 先頭2バイト読み出す。
			m_bfType = reader.ReadUInt16();

			// 'BM'で始まるときは、BMPファイルとして処理を続ける。
			if (m_bfType == ('M' << 8 | 'B')) 
			{
				m_bfSize = reader.ReadUInt32();
				m_bfReserved1 = reader.ReadUInt16();
				m_bfReserved2 = reader.ReadUInt16();
				m_bfOffBits = reader.ReadUInt32();

				m_bBMPFile = true;
			}
		}
	}
}
