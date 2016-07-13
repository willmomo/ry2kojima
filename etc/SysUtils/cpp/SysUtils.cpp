#include <windows.h>
#include <shlwapi.h>
#include <string>
#include "SysUtils.h"

//---------------------------------------------------------------------
// 内部で使用する値など
//---------------------------------------------------------------------

#define LWA_ALPHA		2

typedef BOOL (WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

//---------------------------------------------------------------------
// VC6では、単純に使えない関数です。
//---------------------------------------------------------------------

static BOOL SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	BOOL ret = FALSE;

	HMODULE hlibUser32 = LoadLibrary("user32.dll");
	if (hlibUser32) {
		SLWA pfunc = (SLWA)GetProcAddress(hlibUser32, "SetLayeredWindowAttributes");
		if (pfunc) {
			ret = pfunc(hwnd, crKey, bAlpha, dwFlags);
		}
		FreeLibrary(hlibUser32);
	}

	return ret;
}

//---------------------------------------------------------------------
// [概要]
// 指定したウィンドウが、半透明をサポートするかチェックする
//
// [引数]
// hwnd          ウィンドウへのハンドル
//
// [戻り値]
// サポートする      True
// サポートしない    False
//
// [例外]
// エラーが発生した場合、Win32のエラーコードでランタイムエラーを発生させる。
// エラーコードは、WINERROR.Hを調べてください。
//
// [説明]
// 半透明機能をサポートしているかどうかチェックします。
//---------------------------------------------------------------------

bool IsLayered(HWND hwnd)
{
	// ウィンドウスタイルを取得する
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (exStyle == 0) {
		throw GetLastError();
	}

	// 半透明サポート時は、true, 未サポート時はfalseを返す
	return ((exStyle & WS_EX_LAYERED) == WS_EX_LAYERED) ? true : false;
}

//---------------------------------------------------------------------
// [概要]
// 指定したウィンドウの、半透明サポートをON/OFFします。
//
// [引数]
// hwnd      ウィンドウへのハンドル
// bLayered  半透明サポート(True:ON False:OFF)
//
// [戻り値]
// 設定できた        0
// 設定できなかった  -1
// その他のエラー    Win32のエラーコード
//
// [説明]
// 半透明機能のON/OFFを行う。
//---------------------------------------------------------------------

int SetLayered(HWND hWnd, bool bLayered)
{
	// ウィンドウスタイルを取得する
	long exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	if (exStyle == 0) {
		return GetLastError();
	}

    // 引数に応じて半透明サポートのフラグを変化させる
	if (bLayered) {
		exStyle |= WS_EX_LAYERED;
	} else {
		exStyle &= ~WS_EX_LAYERED;
	}
    
    // ウィンドウスタイルを設定する。
	SetLastError(0);
	exStyle = SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
	DWORD err = GetLastError();
	if (exStyle == 0 && GetLastError() != 0) {
		return err;
	}

    try
	{
		// 半透明のサポート状態が設定できたかどうかを戻す
		return (IsLayered(hWnd) == bLayered) ? 0 : -1;
	}
	catch(DWORD e)
	{
		return e;
	}
}

//---------------------------------------------------------------------
// [概要]
// ウィンドウを不透明化する
//
// [引数]
// hwnd      ウィンドウへのハンドル
// dOpacity  不透明の割合(0.0～1.0)
//
// [戻り値]
// 正常終了          0
// エラー終了        -1
// その他のエラー    Win32エラーコード
//
// [説明]
// ウィンドウを不透明化します。
//---------------------------------------------------------------------

int SetOpacity(HWND hwnd, double dOpacity)
{
    // 半透明未サポートウィンドウを半透明にしようとしたときは、フラグをセットする
    // フラグがセットできないときは、エラーリターン
	try
	{
		if (IsLayered(hwnd) == false) {
			int ret = SetLayered(hwnd, true);
			if (ret != 0) {
				return ret;
			}
		}
	}
	catch(DWORD e)
	{
		return e;
	}
    
    // 半透明化(不透明化)する
    BOOL ret = SetLayeredWindowAttributes(hwnd, 0, 255 * dOpacity, LWA_ALPHA);
	if (ret == FALSE) {
		return GetLastError();
	}

	return 0;
}

//---------------------------------------------------------------------
// [概要]
// 指定フォルダ以下にあるファイルをすべて削除する。
//
// [引数]
// strFolder     このフォルダより下にあるファイルを削除する
// strExcept     この名前に一致するファイル/フォルダは削除しない。
//
// [戻り値]
// 正常終了  0
// エラー    0以外。Error関数でエラー文字列を取得可能。
//
// [説明]
// strFolder以下にあるファイルを、strExceptを除いてすべて削除する。
// strExceptは、単純な文字列。フォルダ名と一致すれば、フォルダを
// 以下すべてを削除対象外にします。
//
// [備考]
// 間違って、c:\とか指定したら、もう大変。
//---------------------------------------------------------------------

