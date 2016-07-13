

#include <windows.h>
#include <windowsx.h>
#include "mainfrm.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	mainfrm wnd;

	if ( !wnd.regist( hInstance, "envchk_mainfrm" ) ) {
		return -1;
	}

	if ( !wnd.create() ) {
		return -1;
	}

	wnd.show( nCmdShow );

	MSG msg;
	while ( true ) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );
		if ( ret == 0 || ret == -1 ) {
			break;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.wParam;
}
