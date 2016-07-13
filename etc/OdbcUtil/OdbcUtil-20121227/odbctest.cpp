// odbctest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <windows.h>
#include <tchar.h>


#include <vector>
#include <string>

#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	C_SETLOCALE_JAPAN;

	std::vector<kjm::_tstring> drivers;

	kjm::odbcUtil::getDriverList(drivers);

	for (int i = 0; i < drivers.size(); i++) {
		_tprintf(_T("%s\n"), drivers[i].c_str());
	}

	_tprintf(_T("find('foxpro'): %s\n"), kjm::odbcUtil::findDriver(_T("foxpro")).c_str());
	_tprintf(_T("find('sql server'): %s\n"), kjm::odbcUtil::findDriver(_T("sql server")).c_str());
	_tprintf(_T("find('sql server'): %s\n"), kjm::odbcUtil::findDriver(_T("OraHome92")).c_str());

	return 0;
}

