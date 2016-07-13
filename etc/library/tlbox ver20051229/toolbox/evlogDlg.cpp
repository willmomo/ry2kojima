//---------------------------------------------------------------------
// evlogDlg: イベントログ情報を表示するモードレスダイアログ
//

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <shlwapi.h>
#include <sstream>
#include "kjm.h"
#include "resource.h"


#pragma comment( lib, "shlwapi.lib" )


//---------------------------------------------------------------------
#define ID_TIMER	1


static HWND s_hwndDlg = NULL;


//---------------------------------------------------------------------
// evtファイルに出力する
//
static void OutputEVTFile( HWND hwnd ) {

	std::vector<char> fname;
	kjm::GetModuleFileName( NULL, fname );

	fname.resize( fname.size() * 2 );	// エリアを十分すぎる大きさに変更
	::PathRemoveFileSpec( fname.begin() );
	::PathAppend( fname.begin(), "system.evt" );

	kjm::eventlog evlog;
	evlog.OpenEventLog( NULL, _T( "System" ) );
	evlog.BackupEventLog( fname.begin() );
}


//---------------------------------------------------------------------
// データを更新する
//
static void UpdateData( HWND hwnd ) {


	LVITEM lvi;
	memset( &lvi, 0, sizeof( lvi ) );
	lvi.mask = LVIF_TEXT;

	ListView_DeleteAllItems( GetDlgItem( hwnd, IDC_EVLOGLIST ) );

	kjm::eventlog evlog;
	evlog.OpenEventLog( NULL, _T( "System" ) );

	kjm::eventlog_record rec;
	while ( evlog.ReadEventLog( EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, rec ) ) {

		std::stringstream s;
		std::string a;

		s.str( "" );
		s << rec.RecordNumber();
		a = s.str();
		lvi.pszText = (char*)a.c_str();
		int itemIdx = ListView_InsertItem( GetDlgItem( hwnd, IDC_EVLOGLIST ), &lvi );

		s.str( "" );
		s << rec.TimeGenerated().ctime().c_str();
		a = s.str();
		ListView_SetItemText( GetDlgItem( hwnd, IDC_EVLOGLIST ), itemIdx, 1, (char*)a.c_str() );

		s.str( "" );
		s << rec.SourceName().c_str();
		a = s.str();
		ListView_SetItemText( GetDlgItem( hwnd, IDC_EVLOGLIST ), itemIdx, 2, (char*)a.c_str() );

		s.str( "" );
		s << rec.EventID();
		a = s.str();
		ListView_SetItemText( GetDlgItem( hwnd, IDC_EVLOGLIST ), itemIdx, 3, (char*)a.c_str() );

		lvi.iItem = itemIdx + 1;
	}
}


//---------------------------------------------------------------------
// ダイアログと子コントロールの初期化
//
static BOOL OnInitDialog( HWND hwnd ) {

	// 子コントロール初期化

	// リストビューを一行選択、グリッド表示にする
	DWORD ex_style = ListView_GetExtendedListViewStyle( GetDlgItem( hwnd, IDC_EVLOGLIST ) );
	ex_style |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle( GetDlgItem( hwnd, IDC_EVLOGLIST ), ex_style );

	LVCOLUMN col;

	memset( &col, 0, sizeof( col ) );
	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 38;
	col.iSubItem = 0;
	col.pszText = _T( "Rec" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_EVLOGLIST), 0, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 145;
	col.iSubItem = 0;
	col.pszText = _T( "日時" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_EVLOGLIST), 1, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT ;
	col.cx = 138;
	col.iSubItem = 0;
	col.pszText = _T( "ソース" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_EVLOGLIST), 2, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_RIGHT ;
	col.cx = 72;
	col.iSubItem = 0;
	col.pszText = _T( "EvID" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_EVLOGLIST), 3, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_RIGHT ;
	col.cx = 32;
	col.iSubItem = 0;
	col.pszText = _T( "閾値" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_EVLOGLIST), 4, &col );

	// 初回画面更新
	UpdateData( hwnd );

	// タイマー起動
	SetTimer( hwnd, ID_TIMER, 30 * 1000, NULL );

	return TRUE;
}


//---------------------------------------------------------------------
// イベントログ監視ダイアログのダイアログプロシージャ
//
static BOOL CALLBACK DlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	switch ( msg ) {
	case WM_INITDIALOG:
		return OnInitDialog( hwnd );

	case WM_COMMAND:
		{
			WORD wID = LOWORD( wParam );
			if ( wParam == IDOK || wParam == IDCANCEL ) {
				ShowWindow( hwnd, SW_HIDE );
				return TRUE;
			} else if ( wParam == IDC_DEBUG ) {
				OutputEVTFile( hwnd );
				return TRUE;
			}
		}
		break;

	case WM_TIMER:
		if ( IsWindowVisible( hwnd ) )	{
			UpdateData( hwnd );
		}
		return TRUE;

	case WM_DESTROY:
		KillTimer( hwnd, ID_TIMER );
		return FALSE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// イベントログ監視ダイアログの作成
//
BOOL CreateEventLogWatchDialog( HINSTANCE hInstance, HWND hWndParent ) {

	s_hwndDlg = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_EVLOG ), hWndParent, DlgProc );

	if ( s_hwndDlg == NULL ) {
		return FALSE;
	}

	return TRUE;
}


//---------------------------------------------------------------------
// イベントログ監視ダイアログの破棄
//
BOOL DestroyEventLogWatchDialog() {
	
	if ( s_hwndDlg ) {
		DestroyWindow( s_hwndDlg );
		s_hwndDlg = NULL;
	}

	return TRUE;
}


//---------------------------------------------------------------------
// イベントログ監視ダイアログの表示
//
void ShowEventLogWatchDialog( int nCmdShow ) {
	
	if ( s_hwndDlg ) {
		ShowWindow( s_hwndDlg, nCmdShow );
	}
}

