//---------------------------------------------------------------------
// kjmlib : Version 0.2011.9.2
//
// プリコンパイル済みヘッダを使用しないように設定してください。
//
// r.kojima
//---------------------------------------------------------------------
#include "kjmlib.h"

#if _MSC_VER <= 1200	// VC++6以下
// 検索のためだけの関数オブジェクトが必要になってしまう。
// 本来の言語仕様では、このようなクラスは不要。
//-------------------------------------------------------------------------
// vector<optionInfo> から ロングオプションを検索するための関数オブジェクト
//-------------------------------------------------------------------------
class optionInfoLongNameFinder {
public:
	std::basic_string<TCHAR> m_targetName;
	optionInfoLongNameFinder(const std::basic_string<TCHAR>& targetName) : m_targetName(targetName) {}
	bool operator()(const kjm::optionInfo& rhs) const { return rhs.m_longOption == m_targetName; }
};

//-------------------------------------------------------------------------
// vector<optionInfo> から ショートオプションを検索するための関数オブジェクト
//-------------------------------------------------------------------------
class optionInfoShortNameFinder {
public:
	std::basic_string<TCHAR> m_targetName;
	optionInfoShortNameFinder(const std::basic_string<TCHAR>& targetName) : m_targetName(targetName) {}
	bool operator()(const kjm::optionInfo& rhs) const { return rhs.m_longOption == m_targetName; }
};
#endif

static bool s_verboseMode = false;

void kjm::setVerboseMode(bool mode) { s_verboseMode = mode; }
bool kjm::getVerboseMode() { return s_verboseMode; }

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

	std::basic_string<TCHAR> module_name = kjm::util::GetModuleName();
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
// ショートカットの作成
//
// CoInitialize(NULL)/CoUninitialize()は、アプリケーションが行ってください。
//-----------------------------------------------------------------------------
bool kjm::util::createShortcut(const std::basic_string<TCHAR>& strFileName,
							   const std::basic_string<TCHAR>& strFile,
							   const std::basic_string<TCHAR>& strArgs,
							   const std::basic_string<TCHAR>& strDir) {
	HRESULT hRes;
	IShellLink *isLink = NULL; 
	IPersistFile *ipFile = NULL;

	//IShellLinkを取得
	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&isLink);
	if (SUCCEEDED(hRes)) {
		//パスの設定
		isLink->SetPath(strFile.c_str());
		//実行時引数（スイッチ）の設定
		isLink->SetArguments(strArgs.c_str());
		//作業フォルダの設定
		isLink->SetWorkingDirectory(strDir.c_str());

		//IPersistFileを取得
		hRes = isLink->QueryInterface(IID_IPersistFile, (void**)&ipFile);
		if (SUCCEEDED(hRes)) {
			//ディスクに保存する
#if defined(UNICODE)
			hRes = ipFile->Save(strFileName.c_str(), TRUE);
#else
			hRes = ipFile->Save(kjm::util::toUnicode(strFileName).c_str(), TRUE);
#endif
			//IPersistFileの破棄
			ipFile->Release();
		}

		//IShellLinkを破棄
		isLink->Release();
	}else{
		//失敗
	}

	return (SUCCEEDED(hRes) ? true : false);
}

//-----------------------------------------------------------------------------
// 複数ファイルを削除出る DeleteFile 
//-----------------------------------------------------------------------------
bool kjm::util::deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard) {
	std::basic_string<TCHAR> module_name = kjm::util::GetModuleName();

	std::basic_string<TCHAR> findPath = kjm::util::append(path, wildcard);
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(findPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		DWORD dwErr = ::GetLastError();
		kjm::log_error(_T("KJMLIB"), _T("<%s> kjm::util::deleteFiles(): FindFirstFile(%s, -) error %d"),
			module_name.c_str(), findPath.c_str(), dwErr);
		return (dwErr == ERROR_FILE_NOT_FOUND) ? true : false;
	}

	bool result = true;
	do {
		// ディレクトリ以外のファイルが削除対象
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			std::basic_string<TCHAR> delFile = kjm::util::append(path, findData.cFileName);
			if (::DeleteFile(delFile.c_str()) == FALSE) {
				DWORD dwErr = ::GetLastError();
				if (dwErr != ERROR_FILE_NOT_FOUND) {
					kjm::log_error(_T("KJMLIB"), _T("<%s> kjm::util::deleteFiles(): DeleteFile(%s) error %d"),
						module_name.c_str(), delFile.c_str(), dwErr);
					result = false;
					break;
				}
			}
		}
	} while (::FindNextFile(hFind, &findData));

	::FindClose(hFind);

	return result;
}

//-----------------------------------------------------------------------------
// 文字列の比較(大文字・小文字を無視して比較)
//-----------------------------------------------------------------------------
bool kjm::util::equalsIgnoreCase(const std::basic_string<TCHAR>& lhs, const std::basic_string<TCHAR>& rhs) {
	return (::lstrcmpi(lhs.c_str(), rhs.c_str()) == 0 ? true : false);
}

