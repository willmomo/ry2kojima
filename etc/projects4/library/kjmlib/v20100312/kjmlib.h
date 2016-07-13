//---------------------------------------------------------------------
// 何度も作ってる codesnipet
//---------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

#if defined(UNICODE)
typedef std::wstring	_tstring;
#else
typedef std::string		_tstring;
#endif

// 安全な sprintf 
int sprintf_vec(std::vector<char>& buf, const char* format, ...);

namespace kjm {
	//-----------------------------------------------------------------
	// 時間クラス
	//-----------------------------------------------------------------
	class time {
	public:
		// デフォルトコンストラクタ
		time();

		// デストラクタ
		~time();

		// 文字列を単純な YYYY/MM/DD HH:MM:SS 文字列に変換します。
		_tstring to_string();

		// 現在時間を100ns単位で返す
		__int64 get_time() { return *(__int64*)&m_ft; };

		// GetLocalTime
		// 現在のローカル日時を取得します。
		static kjm::time get_local_time();
	private:
		FILETIME m_ft;
	};
};
