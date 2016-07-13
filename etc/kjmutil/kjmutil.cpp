#include "kjmutil.h"
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <vector>
#include <share.h>


/* 文字列の最後の改行を取り除く */
void kjmutil::chomp(_tstring& str) {
	while (!str.empty()) {
		TCHAR c = *(str.begin() + str.length() - 1);
		if (c != '\x0d' && c != '\x0a')
			break;
		str.erase(str.begin() + str.length() - 1);
	}
}


/* kjmutil::rectを引数にとるMoveWindow関数 */
BOOL kjmutil::move_window(HWND hWnd, const kjmutil::rect& rc, BOOL bRepaint) {
	return MoveWindow(hWnd, rc.left, rc.top, rc.get_width(), rc.get_height(), bRepaint);
}


/****************************************************************************
  lpExistingFileNameのファイルをlpNewFileNameにコピーします。コピー失敗時には
  指定回数分リトライを行います。

  <パラメータ>
  lpExistingFileName  コピー元
  lpNewFileName       コピー先
  bFailIfExists       上書きを行うか(FALSEで上書きを行う)
  retry               再試行を何回行うか
  wait                再試行を行うまで何秒待つのか

  <戻り値>
  コピーに成功したときTRUE、コピーに失敗したときFALSEを返す。
****************************************************************************/
BOOL kjmutil::IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait) {
	BOOL bRet = FALSE;
	
	for (int i = 0; i <= retry; i++) {
		bRet = CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
		if (bRet) {
			break;
		}
		Sleep(wait * 1000);
	}

	return bRet;
}
/**********************************************************************
参考として、この関数のVB6での実装を示します。
FileSystemObject とSleep APIが使用できるようにしておいてください。
'----------------------------------------------------------------------------
' lpExistingFileNameのファイルをlpNewFileNameにコピーします。コピー失敗時には
' 指定回数分リトライを行います。
'
' <パラメータ>
' lpExistingFileName  コピー元
' lpNewFileName       コピー先
' bFailIfExists       上書きを行うか(Falseで上書きを行う)
' retry               再試行を何回行うか
' wait                再試行を行うまで何秒待つのか
'
' <戻り値>
' コピーに成功したときTrue､コピーに失敗したときFalseを返す｡
'----------------------------------------------------------------------------
Private Function IOUtil__CopyFile(lpExistingFileName As String, lpNewFileName As String, bFailIfExists As Boolean, retry As Long, wait As Long) As Boolean
    Dim objFS As New FileSystemObject
    Dim i As Long
    
    On Error Resume Next
    
    For i = 0 To retry
        Err.Clear
        Call objFS.CopyFile(lpExistingFileName, lpNewFileName, Not bFailIfExists)
        If Err.Number = 0 Then
            IOUtil__CopyFile = True
            Exit For
        End If
        Call Sleep(wait * 1000)
    Next
End Function
**********************************************************************/


/* エラーの説明を取得 */
_tstring kjmutil::err::get_description() {
	_tstring result;
	LPTSTR pszMsg;
	DWORD dwRet;
	LPCTSTR pArgs[] = {_T("%1"), _T("%2"), _T("%3"), NULL};

	dwRet = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		m_dwErrCode,
		0,
		(LPTSTR)&pszMsg,
		0,
		(va_list*)pArgs);

	if (dwRet == 0) {
		_tostringstream oss;
		oss << _T("エラー ") << m_dwErrCode << _T(" のメッセージを取得中、FormatMessage 関数でエラー ") << GetLastError() << _T(" が発生しました。");
		result = oss.str();
	} else {
		result = pszMsg;
		kjmutil::chomp(result);

		LocalFree(pszMsg);
	}

	return result;
}


/* スクリーン座標をクライアント座標に変換 */
kjmutil::rect& kjmutil::rect::screen_to_client(HWND hwnd) {
	POINT pt;

	pt.x = left;
	pt.y = top;
	ScreenToClient(hwnd, &pt);
	left = pt.x;
	top = pt.y;

	pt.x = right;
	pt.y = bottom;
	ScreenToClient(hwnd, &pt);
	right = pt.x;
	bottom = pt.y;

	return *this;
}


/* 矩形をオフセットする */
kjmutil::rect& kjmutil::rect::offset(int dx, int dy) {
	OffsetRect(this, dx, dy);
	return *this;
}


/* ウィンドウのクライアント座標を取得します */
kjmutil::rect kjmutil::rect::get_client_rect(HWND hwnd) {
	kjmutil::rect rc;
	GetClientRect(hwnd, &rc);
	return rc;
}


/* ウィンドウの座標を取得します */
kjmutil::rect kjmutil::rect::get_window_rect(HWND hwnd) {
	kjmutil::rect rc;
	GetWindowRect(hwnd, &rc);
	return rc;
}


/* 指定された 2 つの長方形を完全に含む、最小の長方形を作成します */
kjmutil::rect kjmutil::rect::union_rect(const RECT* lprcSrc1, const RECT* lprcSrc2) {
	kjmutil::rect rcDst;
	UnionRect(&rcDst, lprcSrc1, lprcSrc2);
	return rcDst;
}


