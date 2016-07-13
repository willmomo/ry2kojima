//=============================================================================
//
//=============================================================================

#pragma once


#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <process.h>

#if defined(UNICODE)
#define _tstring	std::wstring
#define _tcout		std::wcout
#else
#define _tstring	std::string
#define _tcout		std::cout
#endif


namespace kjm {

	//-------------------------------------------------------------------------
	// ログクラス
	//
	// プロセス内に、グローバルアクセス可能なインスタンスを一つ生成し、
	// 各スレッドで、使いまわせるクラス。
	//-------------------------------------------------------------------------
	class log {
	public:
		log();
		~log();

	public:
		// ログに書く
		void write(const _tstring& msg);

	private:
		_tstring m_logName;
		CRITICAL_SECTION m_cs;
	};


	//-------------------------------------------------------------------------
	// ユーティリティ関数のクラス
	//-------------------------------------------------------------------------
	class util {
	private:
		util() {}
		~util() {}

	public:
		// CreateDirectory 関数のラッパー
		static bool create_directory(const _tstring& path);

		// GetEnvironmentVariable 関数のラッパー
		static _tstring get_environment_variable(const _tstring& env);

		// GetModuleFileName 関数のラッパー
		static _tstring get_module_file_name();

		// GetPrivateProfileString 関数のラッパー
		static _tstring get_private_profile_string(const _tstring& src, const _tstring& key, const _tstring& def, const _tstring& ini);

		// LoadString 関数のラッパー
		static _tstring load_string(UINT uID);

		// PathAppend 関数のラッパー
		static _tstring path_append(const _tstring& path, const _tstring& more);

		// PathFileExists 関数のラッパー
		static bool path_file_exists(const _tstring& path);

		// PathFindFileName 関数のラッパー
		static _tstring path_find_file_name(const _tstring& path);

		// PathIsRoot 関数のラッパー
		static bool path_is_root(const _tstring& path);

		// PathRemoveExtension 関数のラッパー
		static _tstring path_remove_extension(const _tstring& path);

		// PathRenameExtension 関数のラッパー
		static _tstring path_rename_extension(const _tstring& path, const _tstring& ext);

		// PathRemoveFileSpec 関数のラッパー
		static _tstring path_remove_file_spec(const _tstring& path);

		// フルパスからファイル名部分(拡張子除く)を取り出す
		static _tstring path_split_fname(const _tstring& path);
	};

	//-------------------------------------------------------------------------
	// スレッドクラス
	//-------------------------------------------------------------------------
	class thread {
	public:
		thread() : m_loop(false), m_hThread(NULL) {}
		virtual ~thread() {}

	public:
		// スレッド開始
		bool begin();

		// スレッド関連変数を閉じる
		void close();

		// スレッドを終了
		void end();

		// 終了指示が来ているかどうかを調べる
		bool is_end();

		// スレッドを再開
		DWORD resume();

		// スレッドを一時中断
		DWORD suspend();

		// スレッド終了まで待つ
		void wait();

		// 何もしないダミースレッド(一般実装)
		virtual unsigned worker();

	private:
		// 共通ワーカースレッド
		static unsigned __stdcall worker_func(void* parg);

	private:
		HANDLE m_hThread;
		bool m_loop;
	};
}

