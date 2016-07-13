using System;

namespace BmpChk
{
	/// <summary>
	/// BitmapInfoHeader �̊T�v�̐����ł��B
	/// </summary>
	public class BitmapInfoHeader
	{
		// BMPINFOHEADER�\���̑����̃����o
		private uint m_biSize;
		private int m_biWidth;
		private int m_biHeight; 
		private ushort m_biPlanes; 
		private ushort m_biBitCount; 
		private uint m_biCompression; 
		private uint m_biSizeImage; 
		private int m_biXPelsPerMeter; 
		public int XPixelsPerMeter
		{
			get
			{
				return m_biXPelsPerMeter;
			}
		}
		public double XDotPerInch
		{
			get
			{
				return (double)m_biXPelsPerMeter * 0.0254;
			}
		}
		private int m_biYPelsPerMeter; 
		public int YPixlesPerMeter
		{
			get
			{
				return m_biYPelsPerMeter;
			}
		}
		public double YDotPerInch
		{
			get
			{
				return (double)m_biYPelsPerMeter * 0.0254;
			}
		}
		private uint m_biClrUsed; 
		private uint m_biClrImportant; 

		/// <summary>
		/// �R���X�g���N�^
		/// reader�I�u�W�F�N�g����A�l��ǂݏo���ă����o�[������������B
		/// </summary>
		/// <param name="reader"></param>
		public BitmapInfoHeader(System.IO.BinaryReader reader)
		{
			m_biSize = reader.ReadUInt32();
			m_biWidth = reader.ReadInt32();
			m_biHeight = reader.ReadInt32(); 
			m_biPlanes = reader.ReadUInt16(); 
			m_biBitCount = reader.ReadUInt16(); 
			m_biCompression = reader.ReadUInt32(); 
			m_biSizeImage = reader.ReadUInt32(); 
			m_biXPelsPerMeter = reader.ReadInt32(); 
			m_biYPelsPerMeter = reader.ReadInt32(); 
			m_biClrUsed = reader.ReadUInt32(); 
			m_biClrImportant = reader.ReadUInt32(); 
		}
	}
}
