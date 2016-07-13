

#pragma once

#if !defined( __KJM_WINDOW_H )
#define __KJM_WINDOW_H


#include <windows.h>
#include <windowsx.h>


namespace kjm {

	class window {

	protected:
		WNDCLASS m_wc;
		HWND m_hwnd;

	public:
		window() {};
		virtual ~window() {};

	public:
		BOOL show( int nCmdShow ) { return ::ShowWindow( m_hwnd, nCmdShow ); };
		BOOL move( int x, int y, int nWidth, int nHeight, BOOL bRepaint ) { return ::MoveWindow( m_hwnd, x, y, nWidth, nHeight, bRepaint ); };

	public:
		virtual ATOM regist( HINSTANCE hInstance, LPCTSTR lpszClassName ) { 
			m_wc.lpfnWndProc = common_window_proc;
			m_wc.hInstance = hInstance;
			m_wc.cbWndExtra += sizeof( window* );
			m_wc.lpszClassName = lpszClassName;
			return ::RegisterClass( &m_wc ); 
		};
		virtual BOOL create( LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, window* hWndParent, HMENU hMenu) {
			return ( ( m_hwnd = ::CreateWindow( m_wc.lpszClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, 
				( hWndParent ) ? hWndParent->m_hwnd : NULL, hMenu, m_wc.hInstance, this ) ) != NULL );
		};
		virtual BOOL destroy() { return ::DestroyWindow( m_hwnd ); };
		virtual LRESULT window_proc( UINT msg, WPARAM wParam, LPARAM lParam ) {
			switch ( msg ) {
			case WM_CREATE:
				return on_create( ( LPCREATESTRUCT )lParam );

			case WM_ENDSESSION:
				on_end_session( ( BOOL )wParam, lParam );
				return 0;

			case WM_DESTROY:
				on_destroy();
				return 0;

			case WM_SIZE:
				on_size( wParam, LOWORD( lParam ), HIWORD( lParam ) );
				return 0;

			case WM_COMMAND:
				on_command( HIWORD( wParam ), LOWORD( wParam ), ( HWND )lParam );
				return 0;
			}

			return ::DefWindowProc( m_hwnd, msg, wParam, lParam );
		};
		virtual LRESULT on_create( LPCREATESTRUCT lpcs ) { return 0; };
		virtual void on_destroy() { ::PostQuitMessage( 0 ); };
		virtual void on_end_session( BOOL fEndSession, LONG fLogOff ) { destroy(); };
		virtual void on_size( UINT fwSizeType, int nWidth, int nHeight ) {};
		virtual void on_command( WORD wNotifyCode, WORD wID, HWND hwndCtl ) {};

		friend LRESULT CALLBACK common_window_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
	};

	// ListBox window
	class listbox : public window {
	public:
		listbox() {
			m_wc.lpszClassName = "LISTBOX";
		};
		virtual ~listbox() {};

	};
}

#endif
