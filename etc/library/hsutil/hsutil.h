#if !defined(__HSUTIL_H)
#define __HSUTIL_H

// ���C�u�����ɕK�v�ȃw�b�_

#include <windows.h>
#include <tchar.h>

// export�p�}�N��(def file�ɏ����Ă��邩��s�v�Ƃ����Εs�v)

#ifdef HSUTIL_EXPORTS
#define HSUTIL_API __declspec(dllexport)
#else
#define HSUTIL_API __declspec(dllimport)
#endif

// HsFindWindow �p�萔

#define HSFW_FLAG_SAMEFINDWINDOW	0x00	// FindWindow�Ɠ�������ɂ���
#define HSFW_FLAG_MATCHHEAD			0x01	// �������擪����̕�����v�Ŕ���

HSUTIL_API HWND WINAPI HsFindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwFlags);

#endif
