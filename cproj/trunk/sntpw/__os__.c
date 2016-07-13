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