//-----------------------------------------------------------------------------
// Win32API ExpandEnvironmentStringsを安全に使う関数
//-----------------------------------------------------------------------------
DWORD kjm::util::expandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst ) {
	DWORD dwRet;

	if (rDst.empty()) {
		rDst.resize(1);
	}

	while ((dwRet = ::ExpandEnvironmentStrings(lpSrc, &rDst[0], rDst.size())) > rDst.size()) {
		if (dwRet == 0) {
			break;
		} else {
			rDst.resize(dwRet);
		}
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// 特殊フォルダへのパスを取得する
//
// nFolder の値は、shlobj.h に定義してある、CSIDL_で始まる定数を指定。
// 代表的なものとして、
// CSIDL_STARTUP ... ユーザーごとのスタートアップ
// CSIDL_COMMON_STARTUP ... All Usersのスタートアップ
//
// 少なくとも、vs2008から有効な関数
//-----------------------------------------------------------------------------
#if _WIN32_IE >= 0x0700
std::basic_string<TCHAR> kjm::util::getSpecialFolderLocation(int nFolder) {
	std::vector<TCHAR> path(MAX_PATH);
	SHGetFolderPath(NULL, nFolder, NULL, SHGFP_TYPE_CURRENT, &path[0]);
	return &path[0];
}
#endif

//-----------------------------------------------------------------------------
// %TEMP%フォルダを取得する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getTempPath() {
	std::vector<TCHAR> path(MAX_PATH);
	::GetTempPath(path.size(), &path[0]);
	return &path[0];
}

//-----------------------------------------------------------------------------
// 指定の共有フォルダと共有パスの存在を保証する
//-----------------------------------------------------------------------------
int kjm::util::guaranteeSharedFolder(LPCTSTR pszSharedFolder, LPCTSTR pszSharedPath) {
	int nRet = 0;

	// 共有フォルダ名をUnicodeに変換
#if defined(_MBCS)
	std::wstring wstrSharedFolder = kjm::util::toUnicode(pszSharedFolder);
#else
	std::wstring wstrSharedFolder = pszSharedFolder;
#endif
	if (wstrSharedFolder.empty()) {
		// 共有フォルダの指定が間違っているときの処理
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: 共有フォルダ名の指定が異常です。"));
		return -1;
	}

	// 共有パス名をUnicodeに変換
#if defined(_MBCS)
	std::wstring wstrSharedPath = kjm::util::toUnicode(pszSharedPath);
#else
	std::wstring wstrSharedPath = pszSharedPath;
#endif
	if (wstrSharedPath.empty()) {
		// 共有パスの指定が間違っているときの処理
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: 物理パス名の指定が異常です。"));
		return -1;
	}

	SHARE_INFO_2* pInfo;
	NET_API_STATUS ret;
	int tryCount = 0;

	// ローカルに有る、指定の共有フォルダ情報を取得
retry:
	++tryCount;

	// 少なくとも、vs2008からLPWSTR で宣言されるようになった
#if _WIN32_IE >= 0x0700
	ret = NetShareGetInfo(NULL, (LPWSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
#else
	ret = NetShareGetInfo(NULL, (LPTSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
#endif

	if (ret == NERR_Success) {
	
		// 共有フォルダが指定の共有パスを指しているか確認する
#if defined(_MBCS)
		if (lstrcmpi(kjm::util::toAnsi((LPCWSTR)pInfo->shi2_path).c_str(), pszSharedPath) != 0) {
			// 共有フォルダが共有パスと違うことを警告
			kjm::log_info(_T("KJMLIB"), 
				_T("kjm::util::guaranteeSharedFolder: 共有フォルダはありますが、'%s' を指しています。"), 
				kjm::util::toAnsi((LPCWSTR)pInfo->shi2_path).c_str());
		}
#else
		if (lstrcmpi(std::wstring((LPCWSTR)pInfo->shi2_path).c_str(), pszSharedPath) != 0) {
			// 共有フォルダが共有パスと違うことを警告
			kjm::log_info(_T("KJMLIB"), 
				_T("kjm::util::guaranteeSharedFolder: 共有フォルダはありますが、'%s' を指しています。"), pInfo->shi2_path);
		}
#endif

		NetApiBufferFree(pInfo);

		nRet = 0;	// 正常終了
	} else if (ret == NERR_ServerNotStarted) {
		// The Server service is not started.
		// 1 秒待ってリトライ、最大 60 回試します。
		if (tryCount < 60) {
			Sleep(1000);
			goto retry;
		} else {
			kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: The Server service is not started.(NetShareGetInfo error %d)。"), ret); 
			nRet = -1;
		}
	} else if (ret == NERR_NetNameNotFound) {
		// 共有フォルダが存在しないときの処理
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: 共有フォルダがありません。作成を試みます。"));

		// 指定の共有パスを(無ければ)作成する
		if (::PathFileExists(pszSharedPath) == FALSE) {
			if (kjm::create_directory(pszSharedPath) == FALSE) {
				// 物理パスが作成できない時の処理
				kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: 物理パスが作成できません。"));

				return -1;
			}
		}

		SHARE_INFO_2 info;
	// 少なくとも、vs2008からLPWSTR で宣言されるようになった
#if _WIN32_IE >= 0x0700
		info.shi2_netname = (LPWSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPWSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPWSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPWSTR)L"";
#else
		info.shi2_netname = (LPTSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPTSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPTSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPTSTR)L"";
#endif

		DWORD parm_err = 0;
		ret = NetShareAdd(NULL, 2, (LPBYTE)&info, &parm_err);
		if (ret == NERR_Success) {
			nRet = 0;	// 正常終了
		} else {
			// 共有フォルダが作成できなかったときの処理
			kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: 共有フォルダが作成できません(NetShareAdd error %d)。"), ret); 

			nRet = -1;	// 異常終了
		}
	} else {
		// 共有フォルダの情報が取得できない時の処理
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: 共有フォルダの情報が取得できません(NetShareGetInfo error %d)。"), ret); 
		
		nRet = -1;	// 異常終了
	}

	return nRet;
}

// string を返す GetComputerName 関数
std::basic_string<TCHAR> kjm::util::getComputerName() {
	std::vector<TCHAR> work(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD nSize = work.size();
	GetComputerName(&work.at(0), &nSize);
	return &work.at(0);
}

#if 0	// 不要関数
//-----------------------------------------------------------------------------
// プロセスのモジュール名を取得(GetModuleFileName 関数のラッパー)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getModuleFileName() {
	std::vector<TCHAR> work(MAX_PATH);
	::GetModuleFileName(NULL, &work.at(0), work.size());
	return &work.at(0);
}
#endif

//---------------------------------------------------------------------
// Win32API GetModuleFileName を安全に使う関数
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getModuleFileName(HMODULE hModule/*=NULL*/) {
	std::vector<TCHAR> work(MAX_PATH);

	DWORD dwRet;
	while ((dwRet = ::GetModuleFileName(hModule, &work[0], work.size())) == work.size()) {
		work.resize(work.size() * 2);
	}

	return &work[0];
}

//-----------------------------------------------------------------------------
// ini ファイルから文字列を取得する(GetPrivateProfileString 関数のラッパー)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getPrivateProfileString(const std::basic_string<TCHAR>& section,
															const std::basic_string<TCHAR>& key,
															const std::basic_string<TCHAR>& defValue,
															const std::basic_string<TCHAR>& iniFile) {
	std::vector<TCHAR> work(MAX_PATH);
	while (::GetPrivateProfileString(section.c_str(), key.c_str(), defValue.c_str(), &work.at(0), work.size(), iniFile.c_str()) == (work.size() - 1)) {
		work.resize(work.size() + MAX_PATH);
	}
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// ファイル名部分を取得
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::findFileName(const std::basic_string<TCHAR>& path) {
	return ::PathFindFileName(path.c_str());
}

//---------------------------------------------------------------------
// PathRenameExtensionのラッパー
//---------------------------------------------------------------------
std::basic_string<TCHAR>& kjm::util::pathRenameExtension(std::basic_string<TCHAR>& strPath, LPCTSTR pszExt) {
	std::vector<TCHAR> work(strPath.begin(), strPath.end());
	work.push_back('\0');
	work.resize(work.size() + lstrlen(pszExt) + MAX_PATH );	// 十分なバッファを確保

	if (::PathRenameExtension(&work[0], pszExt)) {
		strPath = &work[0];
	}

	return strPath;
}

//-----------------------------------------------------------------------------
// パス文字列にパスを追加
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathAppend(::lstrcpy(&work.at(0), path.c_str()), more.c_str());
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// 行末の改行を取り除く
//-----------------------------------------------------------------------------
std::basic_string<TCHAR>& kjm::util::chomp(std::basic_string<TCHAR>& s) {

	// 行末に改行文字があるときは、取り除く
	while (!s.empty() && (*(s.rbegin()) == '\n')) {
		s.erase(s.end() - 1);
	}

	return s;
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

//-----------------------------------------------------------------------------
// カレントディレクトリを変更する
//-----------------------------------------------------------------------------
bool kjm::util::setCurrentDirectory(const std::basic_string<TCHAR>& pathName) {
#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		Winssa_Log(_T("kjmlog"), LOGLV_INFO, _T("<%s> kjm::util::setCurrentDirectory(): カレントディレクトリを '%s' に変更します。"),
			kjm::util::GetModuleName().c_str(), pathName.c_str());
	}
#endif
	BOOL bRet = ::SetCurrentDirectory(pathName.c_str());
#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		if (bRet) {
			Winssa_Log(_T("kjmlog"), LOGLV_INFO,
				_T("<%s> kjm::util::setCurrentDirectory(): 変更しました。"), kjm::util::GetModuleName().c_str());
		} else {
			DWORD dwErr = ::GetLastError();
			Winssa_Log(_T("kjmlog"), LOGLV_INFO,
				_T("<%s> kjm::util::setCurrentDirectory(): error %d"), kjm::util::GetModuleName().c_str(), dwErr);
		}
	}
#endif
	return (bRet) ? true : false;
}

//---------------------------------------------------------------------
// 文字列を返す安全な sprintf 関数
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::sprintf_str(LPCTSTR format, ...) {
	std::vector<TCHAR> buffer(256);
	va_list marker;
	va_start(marker, format);
	while (_vsntprintf(&buffer[0], buffer.size(), format, marker) == -1) {
		buffer.resize(buffer.size() * 2);
	}
	va_end(marker);
	return &buffer[0];
}

//-----------------------------------------------------------------------------
// 整数を文字列に変換
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::to_a(int value, int radix/* = 10*/) {
	TCHAR str[80];

#if _WIN32_IE >= 0x0700
	return (_itot_s(value, str, _countof(str), radix) == 0) ? str : _T("");
#else
	return _itot(value, str, radix);
#endif
}

//-----------------------------------------------------------------------------
// 整数を文字列に変換(printf相当のフォーマットが指定できる)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::to_a(int value, const std::basic_string<TCHAR>& format) {
	return kjm::util::sprintf_str(format.c_str(), value);
}

//-----------------------------------------------------------------------------
// パス文字列の終端 \ を削除する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeBackslash(const std::basic_string<TCHAR>& path) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveBackslash(::lstrcpy(&work.at(0), path.c_str()));
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// パス文字列のファイル名を削除する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeFileSpec(const std::basic_string<TCHAR> &path) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveFileSpec(::lstrcpy(&work.at(0), path.c_str()));
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// Win32API FormatMessageを安全に使う関数
//-----------------------------------------------------------------------------
DWORD kjm::util::formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments ) {

	LPTSTR pMessage = NULL;

	DWORD dwRet = ::FormatMessage(
		dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpSource,
		dwMessageId,
		dwLanguageId,
		(LPTSTR)&pMessage,
		0,
		Arguments);

	if ( pMessage ) {
		buffer.resize( lstrlen( pMessage ) + 1 );
		lstrcpy( &buffer[0], pMessage );
		::LocalFree( pMessage );
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// システム標準のエラーメッセージを作成
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// ファイルの有無を確認する
//-----------------------------------------------------------------------------
bool kjm::util::fileExists(const std::basic_string<TCHAR> &path) {
	return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
}

//=============================================================================
// CSV ファイルを扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// CSV ファイルを開く
//-----------------------------------------------------------------------------
bool kjm::csvFile::open(LPCTSTR pszFileName) {
	if (!m_fr.is_open()) {
		return false;
	}

#if (_MSC_VER <= 1200) && defined(UNICODE)	// VC++6以下でUNICODE指定のとき
	// basic_ifstream<wchar_t>は、【ストリームがwchar_t】になるだけで
	// 関数などは、すべて、charを受け取る。不思議な実装になっている。
	m_fr.open(kjm::util::toAnsi(pszFileName).c_str());
#else
	m_fr.open(pszFileName);
#endif
	if (!m_fr.is_open()) {
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// CSV ファイルを閉じる
//-----------------------------------------------------------------------------
void kjm::csvFile::close() {
	if (!m_fr.is_open()) {
		m_fr.close();
	}
}

//-----------------------------------------------------------------------------
// CSV ファイルから 1 行読み込む
//
// [備考]
// "...","...","..." の形式しかサポートしていません!!
//-----------------------------------------------------------------------------
int kjm::csvFile::parseCsvCol(std::vector<std::basic_string<TCHAR> >& cols) {
	std::basic_string<TCHAR> oneLine;

	cols.clear();

	if (std::getline(m_fr, oneLine).fail()) {
		return -1;
	}

	if (oneLine.empty()) {
		return 0;
	}
	
	LPCTSTR pTop = NULL;
	LPCTSTR pEnd = NULL;
	std::basic_string<TCHAR> oneCol;

	for (LPCTSTR p = oneLine.c_str(); *p != 0; p++) {
		TCHAR c = *p;
		if (c == '\"' && pTop == NULL) {			// ダブルコーテーション " だったら
			pTop = p;
		} else if (c == '\"' && pTop != NULL) {		
			if (*(p + 1) == '\"') {					// 次も " だったら、
				oneCol += _T("\"");						// １Colのデータを示す区切り文字でなく、本当の " とみなす 
				++p;
			} else {
				pEnd = p;
			}
		} else if (c == ',' && pEnd != NULL) {
			cols.push_back(oneCol);
			oneCol.erase();
			pTop = pEnd = NULL;
		} else {
			oneCol += c;
		}
	}

	if (pTop != NULL && pEnd != NULL) {
		cols.push_back(oneCol);
		oneCol.erase();
		pTop = pEnd = NULL;
	}

	// NULLだったら、""にする（atolなどで落ちるのを防ぐため）
	for (size_t l = 0; l < cols.size(); l++) {
		if (cols[l].c_str() == NULL) {
			cols[l] = _T(""); 
			assert(0);
		}
		if (cols[l].empty())	{
			cols[l] = _T("0"); 
			assert(0);
		}
	}

	return cols.size();
}

//=============================================================================
// 基本ソケットクラス
//=============================================================================
//-----------------------------------------------------------------------------
// sockaddr_in を初期化するための便利関数
//-----------------------------------------------------------------------------
void kjm::baseSocket::initSockaddrIn(sockaddr_in& name, const std::basic_string<TCHAR>& host, short port) {
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.S_un.S_addr = kjm::util::get_ipaddr(host.c_str());
	name.sin_port = htons(port);
}

//-----------------------------------------------------------------------------
// ソケットを開く
//-----------------------------------------------------------------------------
bool kjm::baseSocket::openSocket(int af, int type, int protocol) {
	if (this->m_sock != INVALID_SOCKET) {
		return false;
	}

	this->m_sock = socket(af, type, protocol);

	return (this->m_sock != INVALID_SOCKET) ? true : false;
}

//-----------------------------------------------------------------------------
// 接続する
//-----------------------------------------------------------------------------
bool kjm::baseSocket::connectSocket(const sockaddr_in& name) {
	bool ret = (connect(this->m_sock, (const sockaddr*)&name, sizeof(name)) == 0) ? true : false;
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//-----------------------------------------------------------------------------
// 受信タイムアウトの設定
//-----------------------------------------------------------------------------
bool kjm::baseSocket::setRecvTimeout(int msTimeout) {
	bool ret = (setsockopt(this->m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&msTimeout, sizeof(msTimeout)) == 0) ? true : false;
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//-----------------------------------------------------------------------------
// バインドする(ポート番号を指定する簡単バージョンだけ準備)
//-----------------------------------------------------------------------------
bool kjm::baseSocket::bindSocket(unsigned short port) {
	struct sockaddr_in me;
	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.S_un.S_addr = INADDR_ANY;
	me.sin_port = htons(port);
	bool ret = (bind(this->m_sock, (sockaddr*)&me, sizeof(me)) == 0) ? true : false;
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//-----------------------------------------------------------------------------
// 受信する
//-----------------------------------------------------------------------------
int kjm::baseSocket::recvfrom(std::vector<char>& buf, sockaddr_in& from) {
	int fromlen = sizeof(from);
	buf.resize(8192);
	int ret = ::recvfrom(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&from, &fromlen);
	this->m_lastError = ::WSAGetLastError();
	if (ret > 0) {
		buf.resize(ret);
	}
	return ret;
}

//-----------------------------------------------------------------------------
// 受信する
//-----------------------------------------------------------------------------
int kjm::baseSocket::recv(std::vector<char>& buf) {
	buf.resize(8192);
	int ret = ::recv(this->m_sock, &buf[0], buf.size(), 0);
	this->m_lastError = ::WSAGetLastError();
	if (ret > 0) {
		buf.resize(ret);
	}
	return ret;
}

//-----------------------------------------------------------------------------
// 受信する
//-----------------------------------------------------------------------------
void kjm::baseSocket::recvAll(std::vector<char>& buf) {
	std::vector<char> recvBuf;
	while (this->recv(recvBuf) > 0) {
		buf.insert(buf.end(), recvBuf.begin(), recvBuf.end());
		if (this->isRecvAll(buf))
			break;
	}
}

//-----------------------------------------------------------------------------
// 全て受信したか判断する関数
//-----------------------------------------------------------------------------
bool kjm::baseSocket::isRecvAll(const std::vector<char>& buf) {
	return false;
}

//-----------------------------------------------------------------------------
// 送信する
//-----------------------------------------------------------------------------
int kjm::baseSocket::sendto(const std::vector<char>& buf, const sockaddr_in& to) {
	int ret = ::sendto(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&to, sizeof(to));
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//-----------------------------------------------------------------------------
// 送信する
//-----------------------------------------------------------------------------
int kjm::baseSocket::send(const char* pStr) {
	int ret = ::send(this->m_sock, pStr, strlen(pStr), 0);
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//-----------------------------------------------------------------------------
// 切断する
//-----------------------------------------------------------------------------
int kjm::baseSocket::shutdown(int how) {
	int ret = ::shutdown(this->m_sock, how);
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//-----------------------------------------------------------------------------
// 優雅に切断する
//-----------------------------------------------------------------------------
void kjm::baseSocket::gracefulShutdown() {
	std::vector<char> buf;
	this->shutdown(SD_SEND);
	while (this->recv(buf) > 0)
		;
	this->closeSocket();
}

//-----------------------------------------------------------------------------
// ソケットを閉じる
//-----------------------------------------------------------------------------
bool kjm::baseSocket::closeSocket() {
	bool result = true;

	if (this->m_sock != INVALID_SOCKET) {
		result = (closesocket(this->m_sock) == 0) ? true : false;
		this->m_lastError = ::WSAGetLastError();
		this->m_sock = INVALID_SOCKET;
	}

	return result;
}

//-----------------------------------------------------------------------------
// Winsock初期化
//-----------------------------------------------------------------------------
int kjm::baseSocket::startup() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 0), &data);
}

//=============================================================================
// タスクトレイアイコンクラス
//=============================================================================
//-----------------------------------------------------------------------------
// デフォルト・コンストラクタ
//-----------------------------------------------------------------------------
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
BOOL kjm::BiNotifyIcon::Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip) {

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
BOOL kjm::BiNotifyIcon::Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip) {

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
void kjm::log_info(LPCTSTR signature, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);	// 可変個の引数の初期化

	std::vector<TCHAR> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsntprintf(&buffer[0], buffer.size(), format, marker) == -1);
	va_end(marker);
#if defined(USE_WSSLOG)
	Winssa_Log(signature, LOGLV_INFO, "%s", &buffer[0]);
#else
	std::basic_string<TCHAR> fname = 
		kjm::util::renameExtension(kjm::util::append(kjm::util::getTempPath(), kjm::util::GetModuleName()), _T(".log"));
	FILE* fp = _tfopen(fname.c_str(), _T("a+"));
	if (fp) {
		SYSTEMTIME st;
		::GetLocalTime(&st);
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d %s\n"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			&buffer[0]);
		fclose(fp);
	}
#endif
}


//---------------------------------------------------------------------
// エラーログを残す関数
//---------------------------------------------------------------------
void kjm::log_error(LPCTSTR signature, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);	// 可変個の引数の初期化

	std::vector<TCHAR> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsntprintf(&buffer[0], buffer.size(), format, marker) == -1);
	va_end(marker);
#if defined(USE_WSSLOG)
	Winssa_Log(signature, LOGLV_ERR, "%s", &buffer[0]);
#else
	std::basic_string<TCHAR> fname = 
		kjm::util::renameExtension(kjm::util::append(kjm::util::getTempPath(), kjm::util::GetModuleName()), _T(".log"));
	FILE* fp = _tfopen(fname.c_str(), _T("a+"));
	if (fp) {
		SYSTEMTIME st;
		::GetLocalTime(&st);
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d %s\n"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			&buffer[0]);
		fclose(fp);
	}
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


//-----------------------------------------------------------------------------
// ワークグループ名を取得する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::netapi32::getWorkgroupName() {
	std::basic_string<TCHAR> result;

	LPWKSTA_INFO_100 pBuf = NULL;
	NET_API_STATUS ret = ::NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf);
	if (ret == NERR_Success) {
#if defined(UNICODE)
		result = pBuf->wki100_langroup;
#else
		// LPWSTR で宣言してなければいけないヘッダが、LPTSTR で宣言しているため
		// LPWSTR にキャストが必要。OS のヘッダの不備。
		result = kjm::util::toAnsi((LPWSTR)pBuf->wki100_langroup);
#endif
	}

	if (pBuf != NULL) {
		::NetApiBufferFree(pBuf);
	}

	return result;
}

//=============================================================================
// win32apiのラッパー
//=============================================================================
//-----------------------------------------------------------------------------
// 文字列を置換する。VBのStrReplaceのような関数。
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::win32::str_replace(const std::basic_string<TCHAR>& src, LPCTSTR pFind, LPCTSTR pReplace) {
	std::basic_string<TCHAR> result = src;
	std::basic_string<TCHAR>::size_type pos = 0;
	while ((pos = result.find(pFind, pos)) != std::basic_string<TCHAR>::npos) {
		result.replace(pos, lstrlen(pFind), pReplace);
	}
	return result;
}

//-----------------------------------------------------------------------------
// エラーコードからシステムエラーメッセージを取得する。
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::win32::format_message(DWORD dwErrorCode) {

	std::basic_string<TCHAR> result;

	LPTSTR pBuffer = NULL;

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
		result = str_replace(std::basic_string<TCHAR>(pBuffer), _T("\r\n"), _T(""));

		::LocalFree(pBuffer);
	} else {
		TCHAR buf[1024];
		wsprintf(buf, _T("エラー %d (0x%08X) のメッセージは、ありません。"), dwErrorCode, dwErrorCode);
		result = buf;
	}

	return result;
}

//-----------------------------------------------------------------------------
// ワークエリアの中心にウィンドウを移動させる。
//-----------------------------------------------------------------------------
BOOL kjm::win32::MoveCenterWorkarea(HWND hwnd) {
	// ワークスエリアの矩形を取得
	RECT rcWorkarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkarea, 0);

	// 移動するウィンドウの矩形を取得
	RECT rcClient;
	GetWindowRect(hwnd, &rcClient);

	int nNewLeft = ((rcWorkarea.right - rcWorkarea.left) - (rcClient.right - rcClient.left)) / 2;
	int nNewTop = ((rcWorkarea.bottom - rcWorkarea.top) - (rcClient.bottom - rcClient.top)) / 2;

	// 移動予定の左上がWorkareaからはみ出ている場合は、座標を0にする
	if (nNewLeft < 0)	nNewLeft = 0;
	if (nNewTop < 0)	nNewTop = 0;

	return MoveWindow(hwnd, nNewLeft, nNewTop,
		(rcClient.right - rcClient.left), (rcClient.bottom - rcClient.left), FALSE);
}


