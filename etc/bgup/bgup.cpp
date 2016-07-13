#include <windows.h>
#include <sstream>
#include "../kjmlib3_v1/kjmlib3.h"

using namespace std;

#define LOGC_NAME	"BGUP"

#define SRC_FILE	"c:\\bgup\\BackupTool.exe"
#define DST_FILE	"c:\\mpc2\\BackupTool\\BackupTool.exe"
#define DST_INIFILE	"c:\\mpc2\\BackupTool\\BackupTool.ini"

kl3::log g_log(LOGC_NAME);

string g_strMyFileName = kl3::util::get_module_file_name();		// 自分自身のフルパス名を保持する

//---------------------------------------------------------------------
// bgup.ini [option] BackupPath を取得
// デフォルトは、実行ファイルのパス。
//---------------------------------------------------------------------
string GetIni_Option_BackupPath() {
	char szTemp[MAX_PATH];

	GetPrivateProfileString(
		"option",
		"BackupPath",
		kl3::path::remove_file_spec(g_strMyFileName).c_str(),
		szTemp,
		sizeof(szTemp),
		kl3::path::rename_extension(g_strMyFileName, ".ini").c_str());

	return szTemp;
}

//---------------------------------------------------------------------
// BackupTool.exeの起動を待つ
//---------------------------------------------------------------------
DWORD WaitProcess() {
	g_log.write("[ BackupTool.exe ] が起動するまで待ちます(最大 1 分)。");

	kl3::tool_help th;
	bool bFind = false;
	DWORD dwProcessId = 0;

	DWORD dwStart = GetTickCount();
	while (!bFind) {
		if (th.create_snapshot()) {
			bool bRet = th.process_first();
			while (bRet) {
				if (stricmp("backuptool.exe", th.get_exe_file().c_str()) == 0) {
					bFind = true;
					dwProcessId = th.get_process_id();
					break;
				}

				bRet = th.process_next();
			}
			th.close_handle();
		}

		if ((GetTickCount() - dwStart) > (60 * 1000)) {
			break;
		}

		Sleep(1 * 1000);
	}

	if (dwProcessId != 0) {
		g_log.write("[ BackupTool.exe ] が起動しました。プロセスIDは %d です。", dwProcessId);
	} else {
		g_log.write("[ BackupTool.exe ] が起動してきませんでした。");
	}
		
	return dwProcessId;
}

//---------------------------------------------------------------------
// BackupTool.exeを殺す
//---------------------------------------------------------------------
void KillProcess(DWORD dwProcessId) {
	g_log.write("起動中の BackupTool.exe (プロセスID %d) を強制終了します。", dwProcessId);

	kl3::process::terminate_process(dwProcessId, 0);
}

