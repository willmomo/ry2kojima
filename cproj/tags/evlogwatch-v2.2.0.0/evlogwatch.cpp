//-----------------------------------------------------------------------------
// イベントログ監視プログラム
//-----------------------------------------------------------------------------
#include <windows.h>

#include "./kjmlib_20131115/kjmlib.h"

#import "basp21.dll" named_guids rename("max", "max_on_basp21") rename("DeleteFile", "DeleteFile_on_basp21")

#include "resource.h"

//-----------------------------------------------------------------------------
// 文字列比較用の関数オブジェクト
//-----------------------------------------------------------------------------
class compareIgnoreCase {
	std::basic_string<TCHAR> m_a;
public:
	compareIgnoreCase(const std::basic_string<TCHAR>& a) : m_a(a) {
		std::transform(m_a.begin(), m_a.end(), m_a.begin(), toupper);
	}
	bool operator()(const std::basic_string<TCHAR>& b) {
		std::basic_string<TCHAR> B = b;
		std::transform(B.begin(), B.end(), B.begin(), toupper);
		return m_a == B;
	}
};

//-----------------------------------------------------------------------------
// basp21を使って、メール送信
//-----------------------------------------------------------------------------
void sendmail(const std::basic_string<TCHAR>& body) {
	kjm::iniFile ini(kjm::util::getModuleIniFile());

	std::basic_string<TCHAR> strServer = ini.getString(_T("SendMail"), _T("Server"));
	std::basic_string<TCHAR> strFrom   = ini.getString(_T("SendMail"), _T("From"));
	std::basic_string<TCHAR> strTo     = ini.getString(_T("SendMail"), _T("To"));

	if (strServer.empty() || strFrom.empty() || strTo.empty()) {
		kjm::GetLog().write(_T("メールの設定がありません。"));
		return;
	}

	BASP21Lib::IBasp21Ptr pBasp21;
	HRESULT hr = pBasp21.CreateInstance(BASP21Lib::CLSID_Basp21);
	if (SUCCEEDED(hr)) {
		///std::basic_string<TCHAR> strTitle = "イベントログチェック(" + kjm::util::getComputerNameEx(ComputerNameNetBIOS) + _T(")");
		std::basic_string<TCHAR> strTitle = _T("イベントログチェック(") + kjm::util::getComputerName() + _T(")");
		_bstr_t ret = pBasp21->SendMail(strServer.c_str(), strTo.c_str(), strFrom.c_str(), strTitle.c_str(), body.c_str(), "");
		if (ret.length() > 0) {
			kjm::GetLog().write(_T("メール送信エラー : %s"), (LPCTSTR)ret);
			kjm::GetLog().write(_T("%s , %s , %s"), strServer.c_str(), strFrom.c_str(), strTo.c_str());
		}
	} else {
		kjm::GetLog().write(_T("BASP21がインストールされていません。"));
	}
}