//=============================================================================
// utility 関数クラス
//=============================================================================
//---------------------------------------------------------------------
// ホスト名または、IP文字列からIPアドレスを取得
//---------------------------------------------------------------------
DWORD kjm::util::get_ipaddr(LPCTSTR lpName) {
	DWORD addr = INADDR_ANY;

	if (lpName[0] == '\0')	return INADDR_ANY;

	// まず、IP文字列変換してみる。
#if defined(UNICODE)
	addr = inet_addr(kjm::util::toAnsi(lpName).c_str());
#else
	addr = inet_addr(lpName);
#endif
	if (addr == INADDR_NONE) {

		// DNSを引く
#if defined(UNICODE)
		struct hostent* hent = gethostbyname(kjm::util::toAnsi(lpName).c_str());
#else
		struct hostent* hent = gethostbyname(lpName);
#endif
		if (hent != NULL) {
			addr = *((DWORD *)(hent->h_addr_list[0]));
		} else{
			addr = INADDR_ANY;
		}
	}

	return	addr;
}

//---------------------------------------------------------------------
// ディレクトリを再帰的に削除する
//---------------------------------------------------------------------
bool kjm::util::remove_folder(LPCTSTR pszPath) {
	bool bRet = true;
	std::vector<TCHAR> wc(MAX_PATH);

	// 検索に必要なワイルドカードを作成
	PathAppend(lstrcpyn(&wc[0], pszPath, wc.size()), _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(&wc[0], &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((lstrcmp(data.cFileName, _T(".")) != 0) && (lstrcmp(data.cFileName, _T("..")) != 0)) {
					std::vector<TCHAR> child(MAX_PATH);

					// 子供フォルダを再帰的に削除する
					PathAppend(lstrcpyn(&child[0], pszPath, child.size()), data.cFileName);
					if ((bRet = kjm::util::remove_folder(&child[0])) == false) {
						break;
					}
				}
			} else {
				std::vector<TCHAR> fname(MAX_PATH);
				PathAppend(lstrcpyn(&fname[0], pszPath, fname.size()), data.cFileName);

				// 削除するファイルに readonly がついているときは落とす。
				DWORD dwAttr = GetFileAttributes(&fname[0]);
				if ((dwAttr != 0xffffffff) && ((dwAttr & FILE_ATTRIBUTE_READONLY) != 0)) {
					if (!SetFileAttributes(&fname[0], dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
						bRet = false;
						break;
					}
				}

				if (!DeleteFile(&fname[0])) {
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

//-----------------------------------------------------------------------------
// Unicode 文字列を ANSI 文字列に変換
//-----------------------------------------------------------------------------
std::string kjm::util::toAnsi(const std::wstring& src) {
	DWORD dwSizeNeed = ::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(dwSizeNeed);

	::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

//-----------------------------------------------------------------------------
// ANSI 文字列を Unicode 文字列に変換
//-----------------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string& src) {
	DWORD dwSizeNeed = ::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	std::vector<wchar_t> buf(dwSizeNeed);

	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

//-----------------------------------------------------------------------------
// 指定フォルダ以下の読み込み専用属性を外す
//-----------------------------------------------------------------------------
bool kjm::util::UnsetReadOnly(const std::basic_string<TCHAR>& path) {
	std::vector<std::basic_string<TCHAR> > targetFolder;
	targetFolder.push_back(path);

	bool result = true;

	while (!targetFolder.empty()) {
		std::basic_string<TCHAR> strTarget = targetFolder.back();
		targetFolder.pop_back();

		// ターゲット内のすべてのフォルダの読み込み専用を外す
		std::vector<std::basic_string<TCHAR> > files = kjm::directory::getFiles(strTarget);
		for (size_t i = 0; i < files.size(); i++) {
			if (!::SetFileAttributes(files[i].c_str(), ::GetFileAttributes(files[i].c_str()) & ~FILE_ATTRIBUTE_READONLY)) {
				result = false;
			}
		}

		// ターゲット内のサブフォルダを列挙し、次のターゲットとする。
		std::vector<std::basic_string<TCHAR> > dirs = kjm::directory::getSubFolders(strTarget);
		targetFolder.insert(targetFolder.end(), dirs.begin(), dirs.end());
	}

	return result;
}

//---------------------------------------------------------------------
// 安全な vsprintf
//---------------------------------------------------------------------
int kjm::util::vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr) {
	// バッファを指定されていないとき、256バイト割り当てておく
	if (buf.size() == 0) {
		buf.resize(256);
	}

	while (1) {

		// vsnprintf で文字列を作成。
		// 不十分なバッファ上にデータを作成している間、ループを繰り返す。
		int n = _vsntprintf(&buf[0], buf.size(), format, argptr);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}

	return lstrlen(&buf[0]);
}


//---------------------------------------------------------------------
// 安全な sprintf 
//
// [引数]
// 一番目の引数が、vector<TCHAR>への参照になっている以外、
// sprintf関数と同じです。
//---------------------------------------------------------------------
int kjm::util::sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);
	int result = kjm::util::vsprintf_vec(buf, format, marker);
	va_end(marker);
	return result;
}

//-----------------------------------------------------------------------------
// 文字列の分割
//-----------------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::util::split(const std::basic_string<TCHAR>& src, TCHAR c) {
	std::basic_string<TCHAR> work = src;
	std::vector<std::basic_string<TCHAR> > result;

	int pos;
	while ((pos = work.find(c)) != work.npos) {
		result.push_back(work.substr(0, pos));
		work = work.substr(pos + 1);
	}

	if (work.empty() == false) {
		result.push_back(work);
	}

	return result;
}

//-----------------------------------------------------------------------------
// 文字列に変換
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::toString(DWORD value) {
	TCHAR buf[80];
	return _ultot(value, buf, 10);
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

//-----------------------------------------------------------------------------
// ディレクトリ内のファイルを列挙する
//-----------------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::directory::getFiles(
	const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard) {

	std::vector<std::basic_string<TCHAR> > result;
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(kjm::PathString(path).Append(wildCard).c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return result;
	}

	do {
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			result.push_back(kjm::PathString(path).Append(findData.cFileName).getString());
		}
	} while (::FindNextFile(hFind, &findData) != FALSE);

	::FindClose(hFind);
	return result;
}

//-----------------------------------------------------------------------------
// ディレクトリ内のサブディレクトリを列挙する
//-----------------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::directory::getSubFolders(
	const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard) {

	std::vector<std::basic_string<TCHAR> > result;
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(kjm::PathString(path).Append(wildCard).c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return result;
	}

	do {
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			if (lstrcmp(findData.cFileName, _T(".")) != 0 && lstrcmp(findData.cFileName, _T("..")) != 0) {
				result.push_back(kjm::PathString(path).Append(findData.cFileName).getString());
			}
		}
	} while (::FindNextFile(hFind, &findData) != FALSE);

	::FindClose(hFind);
	return result;
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
std::basic_string<TCHAR> kjm::registry::get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const {
	assert(lpDefault != NULL);

	std::basic_string<TCHAR> result(lpDefault);	// デフォルト値で戻り値を作成しておく

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
	std::vector<TCHAR> buf(cbData);
	if (reg_query_value_ex(lpValueName, &dwType, (LPBYTE)&buf[0], &cbData) != ERROR_SUCCESS) {
		return result;	// 関数に失敗したのでデフォルト値を戻して終了
	}

	// データの種類に応じて、戻り値を作成
	switch (dwType) {
	case REG_SZ:
		result = &buf[0];
		break;
		
	case REG_EXPAND_SZ:
		result = win32::expand_environment_strings(&buf[0]);
		break;

	case REG_DWORD:
		{
			TCHAR temp[32];	// u_long の最大値を十分に表現できるサイズ
			result = _ultot(*(LPDWORD)&buf[0], temp, 10);
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

//---------------------------------------------------------------------
// レジストリを開く
//---------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// すでに開いているかもしれないので、閉じる
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hkey );

	return ret;
}


//---------------------------------------------------------------------
// レジストリを閉じる
//---------------------------------------------------------------------
LONG kjm::registry::RegCloseKey() {
	LONG ret = ERROR_SUCCESS;

	if ( m_hkey ) {
		ret = ::RegCloseKey( m_hkey );
		m_hkey = NULL;
	}

	return ret;
}


//---------------------------------------------------------------------
// レジストリから値を取得
//---------------------------------------------------------------------
LONG kjm::registry::RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data ) {

	LONG ret;
	DWORD cbData = 0;

	// 必要なバッファサイズを取得
	ret = ::RegQueryValueEx( m_hkey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// バッファを確保して正式に呼び出す
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hkey, lpValueName, lpReserved, lpType, &data[0], &cbData );
	}

	return ret;
}