//---------------------------------------------------------------------
// BackupTool.exe をバックアップする
//---------------------------------------------------------------------
bool BackupFile() {

	// 指定されたバックアップ先をあらかじめ作成しておく

	string strBackupPath = GetIni_Option_BackupPath();
	kl3::util::create_directory(strBackupPath);

	// DST_FILE で指定されたファイルを、所定の場所にバックアップします。
	string strBackupFilename = kl3::util::create_backup_filename(
		kl3::path::append(strBackupPath, kl3::path::find_file_name(DST_FILE))
	);

	g_log.write("[ %s ] を [ %s ] にバックアップします。", DST_FILE, strBackupFilename.c_str());

	if (CopyFile(DST_FILE, strBackupFilename.c_str(), FALSE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("コピーに失敗しました。");
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	// DST_INIFILE で指定されたファイルを、所定の場所にバックアップします。
	strBackupFilename = kl3::util::create_backup_filename(
		kl3::path::append(strBackupPath, kl3::path::find_file_name(DST_INIFILE))
	);

	g_log.write("[ %s ] を [ %s ] にバックアップします。", DST_INIFILE, strBackupFilename.c_str());

	if (CopyFile(DST_INIFILE, strBackupFilename.c_str(), FALSE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("コピーに失敗しました。");
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// BackupTool.exeをコピーする
//---------------------------------------------------------------------
bool UpdateFile() {
	g_log.write("[ %s ] を [ %s ] に移動します。", SRC_FILE, DST_FILE);

	// 削除前に読み取り専用属性を外しておく
	DWORD dwAttr = GetFileAttributes(DST_FILE);
	if (dwAttr == 0xffffffff) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の属性取得に失敗しました。", DST_FILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}
	if (SetFileAttributes(DST_FILE, dwAttr & ~FILE_ATTRIBUTE_READONLY) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の属性設定に失敗しました。", DST_FILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (DeleteFile(DST_FILE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の削除に失敗しました。", DST_FILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (MoveFile(SRC_FILE, DST_FILE) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の移動に失敗しました。", SRC_FILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// BackupTool.iniを更新する
//---------------------------------------------------------------------
bool UpdateIni() {
	g_log.write("INIファイル [ %s ] を更新します。", DST_INIFILE);

	// 削除前に読み取り専用属性を外しておく
	DWORD dwAttr = GetFileAttributes(DST_INIFILE);
	if (dwAttr == 0xffffffff) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の属性取得に失敗しました。", DST_INIFILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}
	if (SetFileAttributes(DST_INIFILE, dwAttr & ~FILE_ATTRIBUTE_READONLY) == FALSE) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の属性設定に失敗しました。", DST_INIFILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (WritePrivateProfileString("option", "DstFile", "mpc2", DST_INIFILE) == 0) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の更新に失敗しました(更新1)。", DST_INIFILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (WritePrivateProfileString("option", "DstFile2", "mpc", DST_INIFILE) == 0) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の更新に失敗しました(更新2)。", DST_INIFILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	if (WritePrivateProfileString("option", "DstDump", "mpc2_dump", DST_INIFILE) == 0) {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の更新に失敗しました(更新3)。", DST_INIFILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		return false;
	}

	return true;
}

//---------------------------------------------------------------------
// BackupTool.exeを起動する
//---------------------------------------------------------------------
bool StartFile() {
	bool bRet;

	g_log.write("バックアップツール [ %s ] を再起動します。", DST_FILE);

	kl3::process proc;
	if (proc.create_process(DST_FILE)) {
		proc.close_handle();

		bRet = true;
	} else {
		DWORD dwErr = GetLastError();

		g_log.write("[ %s ] の起動に失敗しました。", DST_FILE);
		g_log.write("エラー詳細 : %d : %s", dwErr, kl3::util::get_error_message(dwErr).c_str());

		bRet = false;
	}

	return bRet;
}

//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_log.write("▼▼▼▼ [ %s ] 開始 ▼▼▼▼", g_strMyFileName.c_str());

	// 起動直後 60 秒待ちを入れる
	g_log.write("システムが安定するまで 60 秒待機します。");
	Sleep(60 * 1000);

	// c:\bgup\BackupTool.exe が存在するかどうか調べる
	if (kl3::path::file_exists(SRC_FILE)) {

		g_log.write("コピー元のファイル [ %s ] が、あります。", SRC_FILE);

		// BackupTool.exeが起動するのを待つ
		DWORD dwProcId = WaitProcess();

		// BackupTool.exeを終了させる
		if (dwProcId != 0) {
			KillProcess(dwProcId);
		}

		// BackupTool.exe をバックアップする
		if (BackupFile() == false) goto end_p;

		// BackupTool.exeを更新する
		if (UpdateFile() == false) goto end_p;

		// BackupTool.iniを更新する
		if (UpdateIni() == false) goto end_p;

		// オカルト的に 3 秒の待ち時間を入れる
		Sleep(3 * 1000);

		// BackupTool.exeを起動する
		if (dwProcId != 0) {
			StartFile();
		}
	} else {
		g_log.write("コピー元のファイル [ %s ] が、ありません。アプリケーションを終了します。", SRC_FILE);
	}

end_p:

	g_log.write("▲▲▲▲ [ %s ] 終了 ▲▲▲▲", g_strMyFileName.c_str());

	return 0;
}
