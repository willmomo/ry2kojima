//---------------------------------------------------------------------------------------
// kjmsub ver.0.0.0.0
//---------------------------------------------------------------------------------------
#include "kjmsub.h"

#include <vector>
#include <sstream>


// システム標準のエラーメッセージを作成
std::basic_string<TCHAR> kjm::util::formatMessageBySystem(DWORD dwErr) {
	std::basic_string<TCHAR> ret;
	PVOID pBuffer;

	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, (LPTSTR)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		std::basic_stringstream<TCHAR> ss;
		ss << TEXT("FormatMessage(") << dwErr << TEXT(") failed: error ") << dwErr;
		ret = ss.str();
	} else {
		ret = (LPTSTR)pBuffer;
		LocalFree(pBuffer);
	}

	return ret;
}

// GetComputerNameEx関数(vc6用)
BOOL kjm::util::__GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize) {
	typedef BOOL (WINAPI *GCNEX)(COMPUTER_NAME_FORMAT, LPTSTR, LPDWORD);

	BOOL bRet = FALSE;
	HMODULE hModule = LoadLibrary(TEXT("kernel32.dll"));
	
	if (hModule != NULL) {
#if defined(_MBCS)
		GCNEX pFunc = (GCNEX)GetProcAddress(hModule, "GetComputerNameExA");
#else
		GCNEX pFunc = (GCNEX)GetProcAddress(hModule, "GetComputerNameExW");
#endif
		if (pFunc != NULL) {
			bRet = pFunc(NameType, lpBuffer, nSize);
		}
		FreeLibrary(hModule);
	}

	return bRet;
}

// stringを返すGetComputerNameEx関数
std::basic_string<TCHAR> kjm::util::getComputerNameEx(COMPUTER_NAME_FORMAT NameType) {
	DWORD dwSize = 0;

	// 必要なバッファサイズを取得(エラーチェックなし)
	kjm::util::__GetComputerNameEx(NameType, NULL, &dwSize);

	// バッファを確保
	std::vector<TCHAR> buf(dwSize);
	if (!kjm::util::__GetComputerNameEx(NameType, &buf[0], &dwSize))
		throw ::GetLastError();

	return &buf[0];
}

// stringを返すGetDlgItemText関数
// [引数] hDlg        ダイアログボックスのハンドル
// [引数] nIDDlgItem  コントロールの識別子
// [例外] throw ::GetLastError();
std::basic_string<TCHAR> kjm::util::getDlgItemText(HWND hDlg, int nIDDlgItem) {
	std::vector<TCHAR> work(32);

	do {
		UINT ret = ::GetDlgItemText(hDlg, nIDDlgItem, &work[0], work.size());
		DWORD dwErr = ::GetLastError();

		if ((ret == 0) && (dwErr != 0))
			throw dwErr;

		if ((ret == 0) || (ret + 1 < work.size()))
			break;

		work.resize(work.size() * 2);
	} while (1);

	return &work[0];
}


// ワークエリアの中心にウィンドウを移動する
BOOL kjm::util::moveCenterWorkarea(HWND hwnd) {
	// ワークスエリアの矩形を取得
	RECT rcWorkarea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkarea, 0);

	// 移動するウィンドウの矩形を取得
	RECT rcClient;
	::GetWindowRect(hwnd, &rcClient);

	int nNewLeft = ((rcWorkarea.right - rcWorkarea.left) - (rcClient.right - rcClient.left)) / 2;
	int nNewTop = ((rcWorkarea.bottom - rcWorkarea.top) - (rcClient.bottom - rcClient.top)) / 2;

	// 移動予定の左上がWorkareaからはみ出ている場合は、座標を0にする
	if (nNewLeft < 0)	nNewLeft = 0;
	if (nNewTop < 0)	nNewTop = 0;

	return ::MoveWindow(hwnd, nNewLeft, nNewTop, 
		(rcClient.right - rcClient.left), (rcClient.bottom - rcClient.left), FALSE);
}


// SetComputerNameEx関数(vc6用)
BOOL kjm::util::__SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer) {
	typedef BOOL (WINAPI *SCNEx)(COMPUTER_NAME_FORMAT, LPCTSTR);

	BOOL bRet = FALSE;

	HMODULE hModule = LoadLibrary(TEXT("kernel32.dll"));
	if (hModule) {
#if defined(_MBCS)
		SCNEx pFunc = (SCNEx)GetProcAddress(hModule, "SetComputerNameExA");
#else
		SCNEx pFunc = (SCNEx)GetProcAddress(hModule, "SetComputerNameExW");
#endif
		if (pFunc) {
			bRet = pFunc(NameType, lpBuffer);
		}

		FreeLibrary(hModule);
	}

	return bRet;
}