//=============================================================================
// イベントログにアクセスするためのクラス
//=============================================================================
//-----------------------------------------------------------------------------
// イベントログをバックアップする
//-----------------------------------------------------------------------------
void kjm::event_log::backup(const kjm::_tstring& sourceName, const kjm::_tstring& backupFileName) {
	// 関数情報作成
	std::basic_ostringstream<TCHAR> ossfunc;
	ossfunc << _T("kjm::event_log::backup(\"") << sourceName << _T("\",\"") << backupFileName << _T("\")");

	// イベントログを開く
	HANDLE hEventLog = ::OpenEventLog(NULL, sourceName.c_str());
	if (hEventLog == NULL) {
		DWORD dwErr = ::GetLastError();
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}

	// イベントログをバックアップ
	DWORD dwErr = NOERROR;
	if (::BackupEventLog(hEventLog, backupFileName.c_str()) == FALSE) {
		dwErr = ::GetLastError();
	}
	::CloseEventLog(hEventLog);

	// バックアップ時にエラーが発生していたときは、例外を返す
	if (dwErr != NOERROR) {
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}
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

#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: '%s' を実行します。"),
			kjm::util::GetModuleName().c_str(), m_szCommandLine);
	}
#endif

	BOOL bRet = CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);

#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		if (bRet) {
			Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: 実行しました。"),
				kjm::util::GetModuleName().c_str(), m_szCommandLine);
		} else {
			DWORD dwErr = ::GetLastError();
			Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: error %d"),
				kjm::util::GetModuleName().c_str(), dwErr);
		}
	}
