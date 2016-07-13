/**
 */
#pragma once


#include <windows.h>
#include "kjmlib.h"
#include "unlha32.h"


/**
 */
class MainDlg : public kjm::Dialog {
public:
	// デフォルト コンストラクタ
	MainDlg() {};

	// デストラクタ
	virtual ~MainDlg() {};

	// WM_INITDIALOG メッセージ
	virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lInitParam);

	// WM_COMMAND メッセージ
	virtual BOOL OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl);

	// WM_TIMER メッセージ
	virtual BOOL OnTimer(WPARAM wTimerID, TIMERPROC* tmprc);

private:
	kjm::Process m_procExp;	// exp コマンド実行用
	DWORD m_dwStart;		// コマンドを実行した時間
};
