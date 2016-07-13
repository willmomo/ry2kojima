using System;

namespace BmpChk
{
	/// <summary>
	/// BmpChkDoc �̊T�v�̐����ł��B
	/// </summary>
	public class BmpChkDoc
	{
		private string m_folderName;
		public string Title
		{
			get
			{
				return (m_folderName == null) ? m_folderName : "(����)";
			}
		}

		private System.Collections.ArrayList m_bmpFiles = new System.Collections.ArrayList();
		public BmpFile[] FileArray
		{
			get
			{
				return (BmpFile[])m_bmpFiles.ToArray(typeof(BmpFile));
			}
		}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		public BmpChkDoc()
		{
			m_folderName = null;
		}

		/// <summary>
		/// �V�����h�L�������g�����
		/// </summary>
		public void NewDocument()
		{
			m_folderName = null;
			m_bmpFiles.Clear();
		}

		/// <summary>
		/// �h�L�������g���J��
		/// openFolder�Ŏw�肳�ꂽ�h�L�������g(�t�H���_)���J���B
		/// </summary>
		/// <param name="openFolder"></param>
		public void OpenDocument(string openFolder)
		{
			m_folderName = openFolder;

			//
			// �w�肳�ꂽ�f�B���N�g�����I�u�W�F�N�g���쐬
			//
			System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(m_folderName);

			//
			// �t�@�C���ꗗ����ABmpFile�I�u�W�F�N�g���쐬
			//
			foreach (System.IO.FileInfo fiTemp in di.GetFiles())
			{
				m_bmpFiles.Add(new BmpFile(fiTemp));
			}
		}

		/// <summary>
		/// �h�L�������g�����
		/// </summary>
		public void CloseDocument()
		{
			m_folderName = null;
			m_bmpFiles.Clear();
		}
	}
}