#endif
	return bRet;
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
// プロセスの終了コードを取得する
//-----------------------------------------------------------------------------
DWORD kjm::process::getExitCode() {
	DWORD exitCode = 0;
	::GetExitCodeProcess(this->m_pi.hProcess, &exitCode);
	return exitCode;
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

//=============================================================================
// コマンドライン解析に渡すオプション情報構造体
//=============================================================================
//-----------------------------------------------------------------------------
// 代入演算子
//-----------------------------------------------------------------------------
kjm::optionInfo& kjm::optionInfo::operator=(const kjm::optionInfo& rhs) {
	if (this == &rhs) return *this;
	m_longOption = rhs.m_longOption;
	m_shortOption = rhs.m_shortOption;
	m_optionArgInfo = rhs.m_optionArgInfo;
	return *this;
}

//=============================================================================
// コマンドライン解析クラス
//=============================================================================
//-----------------------------------------------------------------------------
// コマンドライン解析
//-----------------------------------------------------------------------------
int kjm::cmdLine::parse(int argc, TCHAR** argv, const optionInfoList& opts) {

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];	// オプション文字

			kjm::optionInfo oi(std::basic_string<TCHAR>(1, c), std::basic_string<TCHAR>(1, c), kjm::optional_argument);
#if _MSC_VER <= 1200	// VC++6以下
			// 検索のためだけの関数オブジェクトが必要になってしまう。
			// 本来の言語仕様では、このようなクラスは不要。
			optionInfoList::const_iterator ci = 
				std::find_if(opts.begin(), opts.end(), optionInfoShortNameFinder(std::basic_string<TCHAR>(1, c)));
#elif _MSC_VER <= 1500	// VC++2008
			// VC++2008では、bind2ndに指定する関数は、引数が参照になっていてはいけないため、
			// ポインタを渡す関数を特別に作成して使用している。
			// 本来の言語仕様では、このようなクラスは不要。
			optionInfoList::const_iterator ci = 
				std::find_if(opts.begin(), opts.end(),
				bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption_P), &std::basic_string<TCHAR>(1, c)));