//-----------------------------------------------------------------------------
// メイン処理
//-----------------------------------------------------------------------------
void doMain() {
	kjm::GetLog().write(_T("doMain() enter."));

	kjm::eventLog evlog;
	kjm::time prvTime;

	kjm::iniFile ini(kjm::util::getModuleIniFile());
	kjm::_tstrings excludeList = kjm::util::split(ini.getString(_T("Watch"), _T("Exclude")), _T(';'));
	
	kjm::_tstring strPrvTime = ini.getString(_T("Watch"), _T("PrevTimeSystem"));	// 前回チェックしたログの時間
	if (!strPrvTime.empty()) {
		kjm::GetLog().write(_T("前回チェックしたログの時間(System) : %s"), strPrvTime.c_str());
	}
	prvTime = (!strPrvTime.empty()) ? kjm::time(strPrvTime.c_str()) : kjm::time::GetCurrentTime().add_second(-24 * 60 * 60);

	kjm::_tstring bodySysEvent;
	if (evlog.openSystemEventLog()) {
		kjm::time updTime;
		kjm::eventLogRecord rec;
		while (evlog.readEventLog(EVENTLOG_BACKWARDS_READ|EVENTLOG_SEQUENTIAL_READ, 0, rec)) {
			if (rec.get_TimeWritten() <= prvTime) {
				break;
			}

			if (updTime.is_empty()) {
				updTime = rec.get_TimeWritten();
				ini.writeString(_T("Watch"), _T("PrevTimeSystem"), updTime.strftime(_T("%Y/%m/%d %H:%M:%S")));	// 前回チェックしたログの時間
			}

			if (rec.get_EventType() == EVENTLOG_ERROR_TYPE) {
				// 種類がエラーのレコードだけ処理

				std::vector<kjm::_tstring>::iterator i = std::find_if(excludeList.begin(), excludeList.end(), 
					compareIgnoreCase(rec.get_SourceName() + _T(",") + kjm::util::toString(rec.get_EventID() & 0xffff)));
				if (i != excludeList.end()) {
					// 除外指定されたエラーコード
					kjm::GetLog().write(
						_T("(SYS)除外しました: %s,%s"),
						rec.get_SourceName().c_str(),
						kjm::util::toString(rec.get_EventID() & 0xffff).c_str());
				} else {
					if (bodySysEvent.empty()) {
						bodySysEvent += _T("----------[ システム ]----------\n");
					}

					bodySysEvent += kjm::util::sprintf_str(_T("日付時刻：%s\n種類：エラー\nソース：%s\nイベントID：%d\n\n"),
						rec.get_TimeWritten().ctime().c_str(), rec.get_SourceName().c_str(), rec.get_EventID() & 0xffff);
				}
			}
		}

		evlog.closeEventLog();
	} else {
		kjm::GetLog().write(_T("システムイベントログが開けません: error %d"), GetLastError());
	}

	strPrvTime = ini.getString(_T("Watch"), _T("PrevTimeApplication"));	// 前回チェックしたログの時間
	if (!strPrvTime.empty()) {
		kjm::GetLog().write(_T("前回チェックしたログの時間(Application) : %s"), strPrvTime.c_str());
	}
	prvTime = (!strPrvTime.empty()) ? kjm::time(strPrvTime.c_str()) : kjm::time::GetCurrentTime().add_second(-24 * 60 * 60);

	kjm::_tstring bodyAppEvent;
	if (evlog.openApplicationEventLog()) {
		kjm::time updTime;
		kjm::eventLogRecord rec;
		while (evlog.readEventLog(EVENTLOG_BACKWARDS_READ|EVENTLOG_SEQUENTIAL_READ, 0, rec)) {
			if (rec.get_TimeWritten() <= prvTime) {
				break;
			}

			if (updTime.is_empty()) {
				updTime = rec.get_TimeWritten();
				ini.writeString(_T("Watch"), _T("PrevTimeApplication"), updTime.strftime(_T("%Y/%m/%d %H:%M:%S")));	// 前回チェックしたログの時間
			}

			if (rec.get_EventType() == EVENTLOG_ERROR_TYPE) {
				// 種類がエラーのレコードだけ処理

				std::vector<kjm::_tstring>::iterator i = std::find_if(excludeList.begin(), excludeList.end(), 
					compareIgnoreCase(rec.get_SourceName() + _T(",") + kjm::util::toString(rec.get_EventID() & 0xffff)));
				if (i != excludeList.end()) {
					// 除外指定されたエラーコード
					kjm::GetLog().write(
						_T("(APP)除外しました: %s,%s"),
						rec.get_SourceName().c_str(),
						kjm::util::toString(rec.get_EventID() & 0xffff).c_str());
				} else {
					if (bodyAppEvent.empty()) {
						bodyAppEvent += _T("----------[ アプリケーション ]----------\n");
					}

					bodyAppEvent += kjm::util::sprintf_str(_T("日付時刻：%s\n種類：エラー\nソース：%s\nイベントID：%d\n\n"),
						rec.get_TimeWritten().ctime().c_str(), rec.get_SourceName().c_str(), rec.get_EventID() & 0xffff);
				}
			}
		}

		evlog.closeEventLog();
	} else {
		kjm::GetLog().write(_T("アプリケーションイベントログが開けません: error %d"), GetLastError());
	}

	if (!bodySysEvent.empty() || !bodyAppEvent.empty()) {
		kjm::GetLog().write(_T("エラー発見：メール送信"));
		sendmail(bodySysEvent + bodyAppEvent + _T("\n\n"));
	}

	kjm::GetLog().write(_T("doMain() leave."));
}

//-----------------------------------------------------------------------------
// プログラム スタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	kjm::GetLog().set_log_path(LOGPATH_EXEPATH, _T("log"));
	kjm::GetLog().set_Stop(false);

	CoInitialize(NULL);

	doMain();

	CoUninitialize();

	return 0;
}

