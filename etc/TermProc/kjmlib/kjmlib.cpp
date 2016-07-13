//---------------------------------------------------------------------
// 小島用ライブラリ
//
// すべてのクラスは、namespace kjmで実装される。
//---------------------------------------------------------------------
#include "kjmlib.h"


//---------------------------------------------------------------------
// 簡易ログクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ログを書く
//
// [引数]
// format  書式制御文字列
//---------------------------------------------------------------------
void kjm::log::writeLog(LPCTSTR format, ...) {
	std::vector<TCHAR> buf(8192);
	va_list marker;

	// ログファイルの名前
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::vector<TCHAR> work2(MAX_PATH);
	_stprintf(work2.begin(), _T("%s%04d%02d%02d.log"), 
		kjm::win32::pathFindFileName(kjm::win32::pathRemoveExtension(kjm::win32::getModuleFileName())).c_str(),
		st.wYear, st.wMonth, st.wDay);

	// ログファイルの作成先を決定
	kjm::_tstring fname = kjm::win32::pathAppend(kjm::win32::getEnvironmentVariable(_T("TEMP")), work2.begin());

	// ファイルが開けるまで、500ms待つ
	DWORD dw = GetTickCount();
	FILE* fp = NULL;
	while ((fp = _tfsopen(fname.begin(), _T("a+"), _SH_DENYWR)) == NULL) {
		if ((GetTickCount() - dw) > 500) break;
		Sleep(0);
	}

	if (fp) {
		_ftprintf(fp, _T("[%04d/%02d/%02d %02d:%02d:%02d.%03d][%04X] "), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, GetCurrentProcessId());
		va_start(marker, format);
		_vftprintf(fp, format, marker);
		va_end(marker);
		_fputts(_T("\n"), fp);

		fclose(fp);
	}
}


//---------------------------------------------------------------------
// WIN32関数の簡易呼び出しクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// GetEnvironmentVariableのラッパー
//
// [引数]
// lpName  環境変数の名前へのポインタ
//
// [戻り値]
// 指定された環境変数が持つ値を返す。
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::getEnvironmentVariable(LPCTSTR lpName) {
	// 必要なバッファのサイズを取得
	DWORD dwSizeNeeded = ::GetEnvironmentVariable(lpName, NULL, 0);
	if (dwSizeNeeded == 0) {
		return _T("");
	}

	std::vector<TCHAR> buf(dwSizeNeeded);
	::GetEnvironmentVariable(lpName, buf.begin(), buf.size());

	return buf.begin();
}


//---------------------------------------------------------------------
// GetModuleFileNameのラッパー
//
// [引数]
// hModule  モジュールのハンドル
//
// [戻り値]
// 実行ファイルの名前を返す。
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::getModuleFileName(HMODULE hModule) {
	std::vector<TCHAR> buf(MAX_PATH);
	while (::GetModuleFileName(hModule, buf.begin(), buf.size()) == buf.size()) {
		buf.resize(buf.size() * 2);
	}

	return buf.begin();
}


//---------------------------------------------------------------------
// PathAppend のラッパー
//
// [引数]
// strPath  元になるフルパスのファイル名
// strMore  結合するパスの名前
//
// [戻り値]
// 結合された新しいファイル名文字列
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::pathAppend(const kjm::_tstring& strPath, const kjm::_tstring& strMore) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// 元になるファイル名をワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// パスをくっつける
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}

//---------------------------------------------------------------------
// PathFindFileName のラッパー
//
// [引数]
// strPath  ファイル名を検索するフルパスのファイル名
//
// [戻り値]
// ファイル名だけの文字列
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::pathFindFileName(const kjm::_tstring& strPath) {
	return PathFindFileName(strPath.c_str());
}


//---------------------------------------------------------------------
// PathRemoveExtension のラッパー
//
// [引数]
// strPath  拡張子を取り除きたいファイル名
//
// [戻り値]
// 拡張子を取り除いた新しい文字列
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::pathRemoveExtension(const kjm::_tstring& strPath) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length());

	// 元になるファイル名をワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// 拡張子を取り除く
	PathRemoveExtension(work.begin());

	return work.begin();
}


//---------------------------------------------------------------------
// PathRenameExtension のラッパー
//
// [引数]
// strPath  元にふぁるフルパスのファイル名
// strExt   置き換える拡張子
//
// [戻り値]
// 拡張子を置き換えた新しいファイル名文字列
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::pathRenameExtension(const kjm::_tstring& strPath, const kjm::_tstring& strExt) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strExt.length());

	// 元になるファイル名をワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// 拡張子置き換え
	PathRenameExtension(work.begin(), strExt.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// ファイル名部分を取り除いて新しいファイル名をくっつける
// PathRemoveFileSpec と PathAppend をくっつけたような関数
//
// [引数]
// strPath  元になるフルパスのファイル名
// strMore  変更する FileSpec
//
// [戻り値]
// 新しいフルパスのファイル名。
//---------------------------------------------------------------------
kjm::_tstring kjm::win32::pathRenameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strMore) {
	// 必要十分なバッファを準備
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// 元になるフルパスをワークにコピー
	lstrcpy(work.begin(), strPath.c_str());

	// ファイル名部分を取り除く
	PathRemoveFileSpec(work.begin());

	// 新しいファイル名をくっつける
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// iniファイルアクセスクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// デフォルトコンストラクタ
//
// [説明]
// 実行ファイル名.ini をデフォルトでアクセスするように調整される。
//---------------------------------------------------------------------
kjm::iniFile::iniFile() {
	this->m_strIniFile = kjm::win32::pathRenameExtension(kjm::win32::getModuleFileName(), _T(".ini"));
}


//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kjm::iniFile& kjm::iniFile::operator =(const kjm::iniFile& rhs) {
	if (this == &rhs)	return *this;	// 同じオブジェクトを指している

	this->m_strIniFile = rhs.m_strIniFile;
	return *this;
}
