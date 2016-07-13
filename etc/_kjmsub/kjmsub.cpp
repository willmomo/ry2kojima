//-----------------------------------------------------------------------------
// kjmsub ver.0.2010.7.30
//-----------------------------------------------------------------------------
#include "kjmsub.h"

// kjmsub 内の動作ログを残すためのログオブジェクトを指定する。
// kjm::util::set_logObj を呼ぶことで、内部トレースをすることができるようになる。

static kjm::log* g_kjmlog = NULL;

//-----------------------------------------------------------------------------
// kjmsub が動作を記録するためのログオブジェクトを設定
//-----------------------------------------------------------------------------
void kjm::util::set_logObj(kjm::log* pLogObj) {
	g_kjmlog = pLogObj;
}

//-----------------------------------------------------------------------------
// __int64 を iostream に出力するための関数
//-----------------------------------------------------------------------------
#if _MSC_VER < 1500		// 少なくとも、vs2008からは、__int64を受け取るostreamが存在するので、無効にする。
_otstream& operator<<(_otstream& os, __int64 i) {
	TCHAR buf[20];
	_stprintf(buf, _T("%I64d"), i);
	os << buf;
	return os;
}
#endif


#if _MSC_VER < 1500
errno_t ctime_s(char* buffer, size_t numberOfElements, const time_t *time) {
	strcpy(buffer, ::ctime(time));
	return 0;
}

errno_t _wctime_s(wchar_t* buffer, size_t numberOfElements, const time_t *time) {
	wcscpy(buffer, ::_wctime(time));
	return 0;
}

char* strtok_s(char* strToken, const char* strDelimit, char** context) {
	return strtok(strToken, strDelimit);
}

wchar_t* wcstok_s(wchar_t* strToken, const wchar_t* strDelimit, wchar_t** context) {
	return wcstok(strToken, strDelimit);
}

int _vsnprintf_s(char *buffer, size_t sizeOfBuffer, size_t count, const char *format, va_list argptr) {
	return _vsnprintf(buffer, sizeOfBuffer, format, argptr);
}

int _vsnwprintf_s(wchar_t *buffer, size_t sizeOfBuffer, size_t count, const wchar_t *format, va_list argptr) {
	return _vsnwprintf(buffer, sizeOfBuffer, format, argptr);
}

errno_t localtime_s(struct tm* _tm, const time_t *time) {
	*_tm = *localtime(time);
	return 0;
}

errno_t fopen_s(FILE** pFile, const char *filename, const char *mode) {
//	*pFile = fopen(filename, mode);
	return (*pFile != NULL) ? 0 : errno;
}

errno_t _wfopen_s(FILE** pFile, const wchar_t *filename, const wchar_t *mode) {
//	*pFile = _wfopen(filename, mode);
	return (*pFile != NULL) ? 0 : errno;
}
#endif


//-----------------------------------------------------------------------------
//	ディレクトリ内のファイル一覧を取得
//-----------------------------------------------------------------------------
bool kjm::dir(LPCTSTR pszPath, LPCTSTR pszWildcard, std::vector<_tstring>& result) {
	std::vector<_tstring> tempResult;
	TCHAR wc[MAX_PATH];

	PathAppend(lstrcpy(wc, pszPath), pszWildcard);

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc, &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	do {
		tempResult.push_back(data.cFileName);
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);

	result.insert(result.end(), tempResult.begin(), tempResult.end());

	return true;
}

//=============================================================================
// コマンドライン引数解析
//
// @arg  hasArgShortOpts  引数を持つことがあるショートオプションを列挙した文字列へのポインタ
//=============================================================================
int kjm::commandLine::parse(int argc, TCHAR** argv, TCHAR* hasArgShortOpts) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			// オプションとして処理する
			if (argv[i][1] == '/' || argv[i][1] == '-') {
				// ロングオプションとして処理する

				// = を探す。あれば、引数を取るオプション
				TCHAR* p = std::find(&argv[i][2], &argv[i][lstrlen(argv[i])], '=');
				if (p != &argv[i][lstrlen(argv[i])]) {
					m_optList[_tstring(&argv[i][2], p)] = (p + 1);
				} else {
					m_optList[&argv[i][2]] = _T("");
				}
			} else {
				// ショートオプションとして処理する

				// 引数を取るオプションかどうか調べる
				if (std::find(&hasArgShortOpts[0], &hasArgShortOpts[lstrlen(hasArgShortOpts)], argv[i][1]) != &hasArgShortOpts[lstrlen(hasArgShortOpts)]) {
					// (あれば)引数を取り出す
					if (argv[i][2] != '\0') {
						// オプションに続く文字列があるので、それを、オプションの引数として採用する
						m_optList[_tstring(1, argv[i][1])] = &argv[i][2];
					} else if (((i + 1) < argc) && (argv[i + 1][0] != '-' && argv[i + 1][0] != '/')) {
						// オプションの次のオプションがオプション文字列で始まっていないとき、
						// それを、オプションの引数として採用する
						m_optList[_tstring(1, argv[i][1])] = argv[i + 1];
						++i;
					} else {
						// オプションの引数は無かった
						m_optList[_tstring(1, argv[i][1])] = _T("");
					}
				} else {
					m_optList[_tstring(1, argv[i][1])] = _T("");
				}
			}
		} else {
			// オプション以外の引数として処理する
			m_argList.push_back(argv[i]);
		}
	}

	return 0;
}

//=============================================================================
// イベントログを管理するクラス(class eventLog)
//=============================================================================
//-----------------------------------------------------------------------------
// イベントログを開く
//-----------------------------------------------------------------------------
BOOL kjm::eventLog::openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) {
	// すでに開いているかもしれないので、閉じる
	closeEventLog();

	m_uncServerName = (lpUNCServerName) ? lpUNCServerName : _T("");
	m_sourceName = (lpSourceName) ? lpSourceName : _T("");

	m_hEventLog = ::OpenEventLog(lpUNCServerName, lpSourceName);
	
	return (m_hEventLog) ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
// 保存したイベントログを開く
//-----------------------------------------------------------------------------
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


//=============================================================================
// 1レコード分のeventlogをあらわすクラス(class eventLogRecord)
//=============================================================================
//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
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
int kjm::eventLogRecord::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<LPTSTR>& args) {
	args.clear();

	LPTSTR cp;
	WORD ArgCount;

	if(pBuf->NumStrings == 0) return args.size();

	/* 引数リストを取得 */
	args.resize( pBuf->NumStrings );
	cp = (LPTSTR)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		args[ArgCount] = cp;
		cp += lstrlen(cp) + 1;
	}
	return args.size();
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
	
	_tstring key = _tstring(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\"));
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
				///////kjm::util::expandEnvironmentStrings((LPCTSTR)&ModuleName[0], ExpandedName );

				/* 正常終了 */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

//-----------------------------------------------------------------------------
// メッセージの表示
//-----------------------------------------------------------------------------
_tstring kjm::eventLogRecord::DispMessage(LPCTSTR SourceName, LPCTSTR EntryName, TCHAR **Args, DWORD MessageId) {
	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	LPTSTR oneModuleName;
	DWORD dwRet;

	/* ソースモジュール名を取得 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) return _tstring(_T(""));

	TCHAR* context = NULL;
	oneModuleName = _tcstok_s(&SourceModuleName[0], _T(";"), &context);
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// ソースモジュールをロード
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			/* メッセージを作成 */
			dwRet = kjm::util::formatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
				sourceModule.getInstance() ,
				MessageId,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				message,
				(va_list *)Args);


			/* 正常終了 */
			if ( dwRet != 0 ) {
				bReturn = TRUE;
				break;
			}
		}

		oneModuleName = _tcstok_s(NULL, _T(";"), &context);
	}

	if(message.size() > 0) return _tstring(&message[0]);
	else {
		TCHAR buf[32];
		wsprintf(buf, _T("(%d)\n"), MessageId);
		return _tstring( buf );
	}
}

//-----------------------------------------------------------------------------
// イベントログのコンピュータ名を取得
//-----------------------------------------------------------------------------
_tstring kjm::eventLogRecord::ComputerName() {
	LPTSTR cp = (LPTSTR)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	cp += lstrlen( cp ) + 1;		// SourceName分ポインタを進める

	return _tstring( cp );
}

