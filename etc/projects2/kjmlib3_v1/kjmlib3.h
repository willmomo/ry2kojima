#pragma once

#pragma warning(disable : 4786)

#ifdef UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <share.h>

#ifdef USE_WSSLOG
#include "wsslog.h"
#pragma comment(lib, "../kjmlib3_v1/wsslog.lib")
#endif

#ifdef UNICODE
#define _tstring			std::wstring
#define _tcout				std::wcout
#define _tostringstream		std::wostringstream
#define _tostream			std::wostream
#else
#define _tstring			std::string
#define _tcout				std::cout
#define _tostringstream		std::ostringstream
#define _tostream			std::ostream
#endif

_tostream& operator<<(_tostream& os, __int64 i);

namespace kl3 {

	class util {
	public:
		static _tstring get_module_file_name();

		static _tstring get_file_version(const _tstring &strFileName);

		static _tstring get_error_message(DWORD dwError);

		static _tstring create_backup_filename(const _tstring &strOriginalFilename);

		static _tstring chonp(const _tstring &str);

		// 再帰的にフォルダを作成可能な CreateDirectory 関数
		static bool create_directory(const _tstring& strPathName);

		// 再帰的にフォルダを削除可能な RemoveDirectory 関数
		static bool remove_directory(const _tstring& strPathName);

		// テンポラリファイル用のディレクトリのパスを取得します。
		static _tstring get_temp_path();

		// 文字列を置換する。VBのStrReplaceのような関数。
		static _tstring str_replace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace);

