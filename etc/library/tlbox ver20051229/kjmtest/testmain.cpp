
#include <windows.h>
#include <tchar.h>
#include "resource.h"

#include "kjm.h"

int _tmain( int argc, TCHAR** argv ) {


	int i;
	kjm::eventlog eventLog;

	eventLog.OpenBackupEventLog( NULL, "System", "f:\\_wkt1c\\system.evt" );

	kjm::eventlog_record rec;

	while ( eventLog.ReadEventLog( EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, rec ) ) {

		/* 読み取ったイベントの表示 */
		printf("レコード番号: %d\n", rec.RecordNumber());
		printf("生成時刻: %s\n", rec.TimeGenerated().ctime().c_str() );
		printf("書き込み時刻: %s\n", rec.TimeWritten().ctime().c_str() );
		printf("イベントID: %d (%08x) %d\n", rec.EventID(), rec.EventID(), rec.EventID() & 0xffff);

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


	return 0;
}
