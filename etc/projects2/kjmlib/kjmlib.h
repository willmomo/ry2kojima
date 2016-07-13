#pragma once

#include <windows.h>

#include <string>

namespace kjm {
	//-----------------------------------------------------------------
	// ユーティリティ関数郡
	//
	// ユーティリティクラスなので、インスタンス化しない。
	//-----------------------------------------------------------------
	class util {
	public:
		// MBCSをUNICODEに変換
		static std::wstring to_stringW(const std::string &str);

		// UNICODEをMBCSに変換
		static std::string to_stringA(const std::wstring &str);
		
	private:
		// デフォルト コンストラクタ
		util() {};

		// デストラクタ
		~util() {};
	};

	//-----------------------------------------------------------------
	// GetTickCountを使って経過時間を計る
	//-----------------------------------------------------------------
	class tick_counter {
	public:
		// デフォルト コンストラクタ
		tick_counter();

		// コピー コンストラクタ
		tick_counter(const tick_counter &src);

		// デストラクタ
		~tick_counter();

		// 代入演算子
		tick_counter &operator =(const tick_counter &rhs);

		// 計測開始
		void start();

		// 計測終了
		DWORD stop();

		// 経過時間
		DWORD elapse();

		// 経過時間(秒)
		DWORD elapse_sec();

		// 経過時間(分)
		DWORD elapse_min();

	private:
		DWORD m_t1;		// 開始時間
		DWORD m_t2;		// 終了時間
	};

	//-----------------------------------------------------------------
	// パス文字列を扱う
	//
	// ユーティリティクラスなので、インスタンス化しない。
	//-----------------------------------------------------------------
	class path {
	public:
		// PathAppendW 相当の関数
		static std::wstring appendW(const std::wstring &strPath, const std::wstring &strMore);

		// PathAppendA相当の関数
		static std::string appendA(const std::string &strPath, const std::string &strMore);

#ifdef UNICODE
#define append	appendW
#else
#define append	appendA
#endif

		// PathRemoveFileSpecW 相当の関数
		static std::wstring remove_file_specW(const std::wstring& strPath);

		// PathRemoveFileSpecA 相当の関数
		static std::string remove_file_specA(const std::string& strPath);

#ifdef UNICODE
#define remove_file_spec	remove_file_specW
#else
#define remove_file_spec	remove_file_specA
#endif

	private:
		// デフォルト コンストラクタ
		path() {};

		// デストラクタ
		~path() {};
	};
};
