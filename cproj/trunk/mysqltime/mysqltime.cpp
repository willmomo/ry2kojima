/*
	Windows XP で動かすために、mysql-connector は、6.0.2 を使用。
	(mysql-connector-c-noinstall-6.0.2-win32\include をインクルードパスに追加)

	mysql-connector 6.0.2 の都合で、__LCC__ を定義
	(__LCC__ をプリプロセッサのマクロに追加)

	追加のライブラリパスには、
	mysql-connector-c-noinstall-6.0.2-win32\lib
	を指定。

	追加のライブラリファイルは、
	libmysql.lib
	を追加。

	DLLは mysql-connector-c-noinstall-6.0.2-win32\lib\libmysql.dll を
	実行ファイルと同じ場所において使用する。
*/

#include "kjmlib.h"
#include "kjmmysql.h"
#include "resource.h"

void usage() {
	_tcout << std::setw(22) << std::left << _T("  -h, --host=name") << _T("Connect to host.") << _T("\n");
	_tcout << std::setw(22) << std::left << _T("  -p, --password=name") << _T("Password to use when connecting to server.") << _T("\n");
	_tcout << std::setw(22) << std::left << _T("  -u, --user=name") << _T("User for login if not current user.") << _T("\n");
}

int _tmain(int argc, TCHAR** argv) {
	SET_UNICODE_MODE;

	kjm::optionInfoList opts;
	opts.push_back(kjm::optionInfo(_T("host"), _T("h"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("password"), _T("p"), kjm::required_argument));
	opts.push_back(kjm::optionInfo(_T("user"), _T("u"), kjm::required_argument));

	kjm::cmdLine cmdLine;
	cmdLine.parse(argc, argv, opts);

	kjm::_tstring host = (cmdLine.hasOption(_T("host")) ? cmdLine.getOptArgument(_T("host")) : _T(""));
	kjm::_tstring password = (cmdLine.hasOption(_T("password")) ? cmdLine.getOptArgument(_T("password")) : _T(""));
	kjm::_tstring user = (cmdLine.hasOption(_T("user")) ? cmdLine.getOptArgument(_T("user")) : _T(""));

	if (host.empty() || password.empty() || user.empty()) {
		usage();
		return -1;
	}

	kjm::mysql mysql;
	mysql.init();
	if (mysql.real_connect(host.c_str(), user.c_str(), password.c_str(), NULL, MYSQL_PORT_DEFAULT) == false) {
		_tcout << _T("real_connect error ") << mysql.m_errno << _T(" : ") << mysql.m_error << _T("\n");
		return -1;
	}
	
	kjm::timeSpan ts;
	kjm::dateTime dtNow = kjm::dateTime::now();

	LPCTSTR sql = _T("select now()");
	if (mysql.real_query(sql, lstrlen(sql)) == 0) {
		// 成功
		kjm::mysqlRes result = mysql.store_result();
		if (result.hasResult()) {
			MYSQL_ROW row = result.fetch_row();
			if (row != NULL) {
				kjm::dateTime dt;
				if (kjm::dateTime::tryParse(row[0], dt)) {
					ts = dt - dtNow;

					_tcout << dt.toString() << _T(" - ") << dtNow.toString() << _T("\n");
					_tcout << ts.totalMilliseconds() << _T(" msec\n");
					_tcout << ts.totalSeconds() << _T(" sec\n");
					_tcout << ts.totalMinutes() << _T(" min\n");
				} else {
					_tcout << _T("datetime format error: ") << row[0] << _T("\n");
				}
			}
			result.free_result();
		} else {
			_tcout << _T("store_result error ") << mysql.m_errno << _T(" : ") << mysql.m_error << _T("\n");
		}
	} else {
		// 失敗
		_tcout << _T("real_query error ") << mysql.m_errno << _T(" : ") << mysql.m_error << _T("\n");
	}

	//return abs((int)ts.totalMilliseconds());
	return abs((int)ts.totalSeconds());
	//return abs((int)ts.totalMinutes());
}
