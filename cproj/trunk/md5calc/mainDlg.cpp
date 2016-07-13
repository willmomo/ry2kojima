//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "default.h"
#include "md5.h"

class md5calcThread : public kjm::thread {
private:
	kjm::_tstring calcMD5(LPCTSTR fname) {
		m_hasError = false;
		m_progress = 0;
		kjm::_tstring result;

		kjm::cfile targetFile(fname, _T("rb"));
		if (targetFile.is_open()) {
			//MD5 md5(targetFile.get_handle());
			MD5 md5;
			{
				int fsize = _filelength(fileno(targetFile.get_handle()));

				unsigned char buffer[1024];
				int len;
				int totalRead = 0;
				while (len = fread(buffer, 1, 1024, targetFile.get_handle())) {
					md5.update(buffer, len);
					totalRead += len;

					m_progress = (int)(((double)totalRead / (double)fsize) * 100.0);
				}
			}
			md5.finalize();

			char* pchHexDigest = md5.hex_digest();
			if (pchHexDigest) {
				result = kjm::util::toUnicode(pchHexDigest);
				delete[] pchHexDigest;
			}
		} else {
			m_errDescription = kjm::util::sprintf_str(_T("ファイルがオープンできません(error: %d)"), errno);
			m_hasError = true;
		}

		return result;
	}

public:
	// コンストラクタ
	md5calcThread() : m_hasError(false) {}

	// デストラクタ
	virtual ~md5calcThread() {}

	// メイン処理
	virtual unsigned worker() {
		m_md5 = calcMD5(m_targetFile.c_str());
		return 0;
	}

	// m_targetFile へのアクセサ
	void set_TargetFile(const kjm::_tstring& value) {
		m_targetFile = value;
	}
	kjm::_tstring get_TargetFile() const {
		return m_targetFile;
	}

	// m_md5 へのアクセサ
	kjm::_tstring get_MD5() const {
		return m_md5;
	}

	// m_hasErrorへのアクセサ
	bool hasError() const {
		return m_hasError;
	}

	// m_errDescription へのアクセサ
	kjm::_tstring get_ErrDescription() const {
		return m_errDescription;
	}

	// m_progress へのアクセサ
	int get_Progress() const { 
		return m_progress;
	}

private:
	int m_progress;					// MD5計算の進捗
	kjm::_tstring m_targetFile;		// MD5を計算するファイル名
	kjm::_tstring m_md5;			// MD5計算結果
	bool m_hasError;				// MD5計算時のエラー有無
	kjm::_tstring m_errDescription;	// エラー詳細
};

static int m_edit3Color = RGB(0, 255, 0);
static int s_status = 0;	// 0=状態未決定 -1=異常 1=正常
static md5calcThread s_thread;
static HBRUSH s_hbrW = NULL;
static HBRUSH s_hbrR = NULL;
static HBRUSH s_hbrG = NULL;

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static void writeMD5(LPCTSTR fname, LPCTSTR md5) {
	kjm::_tstring md5fname = kjm::util::renameExtension(fname, _T(".md5"));
	kjm::cfile md5file(md5fname, _T("w"));
	if (md5file.is_open()) {
		_ftprintf(md5file.get_handle(), _T("%s *%s\n"), md5, kjm::util::findFileName(fname).c_str());
	}
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static kjm::_tstring readMD5(LPCTSTR fname) {
	kjm::_tstring result;

	kjm::cfile md5file(fname, _T("r"));
	if (md5file.is_open()) {
		kjm::_tstring line;
		if (md5file.getline(line) >= 0) {
			kjm::_tstrings cols = kjm::util::split(line, _T(' '));
			result = cols.at(0);
		}
	}

	return result;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static BOOL OnDropFiles(HWND hwnd, HDROP hDrop) {
	TCHAR szFileName[MAX_PATH];

	UINT uFileNo = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if (uFileNo == 1) {
		DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName));

		SetDlgItemText(hwnd, IDC_FNAME, szFileName);
		SetDlgItemText(hwnd, IDC_EDIT1, _T("計算中"));

		// ファイル名を与えてMD5を計算させる
		// 計算終了は、WM_TIMERで監視
		s_thread.set_TargetFile(szFileName);
		s_thread.run();
		SetTimer(hwnd, 1, 500, NULL);

		EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDCANCEL), FALSE);
		EnableWindow(hwnd, FALSE);
		///InvalidateRect(hwnd, NULL, TRUE);
	}
	DragFinish(hDrop);
	return TRUE;
}

