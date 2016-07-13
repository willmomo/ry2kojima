//-----------------------------------------------------------------------------
// kjmsub ver.0.2010.7.29
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
				kjm::util::expandEnvironmentStrings((LPCTSTR)&ModuleName[0], ExpandedName );

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

	oneModuleName = _tcstok(&SourceModuleName[0], _T(";") );
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

		oneModuleName = _tcstok(NULL, _T(";"));
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

	for ( int i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


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
	for (int i = 0; i < sections.size(); i++) {

		// マージするINIファイルのセクションから、全キーを読み込む
		std::vector<_tstring> newKeys = newIniFile.getKeyNames(sections[i]);

		// マージされるINI(この)ファイルのセクションからも全キーを読み込む
		std::vector<_tstring> orgKeys = this->getKeyNames(sections[i]);

		for (int j = 0; j < newKeys.size(); j++) {

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
//-----------------------------------------------------------------------------
// 古いログの削除
//-----------------------------------------------------------------------------
void kjm::log::deleteOldLog() {
#if defined(USE_WSSLOG)
	WLDeleteOldLog();
#endif
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
	Winssa_Log(get_Ident().c_str(), LOGLV_ALWAYS, _T("%s"), buf.begin());
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
	Winssa_Log(get_Ident().c_str(), LOGLV_ERR, _T("%s"), buf.begin());
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
	Winssa_Log(get_Ident().c_str(), LOGLV_INFO, _T("%s"), buf.begin());
#endif
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
	int ret = _stscanf(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s);

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
_tstring kjm::dateTime::toString() {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_otstringstream oss;
	oss << std::setfill(_T('0'))
		<< st.wYear << _T("/")
		<< std::setw(2) << st.wMonth << _T("/") 
		<< std::setw(2) << st.wDay << _T(" ") 
		<< std::setw(2) << st.wHour << _T(":") 
		<< std::setw(2) << st.wMinute << _T(":") 
		<< std::setw(2) << st.wSecond;
	return oss.str();
}


//=============================================================================
// ユーティリティ関数用のstaticクラス
//=============================================================================
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

	FILE* fp = _tfopen(pfname.c_str(), _T("rb"));
	if (fp) {
		crc = kjm::util::calcCRC16(fp);
		fclose(fp);
	}

	return crc;
}

//-----------------------------------------------------------------------------
// 行末の改行を取り除く
//-----------------------------------------------------------------------------
_tstring& kjm::util::chomp(_tstring& s) {

	// 行末に改行文字があるときは、取り除く
	while (!s.empty() && (*(s.rbegin()) == '\n')) {
		s.erase(s.end() - 1);
	}

	return s;
}

//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
int kjm::util::copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists) {
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
DWORD kjm::util::expandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst ) {

	DWORD dwRet;
	
	while ( ( dwRet = ::ExpandEnvironmentStrings( lpSrc, &rDst[0], rDst.size() ) ) > rDst.size() ) {
		if ( dwRet == 0 ) {
			break;
		} else {
			rDst.resize( dwRet );
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
// PathRenameExtensionのラッパー
//-----------------------------------------------------------------------------
_tstring& kjm::util::pathRenameExtension(_tstring& strPath, LPCTSTR pszExt) {
	std::vector<TCHAR> work(strPath.begin(), strPath.end());
	work.push_back('\0');
	work.resize(work.size() + lstrlen(pszExt) + MAX_PATH );	// 十分なバッファを確保

	if (::PathRenameExtension(&work[0], pszExt)) {
		strPath = &work[0];
	}

	return strPath;
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
		int n = _vsntprintf(&buf[0], buf.size(), format, argptr);
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