#else	// VC++2010以降
			optionInfoList::const_iterator ci = 
				std::find_if(opts.begin(), opts.end(),
				bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption), std::basic_string<TCHAR>(1, c)));
#endif
			if (ci != opts.end()) {
				oi = *ci;
			}

			if (oi.m_optionArgInfo == kjm::no_argument) {
				// オプションには引数が無い場合
				this->m_options[std::basic_string<TCHAR>(1, c)] = _T("");
			} else if (oi.m_optionArgInfo == kjm::required_argument) {
				if (argv[i][2] != 0) {
					// オプションの引数が続いている場合
					this->m_options[std::basic_string<TCHAR>(1, c)] = &argv[i][2];
				} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
					// オプションの引数が次の argv に続いている場合
					this->m_options[std::basic_string<TCHAR>(1, c)] = argv[++i];
				} else {
					// どうしよう・・・
					assert(0);
				}
			} else {
				if (argv[i][2] != 0) {
					// オプションの引数が続いている場合
					this->m_options[std::basic_string<TCHAR>(1, c)] = &argv[i][2];
				} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
					// オプションの引数が次の argv に続いている場合
					this->m_options[std::basic_string<TCHAR>(1, c)] = argv[++i];
				} else {
					// オプションには引数が無い場合
					this->m_options[std::basic_string<TCHAR>(1, c)] = _T("");
				}
			}
		} else {
			this->m_arguments.push_back(argv[i]);
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// オプションの引数を取得する
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::cmdLine::getOptArgument(const std::basic_string<TCHAR>& key) {
	return (this->hasOption(key) ? this->m_options[key] : _T(""));
}

//---------------------------------------------------------------------
// OpenEventLog/CloseEventLogを対にするクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// イベントログを開く
//---------------------------------------------------------------------
BOOL kjm::eventLog::openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) {
	// すでに開いているかもしれないので、閉じる
	closeEventLog();

	m_uncServerName = (lpUNCServerName) ? lpUNCServerName : _T("");
	m_sourceName = (lpSourceName) ? lpSourceName : _T("");

	m_hEventLog = ::OpenEventLog(lpUNCServerName, lpSourceName);
	
	return (m_hEventLog) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// 保存したイベントログを開く
//---------------------------------------------------------------------
BOOL kjm::eventLog::OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName ) {

	// すでに開いているかもしれないので、閉じる
	closeEventLog();

	m_uncServerName = ( lpUNCServerName ) ? lpUNCServerName : _T("");
	m_sourceName = ( lpSourceName ) ? lpSourceName : _T("");
	m_fileName = ( lpFileName ) ? lpFileName : _T("");

	m_hEventLog = ::OpenBackupEventLog( lpUNCServerName, lpFileName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
// イベントログを閉じる
//-----------------------------------------------------------------------------
BOOL kjm::eventLog::closeEventLog() {
	BOOL ret = TRUE;
	if (m_hEventLog) {
		ret = ::CloseEventLog(m_hEventLog);
		m_hEventLog = NULL;
	}
	return ret;
}


//-----------------------------------------------------------------------------
// イベントログを読む
//-----------------------------------------------------------------------------
BOOL kjm::eventLog::readEventLog(DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventLogRecord& record) {

	DWORD dwReadBytes = 0;
	DWORD dwNextSize = 0;
	std::vector<BYTE> buffer;

	// 極小のバッファを確保してサイズをもらう
	buffer.resize( 1 );

	BOOL bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		&buffer[0],
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	// バッファ不足エラー以外は、エラー終了
	if(!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		return bResult;
	}

	// バッファの再割り当て
	buffer.resize( dwNextSize );

	/* イベントログの読み取り */
	bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		&buffer[0],
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	record = kjm::eventLogRecord(m_sourceName.c_str(), (EVENTLOGRECORD*)&buffer[0]);

	return bResult;
}

//-----------------------------------------------------------------------------
// イベントログをバックアップする
//-----------------------------------------------------------------------------
void kjm::eventLog::backup(LPCTSTR sourceName, LPCTSTR backupFileName) {
	// 関数情報作成
	std::basic_ostringstream<TCHAR> ossfunc;
	ossfunc << _T("kjm::event_log::backup(\"") << sourceName << _T("\",\"") << backupFileName << _T("\")");

	// イベントログを開く
	HANDLE hEventLog = ::OpenEventLog(NULL, sourceName);
	if (hEventLog == NULL) {
		DWORD dwErr = ::GetLastError();
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}

	// イベントログをバックアップ
	DWORD dwErr = NOERROR;
	if (::BackupEventLog(hEventLog, backupFileName) == FALSE) {
		dwErr = ::GetLastError();
	}
	::CloseEventLog(hEventLog);

	// バックアップ時にエラーが発生していたときは、例外を返す
	if (dwErr != NOERROR) {
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}
}

//---------------------------------------------------------------------
// 1レコード分のeventlogをあらわすクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------
kjm::eventLogRecord& kjm::eventLogRecord::operator= ( const kjm::eventLogRecord& src ) {
	m_source_name = src.m_source_name;
	m_raw_record = src.m_raw_record;

	return *this;
}


//-----------------------------------------------------------------------------
// イベントログの生情報を元にするコンストラクタ
//-----------------------------------------------------------------------------
kjm::eventLogRecord::eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src) : m_source_name(lpSourceName) {
	// 生情報のコピー
	m_raw_record.resize(src->Length);
	memmove(&m_raw_record[0], src, src->Length);
}

//-----------------------------------------------------------------------------
// 埋込み文字列の取得
//-----------------------------------------------------------------------------
int kjm::eventLogRecord::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<std::basic_string<TCHAR> >& strings) {
	if(pBuf->NumStrings == 0) {
		return strings.size();
	}

	// 引数リストを取得
	LPTSTR cp = (LPTSTR)((LPBYTE)pBuf + pBuf->StringOffset);

	for (int i = 0; i < pBuf->NumStrings; i++) {
		strings.push_back(cp);
		cp += lstrlen(cp) + 1;
	}

	return strings.size();
}

