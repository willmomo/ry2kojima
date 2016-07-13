//-----------------------------------------------------------------------------
// プリコンパイル済みヘッダを使用しないように設定してください。
//
// r.kojima
//-----------------------------------------------------------------------------

#include "kjmlib.h"

//-----------------------------------------------------------------------------
// OutputDebugString を簡単に使うための関数
//-----------------------------------------------------------------------------
void kjm::ods(LPCTSTR format, ...) {
	std::vector<TCHAR> work(512);
	va_list args;
	va_start(args, format);
	while (_vsntprintf(&work.at(0), work.size(), format, args) == -1) {
		work.resize(work.size() + 512);
	}
	va_end(args);
	OutputDebugString(&work.at(0));
}

//=============================================================================
// kjm::utilの実装
//=============================================================================
//---------------------------------------------------------------------
// コピー元の直下にあるすべてのファイルをコピー先にコピーする
//---------------------------------------------------------------------
DWORD kjm::util::CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath) {

	assert(pszSrcPath != NULL);
	assert(pszSrcPath != NULL);

	std::string module_name = kjm::util::GetModuleName();
	kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(%s, %s) enter"), module_name.c_str(), pszSrcPath, pszDstPath);

	DWORD dwRet = NO_ERROR;

	// 検索用ワイルドカード作成
	kjm::PathString wc = kjm::PathString(pszSrcPath).Append(_T("*.*"));

	// ファイルを検索
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// フォルダ以外のファイルをコピーする。
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {

				// lockfile以外のファイルをコピーする。
				// lockfileは、コピーしようとしても常にエラーになる。
				if (lstrcmpi(data.cFileName, _T("lockfile")) != 0) {
					kjm::PathString srcFile = kjm::PathString(pszSrcPath).Append(data.cFileName);
					kjm::PathString dstFile = kjm::PathString(pszDstPath).Append(data.cFileName);

					kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(): CopyFile(%s, %s, FALSE)"), module_name.c_str(), srcFile.c_str(), dstFile.c_str());
					int retryCnt = 0;
retry:
					BOOL ret = CopyFile(srcFile.c_str(), dstFile.c_str(), FALSE);
					if (ret == FALSE) {
						dwRet = GetLastError();
						kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(): CopyFile error: %u"), module_name.c_str(), dwRet);

						if (retryCnt < 10) {
							Sleep(1000);
							++retryCnt;
							goto retry;
						} else {
							break;
						}
					} else {
						dwRet = NO_ERROR;
					}
				}
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	} else {
		kjm::log_error(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(): FindFirstFile(%s, -)"), module_name.c_str(), wc.c_str());
	}

	kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles() leave with %u"), module_name.c_str(), dwRet);

	return dwRet;
}

//-----------------------------------------------------------------------------
// プロセスのモジュール名を取得(GetModuleFileName 関数のラッパー)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getModuleFileName() {
	std::vector<TCHAR> work(MAX_PATH);
	::GetModuleFileName(NULL, &work.at(0), work.size());
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// ini ファイルから文字列を取得する(GetPrivateProfileString 関数のラッパー)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getPrivateProfileString(LPCTSTR section, LPCTSTR key, LPCTSTR defValue, LPCTSTR iniFile) {
	std::vector<TCHAR> work(MAX_PATH);
	while (::GetPrivateProfileString(section, key, defValue, &work.at(0), work.size(), iniFile) == (work.size() - 1)) {
		work.resize(work.size() + MAX_PATH);
	}
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// パス文字列の拡張子を変更する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameExtension(const std::basic_string<TCHAR> &path, const std::basic_string<TCHAR> &ext) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRenameExtension(::lstrcpy(&work.at(0), path.c_str()), ext.c_str());
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// パス文字列のファイル名を変更する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameFileSpec(const std::basic_string<TCHAR> &path, const std::basic_string<TCHAR> &fname) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveFileSpec(::lstrcpy(&work.at(0), path.c_str()));
	::PathAppend(&work.at(0), fname.c_str());
	return &work.at(0);
}

/// デフォルト・コンストラクタ
kjm::BiNotifyIcon::BiNotifyIcon() {

	// Add メソッドで sizeof(NOTIFYICON)、
	// Delete メソッドで 0 を代入する。
	cbSize = 0;
}


