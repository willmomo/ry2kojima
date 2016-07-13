//-----------------------------------------------------------------------------
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

namespace kjm {
	//-------------------------------------------------------------------------
	// �^�X�N�g���C�A�C�R���������N���X
	//-------------------------------------------------------------------------
	class taskTrayIcon {
	public:
		// �R���X�g���N�^
		taskTrayIcon() { memset(&m_nid, 0, sizeof(m_nid)); };

		// �f�X�g���N�^
		virtual ~taskTrayIcon() { deleteIcon(); };

		// �^�X�N�g���C�ɃA�C�R����ǉ�����
		BOOL addIcon(HWND hwnd, UINT uID, HICON hIcon, UINT uCallback, LPCTSTR pszTip);

		// �^�X�N�g���C����A�C�R������菜��
		BOOL deleteIcon();

	private:
		// �R�s�[�E����͋֎~
		taskTrayIcon(const taskTrayIcon& src) {};
		taskTrayIcon& operator =(const taskTrayIcon& src) { return *this; };

	private:
		NOTIFYICONDATA m_nid;
	};
}