	private:
		util() {};
		~util() {};
	};

	class path {
	public:
		static _tstring append(const _tstring& strPath, const _tstring& strMore);

		static _tstring remove_file_spec(const _tstring& strPath);

		static _tstring rename_file_spec(const _tstring& strPath, const _tstring& strFileSpec);

		static bool file_exists(const _tstring& strPath);

		static _tstring find_file_name(const _tstring& strPath);

		// PathRenameExtension 関数のラッパー
		static _tstring rename_extension(const _tstring& strPath, const _tstring& strExt);

		// PathRemoveExtension 関数のラッパー
		static _tstring remove_extension(const _tstring& strPath);

	private:
		path() {};
		~path() {};
	};

	class tool_help {
	public:
		tool_help() : m_hSnapshot(INVALID_HANDLE_VALUE) {};

		virtual ~tool_help();

		virtual bool create_snapshot();

		virtual void close_handle();

		virtual bool process_first();

		virtual bool process_next();

		virtual _tstring get_exe_file() {
			return m_pe.szExeFile;
		};

		virtual DWORD get_process_id() {
			return m_pe.th32ProcessID;
		};

	private:
		HANDLE m_hSnapshot;
		PROCESSENTRY32 m_pe;
	};

	class process {
	public:
		process();

		virtual ~process();

		virtual bool create_process(const _tstring& strCommandLine);

		virtual bool open_process(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);

		virtual void close_handle();

		virtual bool terminate_process(UINT uExitCode);

	private:
		PROCESS_INFORMATION m_processInformation;

	public:
		static bool terminate_process(DWORD dwProcessId, UINT uExitCode);
	};

	//-----------------------------------------------------------------
	// 比較的汎用ログクラス
	//-----------------------------------------------------------------
	class log {
	public:
		// デフォルトコンストラクタ
		log();
		
		// 識別子付きコンストラクタ
		log(const _tstring& strIdent) : m_strIdent(strIdent) {};

		// デストラクタ
		virtual ~log() {};

		// ログの出力関数
		virtual void write(LPCTSTR format, ...);

		// ログの出力関数(画面出力あり)
		virtual void writee(LPCTSTR format, ...);

		// 識別子の設定
		virtual void set_ident(const _tstring& strNewIdent) {
			m_strIdent = strNewIdent;
		};

		// ログファイルのプレフィクスを設定する
		virtual void set_prefix(const _tstring& strPrefix);

	public:
		_tstring m_strIdent;		// ログの識別子
		_tstring m_strBaseFileName;	// 基本になるログファイル名
	};

	//-----------------------------------------------------------------
	// 時間を扱うクラス
	//-----------------------------------------------------------------
	class time {
	public:
//#define FT_1SEC		(__int64)(FT_1MSEC * 1000)

		static const __int64 FT_1USEC;	// = 10
		static const __int64 FT_1MSEC;	// = FT_1USEC * 1000
		static const __int64 FT_1SEC;	// = FT_1MSEC * 1000

	public:
		time() { memset(&m_ft, 0, sizeof(m_ft)); };

		time(FILETIME ft) : m_ft(ft) {};

		// SYSTEMTIMEを受け取るコンストラクタ
		time(const SYSTEMTIME& st);

		virtual ~time() {};

		// 内部時間をローカル時間に変換
		virtual kl3::time &to_local_time();

		// 時間を文字列(yyyy/mm/dd hh:mm:ss)に変換
		virtual _tstring to_string() const;

		// 時間を文字列(YYYYMMDD)に変換
		virtual _tstring to_string_yyyymmdd() const;

		// 時間が同じかどうか比較する
		virtual bool operator==(const kl3::time& rhs) const;

		// 時間の引き算をする
		virtual __int64 operator-(const kl3::time& rhs) const;

		// 内部のFILETIMEを__int64の値として返す
		virtual __int64 to_int64() const;

		// 現在のローカル日時を取得する
		static kl3::time get_local_time();

	private:
		FILETIME m_ft;
	};

	//-----------------------------------------------------------------
	// Find*File関数をラップしたクラス
	//-----------------------------------------------------------------
	class find_file {
	public:
		find_file() : m_hFind(INVALID_HANDLE_VALUE), m_strWildcard(_T("")) {};

		virtual ~find_file();

		virtual bool find_first_file(const _tstring& strFileName);

		virtual bool find_next_file();

		virtual void find_close();

		virtual bool is_dots() {
			return ((lstrcmp(m_findData.cFileName, _T(".")) == 0) || (lstrcmp(m_findData.cFileName, _T("..")) == 0));
		};

		virtual bool is_directory() {
			return ((m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		};

		virtual _tstring get_file_name() {
			return m_findData.cFileName;
		};

		virtual _tstring get_full_name() {
			return kl3::path::rename_file_spec(m_strWildcard, m_findData.cFileName);
		};

		virtual kl3::time get_last_write_time() {
			return kl3::time(m_findData.ftLastWriteTime);
		};

		// ファイルのサイズを返す
		virtual __int64 get_file_size() {
			LARGE_INTEGER li = {m_findData.nFileSizeLow, m_findData.nFileSizeHigh};
			return li.QuadPart;
		};

	private:
		_tstring m_strWildcard;
		HANDLE m_hFind;
		WIN32_FIND_DATA m_findData;
	};

	//---------------------------------------------------------------------
	// ドライブの情報を保持するクラス
	//---------------------------------------------------------------------
	class drive {
	public:
		// デフォルトコンストラクタ
		drive() : m_driveNo(-1) {};

		// コピーコンストラクタ
		drive(const drive& src) : m_driveNo(src.m_driveNo) {};

		// ドライブ番号を指定するコンストラクタ
		drive(int driveNo) : m_driveNo(driveNo) {};

		// ドライブレターを指定するコンストラクタ
		drive(const _tstring& driveLetter);

		// デストラクタ
		virtual ~drive() {};

		// 代入演算子
		drive& operator=(const drive& rhs);

		// ドライブ番号を取得
		int get_DriveNo() { return m_driveNo; };

		// ドライブレターを取得
		_tstring get_DriveLetter() { return _tstring(1, (m_driveNo + 'A')) + _T(":"); };

		// ドライブタイプを取得
		UINT get_DriveType() { return GetDriveType(get_DriveLetter().c_str()); };

		// ドライブタイプを文字列で取得
		_tstring get_DriveTypeString();

	private:
		int m_driveNo;	// ドライブ番号(-1=未選択 0=A 1=B...)
	};

	//---------------------------------------------------------------------
	// ドライブの一覧を保持するクラス
	//---------------------------------------------------------------------
	class drive_list {
	public:
		// デフォルトコンストラクタ
		drive_list();

		// コピーコンストラクタ
		drive_list(const drive_list& src) : m_driveList(src.m_driveList) {};

		// デストラクタ
		virtual ~drive_list() {};

		// 代入演算子
		drive_list& operator=(const drive_list& rhs);

		// ドライブ一覧の数を取得
		int get_Size() { return m_driveList.size(); };

		// ドライブオブジェクトを取得
		drive get_Drive(int index) { return m_driveList[index]; };

	private:
		std::vector<drive> m_driveList;	// PCにつながっているドライブ一覧
	};

	//-----------------------------------------------------------------
	// GetTickCountで経過時間を計るクラス
	//-----------------------------------------------------------------
	class tick_counter {
	public:
		// デフォルトコンストラクタ
		tick_counter() : m_t1(GetTickCount()), m_t2(0) {};

		// デストラクタ
		~tick_counter() {};

		// 計測開始
		void start() { m_t1 = GetTickCount(); m_t2 = 0; };

		// 計測終了
		void stop() { m_t2 = GetTickCount(); };

		// 経過時間(ms)
		DWORD elapse() { return (m_t2 == 0) ? (GetTickCount() - m_t1) : (m_t2 - m_t1); };

		// 経過時間(sec)
		DWORD elapse_sec() { return elapse() / 1000; };

		// 経過時間(min)
		DWORD elapse_min() { return elapse_sec() / 60; };

	private:
		DWORD m_t1;	// 開始した時の時間(コンストラクタ、startで更新)
		DWORD m_t2;	// 終了した時の時間(stopで更新)
	};

	//-----------------------------------------------------------------
	// folder オブジェクト
	//-----------------------------------------------------------------
	class folder {
	public:
		// デフォルトコンストラクタ
		folder();

		// コピーコンストラクタ
		folder(const folder &src);

		// フォルダ名を引数にとるコンストラクタ
		folder(const _tstring &strPath);

		// デストラクタ
		virtual ~folder();

		// フォルダ内のすべてのファイルを返す
		virtual std::vector<_tstring> get_files();

		// フォルダ内のすべてのサブフォルダを返す
		virtual std::vector<_tstring> get_sub_folders();

		// 代入演算子
		folder &operator=(const folder &rhs);

	private:
		_tstring m_path;
		std::vector<_tstring> m_files;
		std::vector<_tstring> m_sub_folders;
	};

	//-----------------------------------------------------------------
	// ライブラリがログ出力に使用する kl3::log オブジェクトを設定
	//-----------------------------------------------------------------
	void set_log_object(kl3::log* pLog);
};
