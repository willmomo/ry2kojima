#include <windows.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include "kjm.h"


/* イベントログの読み取り */
void ReadLog( LPCTSTR lpSourceName )
{
	DWORD i;

	/* イベントログのオープン */
	kjm::eventlog eventLog;

	if((eventLog.OpenEventLog(NULL, lpSourceName)) == FALSE) {
		printf("event log can not open.\n");
		return;
	}

	kjm::eventlog_record rec;
	while ( eventLog.ReadEventLog( EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, rec ) ) {

		/* 読み取ったイベントの表示 */
		printf("レコード番号: %d\n", rec.RecordNumber());
		printf("生成時刻: %s", rec.TimeGenerated().ctime().c_str() );
		printf("書き込み時刻: %s", rec.TimeWritten().ctime().c_str() );
		printf("イベントID: %08x\n", rec.EventID());

		printf("イベントの種別: ");
		switch(rec.EventType()) {
			case EVENTLOG_SUCCESS: printf("成功\n"); break;
			case EVENTLOG_ERROR_TYPE: printf("エラー\n"); break;
			case EVENTLOG_WARNING_TYPE: printf("警告\n"); break;
			case EVENTLOG_INFORMATION_TYPE: printf("情報\n"); break;
			case EVENTLOG_AUDIT_SUCCESS: printf("監査成功\n"); break;
			case EVENTLOG_AUDIT_FAILURE: printf("監査失敗\n"); break;
			default: printf("不明\n"); break;
		}

		printf("ソース名: %s\n", rec.SourceName().c_str());
		printf("コンピュータ名: %s\n", rec.ComputerName().c_str());

		/* カテゴリの表示 */
		printf("二次カテゴリ: %s", rec.EventCategoryText().c_str() );

		/* メッセージの表示 */
		printf("メッセージ: %s", rec.EventIDText().c_str() );

		/* 固有データの表示 */
		std::vector<BYTE> data = rec.Data();

		if (data.size() > 0) {

			printf("固有データ: ");
			for ( i = 0; i < data.size(); i++) {
				printf("%02x ", data.at( i ));
			}
			printf("\n");
		}

		printf("\n");

	}

}

int _tmain() {

	ReadLog( "Application" );
	ReadLog( "System" );

	return 0;
}
