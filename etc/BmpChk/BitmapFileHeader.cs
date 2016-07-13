using System;

namespace BmpChk
{
	/// <summary>
	/// BitmapFileHeader �̊T�v�̐����ł��B
	/// </summary>
	public class BitmapFileHeader
	{
		// BMP�t�@�C�����ۂ��̃t���O
		private bool m_bBMPFile;
		public bool IsBMPFile
		{
			get
			{
				return m_bBMPFile;
			}
		}

		// BITMAPFILEHEADER�\���̂̃����o����
		private ushort m_bfType;
		private uint m_bfSize;
		private ushort m_bfReserved1;
		private ushort m_bfReserved2;
		private uint m_bfOffBits;

		/// <summary>
		/// �R���X�g���N�^
		/// �w�肳�ꂽreader�I�u�W�F�N�g����A�f�[�^�ǂݏo���āA�����o������������B
		/// </summary>
		/// <param name="reader"></param>
		public BitmapFileHeader(System.IO.BinaryReader reader)
		{
			m_bBMPFile = false;

			// �擪2�o�C�g�ǂݏo���B
			m_bfType = reader.ReadUInt16();

			// 'BM'�Ŏn�܂�Ƃ��́ABMP�t�@�C���Ƃ��ď����𑱂���B
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