//-----------------------------------------------------------------------------
// イベントログのデータを取得
//-----------------------------------------------------------------------------
std::vector<BYTE> kjm::eventLogRecord::Data() {

	std::vector<BYTE> data( DataLength() );

	for (std::vector<BYTE>::size_type i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//=============================================================================
// ファイルの一般的な操作を提供するクラス
//=============================================================================
//-----------------------------------------------------------------------------
// ファイルを削除する
//-----------------------------------------------------------------------------
void kjm::file::deleteFile(const _tstring& path, bool force) {
	// 「強制削除」が指定されているときは、とりあえず、読み込み専用属性を落とす
	if (force) {
		kjm::file::setAttributes(path, (kjm::file::getAttributes(path) & ~FILE_ATTRIBUTE_READONLY));
	}

	// 実際にファイルを削除する
	if (::DeleteFile(path.c_str()) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("ファイル '") << path << _T("' が削除できません。") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}

//-----------------------------------------------------------------------------
// ファイルの属性を取得する。
//-----------------------------------------------------------------------------
DWORD kjm::file::getAttributes(const _tstring& path) {
	DWORD dwAttr = ::GetFileAttributes(path.c_str());
	if (dwAttr == INVALID_FILE_ATTRIBUTES) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("ファイル '") << path << _T("' の属性が取得できません。") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}

	return dwAttr;
}

//-----------------------------------------------------------------------------
// ファイルの属性を設定する。
//-----------------------------------------------------------------------------
void kjm::file::setAttributes(const _tstring& path, DWORD fa) {
	if (::SetFileAttributes(path.c_str(), fa) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("ファイル '") << path << _T("' の属性を設定できません。") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}


//=============================================================================
// ファイルの一般的な操作を提供するクラス
//=============================================================================
//-----------------------------------------------------------------------------
// ディレクトリを再帰的に作成する
//-----------------------------------------------------------------------------
void kjm::directory::createDirectory(const _tstring& path) {
	if (::PathIsRoot(path.c_str()))		return;
	if (::PathFileExists(path.c_str()))	return;

	// 親ディレクトリの作成を試みる。
	std::vector<TCHAR> parent(MAX_PATH);
	if (::PathRemoveFileSpec(lstrcpy(&parent[0], path.c_str())) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("フォルダ '") << path << _T("' の親ディレクトリを作成できませんでした。") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
	kjm::directory::createDirectory(&parent[0]);

	// 指定されたディレクトリの作成を試みえる。
	if (::CreateDirectory(path.c_str(), NULL) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("フォルダ '") << path << _T("' の作成に失敗しました。") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}


//=============================================================================
// ディレクトリ を表現するクラス
//=============================================================================
//-----------------------------------------------------------------------------
// ディレクトリ直下のファイル一覧を取得
//-----------------------------------------------------------------------------
std::vector<kjm::fileInfo> kjm::folderInfo::getFiles(const _tstring& strWC/* = _T("*.*")*/) {
	std::vector<kjm::fileInfo> result;

	findFile ff;
	if (ff.findFirstFile(kjm::path::append(m_fullPath, strWC))) {
		do {
			if (!ff.isDirectory()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);
				result.push_back(kjm::fileInfo(m_fullPath, findData));
			}
		} while (ff.findNextFile());
		ff.findClose();
	}

	return result;
}

//-----------------------------------------------------------------------------
// ディレクトリ直下のディレクトリ一覧を取得
//-----------------------------------------------------------------------------
std::vector<kjm::folderInfo> kjm::folderInfo::getSubFolders() {
	std::vector<kjm::folderInfo> result;

	findFile ff;
	if (ff.findFirstFile(path::append(m_fullPath, _T("*.*")))) {
		do {
			if (ff.isDirectory() && !ff.isDots()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);
				result.push_back(kjm::folderInfo(m_fullPath, findData));
			}
		} while (ff.findNextFile());
		ff.findClose();
	}

	return result;
};


//=============================================================================
// iniファイルを処理するクラス
//=============================================================================
//-----------------------------------------------------------------------------
// セクション一覧を取得
//-----------------------------------------------------------------------------
std::vector<_tstring> kjm::iniFile::getSectionNames() const {
	assert(!m_fileName.empty());

	std::vector<_tstring> result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileSectionNames(&work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(_tstring(sp, ep));
		sp = ep + 1;
	}

	return result;
}

//-----------------------------------------------------------------------------
// セクション内のキー一覧を取得
//-----------------------------------------------------------------------------
std::vector<_tstring> kjm::iniFile::getKeyNames(const _tstring& strSectionName) const {
	assert(!m_fileName.empty());

	std::vector<_tstring> result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSectionName.c_str(), NULL, _T(""), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(_tstring(sp, ep));
		sp = ep + 1;
	}

	return result;
}

//-----------------------------------------------------------------------------
// iniファイルから値を取得
//-----------------------------------------------------------------------------
_tstring kjm::iniFile::getString(const _tstring& strSection, const _tstring& strKey, const _tstring& strDefault, BOOL *pbUseDefault/*=NULL*/) const {
	assert(!m_fileName.empty());

	std::vector<TCHAR> work;
	_tstring value1;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 1);

	value1 = &work[0];

	// デフォルトを使ったかどうかを判定する
	if (pbUseDefault) {
		_tstring strDefault2 = _T("x") + strDefault;
		_tstring value2 = getString(strSection, strKey, strDefault2);

		*pbUseDefault = (value1 != value2) ? TRUE : FALSE;
	}

	return &work[0];
}

//-----------------------------------------------------------------------------
// iniファイルに値を書き込む
//-----------------------------------------------------------------------------
BOOL kjm::iniFile::writeString(const _tstring& strSection, const _tstring& strKey, const _tstring& strValue) {
	assert(!m_fileName.empty());

	return ::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), this->m_fileName.c_str());
}

//-----------------------------------------------------------------------------
// 別のiniファイルの内容をマージする
//-----------------------------------------------------------------------------
void kjm::iniFile::mergeIniFile(const iniFile& newIniFile) {
	assert(!m_fileName.empty());

	// マージするINIファイルから、全セクションを読み込む
	std::vector<_tstring> sections = newIniFile.getSectionNames();
	for (std::vector<_tstring>::size_type i = 0; i < sections.size(); i++) {

		// マージするINIファイルのセクションから、全キーを読み込む
		std::vector<_tstring> newKeys = newIniFile.getKeyNames(sections[i]);

		// マージされるINI(この)ファイルのセクションからも全キーを読み込む
		std::vector<_tstring> orgKeys = this->getKeyNames(sections[i]);

		for (std::vector<_tstring>::size_type j = 0; j < newKeys.size(); j++) {

			// マージされるINI(この)ファイルに同じキーが存在するかどうか調べる
			// マージされるINI(この)ファイルにキーが存在しない場合、新しいINIファイルからキーの値を書き込む
			// (マージされるINI(この)ファイルに同じキーが存在するときは何もしないので、内容が保障される感じ)

			std::vector<_tstring>::iterator p = std::find(orgKeys.begin(), orgKeys.end(), newKeys[j]);
			if (p == orgKeys.end()) {
				_tstring value = newIniFile.getString(sections[i], newKeys[j], _T(""));
				this->writeString(sections[i], newKeys[j], value);

				if (g_kjmlog) g_kjmlog->writeInfo(_T("[%s] %s=%s をマージ。"), sections[i].c_str(), newKeys[j].c_str(), value.c_str());
			}
		}
	}
}


//=============================================================================
// LoadLibraryEx/FreeLibraryを対にするクラス
//=============================================================================
//-----------------------------------------------------------------------------
// ライブラリのロード
//-----------------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// すでにロードしているかもしれないので、開放
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// ライブラリの開放
//-----------------------------------------------------------------------------
BOOL kjm::library::FreeLibrary() {
	BOOL ret = TRUE;

	if ( m_hLibModule ) {
		ret = ::FreeLibrary( m_hLibModule );
		m_hLibModule = NULL;
	}

	return ret;
}


