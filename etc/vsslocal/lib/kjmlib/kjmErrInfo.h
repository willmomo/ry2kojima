//-----------------------------------------------------------------------------
// エラー情報を保持するクラス
//-----------------------------------------------------------------------------
#pragma once

#include "kjmBase.h"

namespace kjm {
	class errInfo {
	public:
		// デフォルトコンストラクタ
		errInfo() : m_dwInnerErr(0), m_dwErr(0), m_strMsg() {}

		// エラー番号とメッセージを受けるコンストラクタ(Unicode)
		errInfo(DWORD dwErr, const std::wstring& strMsg) : m_dwInnerErr(0), m_dwErr(dwErr), m_strMsg(strMsg) {}

		// エラー番号とメッセージを受けるコンストラクタ(Ascii)
		errInfo(DWORD dwErr, const std::string& strMsg) : m_dwInnerErr(0), m_dwErr(dwErr), m_strMsg(kjm::util::toUnicode(strMsg)) {}

		// エラー番号だけを受け取るコンストラクタ
		errInfo(DWORD dwErr);

		// デストラクタ
		virtual ~errInfo() {}

	public:
		// FormatError 実行時のエラーコード
		DWORD get_innerErrNumber() { return m_dwInnerErr; }

		// エラー番号を返す
		DWORD get_errNumber() { return m_dwErr; }

		// エラー文字列を返す(Unicode版)
		std::wstring get_errMessageW() { return m_strMsg; }

		// エラー文字列を返す(SJIS版)
		std::string get_errMessageA() { return kjm::util::toSJIS(m_strMsg); }

#if defined(UNICODE) || defined(_UNICODE)
#define get_errMessage	get_errMessageW
#else
#define get_errMessage	get_errMessageA
#endif

	private:
		DWORD m_dwInnerErr;
		DWORD m_dwErr;
		std::wstring m_strMsg;
	};
};
