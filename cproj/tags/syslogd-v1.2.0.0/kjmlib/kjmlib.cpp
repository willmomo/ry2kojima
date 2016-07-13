#include "kjmlib.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//---------------------------------------------------------------------
// ASCII文字列をUNICODE文字列に変換する
//---------------------------------------------------------------------
std::wstring kjm::util::strA2strW(const char* src) {
	// 必要な文字数を求める
	int nRet = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if (nRet == 0) {
		return std::wstring();	// エラーの時は、空文字列を返す
	}

	std::vector<wchar_t> result(nRet);	// 必要なバッファを確保

	MultiByteToWideChar(CP_ACP, 0, src, -1, &result[0], result.size());
	return &result[0];
}

//---------------------------------------------------------------------
// UNICODE文字列をASCII文字列に変換する
//---------------------------------------------------------------------
std::string kjm::util::strW2strA(const wchar_t* src) {
	// 必要な文字数を求める
	int nRet = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	if (nRet == 0) {
		return std::string();	// エラーの時は、空文字列を返す
	}

	std::vector<char> result(nRet);	// 必要なバッファを確保
	
	WideCharToMultiByte(CP_ACP, 0, src, -1, &result[0], result.size(), NULL, NULL);
	return &result[0];
}

//---------------------------------------------------------------------
// pszPathで指定したディレクトリ内の、現在日時からnDay以上古いファイル
// を削除します。
//
// 削除対象は、pszPath直下のファイルだけです。
// 削除の基準は、(現在日時 - 最終更新日時)の値なので、時間まで考慮して、
// nDayと比較します。
//---------------------------------------------------------------------
void kjm::util::cleanup_dir(const _tstring& pszPath, int nDay) {
	_tstring findPath = kjm::util::path_append(pszPath, _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(findPath.c_str(), &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			__int64 diff = (kjm::time::get_system_time() - kjm::time::time(data.ftLastWriteTime)) / FT_1DAY;
			if (diff >= nDay) {
				_tstring delFile = kjm::util::path_append(pszPath, data.cFileName);
				DeleteFile(delFile.c_str());
			}
		}
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);
}

//---------------------------------------------------------------------
// GetEnvironmentVariable関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::get_environment_variable(const _tstring& lpName) {
	// 必要な文字数を求める
	DWORD dwRet = GetEnvironmentVariable(lpName.c_str(), NULL, 0);
	if (dwRet == 0) {
		return _tstring();	// 環境変数なしの時は、空文字列を返す
	}

	std::vector<TCHAR> buffer(dwRet);	// 必要なバッファを確保

	GetEnvironmentVariable(lpName.c_str(), &buffer[0], buffer.size());
	return &buffer[0];
}

//---------------------------------------------------------------------
// GetModuleFileName関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::get_module_filename(HMODULE hModule) {
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(hModule, buffer, countof(buffer));
	return buffer;
}

//---------------------------------------------------------------------
// GetPrivateProfileString関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::get_private_profile_string(const _tstring& lpSectionName, const _tstring& lpKeyName, const _tstring& lpDefault, const _tstring& lpFileName) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = GetPrivateProfileString(lpSectionName.c_str(), lpKeyName.c_str(), lpDefault.c_str(), &buffer[0], buffer.size(), lpFileName.c_str());
		if (buffer.size() - 1 > nRet) {
			// バッファのサイズ - 1が、戻り値よりも大きいときは、
			// すべての文字を読み込んだことが確実なので、ループを抜ける。
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
}

//---------------------------------------------------------------------
// LoadString関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::load_string(UINT uID, HINSTANCE hInstance) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = LoadString(hInstance, uID, &buffer[0], buffer.size());
		if (nRet == 0) {
			return _tstring();	// 文字列リソースが無い時は、空文字列を返す
		} else if (buffer.size() - 1 > nRet) {
			// バッファのサイズ - 1が、戻り値よりも大きいときは、
			// すべての文字を読み込んだことが確実なので、ループを抜ける。
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
}

//---------------------------------------------------------------------
// PathAppend関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::path_append(const _tstring& pPath, const _tstring& pMore) {
	TCHAR buffer[MAX_PATH];
	if (PathAppend(lstrcpy(buffer, pPath.c_str()), pMore.c_str()) == FALSE) {
		return _tstring();	// 関数失敗の時は、空文字列を返す
	}

	return buffer;
}

//---------------------------------------------------------------------
// PathRemoveFileSpecのラッパー
//---------------------------------------------------------------------
_tstring kjm::util::path_remove_filespec(const _tstring& path) {
	TCHAR buffer[MAX_PATH];
	if (PathRemoveFileSpec(lstrcpy(buffer, path.c_str())) == FALSE) {
		return _tstring();	// 関数失敗のときは、空文字列を返す
	}

	return buffer;
}


//---------------------------------------------------------------------
// PathRenameExtensionのラッパー
//---------------------------------------------------------------------
_tstring kjm::util::path_rename_extension(const _tstring& pszPath, const _tstring& pszExt) {
	TCHAR buffer[MAX_PATH];
	if (PathRenameExtension(lstrcpy(buffer, pszPath.c_str()), pszExt.c_str()) == FALSE) {
		return _tstring();	// 関数失敗の時は、空文字列を返す
	}

	return buffer;
}

//---------------------------------------------------------------------
// 再帰的にディレクトリを作成するCreateDirectory関数
//---------------------------------------------------------------------
bool kjm::util::create_directory(const _tstring& path) {
	// 指定フォルダがルートを指しているときは、正常終了
	if (PathIsRoot(path.c_str())) {
		return true;
	}

	// すでにフォルダが存在するので、正常終了
	if (PathFileExists(path.c_str())) {
		return true;
	}

	// 親フォルダの作成を試みる
	TCHAR work[MAX_PATH];
	PathAppend(lstrcpy(work, path.c_str()), _T(".."));
	if (kjm::util::create_directory(work)) {
		if (CreateDirectory(path.c_str(), NULL)) {
			return true;
		}
	}

	return false;
}
