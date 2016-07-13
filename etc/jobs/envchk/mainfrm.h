

#pragma once

#if !defined( __MAINFRM_H )
#define __MAINFRM_H


#include <vector>
#include "kjm.h"
#include "resource.h"


class mainfrm : public kjm::window {

private:
	kjm::listbox m_listbox;

public:
	mainfrm() {};
	virtual ~mainfrm() {};

public:
	ATOM regist( HINSTANCE hInstance, LPCTSTR lpszClassName );
	BOOL create();

	virtual LRESULT on_create( LPCREATESTRUCT lpcs ) {

		if ( !m_listbox.create( "", LBS_NOINTEGRALHEIGHT|WS_BORDER|WS_CHILD|WS_VISIBLE, 0, 0, 100, 100, this, ( HMENU )1 ) ) {
			return -1;
		}

		return window::on_create( lpcs );
	};

	virtual void on_size( UINT fwSizeType, int nWidth, int nHeight ) {
		m_listbox.move( 0, 0, nWidth, nHeight, TRUE );
	};

	virtual void on_command( WORD wNotifyCode, WORD wID, HWND hwndCtl ) {
		
		if ( wID == IDM_TEST ) {
			on_test_command();
		}
	};

	virtual LRESULT window_proc( UINT msg, WPARAM wParam, LPARAM lParam ) {
		return window::window_proc( msg, wParam, lParam );
	};

public:
	void on_test_command();
};


#endif
