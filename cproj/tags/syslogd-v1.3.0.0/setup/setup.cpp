//---------------------------------------------------------------------
// syslogd.exeをインストールするための超簡単プログラム
//---------------------------------------------------------------------
#include "setup.h"

//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR cmd[MAX_PATH];
	kjm::process proc;

	// インストール済みの SyslogdService を停止する
	if (proc.run(lstrcpy(cmd, _T("net stop SyslogdService")))) {
		proc.wait();
		proc.close();
	}

	// コピー先ファイル名を決定
	_tstring dstFile = kjm::util::path_append(
		kjm::util::get_environment_variable(_T("windir")).c_str(), _T("system32\\syslogd.exe"));

	// syslogd.exe をコピーする
	if (!CopyFile(_T("syslogd.exe"), dstFile.c_str(), FALSE)) {
		MessageBox(NULL, _T("ファイルのコピーに失敗しました。"), _T("syslogd setup"), MB_OK | MB_ICONERROR);
		return -1;
	}

	// syslogd.exe をサービスに登録する
	if (proc.run(lstrcat(lstrcpy(cmd, dstFile.c_str()), _T(" install")))) {
		proc.wait();
		proc.close();
	} else {
		MessageBox(NULL, _T("サービスの登録に失敗しました。"), _T("syslogd setup"), MB_OK | MB_ICONERROR);
		return -1;
	}

	// syslogd.exe をスタートさせる
	if (proc.run(lstrcpy(cmd, _T("net start SyslogdService")))) {
		proc.wait();
		proc.close();
	} else {
		MessageBox(NULL, _T("サービスの開始に失敗しました。"), _T("syslogd setup"), MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}
