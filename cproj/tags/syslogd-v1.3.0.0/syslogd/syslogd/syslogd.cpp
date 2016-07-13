#include "syslogd.h"

// ログの出力先(念のために初期フォルダは指定しておく)
_tstring s_logPath = kjm::util::path_append(kjm::util::get_environment_variable(_T("TEMP")).c_str(), _T("syslogd"));

//---------------------------------------------------------------------
// 受信データをログに残す
//---------------------------------------------------------------------
void Logging(const sockaddr_in* from, LPCTSTR pszText) {
	EnterCriticalSection(&g_criticalSection);

	SYSTEMTIME st;
	GetLocalTime(&st);

	// 出力ファイル名の作成
	///_tstring tempPath = kjm::util::get_environment_variable(_T("TEMP"));
	///tempPath = kjm::util::path_append(tempPath.c_str(), _T("syslogd"));

	static DWORD s_lastDelTime = 0;	// 最終削除日時
	if ((s_lastDelTime == 0) ||
		((GetTickCount() - s_lastDelTime) > 24 * 60 * 60 * 1000)) {

		int nSave = GetPrivateProfileInt(_T("option"), _T("savedays"), 90,
			kjm::util::path_rename_extension(kjm::util::get_module_filename().c_str(), _T(".ini")).c_str());

		kjm::util::cleanup_dir(s_logPath.c_str(), nSave);

		s_lastDelTime = GetTickCount();
	}

	// ファイル名部分の作成
	TCHAR fname[MAX_PATH];
	wsprintf(fname, _T("syslogd%04d%02d%02d.log"), st.wYear, st.wMonth, st.wDay);

	// フルパス名の作成
	_tstring fullPath = kjm::util::path_append(s_logPath.c_str(), fname);

	FILE* fp = _tfopen(fullPath.c_str(), _T("a+"));
	if (fp == NULL && errno == ENOENT) {
		// フォルダが無くて失敗しているようなので、フォルダの作成して
		// 再度、オープンを試みる
		CreateDirectory(s_logPath.c_str(), NULL);
		fp = _tfopen(fullPath.c_str(), _T("a+"));
	}

	if (fp) {
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d "), 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		_ftprintf(fp, _T(" [%04X] "), GetCurrentThreadId());
		if (from == NULL) {
			_ftprintf(fp, _T("%-16s %5u "), _T("0.0.0.0"), 0);
		} else {
			_ftprintf(fp, _T("%-16s %5u "), inet_ntoa(from->sin_addr), ntohs(from->sin_port));
		}

		_ftprintf(fp, _T("%s\n"), pszText);

		fclose(fp);
	}

	LeaveCriticalSection(&g_criticalSection);
}

