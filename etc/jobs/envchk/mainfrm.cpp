

#include <windows.h>
#include <windowsx.h>
#include "mainfrm.h"


//---------------------------------------------------------------------
// MainFrmのためのウィンドウを作成
//---------------------------------------------------------------------
BOOL mainfrm::create() {

	return window::create( "mainfrm", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL );
}


//---------------------------------------------------------------------
// MainFrmのためのウィンドウクラスを登録
//---------------------------------------------------------------------
ATOM mainfrm::regist( HINSTANCE hInstance, LPCTSTR lpszClassName ) {

	m_wc.style = CS_HREDRAW | CS_VREDRAW;
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	m_wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	m_wc.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	m_wc.lpszMenuName = MAKEINTRESOURCE( IDR_MAINMENU );

	return window::regist( hInstance, lpszClassName );
}


void mainfrm::on_test_command() {

	kjm::registry reg;
	if ( reg.RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\W32Time\\Parameters", 0, KEY_ALL_ACCESS ) == ERROR_SUCCESS ) {

		std::vector<BYTE> data;
		reg.RegQueryValueEx( "Type", NULL, NULL, data );

		std::string value( (char*)data.begin(), (char*)data.end() );
		MessageBox( m_hwnd, value.c_str(), "", MB_OK );
	}
}

