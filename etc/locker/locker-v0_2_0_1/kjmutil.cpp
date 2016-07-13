#include "kjmutil.h"
#include <tchar.h>


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