//=============================================================================
// 共通のログ処理
//=============================================================================
//---------------------------------------------------------------------
// デフォルトコンストラクタ
//---------------------------------------------------------------------
kjm::log::log() {

	// 識別子に空文字列を設定
	m_strIdent = _T("");

	// テンポラリパスを取得
	m_strLogPath = kjm::util::getTempPath();

	// ログファイルの基本になるファイル名部分を実行ファイル名から取得
	_tstring strFileSpec = kjm::path::findFileName(kjm::util::getModuleFileName());
	m_strPrefix = kjm::path::removeExtension(strFileSpec);

	// ファイル名を生成する
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
}

//-----------------------------------------------------------------------------
// 古いログの削除
//-----------------------------------------------------------------------------
void kjm::log::deleteOldLog() {
#if defined(USE_WSSLOG)
	WLDeleteOldLog();
#endif
}

//-----------------------------------------------------------------------------
// ログシステムの名前を返す
//-----------------------------------------------------------------------------
_tstring kjm::log::get_MyName() {
	_tstring result;
#if defined(USE_WSSLOG)
	char buf[MAX_PATH * 2];
	WLGetMyName(buf, MAX_PATH * 2);
	result = kjm::util::toUnicode(buf);
#else
	result = _T("(ローカル)");
#endif
	return result;
}

//-----------------------------------------------------------------------------
// ログシステムの情報をログに残す
//-----------------------------------------------------------------------------
void kjm::log::writeMyName() {
#if defined(USE_WSSLOG)
	char buf[MAX_PATH * 2];
	WLGetMyName(buf, MAX_PATH * 2);
	write(
		_T("利用ログシステム: WssLog [%s]"),
		kjm::util::toUnicode(buf).c_str()
	);
#else
	writeInfo(_T("利用ログシステム: Local  [%s]"), _T("local"));
#endif
}

//-----------------------------------------------------------------------------
// ログファイルの出力先を設定する
//-----------------------------------------------------------------------------
void kjm::log::set_log_path(const _tstring& strPath) {
#if defined(USE_WSSLOG)
	WLSetLogPath(kjm::util::toAnsi(strPath).c_str());
#else
	m_strLogPath = strPath;

	// 識別子に空文字列を設定
	m_strIdent = _T("");

	// ファイル名を生成する
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
#endif
}

//-----------------------------------------------------------------------------
// ログファイルの出力先を取得する
//-----------------------------------------------------------------------------
_tstring kjm::log::get_log_path() {
#if defined(USE_WSSLOG)
	char strPath[MAX_PATH];
	WLGetLogPath(strPath, MAX_PATH);
	return kjm::util::toUnicode(strPath);
#else
	return m_strLogPath;
#endif
}

//-----------------------------------------------------------------------------
// ログファイルのプレフィクスを設定する
//-----------------------------------------------------------------------------
void kjm::log::set_prefix(const _tstring& strPrefix) {
	m_strPrefix = strPrefix;

	// 識別子に空文字列を設定
	m_strIdent = _T("");

	// ファイル名を生成する
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
}

//-----------------------------------------------------------------------------
// 全ログ
//-----------------------------------------------------------------------------
void kjm::log::writeAll(LPCTSTR pszFormat, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_ALWAYS, "%s", kjm::util::toAnsi(&buf[0]).c_str());
#endif
}

//-----------------------------------------------------------------------------
// エラーログ
//-----------------------------------------------------------------------------
void kjm::log::writeError(LPCTSTR pszFormat, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_ERR, "%s", kjm::util::toAnsi(&buf[0]).c_str());
#endif
}

//-----------------------------------------------------------------------------
// 情報ログ
//-----------------------------------------------------------------------------
void kjm::log::writeInfo(LPCTSTR pszFormat, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_INFO, "%s", kjm::util::toAnsi(&buf[0]));
#endif
}

//-----------------------------------------------------------------------------
// ログの出力関数
//-----------------------------------------------------------------------------
void kjm::log::write(LPCTSTR format, ...) {
#ifdef USE_WSSLOG
	va_list marker;
	std::vector<TCHAR> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
		n = _vsntprintf(&work[0], work.size(), format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

	Winssa_Log(kjm::util::toAnsi(m_strIdent).c_str(), LOGLV_INFO, "%s", kjm::util::toAnsi(&work[0]).c_str());
#else
	va_list marker;
	va_start(marker, format);

	// 基本ファイル名から、実際のログファイル名を生成する
	_tstring fname = kjm::util::strReplace(m_strBaseFileName, _T("{YMD}"), kjm::dateTime::getLocalTime().toString_yyyymmdd() );

	FILE* fp = NULL;
	for (int i = 0; i < 100; i++) {
		fp = _tfsopen(fname.c_str(), _T("a+"), _SH_DENYWR);
		if (fp) break;
		Sleep(0);
	}

	if (fp) {
		_ftprintf(fp, _T("%s "), kjm::dateTime::getLocalTime().toString().c_str());
		_vftprintf(fp, format, marker);
		_ftprintf(fp, _T("\n"));

		fclose(fp);
	}

	va_end(marker);
#endif
}

//-----------------------------------------------------------------------------
// ログの出力関数(画面出力あり)
//-----------------------------------------------------------------------------
void kjm::log::writee(LPCTSTR format, ...) {

	va_list marker;
	std::vector<TCHAR> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
		n = _vsntprintf_s(&work[0], work.size(), _TRUNCATE, format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

	write(_T("%s"), &work[0]);

	// 標準出力にも出力
	_tprintf(_T("%s\n"), &work[0]);
}

//=============================================================================
// レジストリを管理するクラス(class registry)
//=============================================================================
//-----------------------------------------------------------------------------
// レジストリを開く
//-----------------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// すでに開いているかもしれないので、閉じる
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hKey );

	return ret;
}

//-----------------------------------------------------------------------------
// レジストリを閉じる
//-----------------------------------------------------------------------------
LONG kjm::registry::RegCloseKey() {
	LONG ret = ERROR_SUCCESS;

	if ( m_hKey ) {
		ret = ::RegCloseKey( m_hKey );
		m_hKey = NULL;
	}

	return ret;
}

//-----------------------------------------------------------------------------
// レジストリから値を取得
//-----------------------------------------------------------------------------
LONG kjm::registry::RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data ) {

	LONG ret;
	DWORD cbData = 0;

	// 必要なバッファサイズを取得
	ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// バッファを確保して正式に呼び出す
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, &data[0], &cbData );
	}

	return ret;
}



//=============================================================================
// 時間を簡単に扱うクラス
//=============================================================================
//-----------------------------------------------------------------------------
// 文字列で時間を指定するコンストラクタ
//-----------------------------------------------------------------------------
void kjm::dateTime::parse(LPCTSTR time_text) {
	int y, m, d, h, n, s;
#if _MSC_VER < 1500
	int ret = _stscanf(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s);
#else
	int ret = _stscanf_s(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s);
#endif

	tm t;
	memset(&t, 0, sizeof(t));

	t.tm_year = y - 1900;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = h;
	t.tm_min = n;
	t.tm_sec = s;

	setTimeT(mktime(&t));
}

//-----------------------------------------------------------------------------
// kjm::dateTimeクラスをファイルに保存
//-----------------------------------------------------------------------------
bool kjm::dateTime::save(FILE* fp) {
	time_t t = getTimeT();
	return (fwrite(&t, sizeof(t), 1, fp) == 1);
}

//-----------------------------------------------------------------------------
// kjm::dateTimeクラスをファイルから読込
//-----------------------------------------------------------------------------
bool kjm::dateTime::load(FILE* fp) {
	time_t t = getTimeT();
	return (fread(&t, sizeof(t), 1, fp) == 1);
}

//-----------------------------------------------------------------------------
// 文字列に変換
//-----------------------------------------------------------------------------
_tstring kjm::dateTime::toString() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_otstringstream oss;

	// std::locale::global(std::locale("japanese")) で全体を日本語にしてあると、
	// 2,010/10/15 みたいな文字列が出来上がってしまう。
	// これを確実に回避するために、***.imbue(std::locale("C")) してある。

	oss.imbue(std::locale("C"));
	oss << std::setfill(_T('0'))
		<< st.wYear << _T("/")
		<< std::setw(2) << st.wMonth << _T("/") 
		<< std::setw(2) << st.wDay << _T(" ") 
		<< std::setw(2) << st.wHour << _T(":") 
		<< std::setw(2) << st.wMinute << _T(":") 
		<< std::setw(2) << st.wSecond;
	return oss.str();
}