/**
 *	@brief	デストラクタ
 *
 *	@note	タスクトレイに登録済みのときは、Delete する。
 */
kjm::BiNotifyIcon::~BiNotifyIcon() {

	// タスクトレイに登録さているときは、削除する。
	// Delete メソッド内部で自動判断。
	Delete();
}


/**
 *	@brief	タスクトレイにアイコンを追加する。
 *			引数は、NOTIFYICONDATA のメンバーに対応する。
 *
 *	@param[in]	hWnd				ウィンドウへのハンドル
 *	@param[in]	uID					アプリケーションが識別するためのタスクバーアイコンのID
 *	@param[in]	uFlags				uCallbackMessage, hIcon, pszTip の有効/無効を指定するフラグ
 *	@param[in]	uCallbackMessage	通知メッセージに使われるメッセージID
 *	@param[in]	hIcon				登録するアイコンへのハンドル
 *	@param[in]	pszTip				ツールチップの文字列
 *
 *	@return	成功したときは TRUE を返す。
 *			失敗したときは FALSE を返す。
 *
 *	@note	アイコンがすでに登録済みのときは、Delete してから呼び出される。
 */
BOOL kjm::BiNotifyIcon::Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, const char* pszTip) {

	// すでに登録されているかもしれないので、削除する
	Delete();

	// NOTIFYICONDATA のメンバーを埋める
	cbSize = sizeof(NOTIFYICONDATA);
	hWnd = hWnd;
	uID = uID;
	uFlags = uFlags;
	uCallbackMessage = uCallbackMessage;
	hIcon = hIcon;
	if (pszTip)
		lstrcpy(this->szTip, pszTip);
	else
		this->szTip[0] = '\0';

	BOOL rc = Shell_NotifyIcon(NIM_ADD, this);

	// 登録に失敗したときは、未登録状態(cbSize == 0)にする。
	if (rc == FALSE) {
		cbSize = 0;
	}

	return rc;
}


/**
 *	@brief	タスクトレイからアイコンを削除する
 *
 *	@note	アイコンが未登録でも、正常に処理される。
 */
BOOL kjm::BiNotifyIcon::Delete() {

	BOOL rc = TRUE;

	// Add メソッドが呼ばれているときだけ呼び出す
	if (cbSize != 0) {
		rc = Shell_NotifyIcon(NIM_DELETE, this);

		// 登録解除に成功したとき、未登録状態(cbSize == 0)にする。
		if (rc != FALSE) {
			cbSize = 0;
		}
	}

	return rc;
}


/**
 *	@brief	タスクトレイアイコンの情報を変更する。
 *			引数は、NOTIFYICONDATA のメンバーに対応する。
 *
 *	@param[in]	uFlags				uCallbackMessage, hIcon, pszTip の有効/無効を指定するフラグ
 *	@param[in]	uCallbackMessage	通知メッセージに使われるメッセージID
 *	@param[in]	hIcon				登録するアイコンへのハンドル
 *	@param[in]	pszTip				ツールチップの文字列
 *
 *	@return	成功したときは TRUE を返す。
 *			失敗したときは FALSE を返す。
 *
 *	@note	アイコンが未登録のときは、失敗する。
 */
BOOL kjm::BiNotifyIcon::Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, char* pszTip) {

	// Add メソッドが、まだ呼ばれていないときは、エラー終了
	if (cbSize == 0)	return FALSE;

	uFlags = uFlags;

	// フラグに応じて、NOTIFYICONDATA のメンバーを埋める
	if (uFlags & NIF_MESSAGE)
		uCallbackMessage = uCallbackMessage;

	if (uFlags & NIF_ICON)
		hIcon = hIcon;

	if (uFlags & NIF_TIP) {
		if (pszTip) {
			lstrcpy(szTip, pszTip);
		} else {
			szTip[0] = '\0';
		}
	}

	return Shell_NotifyIcon(NIM_MODIFY, this);
}


