// GuaranteeUser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

// 自分用ユーティリティクラス
//
class KjmUtil {
public:

	// charな文字列をwchar_t文字列に変換
	static std::wstring StrToWStr(const char *src);
};

std::wstring KjmUtil::StrToWStr(const char *src)
{
	size_t needed = mbstowcs(NULL, src, strlen(src));

	std::vector<std::wstring::value_type> dst(needed);

	mbstowcs( &dst[0], src, strlen(src) );

	return std::wstring(dst.begin(), dst.end());
}

// ユーザー情報管理クラス
//
class UserInfo {
private:
	USER_INFO_3* m_pui;

	// 確保済みのバッファを破棄する
	// バッファ未確保の場合は、何もしない
	void ReleaseBuffer() {
		if (m_pui) {
			::NetApiBufferFree(m_pui);
			m_pui = NULL;
		}
	};

public:
	// デフォルトコンストラクタ
	UserInfo() : m_pui(NULL) {};

	// ユーザー名を指定しての初期化付きコンストラクタ
	UserInfo(const _TCHAR* userName) {
		m_pui = NULL;

		// ユーザー情報の取得
		GetInfo(userName);
	};

	// デストラクタ
	virtual ~UserInfo() {
		// バッファを開放。
		ReleaseBuffer();
	};

	// このインスタンスが有効なユーザー情報を保持しているかどうかの判定
	bool IsAvailable() {
		return (m_pui) ? true : false;
	};

	// 指定したユーザーの情報を取得
	bool GetInfo(const _TCHAR* userName);

	// パスワードが無期限かどうか調べる
	bool IsDontExpirePassword() {
		return (m_pui->usri3_flags & UF_DONT_EXPIRE_PASSWD) ? true : false;
	};
};

// 指定したユーザーの情報を取得
//
bool UserInfo::GetInfo(const _TCHAR* userName) {

	// すでに別のユーザー情報を保持しているときは、バッファを開放。
	ReleaseBuffer();

	// ユーザー情報を取得
	NET_API_STATUS ret = ::NetUserGetInfo( NULL, KjmUtil::StrToWStr(userName).c_str(), 3, (LPBYTE*)&m_pui );

	return (ret == NERR_Success) ? true : false;
}

// プログラムスタート
//
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1) {
		UserInfo ui(argv[1]);
		std::cout << ui.IsDontExpirePassword() << std::endl;
	}

	return 0;
}

