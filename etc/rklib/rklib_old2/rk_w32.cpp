// rk_w32.cpp : Win32APIで使える関数など
//
// 関数、クラスの先頭は必ずrkで始まる。
// 基本的に、Unicodeは無視

#include "rk_cpp.h"
#include "rk_w32.h"

// FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM ... の簡易呼び出し
//
// [引数]
// DWORD dwError     : GetLastError()の戻り値
// LPTSTR *lppBuffer : 文字列バッファのアドレスを受け取るバッファへのポインタ
//
// [戻り値]
// FormatMessage()と同じ
//
// [備考]
// 行末の改行は取り除かれます。
// lppBufferで受け取ったバッファは、LocalFree()で開放してください。

DWORD rkFormatMessageFromSystem(DWORD dwError, LPTSTR *lppBuffer)
{
	DWORD dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, dwError, 0, (LPTSTR)lppBuffer, 256, NULL);
	if (dwRet != 0) {
		while (rkChomp(*lppBuffer)) ;
	} else {
		DWORD dwErr = GetLastError();	// FormatMessageでエラーが出た
		
		if (dwErr == ERROR_MR_MID_NOT_FOUND) {
			*lppBuffer = (LPTSTR)LocalAlloc(LPTR, 256);
			wsprintf(*lppBuffer, "The system cannot find message text for message number 0x%08X.", dwError);
		} else if (dwErr == ERROR_INVALID_PARAMETER) {
			*lppBuffer = (LPTSTR)LocalAlloc(LPTR, 256);
			wsprintf(*lppBuffer, "The parameter is incorrect.");
		} else {
			*lppBuffer = (LPTSTR)LocalAlloc(LPTR, 256);
			wsprintf(*lppBuffer, "FormatMessage function error: 0x%08X.", dwErr);
		}
	}

	return dwRet;
}
