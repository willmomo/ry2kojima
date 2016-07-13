
#include "proctools.h"

// コールバック関数に渡すパラメータ
typedef struct
{
	DWORD processId;	// 検索するプロセスID
	int count;			// 見つかったウィンドウハンドルの数
	HWND* hwnds;		// 見つかったウィンドウハンドルへの配列
} EnumParam;

// プロセスIDからウィンドウハンドルを見つけ出すコールバック関数
//
static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	EnumParam* param = (EnumParam*)lParam;

	// ウィンドウハンドルからプロセスIDを取得して、
	// 検索しているプロセスIDと一致するかどうか調べる。
	if (WindowHandleToProcessId(hwnd) == param->processId)
	{
		// 初回呼び出し
		if (param->hwnds == NULL)
		{
			// メモリ確保
			param->hwnds = (HWND*)GlobalAlloc(GPTR, sizeof(HANDLE));

			// メモリ確保に失敗したときは、列挙中止
			if (param->hwnds == NULL)
			{
				param->count = -1;
				return FALSE;
			}

			param->count = 1;
		}
		else
		{
			// メモリを拡張
			HGLOBAL temp = GlobalReAlloc(param->hwnds, sizeof(HANDLE) * (param->count + 1), 0);

			// メモリ拡張に失敗したときは、列挙中止
			if (temp == NULL)
			{
				// 後始末
				param->hwnds = (HWND*)GlobalFree(param->hwnds);
				param->count = -1;
				return FALSE;
			}

			// ハンドルを新しくして、件数をひとつ増やす
			param->hwnds = (HWND*)temp;
			param->count++;
		}

		// 確保したメモリの最後にウィンドウハンドルを保存する。
		param->hwnds[param->count-1] = hwnd;
	}

	return TRUE;
}

// プロセスIDからプロセスハンドルを取得する
//
// [引数]
// processId	プロセスハンドルを取得したいプロセスのID
//
// [戻り値]
// プロセスハンドル。関数が失敗したときは、NULLを返す。
// ハンドルが不要になったときは、CloseHandleを使ってクローズする。
//
// [備考]
// OpenProcess API のラッパー
//
HANDLE ProcessIdToProcessHandle(DWORD processId)
{
	return OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
}

// ウィンドウハンドルからプロセスIDを取得する
//
// [引数]
// hwnd		プロセスIDを取得したいウィンドウへのハンドル
//
// [戻り値]
// プロセスID。関数が失敗したときは0を返す。
//
// [備考]
// GetWindowThreadProcessId API のラッパー
//
DWORD WindowHandleToProcessId(HWND hwnd)
{
	DWORD processId;
	if (GetWindowThreadProcessId(hwnd, &processId) == 0)
		processId = 0;

	return processId;
}

// ウィンドウハンドルからプロセスハンドルを取得する
//
// [引数]
// hwnd		プロセスハンドルを取得したいウィンドウへのハンドル
//
// [戻り値]
// プロセスハンドル。関数が失敗したときは、NULLを返す。
// ハンドルが不要になったときは、CloseHandleを使ってクローズする。
//
HANDLE WindowHandleToProcessHandle(HWND hwnd)
{
	DWORD processId = WindowHandleToProcessId(hwnd);

	return (processId == 0) ? NULL : ProcessIdToProcessHandle(processId);
}

// プロセスIDからウィンドウハンドルを取得する
//
// [引数]
// processId	ウィンドウハンドルを取得したいプロセスのID
// buffer		HWND配列へのポインタ(見つけたウィンドウハンドルをすべて保存する)
// count		bufferの要素数
//
// [戻り値]
// 見つけたウィンドウハンドルの数。戻り値 > count の時は、bufferサイズを
// 増やして、再度関数を呼ぶ。
// 関数が失敗したときは、-1を返す。
//
int ProcessIdToWindowHandle(DWORD processId, HWND* buffer, int count)
{
	EnumParam param = {processId, 0, NULL};

	// processIdに関連するウィンドウハンドルを列挙する
	// 関数失敗 or 列挙失敗(メモリ確保エラー)の時は、エラーリターン
	if (!EnumWindows(EnumWndProc, (LPARAM)&param) || param.count < 0)
	{
		return -1;
	}

	// 見つけたウィンドウハンドルをバッファに保存する。
	// ただし、オーバーフローしないようにする。
	for (int i = 0; i < min(count, param.count); i++)
	{
		buffer[i] = param.hwnds[i];
	}

	// 列挙関数で確保されたメモリを破棄する。
	GlobalFree(param.hwnds);

	return param.count;
}

// プロセスハンドルからプロセスIDを取得する
// (WinXPsp1以降で有効)
//
// [引数]
// hproc	プロセスIDを取得したいプロセスへのハンドル
//
// [戻り値]
// プロセスID。関数が失敗した場合は、0を返す。
//
// [備考]
// Windows XP sp1 以降に実装されているGetProcessId API を使用する以外、
// プロセスハンドルからプロセスIDを取得することは、できない模様。
//
DWORD ProcessHandleToProcessId(HANDLE hproc)
{
	return GetProcessId(hproc);
}

// プロセスハンドルからウィンドウハンドルを取得する
// (WinXPsp1以降で有効)
//
// [引数]
// hproc	ウィンドウハンドルを取得したいプロセスへのハンドル
// buffer	HWND配列へのポインタ(見つけたウィンドウハンドルをすべて保存する)
// count	bufferの要素数
//
// [戻り値]
// 見つけたウィンドウハンドルの数。戻り値 > count の時は、bufferサイズを
// 増やして、再度関数を呼ぶ。
// 関数が失敗したときは、-1を返す。
//
// [備考]
// 内部で、ProcessHandleToProcessId関数を使用するため、Windows XP sp1 以降のみ対応。
//
int ProcessHandleToWindowHandle(HANDLE hproc, HWND* buffer, int count)
{
	// プロセスIDを取得
	DWORD processId = ProcessHandleToProcessId(hproc);
	if (processId == 0)
	{
		return -1;
	}

	// プロセスIDからウィンドウハンドルを取得する
	return ProcessIdToWindowHandle(processId, buffer, count);
}