//---------------------------------------------------------------------
// 情報ログを残す関数
//---------------------------------------------------------------------
void kjm::log_info(const char* signature, const char* format, ...) {
#if defined(USE_WSSLOG)
	va_list marker;
	va_start(marker, format);	// 可変個の引数の初期化

	std::vector<char> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsnprintf(buffer.begin(), buffer.size(), format, marker) == -1);
	va_end(marker);

	Winssa_Log(signature, LOGLV_INFO, buffer.begin());
#endif
}


//---------------------------------------------------------------------
// エラーログを残す関数
//---------------------------------------------------------------------
void kjm::log_error(const char* signature, const char* format, ...) {
#if defined(USE_WSSLOG)
	va_list marker;
	va_start(marker, format);	// 可変個の引数の初期化

	std::vector<char> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsnprintf(buffer.begin(), buffer.size(), format, marker) == -1);
	va_end(marker);

	Winssa_Log(signature, LOGLV_ERR, buffer.begin());
#endif
}


//---------------------------------------------------------------------
// 再帰的にディレクトリを作成できる CreateDirectory 関数
//
// [引数]
// lpPathName  ディレクトリ文字列へのポインタ
//
// [戻り値]
// TRUE   正常終了
// FALSE  エラー終了
//
// [説明]
// すでにフォルダがあるときも正常終了。UNC共有パスのみを指定したときは、
// パスがなくても正常終了になる。
//---------------------------------------------------------------------
BOOL kjm::create_directory(LPCTSTR lpPathName) {
	
	// 最後の \ は、邪魔なので取り除いてから、処理に入る。
	TCHAR path_name[MAX_PATH];
	::PathRemoveBackslash(lstrcpy(path_name, lpPathName));

	if (::PathFileExists(path_name))	return TRUE;
	if (::PathIsRoot(path_name))		return TRUE;

	TCHAR parent_path[MAX_PATH];
	::PathRemoveFileSpec(lstrcpy(parent_path, path_name));

	if (kjm::create_directory(parent_path))
		return ::CreateDirectory(path_name, NULL);

	return FALSE;
}


//---------------------------------------------------------------------
// 文字列を置換する。VBのStrReplaceのような関数。
//---------------------------------------------------------------------
std::string kjm::win32::str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace) {
	std::string result = src;
	std::string::size_type pos = 0;
	while ((pos = result.find(pFind, pos)) != std::string::npos) {
		result.replace(pos, lstrlen(pFind), pReplace);
	}
	return result;
}


//---------------------------------------------------------------------
// エラーコードからシステムエラーメッセージを取得する。
//---------------------------------------------------------------------
std::string kjm::win32::format_message(DWORD dwErrorCode) {

	std::string result;

	char* pBuffer = NULL;

	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pBuffer,
		0,
		NULL);

	if (pBuffer) {
		result = pBuffer;
		result = str_replace(std::string(pBuffer), "\r\n", "");

		::LocalFree(pBuffer);
	} else {
		char buf[1024];
		wsprintf(buf, "エラー %d (0x%08X) のメッセージは、ありません。", dwErrorCode, dwErrorCode);
		result = buf;
	}

	return result;
}


//---------------------------------------------------------------------
// ホスト名または、IP文字列からIPアドレスを取得
//---------------------------------------------------------------------
DWORD kjm::util::get_ipaddr(LPCSTR lpName) {
	DWORD addr = INADDR_ANY;
#if 0
	if (lpName[0] == '\0')	return INADDR_ANY;

	DWORD addr = INADDR_ANY;

	// まず、IP文字列変換してみる。
	addr = inet_addr(lpName);
	if (addr == INADDR_NONE) {

		// DNSを引く
		struct hostent* hent = gethostbyname(lpName);
		if (hent != NULL) {
			addr = *((DWORD *)(hent->h_addr_list[0]));
		} else{
			addr = INADDR_ANY;
		}
	}
#endif
	return	addr;
}

