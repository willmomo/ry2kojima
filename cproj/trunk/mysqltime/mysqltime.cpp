/*
	Windows XP �œ��������߂ɁAmysql-connector �́A6.0.2 ���g�p�B
	(mysql-connector-c-noinstall-6.0.2-win32\include ���C���N���[�h�p�X�ɒǉ�)

	mysql-connector 6.0.2 �̓s���ŁA__LCC__ ���`
	(__LCC__ ���v���v���Z�b�T�̃}�N���ɒǉ�)

	�ǉ��̃��C�u�����p�X�ɂ́A
	mysql-connector-c-noinstall-6.0.2-win32\lib
	���w��B

	�ǉ��̃��C�u�����t�@�C���́A
	libmysql.lib
	��ǉ��B

	DLL�� mysql-connector-c-noinstall-6.0.2-win32\lib\libmysql.dll ��
	���s�t�@�C���Ɠ����ꏊ�ɂ����Ďg�p����B
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
		// ����
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
		// ���s
		_tcout << _T("real_query error ") << mysql.m_errno << _T(" : ") << mysql.m_error << _T("\n");
	}

	//return abs((int)ts.totalMilliseconds());
	return abs((int)ts.totalSeconds());
	//return abs((int)ts.totalMinutes());
}