//-----------------------------------------------------------------------------
// ソース名からモジュール名を取得
//-----------------------------------------------------------------------------
BOOL kjm::eventLogRecord::GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName) {
	DWORD lResult;
	std::vector<BYTE> ModuleName;
	BOOL bReturn = FALSE;

	// Applicationログ用のレジストリキーをオープン
	kjm::registry appKey;
	
	std::basic_string<TCHAR> key = std::basic_string<TCHAR>(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\"));
	key += m_source_name;

	lResult = appKey.RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key.c_str(),
		0,
		KEY_READ);

	if ( lResult == ERROR_SUCCESS ) {

		// ソースの情報が格納されているレジストリをオープン

		kjm::registry sourceKey;

		lResult = sourceKey.RegOpenKeyEx(
			appKey.getKey(),
			SourceName,
			0,
			KEY_READ);

		if ( lResult == ERROR_SUCCESS ) {

			// ソースモジュール名を取得
			lResult = sourceKey.RegQueryValueEx(
				_T("EventMessageFile"),
				NULL,
				NULL,
				ModuleName);

			if ( lResult == ERROR_SUCCESS ) {

				// 環境変数を展開
				kjm::util::expandEnvironmentStrings( (LPCTSTR)&ModuleName[0], ExpandedName );

				/* 正常終了 */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

//-----------------------------------------------------------------------------
// 表示メッセージの作成
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::eventLogRecord::DispMessage(
	const std::basic_string<TCHAR>& SourceName, LPCTSTR EntryName, const std::vector<std::basic_string<TCHAR> >& strings, DWORD MessageId) {

	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	TCHAR* oneModuleName;
	DWORD dwRet;

	/* ソースモジュール名を取得 */	
	bResult = GetModuleNameFromSourceName(SourceName.c_str(), EntryName, SourceModuleName);
	if(!bResult) return std::basic_string<TCHAR>(_T(""));

	oneModuleName = _tcstok( &SourceModuleName[0], _T(";") );
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// ソースモジュールをロード
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			std::vector<LPCTSTR> Args;
			for (size_t i = 0; i < strings.size(); i++) {
				Args.push_back(strings[i].c_str());
			}

			/* メッセージを作成 */
			dwRet = kjm::util::formatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
				sourceModule.getInstance() ,
				MessageId,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				message,
				(va_list *)(Args.empty() ? NULL : &Args[0]));


			/* 正常終了 */
			if ( dwRet != 0 ) {
				bReturn = TRUE;
				break;
			}
		}

		oneModuleName = _tcstok( NULL, _T(";") );
	}

	if(message.size() > 0) {
		return std::basic_string<TCHAR>(&message[0]);
	}

	return _T("");
}

//-----------------------------------------------------------------------------
// イベントログに記録された文字列をとりだす
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::eventLogRecord::EventIDText() {
	std::vector<std::basic_string<TCHAR> > strings;
	GetArgs(getRaw(), strings);

	return DispMessage(get_SourceName(), _T("EventMessageFile"), strings, get_EventID());
}

//-----------------------------------------------------------------------------
// イベントに記録されたコンピュータ名を取得
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::eventLogRecord::ComputerName() {
	LPCTSTR cp = (LPCTSTR)(&m_raw_record[0] + sizeof(EVENTLOGRECORD));	// SourceName の先頭に移動
	return cp + (lstrlen(cp) + 1);	// SourceName の次の文字列がコンピュータ名
}

std::vector<BYTE> kjm::eventLogRecord::Data() {

	std::vector<BYTE> data( DataLength() );

	for ( size_t i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//---------------------------------------------------------------------
// LoadLibraryEx/FreeLibraryを対にするクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ライブラリのロード
//---------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// すでにロードしているかもしれないので、開放
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// ライブラリの開放
//---------------------------------------------------------------------
BOOL kjm::library::FreeLibrary() {
	BOOL ret = TRUE;

	if ( m_hLibModule ) {
		ret = ::FreeLibrary( m_hLibModule );
		m_hLibModule = NULL;
	}

	return ret;
}


//=============================================================================
// 時間を簡単に扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::time::strftime(LPCTSTR format) {
	TCHAR buf[ 512 ];
	::_tcsftime( buf, sizeof( buf ), format, localtime( &m_time ) );
	return std::basic_string<TCHAR>( buf );
}

//-----------------------------------------------------------------------------
// 文字列で時間を指定するコンストラクタ
//-----------------------------------------------------------------------------
void kjm::time::parse(LPCTSTR time_text) {

	int y, m, d, h, n, s;
	int ret = _stscanf(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s );

	tm t;
	memset( &t, 0, sizeof( t ) );

	t.tm_year = y - 1900;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = h;
	t.tm_min = n;
	t.tm_sec = s;

	m_time = mktime( &t );
}

//-----------------------------------------------------------------------------
// 秒の加減算
//-----------------------------------------------------------------------------
kjm::time& kjm::time::add_second( int sec ) {

	tm tm_work = *localtime( &m_time );

	tm_work.tm_sec += sec;

	m_time = mktime( &tm_work );

	return *this;
}

//-----------------------------------------------------------------------------
// kjm::timeクラスをファイルに保存
//-----------------------------------------------------------------------------
bool kjm::time::save(FILE* fp) {
	return (fwrite(&m_time, sizeof(m_time), 1, fp) == 1);
}

//-----------------------------------------------------------------------------
// kjm::timeクラスをファイルから読込
//-----------------------------------------------------------------------------
bool kjm::time::load(FILE* fp) {
	return (fread(&m_time, sizeof(m_time), 1, fp) == 1);
}


//=============================================================================
// 時差を扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// 代入演算子
//-----------------------------------------------------------------------------
kjm::timeSpan& kjm::timeSpan::operator =(const kjm::timeSpan &rhs) {
	if (this == &rhs) return *this;
	m_span = rhs.m_span;
	return *this;
}


//=============================================================================
// 時間を簡単に扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// 代入演算子
//-----------------------------------------------------------------------------
kjm::dateTime& kjm::dateTime::operator =(const kjm::dateTime &rhs) {
	if (this == &rhs) return *this;
	m_ft = rhs.m_ft;
	return *this;
}

//-----------------------------------------------------------------------------
// ローカル現在日時を持つdateTimeオブジェクトを生成
//-----------------------------------------------------------------------------
kjm::dateTime kjm::dateTime::now() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	return kjm::dateTime(ft, DTK_LOCAL);
}

//-----------------------------------------------------------------------------
// UTC現在日時を持つdateTimeオブジェクトを生成
//-----------------------------------------------------------------------------
kjm::dateTime kjm::dateTime::utcNow() {
	SYSTEMTIME st;
	GetSystemTime(&st);

	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	return kjm::dateTime(ft, DTK_UTC);
}

//-----------------------------------------------------------------------------
// ローカル時間に変更
//-----------------------------------------------------------------------------
kjm::dateTime kjm::dateTime::toLocalTime() {
	dateTime result;

	if (this->m_kind == DTK_LOCAL) {
		result = *this;
	} else {
		FILETIME ftLocal;
		FileTimeToLocalFileTime(&this->m_ft, &ftLocal);
		result = kjm::dateTime(ftLocal, DTK_LOCAL);
	}
	return result;
}

//-----------------------------------------------------------------------------
// 日付をフォーマットして出力
//-----------------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toLongDateString() {
	SYSTEMTIME st;
	FileTimeToSystemTime(&this->m_ft, &st);
	return kjm::util::sprintf_str(_T("%d年%d月%d日"), st.wYear, st.wMonth, st.wDay);
}