//---------------------------------------------------------------------
// ディレクトリを再帰的に削除する
//---------------------------------------------------------------------
bool kjm::util::remove_folder(LPCTSTR pszPath) {
	bool bRet = true;
	std::vector<TCHAR> wc(MAX_PATH);

	// 検索に必要なワイルドカードを作成
	///PathAppend(lstrcpyn(wc.begin(), pszPath, wc.size()), _T("*.*"));
	PathAppend(lstrcpyn(&wc.at(0), pszPath, wc.size()), _T("*.*"));

	WIN32_FIND_DATA data;
	///HANDLE hFind = FindFirstFile(wc.begin(), &data);
	HANDLE hFind = FindFirstFile(&wc.at(0), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((lstrcmp(data.cFileName, _T(".")) != 0) && (lstrcmp(data.cFileName, _T("..")) != 0)) {
					std::vector<TCHAR> child(MAX_PATH);

					// 子供フォルダを再帰的に削除する
					///PathAppend(lstrcpyn(child.begin(), pszPath, child.size()), data.cFileName);
					PathAppend(lstrcpyn(&child.at(0), pszPath, child.size()), data.cFileName);
					///if ((bRet = kjm::util::remove_folder(child.begin())) == false) {
					if ((bRet = kjm::util::remove_folder(&child.at(0))) == false) {
						break;
					}
				}
			} else {
				std::vector<TCHAR> fname(MAX_PATH);
				///PathAppend(lstrcpyn(fname.begin(), pszPath, fname.size()), data.cFileName);
				PathAppend(lstrcpyn(&fname.at(0), pszPath, fname.size()), data.cFileName);

				// 削除するファイルに readonly がついているときは落とす。
				///DWORD dwAttr = GetFileAttributes(fname.begin());
				DWORD dwAttr = GetFileAttributes(&fname.at(0));
				if ((dwAttr != 0xffffffff) && ((dwAttr & FILE_ATTRIBUTE_READONLY) != 0)) {
					///if (!SetFileAttributes(fname.begin(), dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
					if (!SetFileAttributes(&fname.at(0), dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
						bRet = false;
						break;
					}
				}

				///if (!DeleteFile(fname.begin())) {
				if (!DeleteFile(&fname.at(0))) {
					bRet = false;
					break;
				}
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	} else {
		bRet = false;
	}

	if (bRet) {
		if (!RemoveDirectory(pszPath)) {
			bRet = false;
		}
	}

	return bRet;
}


//=============================================================================
// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
// 静的メソッドを公開します。
//=============================================================================
//-----------------------------------------------------------------------------
// ファイルまたはディレクトリ、およびその内容を新しい場所にコピーします。
//-----------------------------------------------------------------------------
BOOL kjm::directory::copy(LPCTSTR sourceDirName, LPCTSTR destDirName) {
	BOOL bRet = FALSE;

	// とりあえず、コピー先ディレクトリを作成してみる。
	if (kjm::create_directory(destDirName) == FALSE) {

		DWORD dwError = ::GetLastError();
		kjm::log_error(_T("kjmlib"),
			_T("kjm::directory::copy(\"%s\", \"%s\"): kjm::create_directory(\"%s\") error: %d : %s"),
			sourceDirName, destDirName, destDirName, dwError, kjm::win32::format_message(dwError).c_str());

		return bRet;
	}

	// ファイルとフォルダを列挙して、コピーと再帰呼び出しを続ける。
	kjm::PathString findPath = kjm::PathString(sourceDirName).Append(_T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = ::FindFirstFile(findPath.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		bRet = TRUE;	// 正常終了扱いにして、以降の処理を続ける
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
					// フォルダを再帰的にコピーする。
					if (kjm::directory::copy(
							kjm::PathString(sourceDirName).Append(data.cFileName).c_str(),
							kjm::PathString(destDirName).Append(data.cFileName).c_str()) == FALSE) {

						bRet = FALSE;
						break;
					}
				}
			} else {
				// 見つかったファイルをコピーする。
				kjm::PathString sourceFileName = kjm::PathString(sourceDirName).Append(data.cFileName);
				kjm::PathString destFileName = kjm::PathString(destDirName).Append(data.cFileName);

				if (::CopyFile(sourceFileName.c_str(), destFileName.c_str(), FALSE) == FALSE) {
					bRet = FALSE;

					DWORD dwError = ::GetLastError();
					kjm::log_error(_T("kjmlib"),
						_T("kjm::directory::copy(\"%s\", \"%s\"): ::CopyFile(\"%s\", \"%s\", FALSE) error: %d : %s"),
						sourceDirName, destDirName, sourceFileName.c_str(), destFileName.c_str(), dwError, kjm::win32::format_message(dwError).c_str());

					break;
				}
			}
		} while (::FindNextFile(hFind, &data));

		::FindClose(hFind);
	} else {
		DWORD dwError = ::GetLastError();
		kjm::log_error(_T("kjmlib"),
			_T("kjm::directory::copy(\"%s\", \"%s\"): ::FindFirstFile(\"%s\", ...) error: %d : %s"),
			sourceDirName, destDirName, findPath.c_str(), dwError, kjm::win32::format_message(dwError).c_str());
	}

	return bRet;
}


