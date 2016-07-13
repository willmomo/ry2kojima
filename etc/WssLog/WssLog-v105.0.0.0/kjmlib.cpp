//---------------------------------------------------------------------
// kjmlib : Version 0.2011.6.7
//
// プリコンパイル済みヘッダを使用しないように設定してください。
//
// r.kojima
//---------------------------------------------------------------------
#include "kjmlib.h"

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

//-----------------------------------------------------------------------------
// パス文字列にパスを追加
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathAppend(::lstrcpy(&work.at(0), path.c_str()), more.c_str());
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
	while (_vsntprintf(&buffer.at(0), buffer.size(), format, marker) == -1) {
		buffer.resize(buffer.size() * 2);
	}
	va_end(marker);
	return &buffer.at(0);
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

	m_fr.open(pszFileName);
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
	for (int l = 0; l < cols.size(); l++) {
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
// 受信タイムアウトの設定
//-----------------------------------------------------------------------------
bool kjm::baseSocket::setRecvTimeout(int msTimeout) {
	return (setsockopt(this->m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&msTimeout, sizeof(msTimeout)) == 0) ? true : false;
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
	return (bind(this->m_sock, (sockaddr*)&me, sizeof(me)) == 0) ? true : false;
}

//-----------------------------------------------------------------------------
// 受信する
//-----------------------------------------------------------------------------
int kjm::baseSocket::recvfrom(std::vector<char>& buf, sockaddr_in& from) {
	int fromlen = sizeof(from);
	buf.resize(8192);
	int ret = ::recvfrom(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&from, &fromlen);
	if (ret > 0) {
		buf.resize(ret);
	}
	return ret;
}

//-----------------------------------------------------------------------------
// 送信する
//-----------------------------------------------------------------------------
int kjm::baseSocket::sendto(const std::vector<char>& buf, const sockaddr_in& to) {
	return ::sendto(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&to, sizeof(to));
}

//-----------------------------------------------------------------------------
// ソケットを閉じる
//-----------------------------------------------------------------------------
bool kjm::baseSocket::closeSocket() {
	bool result = true;

	if (this->m_sock != INVALID_SOCKET) {
		result = (closesocket(this->m_sock) == 0) ? true : false;
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


//---------------------------------------------------------------------
// 文字列を置換する。VBのStrReplaceのような関数。
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::win32::str_replace(const std::basic_string<TCHAR>& src, LPCTSTR pFind, LPCTSTR pReplace) {
	std::basic_string<TCHAR> result = src;
	std::basic_string<TCHAR>::size_type pos = 0;
	while ((pos = result.find(pFind, pos)) != std::basic_string<TCHAR>::npos) {
		result.replace(pos, lstrlen(pFind), pReplace);
	}
	return result;
}


//---------------------------------------------------------------------
// エラーコードからシステムエラーメッセージを取得する。
//---------------------------------------------------------------------
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
// コマンドライン解析クラス
//=============================================================================
//-----------------------------------------------------------------------------
// コマンドライン解析
//-----------------------------------------------------------------------------
int kjm::cmdLine::parse(int argc, TCHAR** argv) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];	// オプション文字
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