int RemoveSubFolders(const char *strFolder, const char *strExcept)
{
	int nRet = 0;

	char strPathName[MAX_PATH];		// strFolder + '\\'
	char strFileName[MAX_PATH];		// strPathName + '*.*'

	lstrcpyn(strPathName, strFolder, MAX_PATH);
	if (PathAddBackslash(strPathName) == NULL) {
		return -1;
	}

	lstrcpyn(strFileName, strPathName, MAX_PATH);
	if (PathAppend(strFileName, "*.*") == FALSE) {
		return -2;
	}

	// ファイル検索開始
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strFileName, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		std::string strFrom;
		BOOL ret;
		do {
			if (lstrcmp(data.cFileName, ".") && lstrcmp(data.cFileName, "..")) {
				if (lstrcmpi(data.cFileName, strExcept)) {
					strFrom += strPathName;
					strFrom += data.cFileName;
					strFrom += '\0';
				}
			}

			ret = FindNextFile(hFind, &data);
		} while (ret);
		FindClose(hFind);

		// 削除するファイルがあったときは、削除！
		if (!strFrom.empty()) {
			SHFILEOPSTRUCT fo;

			memset(&fo, 0, sizeof(fo));
			fo.wFunc = FO_DELETE;
			fo.pFrom = strFrom.c_str();
			fo.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

			nRet = SHFileOperation(&fo);
		}
	} else {
		nRet = GetLastError();
	}

	return nRet;
}

//---------------------------------------------------------------------
// [概要]
// 文字列を置換する
//
// [引数]
// pszSrc			文字列を置換する前の文字列
// pszFind			pszSrcの中から探す文字列
// pszReplaceWith	pszFindを置き換える文字列
//
// [戻り値]
// 置換後の文字列(std::string)。
// pszSrcからpszFindが見つからないときは、std::string(pszSrc)を返す
// 引数がエラーのときは、empty() 文字列を返す
//
// [説明]
// VBのReplace関数ライクな関数。細かい制御はできないが、単純な文字列
// 置換をサポート
//---------------------------------------------------------------------

std::string ReplaceStr(const char* pszSrc, const char* pszFind, const char* pszReplaceWith, int nStart/* = 1*/, int nCount/* = -1*/)
{
	// 引数が異常のときは、空文字列を返す
	if (!pszSrc || !pszFind || !pszReplaceWith) {
		return std::string();
	}

	// 元文字列, 検索文字列, 置換文字列をwstring化
	std::wstring src = StrToWStr(pszSrc);
	std::wstring fnd = StrToWStr(pszFind);
	std::wstring rep = StrToWStr(pszReplaceWith);

	// 検索文字列を探して見つかれば、置換
	std::wstring::size_type n = (nStart < 1) ? 0 : nStart - 1;

	int nCntRep = 0;	// 置換した数
	while ((n = src.find(fnd, n)) != std::wstring::npos) {
		src.replace(n, fnd.length(), rep);
		n += rep.length();

		// 総置換数を管理する
		if (nCount > 0 && ++nCntRep >= nCount) {
			break;
		}
	}

	// 結果を戻す
	return WStrToStr(src.c_str());
}

//---------------------------------------------------------------------
// [概要]
// ASCII文字列をUnicode文字列に変換
//
// [引数]
// pszSrc	変換元ASCII文字列へのポインタ
//
// [戻り値]
// 変換後のUnicode文字列へのポインタ。変換できない場合は、空文字列を返す。
//---------------------------------------------------------------------

std::wstring StrToWStr(LPCSTR pszSrc)
{
	std::wstring wstr;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nSizeNeeded == 0) {
		return wstr;
	}

	// バッファを確保
	LPWSTR pwsz = (LPWSTR)malloc(sizeof(WCHAR) * nSizeNeeded);
	if (pwsz == NULL) {
		return wstr;
	}

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr = pwsz;
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
// [概要]
// Unicode文字列をASCII文字列に変換
//
// [引数]
// pwszSrc	変換元Unicode文字列へのポインタ
//
// [戻り値]
// 変換後のASCII文字列へのポインタ。変換できない場合は、空文字列を返す。
//---------------------------------------------------------------------

std::string WStrToStr(LPCWSTR pwszSrc)
{
	std::string str;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0) {
		return str;
	}

	// バッファを確保
	LPSTR psz = (LPSTR)malloc(sizeof(CHAR) * nSizeNeeded);
	if (psz == NULL) {
		return str;
	}

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str = psz;
	free(psz);

	return str;
}