//=============================================================================
// レジストリを扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// コピーコンストラクタ
//-----------------------------------------------------------------------------
kjm::registry::registry(const kjm::registry& src) {
	if (src.m_hkey == NULL) {
		this->m_hkey = NULL;
	} else {
		::DuplicateHandle(::GetCurrentProcess(), src.m_hkey, ::GetCurrentProcess(), (LPHANDLE)&this->m_hkey, 0, TRUE, DUPLICATE_SAME_ACCESS);
	}
}

//-----------------------------------------------------------------------------
// RegOpenKeyEx のラッパー
//-----------------------------------------------------------------------------
LONG kjm::registry::reg_open_key_ex(HKEY hkey, LPCTSTR lpSubKey, REGSAM samDesired) {
	assert(m_hkey == NULL);

	return ::RegOpenKeyEx(hkey, lpSubKey, 0, samDesired, &m_hkey);
}

//-----------------------------------------------------------------------------
// RegQueryValueEx のラッパー
//-----------------------------------------------------------------------------
LONG kjm::registry::reg_query_value_ex(LPTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) const {
	assert(m_hkey != NULL);

	return ::RegQueryValueEx(m_hkey, lpValueName, NULL, lpType, lpData, lpcbData);
}

//-----------------------------------------------------------------------------
// RegCloseKey のラッパー
//-----------------------------------------------------------------------------
LONG kjm::registry::reg_close_key() {
	LONG result = ERROR_SUCCESS;
	if (m_hkey != NULL) {
		result = ::RegCloseKey(m_hkey);
		m_hkey = NULL;
	}
	return result;
}

//-----------------------------------------------------------------------------
// 文字列を取得する関数
//
// 値のタイプが
//   REG_SZ のときは、そのまま返す。
//   REG_EXPAND_SZ のときは、ExpandEnvironmentStrings を通してから返す。
//   REG_DWORD のときは、_ultoa を通してから返す。
//-----------------------------------------------------------------------------
std::string kjm::registry::get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const {
	assert(lpDefault != NULL);

	std::string result(lpDefault);	// デフォルト値で戻り値を作成しておく

	// 値に必要な情報を取得
	DWORD dwType;
	DWORD cbData = 0;
	if (reg_query_value_ex(lpValueName, &dwType, NULL, &cbData) != ERROR_SUCCESS) {
		return result;	// 関数に失敗したのでデフォルト値を戻して終了
	}

	// データの種類が未対応のときは、デフォルト値を戻して終了
	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ && dwType != REG_DWORD) {
		return result;
	}

	// バッファを確保して、データを取得
	std::vector<char> buf(cbData);
	///if (reg_query_value_ex(lpValueName, &dwType, (LPBYTE)buf.begin(), &cbData) != ERROR_SUCCESS) {
	if (reg_query_value_ex(lpValueName, &dwType, (LPBYTE)&buf.at(0), &cbData) != ERROR_SUCCESS) {
		return result;	// 関数に失敗したのでデフォルト値を戻して終了
	}

	// データの種類に応じて、戻り値を作成
	switch (dwType) {
	case REG_SZ:
		///result = buf.begin();
		result = &buf.at(0);
		break;
		
	case REG_EXPAND_SZ:
		///result = win32::expand_environment_strings(buf.begin());
		result = win32::expand_environment_strings(&buf.at(0));
		break;

	case REG_DWORD:
		{
			char temp[32];	// u_long の最大値を十分に表現できるサイズ
			///result = _ultoa(*(LPDWORD)buf.begin(), temp, 10);
			result = _ultoa(*(LPDWORD)&buf.at(0), temp, 10);
		}
		break;
	}

	return result;	// 取得した文字列を戻す
}

