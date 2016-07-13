// 動作環境チェッカー
//

#if defined(_UNICODE)
#define UNICODE
#endif

#include <windows.h>
#include <lm.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <tchar.h>
#include <conio.h>

// 配列クラス
//
template <class T> class Array {
private:
	T* m_p;			// バッファへのポインタ
	size_t m_size;	// バッファの要素数

public:
	// デフォルト コンストラクタ
	//
	Array() : m_p(NULL), m_size(0) {};

	// コピーコンストラクタ
	//
	Array(const Array<T>& src) {
		// 空Arrayの時は、空にして終了
		if (src.GetSize() == 0) {
			m_p = NULL;
			m_size = 0;
			return;
		}

		m_p = new T[m_size = src.GetSize()];
		for (size_t i = 0; i < m_size; i++) {
			m_p[i] = src.GetAt(i);
		}
	};

	// 初期サイズ指定 コンストラクタ
	//
	Array(size_t size) : m_p(NULL), m_size(size) {
		if (m_size > 0) {
			m_p = new T[m_size];
		}
	};

	// 配列指定した コンストラクタ
	Array(const T* src, size_t size) : m_p(NULL), m_size(size) {
		if (m_size > 0) {
			m_p = new T[m_size];
			for (size_t i = 0; i < m_size; i++)
				m_p[i] = src[i];
		}
	};

	// デストラクタ
	//
	~Array() { RemoveAll(); };

public:
	// 要素数を取得
	//
	size_t GetSize() const { return m_size; };

	// 要素数を設定
	//
	void SetSize(size_t nNewSize) {
		if (m_size == nNewSize)	return;	// サイズに変更が無い
		
		if (nNewSize == 0) {	// 空にする(RemoveAll相当)
			RemoveAll();
			return;
		}

		T* temp = new T[nNewSize];
		for (size_t i = 0; i < min(m_size, nNewSize); i++) {
			temp[i] = m_p[i];
		}
		if (m_p)	delete[] m_p;
		m_p = temp;
		m_size = nNewSize;
	};

	// 要素数を0にする
	//
	void RemoveAll() {
		if (m_p)	delete[] m_p;
		m_p = NULL;
		m_size = 0;
	};

	// 指定した位置の要素を返す
	//
	T GetAt(int nIndex) const { return m_p[nIndex]; };

	// 指定した位置に要素を設定する
	//
	void SetAt(int nIndex, const T& newElement) { m_p[nIndex] = newElement; };

	// 要素への参照を返す
	//
	T& ElementAt(int nIndex) { return m_p[nIndex]; };

	// 配列の先頭アドレスを返す
	//
	const T* GetData() const { return m_p; };
	T* GetData() { return m_p; };

	// 配列の後ろに要素の追加
	//
	int Add(const T& newElement) {
		
		// 配列をひとつ拡張して、拡張した場所に値を代入
		SetSize(GetSize() + 1);
		m_p[GetSize() - 1] = newElement;

		return GetSize() - 1;
	};

	// 指定した位置の要素、または、参照を返す
	//
	T operator [](int nIndex) const { return m_p[nIndex]; };
	T& operator [](int nIndex) { return m_p[nIndex]; };

	// 型変換
	//
	operator const T*() const { return m_p; };
	operator T*() { return m_p; };

	// 代入演算子
	//
	Array<T>& operator =(const Array<T>& src) {
		if (this == src)	return *this;	// 同一コピーを防止

		RemoveAll();	// 自分を空にする

		if (src.GetSize() == 0)	return *this;	// srcが空のときはここで終了

		m_p = new T[m_size = src.GetSize()];
		for (int i = 0; i < m_size; i++) {
			m_p[i] = src[i];
		}
	};
};

// 各種配列型
//
typedef Array<char>		ar_char;
typedef Array<wchar_t>	ar_wchar;
typedef Array<_TCHAR>	ar_tchar;

// 自身の実行ファイル名を取得する
//
ar_tchar GetExeName()
{
	ar_tchar buf(MAX_PATH);

	while (GetModuleFileName(NULL, buf, buf.GetSize()) == buf.GetSize())
		buf.SetSize(buf.GetSize() * 2);

	return buf;
}

// 拡張子を置き換える
//
ar_tchar RenameExt(const _TCHAR* src, const _TCHAR* ext)
{
	ar_tchar src_buf(src, _tcslen(src) + 1);
	ar_tchar dst_buf(MAX_PATH);
	ar_tchar drive(_MAX_DRIVE), dir(_MAX_DIR), fname(_MAX_FNAME);

	_tsplitpath(src_buf, drive, dir, fname, NULL);
	_tmakepath(dst_buf, drive, dir, fname, ext);

	return dst_buf;
}

