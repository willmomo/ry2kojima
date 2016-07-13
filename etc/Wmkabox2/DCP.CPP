#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include "SbDisk.h"
#include "SbFile.h"
#include "dcp.h"

#define CAPTION		"Wmkabox2"			// MessageBox用のキャプション
#define FD144SIZE	1457664				// 1.44FDの総サイズ
#define RECSIZE		56					// ABOXファイルの1レコードサイズ
#define MAXRECIN144	(FD144SIZE/RECSIZE)	// 1.44FDにコピーできるレコード数

/////////////////////////////////////////////////////////////////////
// ディスクが空かどうかチェックする

static BOOL CheckDiskBlank(HWND hwnd, LPCTSTR lpszCaption)
{
	DWORD dwFree;
	SbDisk disk(0);

	while (1) {
		if (!disk.GetFreeWR(&dwFree, hwnd, lpszCaption))
			return FALSE;

		if (dwFree == FD144SIZE)
			break;

		if (MessageBox(hwnd, "このディスクは空ではありません。\n"
			"フォーマット済みのディスクを入れてください。", 
			lpszCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
			return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// コピー先ファイル名の作成

static BOOL MkDstFileName(LPCTSTR lpszSrc, LPTSTR lpszDst, int nNo=-1)
{
	char szName2[_MAX_FNAME], szName[_MAX_FNAME], szExt[_MAX_EXT];

	_splitpath(lpszSrc, NULL, NULL, szName, szExt);
	if (nNo >= 0)
		wsprintf(szName2, "%s.%02d", szName, nNo);
	else
		lstrcpy(szName2, szName);

	_makepath(lpszDst, "A:", "\\", szName2, szExt);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// 指定されたABOXファイルをFDに(必要なら分割)コピーする

BOOL ABoxCopyDisk(HWND hwnd, LPCTSTR lptSrc)
{
	char szErrMsg[256];			// エラーメッセージ用バッファ
	SbFile fileSrc, fileDst;	// コピー元,先ファイル

	// コピー元ファイルを開く
	if (!fileSrc.Open(lptSrc, GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL)) {
		int nError = GetLastError();
		switch (nError) {
		case ERROR_FILE_NOT_FOUND:
			wsprintf(szErrMsg, "%s がオープンできません。\nファイルが見つかりません。", lptSrc);
			break;
		default:
			wsprintf(szErrMsg, "%s がオープンできません。\nエラーコード %d", lptSrc, nError);
			break;
		}
		MessageBox(hwnd, szErrMsg, CAPTION, MB_OK|MB_ICONERROR);
		return FALSE;
	}

	DWORD dwSrcSize = fileSrc.GetFileSize();	// コピー元のファイルサイズ
	if (dwSrcSize % RECSIZE) {					// レコードで割り切れない(異常)
		MessageBox(hwnd, "ABOXファイルが壊れています。", CAPTION, MB_OK|MB_ICONERROR);
		return FALSE;
	}

	// 必要なFD枚数を計算する
	int nNeed = (dwSrcSize / (MAXRECIN144 * RECSIZE)) + ((dwSrcSize % (MAXRECIN144 * RECSIZE)) ? 1 : 0);
	if (nNeed == 1) {	// 1枚で終了
		// ディスクの挿入を促す
		if (MessageBox(hwnd, "ABOXファイルをFDにコピーします。\n"
			"A ドライブにフォーマット済みのディスクを入れてください", CAPTION, MB_OKCANCEL) == IDCANCEL) {
			MessageBox(hwnd, "コピーを中止します", CAPTION, MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		fileSrc.Close();	// Win32のコピー関数にコピーを任せる

		if (!CheckDiskBlank(hwnd, CAPTION))		// ディスクが空かどうか確認
			return FALSE;						//   空のディスクが挿入されなかった

		// コピー先ファイル名を作成する
		char szDst[MAX_PATH];
		MkDstFileName(lptSrc, szDst);
		if (!CopyFile(lptSrc, szDst, FALSE)) {
			wsprintf(szErrMsg, "システムエラーです。\nエラーコード %d", GetLastError());
			MessageBox(hwnd, szErrMsg, CAPTION, MB_OK|MB_ICONERROR);
			return FALSE;
		}

		return TRUE;
	}

	char* pszBuffer = (char*)malloc(FD144SIZE);

	wsprintf(szErrMsg, "フォーマット済みのディスクが %d 枚必要です。", nNeed);
	MessageBox(hwnd, szErrMsg, CAPTION, MB_OK|MB_ICONINFORMATION);

	// 複数毎にわたるときのコピー
	HCURSOR hcOld = SetCursor(LoadCursor(NULL, IDC_WAIT));
	for (int i = 0; i < nNeed; i++) {
		// ディスクの挿入を促す
		wsprintf(szErrMsg, "%d 枚目のディスクを入れてください", i + 1);
		if (MessageBox(hwnd, szErrMsg, CAPTION, MB_OKCANCEL) == IDCANCEL) {
			MessageBox(hwnd, "コピーを中止します", CAPTION, MB_OK|MB_ICONINFORMATION);
			free(pszBuffer);
			return FALSE;
		}

		if (!CheckDiskBlank(hwnd, CAPTION))		// ディスクが空かどうか確認
			return FALSE;						//   空のディスクが挿入されなかった

		// コピー先ファイル名を作成する
		char szDst[MAX_PATH];
		MkDstFileName(lptSrc, szDst, i + 1);

		// コピー先ファイルを開く
		if (!fileDst.OpenWR(szDst, GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, hwnd, CAPTION)) {
			free(pszBuffer);
			return FALSE;
		}

		// 書き込める最大レコード数を読み込んでから書き込む
		DWORD dwNumberOfBytesRead, dwNumberOfBytesWritten;
		if (!fileSrc.Read(pszBuffer, MAXRECIN144 * RECSIZE, &dwNumberOfBytesRead)) {
			wsprintf(szErrMsg, "システムエラーです。\nエラーコード %d\n", GetLastError());
			MessageBox(hwnd, szErrMsg, CAPTION, MB_OK|MB_ICONERROR);
			break;
		}

		if (!fileDst.Write(pszBuffer, dwNumberOfBytesRead, &dwNumberOfBytesWritten)) {
			wsprintf(szErrMsg, "システムエラーです。\nエラーコード %d\n", GetLastError());
			MessageBox(hwnd, szErrMsg, CAPTION, MB_OK|MB_ICONERROR);
			break;
		}

		fileDst.Close();
	}

	SetCursor(hcOld);

	free(pszBuffer);

	return TRUE;
}
#if 0
int main(int argc, char** argv)
{
	ABoxCopyDisk(NULL, "19990823.abx");
	return 0;
}
#endif
