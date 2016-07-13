#include "kjmsub.h"

#include <iostream>
#include "resource.h"

using namespace std;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
_tstring g_host;	// 送信先ホスト名
short g_port;		// 送信先ポート番号
_tstring g_text;	// 送信する文字列

//-----------------------------------------------------------------------------
// 送信処理
//-----------------------------------------------------------------------------
void doMain() {
	kjm::wsaSocket sock;

	if (sock.createUdpSocket() != 0) {
		return;
	}

	///unsigned int optval;
	///int optlen = sizeof(optlen);
	///getsockopt(s, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&optval, &optlen);
	///printf("max msg size is %d.\n", optval);
	
	sock.sendToEasy(g_text, g_host, g_port);
}

//-----------------------------------------------------------------------------
// 使用法を表示して終了
//-----------------------------------------------------------------------------
void usage() {
	_tout << kjm::util::getModuleFileName() << _T(" ver.") << kjm::util::getFileVersion() << _T("\n");

	_tout << _T("使用法：sendto --host=<ホスト名> [--port=ポート番号] --text=<送信文字列>\n");

	exit(0);
}

//-----------------------------------------------------------------------------
// プログラスタート
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	kjm::wsaSocket::startup();

	kjm::commandLine cl(argc, argv, NULL);

	// 送信先(ホスト名)
	g_host = cl.getOptArg("host");

	// ポート番号
	_tstring tmp = cl.getOptArg("port");
	g_port = (tmp.empty() ? 8108 : _ttoi(tmp.c_str()));

	// 送信文字列
	g_text = cl.getOptArg("text");

	// 送信実行
	doMain();

	kjm::wsaSocket::cleanup();

	return 0;
}

