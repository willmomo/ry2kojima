/* version ����� GUID {E51B0ADA-ADCA-4a04-947A-8ED2E34C390C} */
/* --------------------------------------------------------------------
 * OS�Ɉˑ������R�[�h���u�����邽�߂̃w�b�_
 *
 * �ڍׂ́A__os__.c���Q�Ƃ��Ă��������B
 * ----------------------------------------------------------------- */

#if !defined(____OS__H__)
#define ____OS__H__

#if defined(_WIN32)
/* ---------- �������� _WIN32 �p�̃R�[�h ---------- */

#elif defined(__GNUC__)
/* ---------- �������� __GNUC__ �p�̃R�[�h ---------- */

unsigned long GetTickCount();	/* (win32)GetTickCount �֐��̎��� */

#elif defined(_OS9000)
/* ---------- �������� _OS9000 �p�̃R�[�h ---------- */

/* WINDEF.H ����̃R�s�[ */
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

/* WINSOCK.H ����̃R�s�[ */
/*
 * The new type to be used in all
 * instances which refer to sockets.
 */
typedef unsigned int           SOCKET;

/* WINSOCK.H ����̃R�s�[ */
/*
 * This is used instead of -1, since the
 * SOCKET type is unsigned.
 */
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

/* WINSOCK2.H ����̃R�s�[ */
/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

unsigned long GetTickCount();	/* (win32)GetTickCount �֐��̎��� */

#define closesocket	_os_close	/* (win32)closesocket �֐��̃��l�[�� */

typedef struct { int dummy; } WSADATA;	/* (win32)WSADATA �\���̂��_�~�[�ŗp�� */

#define WSAStartup(A, B)		/* (win32)WSAStartup �֐��͕s�v�Ȃ̂ŋ�ɂ��� */

#define WSACleanup()			/* (win32)WSACleanup �֐��͕s�v�Ȃ̂ŋ�ɂ��� */

#define WSAGetLastError()	errno	/* (win32)WSAGetLastError �֐��� errno �ɍ����ւ��� */

#define Sleep(A)	tsleep((A)/10)	/* (win32)Sleep �֐��� tsleep �ɒu�������B�P�ʂ� 10ms �ɐ؂�l�� */
#else
#error you must define _WIN32, __GNUC__ or _OS9000.
#endif

#endif
