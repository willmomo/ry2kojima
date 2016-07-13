//=============================================================================
//
//=============================================================================

#include "kjmlib.h"


//=============================================================================
// kjm::log クラス
//=============================================================================
//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
kjm::log::log() {
	InitializeCriticalSection(&this->m_cs);

	_tstring sTemp = kjm::util::get_environment_variable(_T("TEMP"));
	_tstring sFile = kjm::util::path_find_file_name(kjm::util::get_module_file_name());

	this->m_logName = sTemp;
	this->m_logName = kjm::util::path_append(this->m_logName, kjm::util::path_remove_extension(sFile));
	this->m_logName = kjm::util::path_append(this->m_logName, kjm::util::path_rename_extension(sFile, _T(".log")));
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
kjm::log::~log() {
	DeleteCriticalSection(&this->m_cs);
}

//-----------------------------------------------------------------------------
// ログに書く
//-----------------------------------------------------------------------------
void kjm::log::write(const _tstring& msg) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	EnterCriticalSection(&this->m_cs);

	kjm::util::create_directory(kjm::util::path_remove_file_spec(this->m_logName));

	FILE* fp = _tfopen(this->m_logName.c_str(), _T("a+t"));
	if (fp) {
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d [%04X:%04X] "),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			GetCurrentProcessId(), GetCurrentThreadId());

		_ftprintf(fp, _T("%s\n"), msg.c_str());
		fclose(fp);
	}

	LeaveCriticalSection(&this->m_cs);
}


//=============================================================================
// kjm::util クラス
//=============================================================================
//-----------------------------------------------------------------------------
// CreateDirectory 関数のラッパー
//-----------------------------------------------------------------------------
bool kjm::util::create_directory(const _tstring& path) {
	if (kjm::util::path_is_root(path))		return true;	// ルートを指定されたら作るまでもない
	if (kjm::util::path_file_exists(path))	return true;	// 存在するフォルダを指定されたら作るまでもない

	if (kjm::util::create_directory(kjm::util::path_remove_file_spec(path))) {
		if (CreateDirectory(path.c_str(), NULL)) {
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// GetEnvironmentVariable 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::get_environment_variable(const _tstring& env) {
	std::vector<TCHAR> work(GetEnvironmentVariable(env.c_str(), NULL, 0));
	GetEnvironmentVariable(env.c_str(), &work[0], work.size());
	return &work[0];
}

//-----------------------------------------------------------------------------
// GetModuleFileName 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::get_module_file_name() {
	std::vector<TCHAR> work;

	do {
		work.resize(work.size() + 128);
	} while (GetModuleFileName(NULL, &work[0], work.size()) == work.size());

	return &work[0];
}

//-----------------------------------------------------------------------------
// GetPrivateProfileString 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::get_private_profile_string(const _tstring& src, const _tstring& key, const _tstring& def, const _tstring& ini) {
	std::vector<TCHAR> work;

	do {
		work.resize(work.size() + 128);
	} while (GetPrivateProfileString(src.c_str(), key.c_str(), def.c_str(), &work[0], work.size(), ini.c_str()) == (work.size() - 1));

	return &work[0];
}

//-----------------------------------------------------------------------------
// LoadString 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::load_string(UINT uID) {
	std::vector<TCHAR> work;

	do {
		work.resize(work.size() + 128);
	} while (LoadString(NULL, uID, &work[0], work.size()) == (work.size() - 1));

	return &work[0];
}

//-----------------------------------------------------------------------------
// PathAppend 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::path_append(const _tstring& path, const _tstring& more) {
	std::vector<TCHAR> work(MAX_PATH);
	PathAppend(lstrcpy(&work[0], path.c_str()), more.c_str());
	return &work[0];
}

//-----------------------------------------------------------------------------
// PathFileExists 関数のラッパー
//-----------------------------------------------------------------------------
bool kjm::util::path_file_exists(const _tstring& path) {
	return (PathFileExists(path.c_str()) != FALSE) ? true : false;
}

