/* version 代わりの GUID {E51B0ADA-ADCA-4a04-947A-8ED2E34C390C} */
/* --------------------------------------------------------------------
 * OSに依存したコードを隔離するためのヘッダ
 *
 * 詳細は、__os__.cを参照してください。
 * ----------------------------------------------------------------- */

#if !defined(____OS__H__)
#define ____OS__H__

#if defined(_WIN32)
/* ---------- ここから _WIN32 用のコード ---------- */

#elif defined(__GNUC__)
/* ---------- ここから __GNUC__ 用のコード ---------- */

unsigned long GetTickCount();	/* (win32)GetTickCount 関数の実装 */

#elif defined(_OS9000)
/* ---------- ここから _OS9000 用のコード ---------- */

/* WINDEF.H からのコピー */
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

/* WINSOCK.H からのコピー */
/*
 * The new type to be used in all
 * instances which refer to sockets.
 */
typedef unsigned int           SOCKET;

/* WINSOCK.H からのコピー */
/*
 * This is used instead of -1, since the
 * SOCKET type is unsigned.
 */
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

/* WINSOCK2.H からのコピー */
/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

unsigned long GetTickCount();	/* (win32)GetTickCount 関数の実装 */

#define closesocket	_os_close	/* (win32)closesocket 関数のリネーム */

typedef struct { int dummy; } WSADATA;	/* (win32)WSADATA 構造体をダミーで用意 */

#define WSAStartup(A, B)		/* (win32)WSAStartup 関数は不要なので空にする */

#define WSACleanup()			/* (win32)WSACleanup 関数は不要なので空にする */

#define WSAGetLastError()	errno	/* (win32)WSAGetLastError 関数は errno に差し替える */

#define Sleep(A)	tsleep((A)/10)	/* (win32)Sleep 関数を tsleep に置き換え。単位は 10ms に切り詰め */
#else
#error you must define _WIN32, __GNUC__ or _OS9000.
#endif

#endif
