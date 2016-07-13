/*
	http://www.venus.dti.ne.jp/~yoshi-o/NTP/NTP-SNTP_Format.html
*/

#include "./kjmlib-20120604/kjmlib.h"

static int s_loop = 1;		/* sntp_thread Çêßå‰Ç∑ÇÈÉtÉâÉO */

/*---------------------------------------------------------------------
	sntp_thread Çí‚é~Ç≥ÇπÇÈ
---------------------------------------------------------------------*/
void worker_thread_end() {
	s_loop = 0;
}

/*---------------------------------------------------------------------
	sntp client
---------------------------------------------------------------------*/
unsigned __stdcall worker_thread(void* argv) {
	s_loop = 1;
	do {
		Sleep(400);
	} while (s_loop);

	return 0;
}