//---------------------------------------------------------------------
// 受信データをバイナリデータとしてファイルに残す
//---------------------------------------------------------------------
void LoggingBinary(const sockaddr_in* from, char* pData, size_t size) {
	static DWORD s_seq = 0;

	SYSTEMTIME st;
	GetLocalTime(&st);

	// 出力ファイル名の作成
	///_tstring tempPath = kjm::util::get_environment_variable(_T("TEMP"));
	///tempPath = kjm::util::path_append(tempPath.c_str(), _T("syslogd"));

	// ファイルの削除は、Logging関数に任せる

	// ファイル名部分の作成
	// ミリ秒まで同じでも、シーケンス番号が変わるので、ファイル名の重複は無いものとして扱う。
	TCHAR fname[MAX_PATH];
	wsprintf(fname, _T("syslogd%04d%02d%02d_%02d%02d%02d_%03d_%u.bin"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, s_seq++);

	// フルパス名の作成
	_tstring fullPath = kjm::util::path_append(s_logPath.c_str(), fname);

	FILE* fp = _tfopen(fullPath.c_str(), _T("wb"));
	if (fp == NULL && errno == ENOENT) {
		// フォルダが無くて失敗しているようなので、フォルダの作成して
		// 再度、オープンを試みる
		CreateDirectory(s_logPath.c_str(), NULL);
		fp = _tfopen(fullPath.c_str(), _T("wb"));
	}

	if (fp) {
		fwrite(pData, sizeof(char), size, fp);
		fclose(fp);

		TCHAR work[512];
		wsprintf(work, _T("%4d バイトのバイナリデータを受信 '%s' に保存。"), size, fname);
		Logging(from, work);
	}
}


//---------------------------------------------------------------------
// 受信したデータがバイナリかどうか調べる
//---------------------------------------------------------------------
bool is_binary(const char* p, size_t size) {
	for (int i = 0; i < size; i++) {
		if (_ismbcprint(p[i] & 0xff) == 0) {
			// バイナリの可能性あり
			if (_ismbblead(p[i] & 0xff) != 0 && _ismbbtrail(p[i+1] & 0xff) != 0) {
				// 二バイト文字だったので、一文字飛ばして次へ
				i += 1;
			} else {
				// 二バイト文字ではなかったので、バイナリと判断
				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------
// syslogの心臓部
//---------------------------------------------------------------------
unsigned syslogd_thread::worker() {
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		return 1;
	}

	//	タイムアウトの設定
	//	ミリ秒単位で設定
	int timeout = 300;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

	sockaddr_in myaddr;
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(514);

	if (bind(s, (sockaddr*)&myaddr, sizeof(myaddr)) != 0) {
		MessageBox(NULL,
			_T("bindに失敗しました。"),
			_T("Syslogd Serviceエラー"),
			MB_OK | MB_ICONERROR | MB_SERVICE_NOTIFICATION);
		return 2;
	}

	char buf1[8192];
	wchar_t buf2[8192];
	char buf3[8192];
	sockaddr_in from;
	int fromlen;

	Logging(NULL, _T("---- logging 開始 ----"));

	// 受信する文字列の文字コードを規定する。今は、UTF8か、SJISのどちらか。
	// 受信先がTeraStationの時、UTF8を指定する。
	_tstring charset = kjm::util::get_private_profile_string(_T("option"), _T("charset"), _T(""),
		kjm::util::path_rename_extension(kjm::util::get_module_filename().c_str(), _T(".ini")).c_str());

	while (still_loop()) {
		fromlen = sizeof(from);
		int ret = recvfrom(s, buf1, sizeof(buf1) - 1, 0, (sockaddr*)&from, &fromlen);

		if (ret > 0) {
			buf1[ret] = '\0';

			if (charset.empty()) {
				if (is_binary(buf1, ret) == false) {
					Logging(&from, buf1);
				} else {
					LoggingBinary(&from, buf1, ret);
				}
			} else {
				MultiByteToWideChar(CP_UTF8, 0, buf1, -1, buf2, sizeof(buf2));
				WideCharToMultiByte(CP_ACP, 0, buf2, -1, buf3, sizeof(buf3), NULL, NULL);

				// 終端の改行を取り除く
				if (buf3[lstrlen(buf3) - 1] == '\n') {
					buf3[lstrlen(buf3) - 1] = '\0';
					--ret;
				}

				Logging(&from, buf3);
			}

		}
	}

	Logging(NULL, _T("---- logging 終了 ----"));

	closesocket(s);

	return 0;
}

//---------------------------------------------------------------------
// スレッドの終了(ログを残す)
//---------------------------------------------------------------------
void syslogd_thread::end() {
	Logging(NULL, _T("---- スレッド終了指示 ----"));
	thread::end();
}

//---------------------------------------------------------------------
// サスペンド(ログを残す)
//---------------------------------------------------------------------
DWORD syslogd_thread::suspend() {
	Logging(NULL, _T("---- 一時停止指示 ----"));
	return thread::suspend();
}

//---------------------------------------------------------------------
// リジューム(ログを残す)
//---------------------------------------------------------------------
DWORD syslogd_thread::resume() {
	Logging(NULL, _T("---- 再開指示 ----"));
	return thread::resume();
}

//---------------------------------------------------------------------
// 終了待機(ログを残す)
//---------------------------------------------------------------------
DWORD syslogd_thread::wait(DWORD dwTimeout) {
	Logging(NULL, _T("---- スレッド終了待機 ----"));
	DWORD dwRet = thread::wait(dwTimeout);
	Logging(NULL, _T("---- スレッド終了 ----"));

	return dwRet;
}