//---------------------------------------------------------------------
// 時間を文字列(YYYYMMDD)に変換
//---------------------------------------------------------------------
_tstring kjm::dateTime::toString_yyyymmdd() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_otstringstream oss;
	oss << std::setfill(_T('0'))
		<< st.wYear
		<< std::setw(2) << st.wMonth
		<< std::setw(2) << st.wDay;
	return oss.str();
}

//-----------------------------------------------------------------------------
// 時間をフォーマットして文字列にする
//-----------------------------------------------------------------------------
_tstring kjm::dateTime::strftime(LPCTSTR format) {
	TCHAR buf[512];
	time_t t = getTimeT();

	struct tm tmBuf;
	if (localtime_s(&tmBuf, &t) != 0)
		return _tstring();

	::_tcsftime(buf, sizeof(buf), format, &tmBuf);
	return _tstring(buf);
}

//=============================================================================
// スレッドクラスの定義
//=============================================================================
// まだ、ありません。


//=============================================================================
// environment クラスの定義
//=============================================================================
//-----------------------------------------------------------------------------
// 指定した文字列に埋め込まれている各環境変数の名前を、その変数の値を表す文字列
// で置換し、置換後の文字列全体を返します。
//-----------------------------------------------------------------------------
_tstring kjm::environment::expandEnvironmentVariables(const _tstring& name) {
	std::vector<TCHAR> work(MAX_PATH);
	kjm::util::expandEnvironmentStrings(name.c_str(), work);
	return &work[0];
}

//=============================================================================
// ユーティリティ関数用のstaticクラス
//=============================================================================
//-----------------------------------------------------------------------------
// Unicode 文字列を Ansi 文字列に変換
//-----------------------------------------------------------------------------
std::string kjm::util::toAnsi(const std::wstring& src) {
	std::vector<char> work;

	DWORD dwSizeNeeded = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);

	work.resize(dwSizeNeeded);

	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &work[0], work.size(), NULL, NULL);

	return &work[0];
}

//-----------------------------------------------------------------------------
// Ansi 文字列を Unicode 文字列に変換
//-----------------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string& src) {
	std::vector<wchar_t> work;

	DWORD dwSizeNeeded = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);

	work.resize(dwSizeNeeded);

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &work[0], work.size());

	return &work[0];
}

//-----------------------------------------------------------------------------
// バッファからCRC16を計算する
//-----------------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(unsigned char *p, unsigned int n) {
	unsigned short crc = 0xffff;

	for (unsigned int i = 0; i < n; i++) {
		unsigned char  dat = p[i];

		for (int b = 0; b < 8; b++) {
			int w = (crc ^ dat) & 0x0001;
			crc = crc >> 1;
			if (w) crc = crc ^ 0x8408;
			dat = dat >> 1;
		}
	}

	return crc;
}

//-----------------------------------------------------------------------------
// ファイルからCRC16を計算する
//-----------------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(FILE* fp) {
	unsigned char buf[8192];		// 一回で読込むバイト数
	unsigned short crc = 0xffff;

	while (1) {
		int nRead = fread(buf, sizeof(char), sizeof(buf), fp);
		for (int i = 0; i < nRead; i++) {
			unsigned char  dat = buf[i];

			for (int b = 0; b < 8; b++) {
				int w = (crc ^ dat) & 0x0001;
				crc = crc >> 1;
				if (w) crc = crc ^ 0x8408;
				dat = dat >> 1;
			}
		}
		
		if (nRead != sizeof(buf)) {
			break;
		}
	}

	return crc;
}

//-----------------------------------------------------------------------------
// ファイルからCRC16を計算する
//-----------------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(const _tstring& pfname) {
	unsigned short crc = 0xffff;
	FILE* fp = NULL;

	if (_tfopen_s(&fp, pfname.c_str(), _T("rb")) == 0) {
		crc = kjm::util::calcCRC16(fp);
		fclose(fp);
	}

	return crc;
}

//-----------------------------------------------------------------------------
// 行末の改行文字を削除する。
//-----------------------------------------------------------------------------
_tstring& kjm::util::chomp(_tstring& s) {

	// 行末に改行文字('\x0d' or '\x0a')があるときは、取り除く
	while (!s.empty() && (*(s.rbegin()) == '\x0d' || *(s.rbegin()) == '\x0a')) {
		s.erase(s.end() - 1);
	}

	return s;
}