//-----------------------------------------------------------------------------
// 代入演算子
//-----------------------------------------------------------------------------
kjm::registry& kjm::registry::operator =(const kjm::registry& rhs) {
	if (this == &rhs)
		return *this;

	if (rhs.m_hkey == NULL) {
		this->m_hkey = NULL;
	} else {
		::DuplicateHandle(::GetCurrentProcess(), rhs.m_hkey, ::GetCurrentProcess(), (LPHANDLE)&this->m_hkey, 0, TRUE, DUPLICATE_SAME_ACCESS);
	}

	return *this;
}


//=============================================================================
// イベントログにアクセスするためのクラス
//=============================================================================
//-----------------------------------------------------------------------------
// イベントログをバックアップする
//-----------------------------------------------------------------------------
BOOL kjm::event_log::backup(LPCTSTR sourceName, LPCTSTR backupFileName) {
	HANDLE hEventLog = ::OpenEventLog(NULL, sourceName);
	if (hEventLog == NULL)
		return FALSE;

	BOOL ret = ::BackupEventLog(hEventLog, backupFileName);

	::CloseEventLog(hEventLog);

	return ret;
}


//=============================================================================
// プロセスの起動に関するクラス
//=============================================================================
//-----------------------------------------------------------------------------
// GetProcessWindow関数で使用するコールバック
//
// [引数]
// HWND hwnd		列挙されたウィンドウへのハンドル
// LPARAM lParam	rkProcessへのポインタ。キャストして使う
//
// [備考]
// 列挙されたウィンドウと同じプロセスIDを持つものを見つけた場合
// そのハンドルをメンバー変数に保存し、列挙を終了する。
//-----------------------------------------------------------------------------
BOOL CALLBACK kjm::process::process_EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	kjm::process* p = reinterpret_cast<kjm::process*>(lParam);

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == p->m_pi.dwProcessId) {
		p->m_hwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// デフォルト・コンストラクタ
//-----------------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));
	memset(&m_szCommandLine, 0, sizeof(m_szCommandLine));
}

//-----------------------------------------------------------------------------
// スタートアップ情報(wShowWindow)を設定する
//-----------------------------------------------------------------------------
void kjm::process::SetSIShowWindow(WORD newShowWindow) {
	m_si.wShowWindow = newShowWindow;
	m_si.dwFlags |= STARTF_USESHOWWINDOW;
}

//-----------------------------------------------------------------------------
// スタートアップ情報(wShowWindow)をクリア(未使用に)する
//-----------------------------------------------------------------------------
void kjm::process::ClearSIShowWindow() {
	m_si.wShowWindow = 0;
	m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
}

//-----------------------------------------------------------------------------
// プロセスの実行
//-----------------------------------------------------------------------------
BOOL kjm::process::createProcess(const std::basic_string<TCHAR>& commandLine) {
	assert(m_pi.hProcess == NULL);

	if (m_pi.hProcess)	return FALSE;	// すでに何か実行している

	lstrcpy(m_szCommandLine, commandLine.c_str());
	m_si.cb = sizeof(m_si);

	return CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);
}

//-----------------------------------------------------------------------------
// プロセスハンドルを閉じる
//
// [説明]
// プロセスハンドルを閉じる。プロセスの終了ではないので注意。
// プロセスの終了は、プロセス間の取り決めによる。
//-----------------------------------------------------------------------------
void kjm::process::Close() {
	if (m_pi.hThread)	CloseHandle(m_pi.hThread);
	if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
	memset(&m_pi, 0, sizeof(m_pi));
}

//-----------------------------------------------------------------------------
// プロセスに関連付けられたウィンドウを返す
//-----------------------------------------------------------------------------
HWND kjm::process::GetProcessWindow() {
	assert(m_pi.hProcess);

	if (!m_pi.hProcess)	return NULL;

	m_hwnd = NULL;
	EnumWindows(kjm::process::process_EnumWindowsProc, reinterpret_cast<LPARAM>(this));
	return m_hwnd;
}
