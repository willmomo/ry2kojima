using System;

namespace BmpChk
{
	/// <summary>
	/// BmpFile �̊T�v�̐����ł��B
	/// </summary>
	public class BmpFile
	{
		private System.IO.FileInfo m_fi;
		public string Name
		{
			get
			{
				return m_fi.Name;
			}
		}
		private BitmapFileHeader m_bfh;
		public bool IsBMPFile
		{
			get
			{
				return m_bfh.IsBMPFile;
			}
		}
		private BitmapInfoHeader m_bih;
		public int XPixelsPerMeter
		{
			get
			{
				return m_bih.XPixelsPerMeter;
			}
		}
		public double XDotPerInch
		{
			get
			{
				return m_bih.XDotPerInch;
			}
		}
		public int YPixlesPerMeter
		{
			get
			{
				return m_bih.YPixlesPerMeter;
			}
		}
		public double YDotPerInch
		{
			get
			{
				return m_bih.YDotPerInch;
			}
		}

		/// <summary>
		/// �R���X�g���N�^
		/// �w�肳�ꂽ�t�@�C����BMP�t�@�C���ł���΁A�ǂݍ��ށB
		/// </summary>
		/// <param name="fi"></param>
		public BmpFile(System.IO.FileInfo fi)
		{
			m_fi = fi;

			// �w��t�@�C�����J��
			System.IO.FileStream fs = new System.IO.FileStream(fi.FullName, System.IO.FileMode.Open, System.IO.FileAccess.Read);

			// BinaryReader�Ɋ֘A�t����
			System.IO.BinaryReader reader = new System.IO.BinaryReader(fs);

			// BitmapFileHeader��ǂݍ���
			m_bfh = new BitmapFileHeader(reader);

			// BMP�t�@�C���������Ƃ��́A
			// ����BITMAPINFOHEADER��ǂݍ��ށB
			if (m_bfh.IsBMPFile)
			{
				m_bih = new BitmapInfoHeader(reader);
			}

			// ��n��
			fs.Close();
		}
	}
}
