//---------------------------------------------------------------------------------------
// kjmsub ver.0.2.0.0
//---------------------------------------------------------------------------------------
#include "kjmsub.h"

#include <vector>
#include <sstream>


//---------------------------------------------------------------------------------------
// CopyFile APIと同様の動きをする関数(エラー箇所を詳細に特定したいときに使用)
// [戻り値]  0   正常終了
// [戻り値] -1   コピー元ファイルのオープンに失敗
// [戻り値] -2   コピー先ファイルのオープンに失敗
// [戻り値] -3   コピー元ファイルの読み込みに失敗
// [戻り値] -4   コピー先ファイルの書き込みに失敗
// [戻り値] -5   コピー先ファイルに指定サイズ書き込みできない
// [戻り値] -6   コピー元ファイルのタイムスタンプ取得失敗
// [戻り値] -7   コピー先ファイルのタイムスタンプ更新失敗
// [戻り値] -8   コピー元ファイルを閉じるのに失敗
// [戻り値] -9   コピー先ファイルを閉じるのに失敗
// [戻り値] -10  コピー元ファイルの属性取得失敗
// [戻り値] -11  コピー先ファイルの属性更新失敗
//---------------------------------------------------------------------------------------
int kjm::util::copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists) {

#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

	SetLastError(NOERROR);

	int result = 0;

	// コピー元ファイルを開く
	HANDLE hFile1 = ::CreateFile(lpExistingFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile1 == INVALID_HANDLE_VALUE) {
		return -1;
	}

	// コピー先ファイルを開く
	HANDLE hFile2 = ::CreateFile(lpNewFileName, GENERIC_WRITE, 0, NULL, (bFailIfExists) ? CREATE_NEW : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE) {
		::CloseHandle(hFile1);
		return -2;
	}

	SetLastError(NOERROR);

	char buffer[8192];
	DWORD dwNumberOfBytesRead;
	DWORD dwNumberOfBytesWritten;

	do {
		if (::ReadFile(hFile1, buffer, _countof(buffer), &dwNumberOfBytesRead, NULL) == FALSE) {
			result = -3;
			break;
		}

		if (dwNumberOfBytesRead > 0) {
			if (::WriteFile(hFile2, buffer, dwNumberOfBytesRead, &dwNumberOfBytesWritten, NULL)) {
				if (dwNumberOfBytesRead != dwNumberOfBytesWritten) {
					result = -5;
					break;
				}
			} else {
				result = -4;
				break;
			}
		}
	} while (dwNumberOfBytesRead == _countof(buffer));

	// コピーが正常に終了したときは、タイムスタンプをコピー元に合わせる
	if (result == 0) {
		FILETIME lastWriteTime;

		if (::GetFileTime(hFile1, NULL, NULL, &lastWriteTime)) {
			if (::SetFileTime(hFile2, NULL, NULL, &lastWriteTime)) {
				// タイムスタンプの複製成功
			} else {
				result = -7;
			}
		} else {
			result = -6;
		}
	}

	if (::CloseHandle(hFile1) == FALSE) {
		result = -8;
	}
	if (::CloseHandle(hFile2) == FALSE) {
		result = -9;
	}

	// コピーが正常に進んでいるときは、属性をコピーする
	if (result == 0) {
		DWORD dwAttributes = ::GetFileAttributes(lpExistingFileName);
		if (dwAttributes != 0xffffffff) {
			if (::SetFileAttributes(lpNewFileName, dwAttributes | FILE_ATTRIBUTE_ARCHIVE)) {
				// 属性の複製成功
			} else {
				result = -11;
			}
		} else {
			result = -10;
		}
	}

	return result;
}


//---------------------------------------------------------------------
// Windowsをシャットダウンさせる
// [戻り値]
// 0  正常終了
// -1 AdjustTokenPrivileges APIでエラー
// -2 ExitWindowsEx APIでエラー
//---------------------------------------------------------------------
int kjm::util::exitWindows() {
	int result = 0;

	// Windows をシャットダウン
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT){
		// NT対応
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
			//SE_SHUTDOWN_NAME権限をもたせる
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS) {
				return -1;
			}
			CloseHandle(hToken);
		}

		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT系] を実行します。\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT系] に失敗しました(error %u)。\n"), GetLastError());
			result = -2;
		}
		
	} else {
		// Windows95/98/Me 用
		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95系] を実行します。\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95系] に失敗しました(error %u)。\n"), GetLastError());
			result = -2;
		}
	}

	return result;
}


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
