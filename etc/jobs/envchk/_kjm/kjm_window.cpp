

#include "kjm_window.h"



//---------------------------------------------------------------------
// MainFrmのためのウィンドウプロシージャ
//---------------------------------------------------------------------
LRESULT CALLBACK kjm::common_window_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	if ( msg == WM_CREATE ) {

		LPCREATESTRUCT lpcs = ( LPCREATESTRUCT )lParam;
		window* p = ( window* )lpcs->lpCreateParams;
		p->m_hwnd = hwnd;

		::SetWindowLong( hwnd, p->m_wc.cbWndExtra - sizeof( window* ), ( LONG )p );
	}

	window* p = ( window* )::GetWindowLong( hwnd, 0 );
	if ( p ) {
		return p->window_proc( msg, wParam, lParam );
	} else {
		return ::DefWindowProc( hwnd, msg, wParam, lParam );
	}
}