static BOOL OnCommand(HWND hwnd, WORD wID, WORD wNotifyCode) {
	switch (wID) {
		case IDOK:
			{
				s_status = 0;
				SetDlgItemText(hwnd, IDC_FNAME, _T("調べるファイルをドロップしてください。"));
				SetDlgItemText(hwnd, IDC_EDIT1, _T(""));
				SetDlgItemText(hwnd, IDC_EDIT2, _T(""));
				SetDlgItemText(hwnd, IDC_EDIT3, _T(""));
			}
			return TRUE;

		case IDCANCEL:
			{
				TCHAR fname[MAX_PATH], md5[256];
				GetDlgItemText(hwnd, IDC_FNAME, fname, _countof(fname));
				GetDlgItemText(hwnd, IDC_EDIT1, md5, _countof(md5));
				if (lstrcmp(fname, _T("調べるファイルをドロップしてください。")) != 0) {
					writeMD5(fname, md5);
				}
			}
			return TRUE;

	}
	return FALSE;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static BOOL OnCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndCtl) {
	BOOL result = FALSE;

	LONG id = GetWindowLong(hwndCtl, GWL_ID);
	if (id == IDC_EDIT3) {
		SetTextColor(hdc, RGB(0, 0, 0));
		//SetBkColor(hdc, GetSysColor( COLOR_3DFACE ));
		//result = (BOOL)(INT_PTR)GetStockObject( WHITE_BRUSH );
		result = (BOOL)(INT_PTR)(s_status == 0 ? s_hbrW : (s_status < 0 ? s_hbrR : s_hbrG));
	}
	return result;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static BOOL OnTimer(HWND hwnd) {
	if (!s_thread.still_active()) {
		KillTimer(hwnd, 1);

		s_status = 0;
		if (s_thread.hasError()) {
			MessageBox(hwnd, s_thread.get_ErrDescription().c_str(), _T("MD5計算エラー"), MB_ICONERROR);
			SetDlgItemText(hwnd, IDC_EDIT1, _T(""));
			SetDlgItemText(hwnd, IDC_EDIT2, _T(""));
			SetDlgItemText(hwnd, IDC_EDIT3, s_thread.get_ErrDescription().c_str());
			s_status = -1;
		} else {
			kjm::_tstring md5 = s_thread.get_MD5();

			SetDlgItemText(hwnd, IDC_EDIT1, md5.c_str());

			kjm::_tstring fnameMD5 = kjm::util::renameExtension(s_thread.get_TargetFile(), _T(".md5"));
			if (kjm::path::fileExists(fnameMD5)) {
				kjm::_tstring md5_2 = readMD5(fnameMD5.c_str());
				SetDlgItemText(hwnd, IDC_EDIT2, md5_2.c_str());

				if (md5 == md5_2) {
					SetDlgItemText(hwnd, IDC_EDIT3, _T("一致：正常"));
					s_status = 1;
				} else {
					SetDlgItemText(hwnd, IDC_EDIT3, _T("不一致：異常"));
					s_status = -1;
				}
			} else {
				SetDlgItemText(hwnd, IDC_EDIT2, _T(""));
				SetDlgItemText(hwnd, IDC_EDIT3, _T("対応するMD5ファイルなし"));
			}
		}

		EnableWindow(hwnd, TRUE);
		EnableWindow(GetDlgItem(hwnd, IDOK), TRUE);
		EnableWindow(GetDlgItem(hwnd, IDCANCEL), TRUE);
	} else {
		/*
		static int dots = 0;
		switch (dots) {
			case 0:
				SetDlgItemText(hwnd, IDC_EDIT1, _T("計算中"));
				++dots;
				break;
			case 1:
				SetDlgItemText(hwnd, IDC_EDIT1, _T("計算中・"));
				++dots;
				break;
			case 2:
				SetDlgItemText(hwnd, IDC_EDIT1, _T("計算中・・"));
				++dots;
				break;
			default:
				SetDlgItemText(hwnd, IDC_EDIT1, _T("計算中・・・"));
				dots = 0;
				break;
		}
		*/
		SetDlgItemText(hwnd, IDC_EDIT1, kjm::util::sprintf_str(_T("計算中 ... %d%%"), s_thread.get_Progress()).c_str());
	}

	return TRUE;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		//SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
		
		// 明るい赤と明るい緑のブラシを作成
		s_hbrW = CreateSolidBrush(RGB(255, 255, 255));
		s_hbrR = CreateSolidBrush(RGB(255, 200, 200));
		s_hbrG = CreateSolidBrush(RGB(200, 255, 200));
		DragAcceptFiles(hwnd, TRUE);
		return TRUE;

	case WM_CLOSE:
		DeleteObject(s_hbrW);
		DeleteObject(s_hbrR);
		DeleteObject(s_hbrG);
		EndDialog(hwnd, 0);
		return TRUE;

	case WM_COMMAND:
		return OnCommand(hwnd, LOWORD(wParam), HIWORD(wParam));

	case WM_CTLCOLORSTATIC:
		return OnCtlColorStatic(hwnd, (HDC)wParam, (HWND)lParam);

	case WM_DROPFILES:
		return OnDropFiles(hwnd, (HDROP)wParam);

	case WM_TIMER:
		return OnTimer(hwnd);
	}

	return FALSE;
}
