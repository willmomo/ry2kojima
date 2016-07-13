//-----------------------------------------------------------------------------
//
// 小島用ライブラリ
//
//-----------------------------------------------------------------------------
#pragma once

// Windowsのヘッダ
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// STLのヘッダ
#include <locale>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// cout / wscout を切り替えるマクロ
#if defined(UNICODE) || defined(_UNICODE)
#define _tout	std::wcout
#else
#define _tout	std::cout
#endif

namespace rk {

	typedef std::basic_string<TCHAR>		string;				// string / wstring を切り替える
	typedef std::vector<string>				strings;			// string[] / wstring[] 風
	typedef std::basic_ostringstream<TCHAR>	_tostringstream;	// ostringstream / wostringstream を切り替える


	//=========================================================================
	// ディレクトリとサブディレクトリを作成、削除、および列挙するためのインスタ
	// ンス メソッドを公開します。
	//=========================================================================
	class DirectoryInfo {
	public:
		DirectoryInfo(const string& path) : m_path(path) {};
		~DirectoryInfo() {};

	private:
		DirectoryInfo() {};

	private:
		string m_path;
	};


	//=========================================================================
	// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
	// 静的メソッドを公開します。
	//=========================================================================
	class Directory {
	public:
		// 既存のフォルダを新しいフォルダにコピーします。
		static void Copy(const string& sourceDirName, const string& destDirName, bool overwrite=false);

		// path で指定したすべてのディレクトリとサブディレクトリを作成します。
		// 関数名が CreateDirectory だと API とかぶるので、CreateFolder を選択。
		static DirectoryInfo CreateFolder(const string& path);

		// 指定したパスがディスク上の既存のディレクトリを参照しているかどうかを確認します。
		static bool Exists(const string& path);

		// 指定したディレクトリ内のサブディレクトリの名前を取得します。
		static strings GetDirectories(const string& path);

		// 指定したディレクトリ内のファイル名を返します。
		static strings GetFiles(const string& path);

	private:
		Directory() {};
		~Directory() {};
	};


	//=========================================================================
	// ファイルの作成、コピー、削除、移動、オープンのための静的メソッドを提供し
	// ます。
	//=========================================================================
	class File {
	public:
		// 既存のファイルを新しいファイルにコピーします。
		static void Copy(const string& sourceFileName, const string& destFileName, bool overwrite=false);

	private:
		File() {};
		~File() {};
	};


	//=========================================================================
	// 例外の基本クラスを定義します。
	//=========================================================================
	class SystemException {
	public:
		SystemException(DWORD dwError, string msg) : m_dwError(dwError), m_message(msg) {};
		virtual ~SystemException() {};

		DWORD get_Code() { return m_dwError; };
		string get_Message() { return m_message; };

		// エラーコードとパラメータから一般的なエラーメッセージを作成する。
		static string mkmsg(DWORD dwErr, const string& p1=_T(""));

	private:
		DWORD m_dwError;
		string m_message;
	};


	//=========================================================================
	// I/O エラーが発生したときにスローされる例外。
	//=========================================================================
	class IOException : public SystemException {
	public:
		IOException(DWORD dwError, string msg) : SystemException(dwError, msg) {};
		virtual ~IOException() {};
	};


	//=========================================================================
	// オペレーティング システムが I/O エラーまたは特定の種類のセキュリティ エラーのためにアクセスを拒否する場合、スローされる例外。
	//=========================================================================
	class UnauthorizedAccessException : public SystemException {
	public:
		UnauthorizedAccessException(DWORD dwError, string msg) : SystemException(dwError, msg) {};
		virtual ~UnauthorizedAccessException() {};
	};


	//=========================================================================
	// ファイルまたはディレクトリのパス情報を格納する string インスタンスで操作
	// を実行します。これらの操作は、プラットフォーム間で実行されます。
	//=========================================================================
	class Path {
	public:
		// 2 つのパス文字列を結合します。
		static string Combine(const string& path1, const string& path2);

		// 指定したパス文字列のディレクトリ情報を返します。
		static string GetDirectoryName(const string& path);

		// 指定したパス文字列のファイル名と拡張子を返します。
		static string GetFileName(const string& path);

	private:
		Path() {};
		~Path() {};
	};
}