//-----------------------------------------------------------------------------
// 時間をフォーマットして出力
//-----------------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toLongTimeString() {
	SYSTEMTIME st;
	FileTimeToSystemTime(&this->m_ft, &st);
	return kjm::util::sprintf_str(_T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
}


//=============================================================================
// ファイルを扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// 最終更新日時を取得(UTC)
//-----------------------------------------------------------------------------
kjm::dateTime kjm::file::getLastWriteTimeUtc(const kjm::_tstring &path) {
	HANDLE hFile = ::CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return kjm::dateTime();
	}

	FILETIME ft;
	::GetFileTime(hFile, NULL, NULL, &ft);
	::CloseHandle(hFile);
	return kjm::dateTime(ft, DTK_UTC);
}

//=============================================================================
// iniファイルを処理するクラス
//=============================================================================
//-----------------------------------------------------------------------------
// セクション一覧を取得
//-----------------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::iniFile::getSectionNames() const {
	assert(!m_fileName.empty());

	std::vector<std::basic_string<TCHAR> > result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileSectionNames(&work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(std::basic_string<TCHAR>(sp, ep));
		sp = ep + 1;
	}

	return result;
}


//---------------------------------------------------------------------------------------
// セクション内のキー一覧を取得
//---------------------------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::iniFile::getKeyNames(const std::basic_string<TCHAR>& strSectionName) const {
	assert(!m_fileName.empty());

	std::vector<std::basic_string<TCHAR> > result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSectionName.c_str(), NULL, _T(""), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(std::basic_string<TCHAR>(sp, ep));
		sp = ep + 1;
	}

	return result;
}


//---------------------------------------------------------------------------------------
// iniファイルから値を取得
//---------------------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::iniFile::getString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strDefault, BOOL *pbUseDefault/*=NULL*/) const {
	assert(!m_fileName.empty());

	std::vector<TCHAR> work;
	std::basic_string<TCHAR> value1;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 1);

	value1 = &work[0];

	// デフォルトを使ったかどうかを判定する
	if (pbUseDefault) {
		std::basic_string<TCHAR> strDefault2 = _T("x") + strDefault;
		std::basic_string<TCHAR> value2 = getString(strSection, strKey, strDefault2);

		*pbUseDefault = (value1 != value2) ? TRUE : FALSE;
	}

	return &work[0];
}


//---------------------------------------------------------------------------------------
// iniファイルに値を書き込む
//---------------------------------------------------------------------------------------
BOOL kjm::iniFile::writeString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strValue) {
	assert(!m_fileName.empty());

	return ::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), this->m_fileName.c_str());
}


//---------------------------------------------------------------------------------------
// 別のiniファイルの内容をマージする
//---------------------------------------------------------------------------------------
void kjm::iniFile::mergeIniFile(const iniFile& newIniFile) {
	assert(!m_fileName.empty());

	// マージするINIファイルから、全セクションを読み込む
	std::vector<std::basic_string<TCHAR> > sections = newIniFile.getSectionNames();
	for (size_t i = 0; i < sections.size(); i++) {

		// マージするINIファイルのセクションから、全キーを読み込む
		std::vector<std::basic_string<TCHAR> > newKeys = newIniFile.getKeyNames(sections[i]);

		// マージされるINI(この)ファイルのセクションからも全キーを読み込む
		std::vector<std::basic_string<TCHAR> > orgKeys = this->getKeyNames(sections[i]);

		for (size_t j = 0; j < newKeys.size(); j++) {

			// マージされるINI(この)ファイルに同じキーが存在するかどうか調べる
			// マージされるINI(この)ファイルにキーが存在しない場合、新しいINIファイルからキーの値を書き込む
			// (マージされるINI(この)ファイルに同じキーが存在するときは何もしないので、内容が保障される感じ)

			std::vector<std::basic_string<TCHAR> >::iterator p = std::find(orgKeys.begin(), orgKeys.end(), newKeys[j]);
			if (p == orgKeys.end()) {
				std::basic_string<TCHAR> value = newIniFile.getString(sections[i], newKeys[j], _T(""));
				this->writeString(sections[i], newKeys[j], value);

				///if (g_kjmlog) g_kjmlog->writeInfo(_T("[%s] %s=%s をマージ。"), sections[i].c_str(), newKeys[j].c_str(), value.c_str());
			}
		}
	}
}
