//-----------------------------------------------------------------------------
// MergeIni 二つのiniファイルをマージするプログラム
//-----------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include "kjmsub.h"

#include "resource.h"


kjm::log g_log;	// ログ用のグローバル変数


//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_log.set_Ident(_T("MRGINI"));
	g_log.writeInfo(_T("----[ %s ]---- start."), __argv[0]);
	g_log.deleteOldLog();
	kjm::util::set_logObj(&g_log);

	if (__argc != 3) {
		g_log.writeError(_T("MergeIni <マージ先ファイル名> <マージ元ファイル名>"));
	}

	g_log.writeInfo(_T("'%s' の内容を '%s' にマージします。"), __argv[2], __argv[1]);

	kjm::iniFile iniOrg(__argv[1]);
	iniOrg.mergeIniFile(kjm::iniFile(__argv[2]));

	g_log.writeInfo(_T("----[ %s ]---- end."), __argv[0]);
	return 0;
}
