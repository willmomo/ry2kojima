using System;

namespace BmpChk
{
	/// <summary>
	/// BmpChkDoc の概要の説明です。
	/// </summary>
	public class BmpChkDoc
	{
		private string m_folderName;
		public string Title
		{
			get
			{
				return (m_folderName == null) ? m_folderName : "(無題)";
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
		/// コンストラクタ
		/// </summary>
		public BmpChkDoc()
		{
			m_folderName = null;
		}

		/// <summary>
		/// 新しいドキュメントを作る
		/// </summary>
		public void NewDocument()
		{
			m_folderName = null;
			m_bmpFiles.Clear();
		}

		/// <summary>
		/// ドキュメントを開く
		/// openFolderで指定されたドキュメント(フォルダ)を開く。
		/// </summary>
		/// <param name="openFolder"></param>
		public void OpenDocument(string openFolder)
		{
			m_folderName = openFolder;

			//
			// 指定されたディレクトリ情報オブジェクトを作成
			//
			System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(m_folderName);

			//
			// ファイル一覧から、BmpFileオブジェクトを作成
			//
			foreach (System.IO.FileInfo fiTemp in di.GetFiles())
			{
				m_bmpFiles.Add(new BmpFile(fiTemp));
			}
		}

		/// <summary>
		/// ドキュメントを閉じる
		/// </summary>
		public void CloseDocument()
		{
			m_folderName = null;
			m_bmpFiles.Clear();
		}
	}
}
