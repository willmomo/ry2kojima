//---------------------------------------------------------------------
// kjm::eventlog class
//---------------------------------------------------------------------


#include "kjm_library.h"
#include "kjm_registry.h"
#include "kjm_util.h"

#include "kjm_eventlog.h"


//---------------------------------------------------------------------
// 1レコード分のeventlogをあらわすクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------
kjm::eventlog_record& kjm::eventlog_record::operator= ( const kjm::eventlog_record& src ) {
	m_source_name = src.m_source_name;
	m_raw_record = src.m_raw_record;

	return *this;
}

kjm::eventlog_record::eventlog_record( LPCTSTR lpSourceName, const EVENTLOGRECORD* src ) {

	// イベントソースの保存
	m_source_name = lpSourceName;

	// 生情報のコピー
	m_raw_record.resize( src->Length );
	memmove( m_raw_record.begin(), src, src->Length );
}


/* 埋込み文字列の取得 */
int kjm::eventlog_record::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<char*>& args )
{
	args.clear();

	char *cp;
	WORD ArgCount;

	if(pBuf->NumStrings == 0) return args.size();

	/* 引数リストを取得 */
	args.resize( pBuf->NumStrings );
	cp = (char *)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		args[ArgCount] = cp;
		cp += strlen(cp) + 1;
	}
	return args.size();
}

/* ソース名からモジュール名を取得 */
BOOL kjm::eventlog_record::GetModuleNameFromSourceName( const char *SourceName, const char *EntryName, std::vector<TCHAR>& ExpandedName) {
	DWORD lResult;
	std::vector<BYTE> ModuleName;
	BOOL bReturn = FALSE;

	// Applicationログ用のレジストリキーをオープン
	kjm::registry appKey;
	
	std::string key = std::string( "SYSTEM\\CurrentControlSet\\Services\\EventLog\\" );
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
				"EventMessageFile",
				NULL,
				NULL,
				ModuleName);

			if ( lResult == ERROR_SUCCESS ) {

				// 環境変数を展開
				kjm::util::ExpandEnvironmentStrings( (LPCTSTR)ModuleName.begin(), ExpandedName );

				/* 正常終了 */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

/* メッセージの表示 */
std::string kjm::eventlog_record::DispMessage( const char *SourceName, const char *EntryName, const char **Args, DWORD MessageId) {
	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	char* oneModuleName;
	DWORD dwRet;

	/* ソースモジュール名を取得 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) return std::string("");

	oneModuleName = strtok( SourceModuleName.begin(), ";" );
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// ソースモジュールをロード
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			/* メッセージを作成 */
			dwRet = kjm::util::FormatMessage(
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

		oneModuleName = strtok( NULL, ";" );
	}

	if(message.size() > 0) return std::string(message.begin());
	else {
		char buf[32];
		wsprintf(buf, "(%d)\n", MessageId);
		return std::string( buf );
	}
}


//---------------------------------------------------------------------
// 各種アクセサ(生情報からデータを切り出す)
//---------------------------------------------------------------------
std::string kjm::eventlog_record::SourceName() {
	char* cp = (char *)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	return std::string( cp );
}

std::string kjm::eventlog_record::ComputerName() {
	char* cp = (char *)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	cp += strlen( cp ) + 1;		// SourceName分ポインタを進める

	return std::string( cp );
}

std::vector<BYTE> kjm::eventlog_record::Data() {

	std::vector<BYTE> data( DataLength() );

	for ( int i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//---------------------------------------------------------------------
// OpenEventLog/CloseEventLogを対にするクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// イベントログを開く
//---------------------------------------------------------------------
BOOL kjm::eventlog::OpenEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName ) {

	// すでに開いているかもしれないので、閉じる
	CloseEventLog();

	m_unc_server_name = ( lpUNCServerName ) ? lpUNCServerName : "";
	m_source_name = ( lpSourceName ) ? lpSourceName : "";

	m_hEventLog = ::OpenEventLog( lpUNCServerName, lpSourceName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// 保存したイベントログを開く
//---------------------------------------------------------------------
BOOL kjm::eventlog::OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName ) {

	// すでに開いているかもしれないので、閉じる
	CloseEventLog();

	m_unc_server_name = ( lpUNCServerName ) ? lpUNCServerName : "";
	m_source_name = ( lpSourceName ) ? lpSourceName : "";
	m_file_name = ( lpFileName ) ? lpFileName : "";

	m_hEventLog = ::OpenBackupEventLog( lpUNCServerName, lpFileName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// イベントログを閉じる
//---------------------------------------------------------------------
BOOL kjm::eventlog::CloseEventLog() {
	BOOL ret = TRUE;

	if ( m_hEventLog ) {
		ret = ::CloseEventLog( m_hEventLog );
		m_hEventLog = NULL;
	}

	return ret;
}


//---------------------------------------------------------------------
// イベントログを読む
//---------------------------------------------------------------------
BOOL kjm::eventlog::ReadEventLog( DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlog_record& record ) {

	DWORD dwReadBytes = 0;
	DWORD dwNextSize = 0;
	std::vector<BYTE> buffer;

	// 極小のバッファを確保してサイズをもらう
	buffer.resize( 1 );

	BOOL bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		buffer.begin(),
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
		buffer.begin(),
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	record = kjm::eventlog_record( m_source_name.c_str(), (EVENTLOGRECORD*)buffer.begin() );

	return bResult;
}