//-----------------------------------------------------------------------------
// PathFindFileName 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::path_find_file_name(const _tstring& path) {
	return PathFindFileName(path.c_str());
}

//-----------------------------------------------------------------------------
// PathIsRoot 関数のラッパー
//-----------------------------------------------------------------------------
bool kjm::util::path_is_root(const _tstring& path) {
	return (PathIsRoot(path.c_str()) != FALSE) ? true : false;
}

//-----------------------------------------------------------------------------
// PathRemoveExtension 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::path_remove_extension(const _tstring& path) {
	TCHAR work[MAX_PATH];
	PathRemoveExtension(lstrcpy(work, path.c_str()));
	return &work[0];
}

//-----------------------------------------------------------------------------
// PathRenameExtension 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::path_rename_extension(const _tstring& path, const _tstring& ext) {
	std::vector<TCHAR> work(MAX_PATH);
	PathRenameExtension(lstrcpy(&work[0], path.c_str()), ext.c_str());
	return &work[0];
}

//-----------------------------------------------------------------------------
// PathRemoveFileSpec 関数のラッパー
//-----------------------------------------------------------------------------
_tstring kjm::util::path_remove_file_spec(const _tstring& path) {
	std::vector<TCHAR> work(MAX_PATH);
	PathRemoveFileSpec(lstrcpy(&work[0], path.c_str()));
	return &work[0];
}

//-----------------------------------------------------------------------------
// フルパスからファイル名部分(拡張子除く)を取り出す
//-----------------------------------------------------------------------------
_tstring kjm::util::path_split_fname(const _tstring& path) {
	return kjm::util::path_remove_extension( kjm::util::path_find_file_name(path) );
}


//=============================================================================
// kjm::thread クラス
//=============================================================================
//-----------------------------------------------------------------------------
// スレッドの開始
//-----------------------------------------------------------------------------
bool kjm::thread::begin() {
	this->m_loop = true;
	this->m_hThread = (HANDLE)_beginthreadex(NULL, 0, kjm::thread::worker_func, this, 0, NULL);

	return (this->m_hThread != NULL) ? true : false;
}

//-----------------------------------------------------------------------------
// スレッド関連変数を閉じる
//-----------------------------------------------------------------------------
void kjm::thread::close() {
	if (this->m_hThread) {
		CloseHandle(this->m_hThread);
	}

	this->m_hThread = NULL;
	this->m_loop = false;
}

//-----------------------------------------------------------------------------
// スレッドを終了
//-----------------------------------------------------------------------------
void kjm::thread::end() {
	this->m_loop = false;
}

//-----------------------------------------------------------------------------
// 終了指示が来ているかどうかを調べる
//-----------------------------------------------------------------------------
bool kjm::thread::is_end() {
	return !this->m_loop;
}

//-----------------------------------------------------------------------------
// スレッドを再開
//-----------------------------------------------------------------------------
DWORD kjm::thread::resume() {
	return ResumeThread(this->m_hThread);
}

//-----------------------------------------------------------------------------
// スレッドを一時中断
//-----------------------------------------------------------------------------
DWORD kjm::thread::suspend() {
	return SuspendThread(this->m_hThread);
}

//-----------------------------------------------------------------------------
// スレッド終了まで待つ
//-----------------------------------------------------------------------------
void kjm::thread::wait() {
	WaitForSingleObject(this->m_hThread, INFINITE);
}

//-----------------------------------------------------------------------------
// 何もしないダミースレッド(一般実装)
//-----------------------------------------------------------------------------
unsigned kjm::thread::worker() {
	while (!this->is_end()) {
		Sleep(200);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 共通ワーカースレッド
//-----------------------------------------------------------------------------
unsigned kjm::thread::worker_func(void* parg) {
	return ((kjm::thread*)parg)->worker();
}