// charな文字列をwchar_t文字列に変換
//
ar_wchar StrToWStr(const char *src)
{
	size_t needed = mbstowcs(NULL, src, strlen(src));

	ar_wchar dst(needed);

	mbstowcs(dst, src, strlen(src));

	dst.Add(L'\0');	// 文字列終端を追加

	return dst;
}

// wchar_tな文字列をchar文字列に変換
//
ar_char WStrToStr(const wchar_t* src)
{
	size_t needed = wcstombs(NULL, src, wcslen(src));

	ar_char dst(needed);

	wcstombs(dst, src, wcslen(src));

	dst.Add('\0');	// 文字列終端を追加

	return dst;
}

// 指定ユーザーでログイン可能かどうかのチェック
//
DWORD LoginCheck(const _TCHAR* name, const _TCHAR* pass)
{
	HANDLE hToken;

	if (LogonUser((LPTSTR)name, NULL, (LPTSTR)pass, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken)) {
		CloseHandle(hToken);
	}

	return GetLastError();
}

// ユーザー情報チェック
//
//  -1: ユーザーが未登録
//  -2: パスワードが無期限ではない
//  -3: ユーザー情報にアクセスする権限が無い
//  -4: コンピューター名が間違っている
// -99: その他のエラー
//
int UserInfo(const _TCHAR* name)
{
	int ret = 0;
	PUSER_INFO_3 pui;

#if !defined(_UNICODE)
	NET_API_STATUS nret = NetUserGetInfo(NULL, StrToWStr(name), 3, (LPBYTE*)&pui);
#else
	NET_API_STATUS nret = NetUserGetInfo(NULL, name, 3, (LPBYTE*)&pui);
#endif

	switch (nret) {
	case NERR_Success:

		if ((pui->usri3_flags & UF_DONT_EXPIRE_PASSWD) == 0) {
			ret = -2;
		}

		NetApiBufferFree(pui);
		break;

	case NERR_UserNotFound:
		ret = -1;
		break;

	case ERROR_ACCESS_DENIED:
		ret = -3;
		break;

	case NERR_InvalidComputer:
		ret = -4;
		break;

	default:
		ret = -99;
		break;
	}

	return ret;
}

// メインループ
//
void do_main(int ac, _TCHAR **av)
{
	DWORD dwRet;
	int err_cnt = 0;
	ar_tchar ini = RenameExt(GetExeName(), _T("ini"));
	ar_tchar key(32);
	ar_tchar tmp(96);
	ar_tchar buf(96);
	ar_tchar pas(96);
	_TCHAR* p;

	for (int i = 1;; i++) {
		_sntprintf(key, key.GetSize(), _T("user%d"), i);
		GetPrivateProfileString(_T("users"), key, _T(""), tmp, tmp.GetSize(), ini);

		if (_tcscmp(tmp, _T("")) == 0)
			break;

		p = _tcstok(tmp, ",");
		if (p)	_tcscpy(buf, p);

		p = _tcstok(NULL, ",");
		if (p)	_tcscpy(pas, p);

		_tprintf(_T("%s ... "), buf);

		int ret = UserInfo(buf);
		if (ret != 0)
			++err_cnt;

		switch (ret) {
		case 0:

			dwRet = LoginCheck(buf, pas);
			if (dwRet != 0) {
				++err_cnt;
				_tprintf(_T("ERROR (ログインできません(error=%u))\n"), dwRet);
			} else {
				_putts(_T("OK"));
			}
			break;
		case -1:
			_putts(_T("ERROR (ユーザーが未登録)"));
			break;
		case -2:
			_putts(_T("ERROR (パスワードが無期限ではない)"));
			break;
		case -3:
			_putts(_T("ERROR (ユーザー情報にアクセスする権限が無い)"));
			break;
		case -4:
			_putts(_T("ERROR (コンピューター名が間違っている)"));
			break;
		default:
			_putts(_T("ERROR (その他)"));
			break;
		}
	}

	if (err_cnt == 0) {
		_putts(_T("ユーザー情報は正常です。"));
	} else {
		_tprintf(_T("%d 件のエラーがあります。設定を見直し再度チェックをしてください。\n"), err_cnt);
	}

	_tprintf(_T("何かキーを押してください・・・"));

	_getch();
}

// プログラム・スタート
//
int _tmain(int argc, _TCHAR **argv)
{
	_tsetlocale(LC_ALL, _T("jpn"));	// 日本語ロケールに変更

	do_main(argc, argv);

	return 0;
}