//-----------------------------------------------------------------------------
// CopyFile APIと同様の動きをする関数(エラー箇所を詳細に特定したいときに使用)
// [戻り値]  1   正常終了(コピー先の読み込み属性を落とした)
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
// [戻り値] -12  コピー先ファイルの属性取得失敗
// [戻り値] -13  コピー先ファイルの読み込み属性リセット失敗
//-----------------------------------------------------------------------------
int kjm::util::copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly/*=TRUE*/) {
	DWORD t1 = GetTickCount();
	DWORD tr = 0, tw = 0;

	SetLastError(NOERROR);

	int result = 0;

	// コピー先が Readonly の時、エラーにしないモード?
	if (bFailIfReadonly == FALSE && kjm::path::fileExists(lpNewFileName) != false) {
		DWORD dwAttributes = ::GetFileAttributes(lpNewFileName);
		if (dwAttributes != 0xffffffff) {
			if ((dwAttributes & FILE_ATTRIBUTE_READONLY) != 0) {
				if (::SetFileAttributes(lpNewFileName, dwAttributes & ~FILE_ATTRIBUTE_READONLY)) {
					// 属性の変更成功
					result = 1;
				} else {
					result = -13;
				}
			}
		} else {
			result = -12;
		}
	}

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
		DWORD tr1 = GetTickCount();
		if (::ReadFile(hFile1, buffer, _countof(buffer), &dwNumberOfBytesRead, NULL) == FALSE) {
			result = -3;
			break;
		}
		DWORD tr2 = GetTickCount();
		tr += (tr2 - tr1);

		if (dwNumberOfBytesRead > 0) {
			DWORD tw1 = GetTickCount();
			if (::WriteFile(hFile2, buffer, dwNumberOfBytesRead, &dwNumberOfBytesWritten, NULL)) {
				if (dwNumberOfBytesRead != dwNumberOfBytesWritten) {
					result = -5;
					break;
				}
			} else {
				result = -4;
				break;
			}
			DWORD tw2 = GetTickCount();
			tw += (tw2 - tw1);
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

	DWORD t2 = GetTickCount();
	DWORD ta = t2 - t1;
	///_tprintf(_T("\n(r[%4d] + w[%4d] + o[%4d]) = a[%4d]"), tr, tw, ta - tr - tw, ta);

	return result;
}

//-----------------------------------------------------------------------------
// コピー処理は CopyFile API を使う。
// コピー先の読込属性を落とす機能付き。
//
// [戻り値]  1   正常終了(コピー先の読み込み属性を落とした)
// [戻り値]  0   正常終了
// [戻り値] -1   CopyFile APIの失敗
// [戻り値] -12  コピー先ファイルの属性取得失敗
// [戻り値] -13  コピー先ファイルの読み込み属性リセット失敗
//-----------------------------------------------------------------------------
int kjm::util::copyFile2(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly/*=TRUE*/) {
	DWORD t1 = GetTickCount();
	DWORD tr = 0, tw = 0;

	SetLastError(NOERROR);

	int result = 0;

	// コピー先が Readonly の時、エラーにしないモード?
	if (bFailIfReadonly == FALSE && kjm::path::fileExists(lpNewFileName) != false) {
		DWORD dwAttributes = ::GetFileAttributes(lpNewFileName);
		if (dwAttributes != 0xffffffff) {
			if ((dwAttributes & FILE_ATTRIBUTE_READONLY) != 0) {
				if (::SetFileAttributes(lpNewFileName, dwAttributes & ~FILE_ATTRIBUTE_READONLY)) {
					// 属性の変更成功
					result = 1;
				} else {
					result = -13;
				}
			}
		} else {
			result = -12;
		}
	}

	BOOL bRet = CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
	if (bRet == FALSE) {
		result = -1;
	}

	DWORD t2 = GetTickCount();
	DWORD ta = t2 - t1;
	///_tprintf(_T("\n(r[%4d] + w[%4d] + o[%4d]) = a[%4d]"), tr, tw, ta - tr - tw, ta);

	return result;
}

//-----------------------------------------------------------------------------
// フォルダをコピーする。
//-----------------------------------------------------------------------------
int kjm::util::copyFolder(const _tstring& srcFolder, const _tstring& dstFolder, bool (*funcIsExclude)(const _tstring& strFname), kjm::copyFolderInfo* pCopyFolderInfo, bool ignoreCopyError) {
	/***
	SHFILEOPSTRUCT fileOp;

	_tstring strFrom = srcFolder + _T('\0');
	_tstring strTo = dstFolder + _T('\0');

	memset(&fileOp, 0, sizeof(fileOp));
	fileOp.hwnd = NULL;
	fileOp.wFunc = FO_COPY;
	fileOp.pFrom = strFrom.c_str();
	fileOp.pTo = strTo.c_str();
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;

	int ret = SHFileOperation(&fileOp);
	return ret;
	***/

	if (!kjm::util::createDirectory(dstFolder.c_str())) {
		if (pCopyFolderInfo) {
			pCopyFolderInfo->srcName = srcFolder;
			pCopyFolderInfo->dstName = dstFolder;
			pCopyFolderInfo->copyFileError = 0;
			pCopyFolderInfo->dwError = GetLastError();
		}

		return -1;
	}

	kjm::folderInfo src(srcFolder);
	std::vector<kjm::fileInfo> files = src.getFiles();
	std::vector<kjm::folderInfo> subFolders = src.getSubFolders();

	int ret;

	for (std::vector<kjm::fileInfo>::size_type i = 0; i < files.size(); i++) {
		if (funcIsExclude && funcIsExclude(kjm::path::append(srcFolder, files[i].name()))) {
			continue;
		}

		ret = kjm::util::copyFile(
			kjm::path::append(srcFolder, files[i].name()).c_str(),
			kjm::path::append(dstFolder, files[i].name()).c_str(),
			FALSE, FALSE);

		/*
		ret = kjm::util::copyFile2(
			kjm::path::append(srcFolder, files[i].name()).c_str(),
			kjm::path::append(dstFolder, files[i].name()).c_str(),
			FALSE, FALSE);
		*/

		if (ignoreCopyError) {
			if (ret != 0 && ret != 1) {
				if (pCopyFolderInfo) {
					pCopyFolderInfo->srcName = kjm::path::append(srcFolder, files[i].name());
					pCopyFolderInfo->dstName = kjm::path::append(dstFolder, files[i].name());
					pCopyFolderInfo->copyFileError = ret;
					pCopyFolderInfo->dwError = GetLastError();
				}
			}
			ret = 0;
		} else {
			if (ret != 0 && ret != 1) {
				if (pCopyFolderInfo) {
					pCopyFolderInfo->srcName = kjm::path::append(srcFolder, files[i].name());
					pCopyFolderInfo->dstName = kjm::path::append(dstFolder, files[i].name());
					pCopyFolderInfo->copyFileError = ret;
					pCopyFolderInfo->dwError = GetLastError();
				}

				return ret;
			}
		}
	}

	{
		for (std::vector<kjm::folderInfo>::size_type i = 0; i < subFolders.size(); i++) {
			if (funcIsExclude && funcIsExclude(kjm::path::append(srcFolder, subFolders[i].getFileName()))) {
				continue;
			}

			ret = kjm::util::copyFolder(
				kjm::path::append(srcFolder, subFolders[i].getFileName()),
				kjm::path::append(dstFolder, subFolders[i].getFileName()),
				funcIsExclude,
				pCopyFolderInfo);

			if (ret != 0) {
				return ret;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ディレクトリを再帰的に作成する
//-----------------------------------------------------------------------------
bool kjm::util::createDirectory(const _tstring& strPathName) {
	if (PathIsRoot(strPathName.c_str()))		return true;
	if (PathFileExists(strPathName.c_str()))	return true;

	std::vector<TCHAR> parent(MAX_PATH);
	PathRemoveFileSpec(lstrcpy(&parent[0], strPathName.c_str()));

	if (kjm::util::createDirectory(&parent[0])) {
		if (CreateDirectory(strPathName.c_str(), NULL)) {
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Windowsをシャットダウンさせる
// [戻り値]
// 0  正常終了
// -1 AdjustTokenPrivileges APIでエラー
// -2 ExitWindowsEx APIでエラー
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// Win32API ExpandEnvironmentStringsを安全に使う関数
//-----------------------------------------------------------------------------
DWORD kjm::util::expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst) {
	DWORD dwRet;
	
	if (rDst.empty()) {
		rDst.resize(MAX_PATH);
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
		lstrcpy(&buffer[0], pMessage);
		::LocalFree( pMessage );
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// システム標準のエラーメッセージを作成
//-----------------------------------------------------------------------------
_tstring kjm::util::formatMessageBySystem(DWORD dwErr) {
	_tstring ret;
	PVOID pBuffer;

	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, (LPTSTR)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		std::basic_stringstream<TCHAR> ss;
		ss << TEXT("FormatMessage(") << dwErr << TEXT(") failed: error ") << dwErr;
		ret = ss.str();
	} else {
		ret = (LPTSTR)pBuffer;
		kjm::util::chomp(ret);
		LocalFree(pBuffer);
	}

	return ret;
}

//-----------------------------------------------------------------------------
// GetComputerNameEx関数(vc6用)
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// stringを返すGetComputerNameEx関数
//-----------------------------------------------------------------------------
_tstring kjm::util::getComputerNameEx(COMPUTER_NAME_FORMAT NameType) {
	DWORD dwSize = 0;

	// 必要なバッファサイズを取得(エラーチェックなし)
	kjm::util::__GetComputerNameEx(NameType, NULL, &dwSize);

	// バッファを確保
	std::vector<TCHAR> buf(dwSize);
	if (!kjm::util::__GetComputerNameEx(NameType, &buf[0], &dwSize))
		throw ::GetLastError();

	return &buf[0];
}

//-----------------------------------------------------------------------------
// stringを返すGetDlgItemText関数
// [引数] hDlg        ダイアログボックスのハンドル
// [引数] nIDDlgItem  コントロールの識別子
// [例外] throw ::GetLastError();
//-----------------------------------------------------------------------------
_tstring kjm::util::getDlgItemText(HWND hDlg, int nIDDlgItem) {
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

//---------------------------------------------------------------------
// GetEnvironmentVariable関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::getEnvironmentVariable(LPCTSTR lpName) {
	// 必要な文字数を求める
	DWORD dwRet = GetEnvironmentVariable(lpName, NULL, 0);
	if (dwRet == 0) {
		return _tstring();	// 環境変数なしの時は、空文字列を返す
	}

	std::vector<TCHAR> buffer(dwRet);	// 必要なバッファを確保

	GetEnvironmentVariable(lpName, &buffer[0], buffer.size());
	return &buffer[0];
}

//-----------------------------------------------------------------------------
// 指定ファイルのサイズを返す(GetFileSize APIのラッパー)
//-----------------------------------------------------------------------------
__int64 kjm::util::getFileSize(LPCTSTR lpszFileName) {
	// サイズを取得するファイルのハンドルを作成
	HANDLE hFile = ::CreateFile(lpszFileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}

	// サイズを取得する
	DWORD dwHigh = 0;
	DWORD dwLow = ::GetFileSize(hFile, &dwHigh);
	if (dwLow == 0xffffffff && ::GetLastError() != NO_ERROR) {
		::CloseHandle(hFile);
		return -2;
	}

	::CloseHandle(hFile);

	LARGE_INTEGER li = {dwLow, dwHigh};
	return li.QuadPart;
}

//-----------------------------------------------------------------------------
// ファイルのバージョンリソースを取得
//-----------------------------------------------------------------------------
_tstring kjm::util::getFileVersion(const _tstring &strFileName) {
	DWORD dwHandle = 0;
	DWORD dwVersionInfoSize = 0;

	TCHAR work[MAX_PATH];
	dwVersionInfoSize = GetFileVersionInfoSize(lstrcpy(work, strFileName.c_str()), &dwHandle);
	if (dwVersionInfoSize == 0) {
		return _T("");
	}

	std::vector<TCHAR> versionInfo(dwVersionInfoSize);

	if (GetFileVersionInfo(work, dwHandle, versionInfo.size(), &versionInfo[0]) == FALSE) {
		return _T("");
	}

	void* pBuf;
	UINT uLen;

	if (VerQueryValue(&versionInfo[0], _T("\\"), &pBuf, &uLen) == FALSE) {
		return _T("");
	}

	VS_FIXEDFILEINFO* pffi = (VS_FIXEDFILEINFO*)pBuf;

	_otstringstream oss;
	oss << ((pffi->dwFileVersionMS & 0xffff0000) >> 16) << _T(".")
		<< ((pffi->dwFileVersionMS & 0x0000ffff) >>  0) << _T(".")
		<< ((pffi->dwFileVersionLS & 0xffff0000) >> 16) << _T(".")
		<< ((pffi->dwFileVersionLS & 0x0000ffff) >>  0);

	return oss.str();
}

//-----------------------------------------------------------------------------
// Win32API GetModuleFileName を安全に使う関数
//-----------------------------------------------------------------------------
_tstring kjm::util::getModuleFileName(HMODULE hModule) {
	std::vector<TCHAR> filename(MAX_PATH);

	DWORD dwRet;
	while ((dwRet = ::GetModuleFileName(hModule, &filename[0], filename.size())) == filename.size()) {
		filename.resize(filename.size() * 2);
	}

	return &filename[0];
}

//---------------------------------------------------------------------
// GetPrivateProfileString関数のラッパー
//---------------------------------------------------------------------
_tstring kjm::util::getPrivateProfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = GetPrivateProfileString(lpSectionName, lpKeyName, lpDefault, &buffer[0], buffer.size(), lpFileName);
		if ((int)buffer.size() - 1 > nRet) {
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
		} else if ((int)buffer.size() - 1 > nRet) {
			// バッファのサイズ - 1が、戻り値よりも大きいときは、
			// すべての文字を読み込んだことが確実なので、ループを抜ける。
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
}

//-----------------------------------------------------------------------------
// ワークエリアの中心にウィンドウを移動する
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// SetComputerNameEx関数(vc6用)
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// ディレクトリを再帰的に削除する
//-----------------------------------------------------------------------------
void kjm::util::removeFolder(const _tstring& pszPath) {
	int i;
	std::list<_tstring> targetFolder;
	targetFolder.push_front(pszPath);

	while (targetFolder.size() != 0) {
		kjm::folderInfo currentFolder(targetFolder.front());

		std::vector<kjm::fileInfo> files = currentFolder.getFiles();			// ファイル一覧を取得
		std::vector<kjm::folderInfo> folders = currentFolder.getSubFolders();	// フォルダ一覧取得

		// 内包するファイルを全て削除する
		for (i = 0; i < files.size(); i++) {
			kjm::file::deleteFile(files[i].fullName(), true);
		}

		// 内包するフォルダがないときは、現在のディレクトリを削除。
		// ある時は、リストに追加して、次のループ処理に回す。
		if (folders.empty()) {
			targetFolder.pop_front();
			if (::RemoveDirectory(currentFolder.getFullPath().c_str()) == FALSE) {
				DWORD dwErr = ::GetLastError();
				_otstringstream oss;
				oss << _T("フォルダ '") << pszPath << _T("' が削除できません。サブフォルダ '") << currentFolder.getFullPath() << _T("' が削除できません。") << kjm::util::formatMessageBySystem(dwErr);
				throw kjm::exception::exception(dwErr, oss.str());
			}
		} else {
			for (i = 0; i < folders.size(); i++) {
				targetFolder.push_front(folders[i].getFullPath());
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 文字列を置換する。VBのStrReplaceのような関数。
//-----------------------------------------------------------------------------
_tstring kjm::util::strReplace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace) {
	_tstring result = src;
	_tstring::size_type pos = 0;
	while ((pos = result.find(strFind, pos)) != _tstring::npos) {
		result.replace(pos, lstrlen(strFind.c_str()), strReplace);
	}
	return result;
}

//-----------------------------------------------------------------------------
// 数値を human readable format な文字列にする
//-----------------------------------------------------------------------------
_tstring kjm::util::toHumanReadable(__int64 value) {
	const int kbyte = 1024;
	const int mbyte = kbyte * 1024;
	const int gbyte = mbyte * 1024;

	_otstringstream ostr;

	if (value >= gbyte) {
		ostr << (value / gbyte) << _T("G");
	} else if (value >= mbyte) {
		ostr << (value / mbyte) << _T("M");
	} else if (value >= kbyte) {
		ostr << (value / kbyte) << _T("K");
	} else {
		ostr << value << _T("B");
	}

	return ostr.str();
}

//-----------------------------------------------------------------------------
// 文字列を小文字化する
//-----------------------------------------------------------------------------
_tstring kjm::util::toLower(const _tstring& src) {
	_tstring result;
	std::transform(src.begin(), src.end(), std::back_inserter(result), ::_totlower);
	return result;
}

//-----------------------------------------------------------------------------
// 文字列を大文字化する
//-----------------------------------------------------------------------------
_tstring kjm::util::toUpper(const _tstring& src) {
	_tstring result;
	std::transform(src.begin(), src.end(), std::back_inserter(result), ::_totupper);
	return result;
}

//-----------------------------------------------------------------------------
// 安全な vsprintf
//-----------------------------------------------------------------------------
int kjm::util::vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr) {
	// バッファを指定されていないとき、256バイト割り当てておく
	if (buf.size() == 0) {
		buf.resize(256);
	}

	while (1) {

		// vsnprintf で文字列を作成。
		// 不十分なバッファ上にデータを作成している間、ループを繰り返す。
		int n = _vsntprintf_s(&buf[0], buf.size(), _TRUNCATE, format, argptr);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}

	return lstrlen(&buf[0]);
}

//-----------------------------------------------------------------------------
// 安全な sprintf 
//
// [引数]
// 一番目の引数が、vector<TCHAR>への参照になっている以外、
// sprintf関数と同じです。
//-----------------------------------------------------------------------------
int kjm::util::sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);
	int result = kjm::util::vsprintf_vec(buf, format, marker);
	va_end(marker);
	return result;
}


//---------------------------------------------------------------------
// pszPathで指定したディレクトリ内の、現在日時からnDay以上古いファイル
// を削除します。
//
// 削除対象は、pszPath直下のファイルだけです。
// 削除の基準は、(現在日時 - 最終更新日時)の値なので、時間まで考慮して、
// nDayと比較します。
//---------------------------------------------------------------------
void kjm::util::cleanupDir(LPCTSTR pszPath, int nDay) {
	_tstring findPath = kjm::path::append(pszPath, _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(findPath.c_str(), &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			__int64 diff = (kjm::dateTime::getSystemTime() - kjm::dateTime(data.ftLastWriteTime)) / FT_1DAY;
			if (diff >= nDay) {
				_tstring delFile = kjm::path::append(pszPath, data.cFileName);
				DeleteFile(delFile.c_str());
			}
		}
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);
}

// UNICODE文字列をShift JIS文字列に変換
std::string kjm::util::convertToAscii(const std::wstring& src) {
	std::vector<char> buf;

	DWORD dwSize = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	buf.resize(dwSize);

	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

// Shift JIS文字列をUNICODE文字列に変換
std::wstring kjm::util::convertToUnicode(const std::string& src) {
	std::vector<wchar_t> buf;

	DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	buf.resize(dwSize);

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

//=============================================================================
// class path の実装
//=============================================================================
//-----------------------------------------------------------------------------
// パスをくっつける
//-----------------------------------------------------------------------------
_tstring kjm::path::append(const _tstring& strPath, const _tstring& strMore) {
	TCHAR szTemp[MAX_PATH];
	PathAppend(lstrcpy(szTemp, strPath.c_str()), strMore.c_str());
	return szTemp;
}

//-----------------------------------------------------------------------------
// 相対パスを作成する
//-----------------------------------------------------------------------------
_tstring kjm::path::relativePathTo(const _tstring& from, DWORD attrFrom, const _tstring& to, DWORD attrTo) {
	std::vector<TCHAR> path(MAX_PATH);
	if (::PathRelativePathTo(&path[0], from.c_str(), attrFrom, to.c_str(), attrTo) == FALSE) {
		_otstringstream oss;
		oss << _T("kjm::path::relativePathTo error: PathRelativePathTo(\"") << from << _T("\", ") 
			<< attrFrom << _T(", \"") << to << _T("\", ") << attrTo << _T(") に失敗しました。");
		throw kjm::exception(-1, oss.str());
	}
	return &path[0];
}

//-----------------------------------------------------------------------------
// PathRemoveExtension 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::path::removeExtension(const _tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}

//-----------------------------------------------------------------------------
// フルパスからファイル名を取り除く
//-----------------------------------------------------------------------------
_tstring kjm::path::removeFileSpec(const _tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}

//-----------------------------------------------------------------------------
// 拡張子を変更する
//-----------------------------------------------------------------------------
_tstring kjm::path::renameExtension(const _tstring& pszPath, const _tstring& pszExt) {
	TCHAR szTemp[MAX_PATH];
	if (::PathRenameExtension(::lstrcpy(szTemp, pszPath.c_str()), pszExt.c_str())) {
		_otstringstream oss;
		oss << _T("kjm::path::renameExtension error: PathRenameExtension(\"")
			<< pszPath << _T("\", \"") << pszExt << _T("\") に失敗しました。");
		throw kjm::exception(-1, oss.str());
	}
	return szTemp;
}

//=============================================================================
// class process の実装
//=============================================================================
//---------------------------------------------------------------------
// デフォルト コンストラクタ
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));

	m_si.cb = sizeof(m_si);
}

//---------------------------------------------------------------------
// 指定したプロセスを実行する
//---------------------------------------------------------------------
BOOL kjm::process::run(LPCTSTR pCommandLine) {
	TCHAR temp[MAX_PATH];

	BOOL bRet = CreateProcess(
		NULL,
		lstrcpy(temp, pCommandLine),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&m_si,
		&m_pi
	);

	return bRet;
}

//---------------------------------------------------------------------
// プロセスハンドルを閉じる。
// プロセスの終了を待つのは、wait関数なので注意。
//---------------------------------------------------------------------
void kjm::process::close() {
	if (m_pi.hThread != NULL) {
		CloseHandle(m_pi.hThread);
	}

	if (m_pi.hProcess != NULL) {
		CloseHandle(m_pi.hProcess);
	}

	memset(&m_pi, 0, sizeof(m_pi));
}


int kjm::encoding::GetTextEncoding(const std::vector<BYTE>& bs) {
	if (bs.size() >= 4 && (bs[0] == 0xFF && bs[1] == 0xFE && bs[2] == 0x00 && bs[3] == 0x00)) {
		return ENCODE_UTF_32; // UTF-32
	}

	if (bs.size() >= 4 && (bs[0] == 0x00 && bs[1] == 0x00 && bs[2] == 0xFE && bs[3] == 0xFF)) {
		return ENCODE_UTF_32_BIG_ENDIAN; // UTF-32 Big Endian
	}

	if (bs.size() >= 2 && (bs[0] == 0xFF && bs[1] == 0xFE)) {
		return ENCODE_UTF_16;  // UTF-16
	}

	if (bs.size() >= 2 && (bs[0] == 0xFE && bs[1] == 0xFF)) {
		return ENCODE_UTF_16_BIG_ENDIAN;  // UTF-16 Big Endian
	}

	if (kjm::encoding::IsJis(bs) == true) {
		return ENCODE_JIS; // 日本語 (JIS)
	}

	if (kjm::encoding::IsAscii(bs) == true) {
		return ENCODE_US_ASCII; // US-ASCII
	}

	int utf8 = 0, sjis = 0, euc = 0;  // 文字コード判定用.
	bool bomFrag = false;             // UTF-8 BOM の判定用.

	bool Utf8Flag = kjm::encoding::IsUTF8(bs, utf8, bomFrag);
	bool SJisFlag = kjm::encoding::IsSJIS(bs, sjis);
	bool EucFlag  = kjm::encoding::IsEUC(bs, euc);

	if (Utf8Flag == true || SJisFlag == true || EucFlag == true) {
		if (euc > sjis && euc > utf8) {
			return ENCODE_EUC_JP; // 日本語 (EUC)
		} else if (sjis > euc && sjis > utf8) {
			return ENCODE_SHIFT_JIS;   // 日本語 (シフト JIS)
		} else if (utf8 > euc && utf8 > sjis) {
			if (bomFrag == true) {
				return ENCODE_UTF_8;    // UTF-8 (BOMあり)
			} else {
				return ENCODE_UTF_8N;   // UTF-8N (BOMなし)
			}
		}
	}
	return 0;
}

bool kjm::encoding::IsJis(const std::vector<BYTE>& bs) {
	int len = bs.size();
	BYTE b1, b2, b3, b4, b5, b6;

	for (int i = 0; i < len; i++) {
		b1 = bs[i];

		if (b1 > 0x7F) {
			return false;   // Not ISO-2022-JP (0x00～0x7F)
		} else if (i < len - 2) {
			b2 = bs[i + 1]; b3 = bs[i + 2];
			if (b1 == 0x1B && b2 == 0x28 && b3 == 0x42) {
				return true;    // ESC ( B  : JIS ASCII
			} else if (b1 == 0x1B && b2 == 0x28 && b3 == 0x4A) {
				return true;    // ESC ( J  : JIS X 0201-1976 Roman Set
			} else if (b1 == 0x1B && b2 == 0x28 && b3 == 0x49) {
				return true;    // ESC ( I  : JIS X 0201-1976 kana
			} else if (b1 == 0x1B && b2 == 0x24 && b3 == 0x40) { 
				return true;    // ESC $ @  : JIS X 0208-1978(old_JIS)
			} else if (b1 == 0x1B && b2 == 0x24 && b3 == 0x42) {
				return true;    // ESC $ B  : JIS X 0208-1983(new_JIS)
			} 
		} else if (i < len - 3) { 
			b2 = bs[i + 1]; b3 = bs[i + 2]; b4 = bs[i + 3];
			if (b1 == 0x1B && b2 == 0x24 && b3 == 0x28 && b4 == 0x44) {
				return true;    // ESC $ ( D  : JIS X 0212-1990（JIS_hojo_kanji）
			}
		} else if (i < len - 5) { 
			b2 = bs[i + 1]; b3 = bs[i + 2]; b4 = bs[i + 3]; b5 = bs[i + 4]; b6 = bs[i + 5];
			if (b1 == 0x1B && b2 == 0x26 && b3 == 0x40 && b4 == 0x1B && b5 == 0x24 && b6 == 0x42) { 
				return true;    // ESC & @ ESC $ B  : JIS X 0208-1990, JIS X 0208:1997
			}
		} else {
			continue;
		}
	}

	return false;
}

bool kjm::encoding::IsAscii(const std::vector<BYTE>& bs) { 
	for (std::vector<BYTE>::size_type i = 0; i < bs.size(); i++) {
		if (bs[i] >= 0x00 && bs[i] <= 0x7F) {
			continue; // ASCII : 0x00～0x7F
		} else { 
			return false;
		}
	}
	return true;
}

bool kjm::encoding::IsSJIS(const std::vector<BYTE>& bs, int& sjis) {
	int len = bs.size();
	BYTE b1, b2;

	for (int i = 0; i < len; i++) {
		b1 = bs[i];

		if (b1 >= 0x00 && b1 <= 0x7F) {
			continue;  // ASCII : 0x00～0x7F
		}

		if (b1 >= 0xA1 && b1 <= 0xDF) {
			continue;  // kana  : 0xA1～0xDF
		}

		if (i < len - 1) {
			b2 = bs[i + 1];

			if (((b1 >= 0x81 && b1 <= 0x9F) || (b1 >= 0xE0 && b1 <= 0xFC)) && ((b2 >= 0x40 && b2 <= 0x7E) || (b2 >= 0x80 && b2 <= 0xFC))) {
				// kanji first byte  : 0x81～0x9F or 0xE0～0xFC
				//       second byte : 0x40～0x7E or 0x80～0xFC
				i += 1; sjis += 2;
				continue;
			}
		}

		return false;
	}

	return true;
}

bool kjm::encoding::IsEUC(const std::vector<BYTE>& bs, int& euc) {
	int len = bs.size();
	BYTE b1, b2, b3;

	for (int i = 0; i < len; i++) { 
		b1 = bs[i];

		if (b1 >= 0x00 && b1 <= 0x7F) {
			continue;  // ASCII : 0x00～0x7F
		}

		if (i < len - 1) {
			b2 = bs[i + 1];

			if ((b1 >= 0xA1 && b1 <= 0xFE) && (b2 >= 0xA1 && b2 <= 0xFE)) {
				i += 1; euc += 2;
				continue;     // kanji - 0xA1～0xFE, 0xA1～0xFE
			}

			if ((b1 == 0x8E) && (b2 >= 0xA1 && b2 <= 0xDF)) {
				i += 1; euc += 2;
				continue;     // kana - 0x8E, 0xA1～0xDF
			}
		}

		if (i < len - 2) {
			b2 = bs[i + 1]; b3 = bs[i + 2];

			if ((b1 == 0x8F) && (b2 >= 0xA1 && b2 <= 0xFE) && (b3 >= 0xA1 && b3 <= 0xFE)) {
				i += 2; euc += 3;
				continue;       // hojo kanji - 0x8F, 0xA1～0xFE, 0xA1～0xFE
			}
		}

		return false;
	}

	return true;
}

bool kjm::encoding::IsUTF8(const std::vector<BYTE>& bs, int& utf8, bool& bomFlag) {
	int len = bs.size();
	BYTE b1, b2, b3, b4;

	for (int i = 0; i < len; i++) {
		b1 = bs[i];

		if (b1 >= 0x00 && b1 <= 0x7F) {
			continue;  // ASCII : 0x00～0x7F
		}

		if (i < len - 1) {
			b2 = bs[i + 1];

			if ((b1 >= 0xC0 && b1 <= 0xDF) && (b2 >= 0x80 && b2 <= 0xBF)) {
				i += 1; utf8 += 2;  // 2 byte char
				continue;
			}
		}

		if (i < len - 2) {
			b2 = bs[i + 1]; b3 = bs[i + 2];

			if (b1 == 0xEF && b2 == 0xBB && b3 == 0xBF) {
				bomFlag = true;     // BOM : 0xEF 0xBB 0xBF
				i += 2; utf8 += 3;

				continue;
			}

			if ((b1 >= 0xE0 && b1 <= 0xEF) && (b2 >= 0x80 && b2 <= 0xBF) && (b3 >= 0x80 && b3 <= 0xBF)) {
				i += 2; utf8 += 3;  // 3 byte char
				continue;
			}
		}

		if (i < len - 3) {
			b2 = bs[i + 1]; b3 = bs[i + 2]; b4 = bs[i + 3];

			if ((b1 >= 0xF0 && b1 <= 0xF7) && (b2 >= 0x80 && b2 <= 0xBF) && (b3 >= 0x80 && b3 <= 0xBF) && (b4 >= 0x80 && b4 <= 0xBF)) {
				i += 3; utf8 += 4;  // 4 byte char
				continue;
			}
		}

		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HRESULT kjm::encoding::convertINetString(
	LPDWORD lpdwMode,
    DWORD dwSrcEncoding,
    DWORD dwDstEncoding,
    LPCSTR lpSrcStr,
    LPINT lpnSrcSize,
    LPBYTE lpDstStr,
    LPINT lpnDstSize
) {
	typedef HRESULT (WINAPI *CINS)(LPDWORD, DWORD, DWORD, LPCSTR, LPINT, LPBYTE, LPINT);

	HRESULT bRet = E_FAIL;
	HMODULE hModule = LoadLibrary(TEXT("mlang.dll"));
	
	if (hModule != NULL) {
		CINS pFunc = (CINS)GetProcAddress(hModule, "ConvertINetString");
		if (pFunc != NULL) {
			bRet = pFunc(lpdwMode, dwSrcEncoding, dwDstEncoding, lpSrcStr, lpnSrcSize, lpDstStr, lpnDstSize);
		}
		FreeLibrary(hModule);
	}

	return bRet;
}

//---------------------------------------------------------------------
// ドライブの情報を保持するクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ドライブレターを指定するコンストラクタ
//---------------------------------------------------------------------
kjm::drive::drive(const _tstring& driveLetter) {
	m_driveNo = (int)CharUpper((LPTSTR)driveLetter[0]) - 'A';
}

//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kjm::drive& kjm::drive::operator =(const kjm::drive& rhs) {
	if (this == &rhs) return *this;

	m_driveNo = rhs.m_driveNo;

	return *this;
}

//---------------------------------------------------------------------
// ドライブタイプを文字列で取得
//---------------------------------------------------------------------
_tstring kjm::drive::get_DriveTypeString() {
	_tstring result;

	switch (get_DriveType()) {
	case DRIVE_UNKNOWN:
		result = _T("不明なタイプ");
		break;

	case DRIVE_NO_ROOT_DIR:
		result = _T("存在しないドライブ");
		break;

	case DRIVE_REMOVABLE:
		result = _T("リムーバブルドライブ");
		break;
		
	case DRIVE_FIXED:
		result = _T("固定ドライブ");
		break;

	case DRIVE_REMOTE:
		result = _T("ネットワークドライブ");
		break;

	case DRIVE_CDROM:
		result = _T("CD-ROMドライブ");
		break;

	case DRIVE_RAMDISK:
		result = _T("RAMディスク");
		break;

	default:
		result = _T("未対応のタイプ");
		break;
	}

	return result;
}

//---------------------------------------------------------------------
// ドライブの一覧を保持するクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// デフォルトコンストラクタ
//---------------------------------------------------------------------
kjm::driveList::driveList() {
	DWORD dwDrives = GetLogicalDrives();

	for (int i = 'A'; i <= 'Z'; i++) {
		if (dwDrives & (1 << (i - 'A'))) {
			m_driveList.push_back(kjm::drive(i - 'A'));
		}
	}
}

//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kjm::driveList& kjm::driveList::operator =(const kjm::driveList& rhs) {
	if (this == &rhs) return *this;

	m_driveList = rhs.m_driveList;

	return *this;
}
void kjm::csvFile::open(LPCTSTR fname) {
#if defined(UNICODE)
	m_ifs.open(kjm::util::convertToAscii(fname).c_str());
#else
	m_ifs.open(fname);
#endif
	m_ifs.imbue(std::locale("japanese"));
}

bool kjm::csvFile::getline() {
	_tstring line;

	if (std::getline(m_ifs, line).fail())
		return false;

	parse_csv(line);

	return true;
}


void kjm::csvFile::parse_csv(const _tstring& line) {
	_tstring::size_type p = 0;

	m_cols.clear();

	bool col_top = true;
	bool esc_mode = false;
	_tstring col_data;

	for (_tstring::const_iterator i = line.begin(); i != line.end(); i++) {
		if (col_top && *i == '\"') {
			col_top = false;
			esc_mode = true;
			continue;
		}

		col_top = false;

		if (esc_mode) {
			if (*i != '\"') {
				col_data += *i;
			} else if ((i + 1) == line.end() || *(i + 1) != '\"') {
				esc_mode = false;
			} else if (*(i + 1) == '\"') {
				col_data += *i;
				i++;
			}
		} else {
			if (*i != ',') {
				col_data += *i;
			} else {
				m_cols.push_back(col_data);
				col_data.erase();
				col_top = true;
			}
		}
	}

	if (!col_top) {
		m_cols.push_back(col_data);
	}
}