//---------------------------------------------------------------------
// 簡易ログクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ログを書く
//
// [引数]
// format  書式制御文字列
//---------------------------------------------------------------------
void kjmutil::log::writeLog(const char* format, ...) {
	std::vector<char> buf(8192);
	va_list marker;

	// ログファイルの名前
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::vector<char> work2(MAX_PATH);
	sprintf(work2.begin(), "%s%04d%02d%02d.log", 
		kjmutil::win32::pathFindFileName(kjmutil::win32::pathRemoveExtension(kjmutil::win32::getModuleFileName())).c_str(),
		st.wYear, st.wMonth, st.wDay);

	// ログファイルの作成先を決定
	std::string fname = kjmutil::win32::pathAppend(kjmutil::win32::getEnvironmentVariable(_T("TEMP")), work2.begin());

	// ファイルが開けるまで、500ms待つ
	DWORD dw = GetTickCount();
	FILE* fp = NULL;
	while ((fp = _fsopen(fname.begin(), "a+", _SH_DENYWR)) == NULL) {
		if ((GetTickCount() - dw) > 500) break;
		Sleep(0);
	}

	if (fp) {
		fprintf(fp, "[%04d/%02d/%02d %02d:%02d:%02d.%03d][%04X] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, GetCurrentProcessId());
		va_start(marker, format);
		vfprintf(fp, format, marker);
		va_end(marker);
		fputs("\n", fp);

		fclose(fp);
	}
}


//---------------------------------------------------------------------
// WIN32関数の簡易呼び出しクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// GetEnvironmentVariableのラッパー
//
// [引数]
// lpName  環境変数の名前へのポインタ
//
// [戻り値]
// 指定された環境変数が持つ値を返す。
//---------------------------------------------------------------------
std::string kjmutil::win32::getEnvironmentVariable(LPCTSTR lpName) {
	// 必要なバッファのサイズを取得
	DWORD dwSizeNeeded = ::GetEnvironmentVariable(lpName, NULL, 0);
	if (dwSizeNeeded == 0) {
		return "";
	}

	std::vector<TCHAR> buf(dwSizeNeeded);
	::GetEnvironmentVariable(lpName, buf.begin(), buf.size());

	return buf.begin();
}


//---------------------------------------------------------------------
// GetModuleFileNameのラッパー
//
// [引数]
// hModule  モジュールのハンドル
//
// [戻り値]
// 実行ファイルの名前を返す。
//---------------------------------------------------------------------
std::string kjmutil::win32::getModuleFileName(HMODULE hModule) {
	std::vector<TCHAR> buf(MAX_PATH);
	while (::GetModuleFileName(hModule, buf.begin(), buf.size()) == buf.size()) {
		buf.resize(buf.size() * 2);
	}

	return buf.begin();
}


//---------------------------------------------------------------------
// PathAppend のラッパー
//
// [引数]
// strPath  元になるフルパスのファイル名
// strMore  結合するパスの名前
//
// [戻り値]
// 結合された新しいファイル名文字列
//---------------------------------------------------------------------
std::string kjmutil::win32::pathAppend(const std::string& strPath, const std::string& strMore) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// 元になるファイル名をワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// パスをくっつける
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}

//---------------------------------------------------------------------
// PathFindFileName のラッパー
//
// [引数]
// strPath  ファイル名を検索するフルパスのファイル名
//
// [戻り値]
// ファイル名だけの文字列
//---------------------------------------------------------------------
std::string kjmutil::win32::pathFindFileName(const std::string& strPath) {
	return PathFindFileName(strPath.c_str());
}


//---------------------------------------------------------------------
// PathRemoveExtension のラッパー
//
// [引数]
// strPath  拡張子を取り除きたいファイル名
//
// [戻り値]
// 拡張子を取り除いた新しい文字列
//---------------------------------------------------------------------
std::string kjmutil::win32::pathRemoveExtension(const std::string& strPath) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length());

	// 元になるファイル名をワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// 拡張子を取り除く
	PathRemoveExtension(work.begin());

	return work.begin();
}


//---------------------------------------------------------------------
// PathRenameExtension のラッパー
//
// [引数]
// strPath  元にふぁるフルパスのファイル名
// strExt   置き換える拡張子
//
// [戻り値]
// 拡張子を置き換えた新しいファイル名文字列
//---------------------------------------------------------------------
std::string kjmutil::win32::pathRenameExtension(const std::string& strPath, const std::string& strExt) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strExt.length());

	// 元になるファイル名をワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// 拡張子置き換え
	PathRenameExtension(work.begin(), strExt.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// ファイル名部分を取り除いて新しいファイル名をくっつける
// PathRemoveFileSpec と PathAppend をくっつけたような関数
//
// [引数]
// strPath  元になるフルパスのファイル名
// strMore  変更する FileSpec
//
// [戻り値]
// 新しいフルパスのファイル名。
//---------------------------------------------------------------------
std::string kjmutil::win32::pathRenameFileSpec(const std::string& strPath, const std::string& strMore) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// 元になるフルパスをワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// ファイル名部分を取り除く
	PathRemoveFileSpec(work.begin());

	// 新しいファイル名をくっつける
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// iniファイルアクセスクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// デフォルトコンストラクタ
//
// [説明]
// 実行ファイル名.ini をデフォルトでアクセスするように調整される。
//---------------------------------------------------------------------
kjmutil::iniFile::iniFile() {
	this->m_strIniFile = kjmutil::win32::pathRenameExtension(kjmutil::win32::getModuleFileName(), _T(".ini"));
}


//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kjmutil::iniFile& kjmutil::iniFile::operator =(const kjmutil::iniFile& rhs) {
	if (this == &rhs)	return *this;	// 同じオブジェクトを指している

	this->m_strIniFile = rhs.m_strIniFile;
	return *this;
}
