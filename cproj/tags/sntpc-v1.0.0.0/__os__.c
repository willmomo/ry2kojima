/*-----------------------------------------------------------------------------
	Visual Studio を使うときは、このソースファイルに、プリンコンパイル済み
	ヘッダを使わないように設定してください。
-----------------------------------------------------------------------------*/
/* version 代わりの GUID {E51B0ADA-ADCA-4a04-947A-8ED2E34C390C} */
/* --------------------------------------------------------------------
 * OSに依存したコードを隔離するためのソース
 *
 * WindowsとOS9000両方で使用できるコードを書くために、依存コードを隔離
 * するためのソースファイルです。
 *
 * 基本的に、__os__で始まる関数を定義し、中を#ifで書き分けます。
 *
 * Windows版を先にコーディングすることが多い関係上、関数名は、Windows
 * のそれであることが多くなっています。
 *
 * -----------------------------------------------------------------
 * bmark , ftpclient
 * で使用しています。
 * ----------------------------------------------------------------- */

#if defined(_WIN32)
#pragma warning(disable : 4996)
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#endif

#if defined(_OS9000)
#include <stddef.h>
#include <sysglob.h>
#include <types.h>
#endif

#include "__os__.h"

/* --------------------------------------------------------------------
 * OSのチックカウントを1ms単位で返す。
 * ----------------------------------------------------------------- */
#if defined(_OS9000)
unsigned long GetTickCount() {
	unsigned long d_ticks;
	_os_getsys(offsetof(sysglobs, d_ticks), 4, (glob_buff *)&d_ticks);

	return(d_ticks * 10);
}
#elif defined(__GNUC__)
unsigned long GetTickCount() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

/* --------------------------------------------------------------------
 * syslog に情報を送信する関数
 * _OS9000用のコードはありません。
 * ----------------------------------------------------------------- */
#if defined(_WIN32) || defined(__GNUC__)
void syslog(const char *fmt, ...) {
	/* 何もしないスタブ */
}
#endif

/* --------------------------------------------------------------------
 * Debug用printf関数
 * _OS9000用のコードはありません。
 * ----------------------------------------------------------------- */
#if defined(_WIN32) || defined(__GNUC__)
void DebugPrintf(int level, const char *fmt, ...) {
	va_list arg;

	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
}
#endif

/* --------------------------------------------------------------------
 * 受信済みデータ数を返す関数
 * _OS9000用のコードはありません。
 * ----------------------------------------------------------------- */
#if defined(_WIN32) || defined(__GNUC__)
int _gs_rdy(int s) {
	int ret = 0;
	char buf[8192];

	ret = recv(s, &buf[0], 8192, MSG_PEEK);
	return ret;
}
#endif
