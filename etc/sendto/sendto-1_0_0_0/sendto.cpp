#include "kjmsub.h"

#include <iostream>
#include "resource.h"

using namespace std;

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
_tstring g_host;	// ���M��z�X�g��
short g_port;		// ���M��|�[�g�ԍ�
_tstring g_text;	// ���M���镶����

//-----------------------------------------------------------------------------
// ���M����
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
// �g�p�@��\�����ďI��
//-----------------------------------------------------------------------------
void usage() {
	_tout << kjm::util::getModuleFileName() << _T(" ver.") << kjm::util::getFileVersion() << _T("\n");

	_tout << _T("�g�p�@�Fsendto --host=<�z�X�g��> [--port=�|�[�g�ԍ�] --text=<���M������>\n");

	exit(0);
}

//-----------------------------------------------------------------------------
// �v���O���X�^�[�g
//-----------------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	STL_SETLOCALE_JAPAN;

	kjm::wsaSocket::startup();

	kjm::commandLine cl(argc, argv, NULL);

	// ���M��(�z�X�g��)
	g_host = cl.getOptArg("host");

	// �|�[�g�ԍ�
	_tstring tmp = cl.getOptArg("port");
	g_port = (tmp.empty() ? 8108 : _ttoi(tmp.c_str()));

	// ���M������
	g_text = cl.getOptArg("text");

	// ���M���s
	doMain();

	kjm::wsaSocket::cleanup();

	return 0;
}

