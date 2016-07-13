#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "eventlog.h"

/* 埋込み文字列の取得 */
char **GetArgs(const EVENTLOGRECORD *pBuf)
{
	char *cp;
	WORD ArgCount;
	char **Args = NULL;

	if(pBuf->NumStrings == 0) return NULL;

	/* 引数リストを取得 */
	Args = (char **)GlobalAlloc(GMEM_FIXED, sizeof(char *) * pBuf->NumStrings);
	cp = (char *)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		Args[ArgCount] = cp;
		cp += strlen(cp) + 1;
	}
	return Args;
}

/* ソース名からモジュール名を取得 */
BOOL GetModuleNameFromSourceName(
	const char *SourceName, 
	const char *EntryName, 
	char *ExpandedName /* 1000バイトのバッファ */)
{
	DWORD lResult;
	DWORD ModuleNameSize;
	char ModuleName[1000];
	HKEY hAppKey = NULL;
	HKEY hSourceKey = NULL;
	BOOL bReturn = FALSE;

	/* Applicationログ用のレジストリキーをオープン */
	lResult = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application",
		0,
		KEY_READ,
		&hAppKey);

	if(lResult != ERROR_SUCCESS) {
		printf("registry can not open.\n");
		goto Exit;
	}

	/* ソースの情報が格納されているレジストリをオープン */
	lResult = RegOpenKeyEx(
		hAppKey,
		SourceName,
		0,
		KEY_READ,
		&hSourceKey);

	if(lResult != ERROR_SUCCESS) goto Exit;

	ModuleNameSize = 1000;

	/* ソースモジュール名を取得 */
	lResult = RegQueryValueEx(
		hSourceKey,
		"EventMessageFile",
		NULL,
		NULL,
		(unsigned char *)ModuleName,
		&ModuleNameSize);

	if(lResult != ERROR_SUCCESS) goto Exit;

	/* 環境変数を展開 */
	ExpandEnvironmentStrings(ModuleName, ExpandedName, 1000);

	/* 正常終了 */
	bReturn = TRUE;

Exit: /* 後処理 */
	if(hSourceKey != NULL) RegCloseKey(hSourceKey);
	if(hAppKey != NULL) RegCloseKey(hAppKey);

	return bReturn;
}

/* メッセージの表示 */
BOOL DispMessage(
	const char *SourceName, 
	const char *EntryName,
	const char **Args, 
	DWORD MessageId)
{
	BOOL bResult;
	BOOL bReturn = FALSE;
	HANDLE hSourceModule = NULL;
	char SourceModuleName[1000];
	char *pMessage = NULL;

	/* ソースモジュール名を取得 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) goto Exit;

	/* ソースモジュールをロード */
	hSourceModule = LoadLibraryEx(
		SourceModuleName,
		NULL,
		DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

	if(hSourceModule == NULL) goto Exit;

	/* メッセージを作成 */
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		hSourceModule,
		MessageId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&pMessage,
		0,
		(va_list *)Args);

	/* 正常終了 */
	bReturn = TRUE;

Exit: /* 後処理 */
	if(pMessage != NULL) printf("%s", pMessage);
	else printf("(%d)\n", MessageId);

	if(hSourceModule != NULL) FreeLibrary((HINSTANCE)hSourceModule);
	if(pMessage != NULL) LocalFree(pMessage);

	return bReturn;
}

/* イベントログの読み取り */
void ReadLog(void)
{
	DWORD BufSize;
	DWORD ReadBytes;
	DWORD NextSize;
	BOOL bResult;
	DWORD i;
	char *cp;
	char *pSourceName;
	char *pComputerName;
	HANDLE hEventLog = NULL;
	EVENTLOGRECORD *pBuf = NULL;
	char **Args = NULL;

	/* イベントログのオープン */
	hEventLog = OpenEventLog(NULL, "Application");

	if(hEventLog == NULL) {
		printf("event log can not open.\n");
		goto Exit;
	}

	for(;;) {
		/* イベントログのサイズ取得 */
		BufSize = 1;
		pBuf = (EVENTLOGRECORD *)GlobalAlloc(GMEM_FIXED, BufSize);

		bResult = ReadEventLog(
			hEventLog,
			EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
			0,
			pBuf,
			BufSize,
			&ReadBytes,
			&NextSize);

		if(!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) break;

		GlobalFree(pBuf);
		pBuf = NULL;

		/* バッファ割り当て */
		BufSize = NextSize;
		pBuf = (EVENTLOGRECORD *)GlobalAlloc(GMEM_FIXED, BufSize);

		/* イベントログの読み取り */
		bResult = ReadEventLog(
			hEventLog,
			EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
			0,
			pBuf,
			BufSize,
			&ReadBytes,
			&NextSize);

		if(!bResult) break;

		eventlog_record rec( pBuf );

		/* 読み取ったイベントの表示 */
		printf("レコード番号: %d\n", rec.RecordNumber() );
		time_t t = rec.TimeGenerated();
		printf("生成時刻: %s", ctime( &t ) );
		t = rec.TimeWritten();
		printf("書き込み時刻: %s", ctime( &t ) );
		printf("イベントID: %08x\n", rec.EventID() );

		printf( "イベントの種別: %s\n", rec.EventTypeText().c_str() );

		cp = (char *)pBuf;
		cp += sizeof(EVENTLOGRECORD);

		pSourceName = cp;
		cp += strlen(cp)+1;

		pComputerName = cp;
		cp += strlen(cp)+1;

		printf("ソース名: %s\n", rec.SourceName().c_str() );
		printf("コンピュータ名: %s\n", rec.ComputerName().c_str() );

		/* カテゴリの表示 */
		printf("二次カテゴリ: ", pBuf->EventCategory);
		DispMessage(pSourceName, "CategoryMessageFile", NULL, pBuf->EventCategory);

		/* メッセージの表示 */
		Args = GetArgs(pBuf);

		printf("メッセージ: ");
		DispMessage(pSourceName, "EventMessageFile", (const char **)Args, pBuf->EventID);

		if(Args != NULL) {
			GlobalFree(Args);
			Args = NULL;
		}

		/* 固有データの表示 */
		if(pBuf->DataLength > 0) {
			printf("固有データ: ");
			for(i=0; i<pBuf->DataLength; i++) printf("%02x ", *(((unsigned char *)pBuf)+(pBuf->DataOffset)+i));
			printf("\n");
		}

		printf("\n");

		/* バッファ解放 */
		GlobalFree(pBuf);
		pBuf = NULL;
	}

Exit: /* 後処理 */
	if(pBuf != NULL) GlobalFree(pBuf);
	if(Args != NULL) GlobalFree(Args);
	if(hEventLog != NULL) CloseEventLog(hEventLog);
}

void main() {
	ReadLog();
}
