//=====================================================================
// イベントログを見る
//=====================================================================
#include <windows.h>
#include <iostream>
#include "../library/kjmlib/v20100317/kjmlib.h"


//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int main(int argc, char** argv) {
	kjm::eventlog evlog;
	kjm::eventlogrecord evlogrec;

	DWORD t = 0;
	while (1) {
		if (t != 0 && (GetTickCount() - t) <= (60 * 1000)) {
			Sleep(1000);
			continue;
		}

		bool bRet;
		bRet = evlog.open_event_log("", "System");
		//bRet = evlog.open_event_log("", "Security");
		//bRet = evlog.open_backup_event_log("", argv[1]);

		if (bRet) {
			while (evlog.read_event_log(EVENTLOG_SEQUENTIAL_READ | EVENTLOG_BACKWARDS_READ, 0, evlogrec) == 1) {
				switch (evlogrec.get_event_type()) {
				case EVENTLOG_ERROR_TYPE:
				case EVENTLOG_WARNING_TYPE:
				case EVENTLOG_AUDIT_FAILURE:
					std::cout << evlogrec.get_event_type() << "\t";
					std::cout << evlogrec.get_time_generated().to_local_time().to_string() << "\t";
					std::cout << evlogrec.get_source_name() << "\n";
					break;
				}

				if (((kjm::time::get_local_time() - evlogrec.get_time_generated().to_local_time()) / FT_DAY) >= 1) {
					break;
				}
			}

			evlog.close_event_log();
		}

		std::cout << "最終確認時間: " << kjm::time::get_local_time().to_string() << "\n";

		t = GetTickCount();
	}

	return 0;
}
