
/*!
	ICMP.DLL���g�p����ping

	http://www.kt.rim.or.jp/~ksk/wskfaq-ja/examples/dllping.html
*/

#include <winsock2.h>
#include <iostream>

using namespace std;


//---------------------------------------------------------------------
// ICMP.DLL�p�\����
//
typedef struct {
	u_char Ttl;
	u_char Tos;
	u_char Flags;
	u_char OptionsSize;
	u_char* OptionsData;
} IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;

typedef struct {
	DWORD Address;
	u_long Status;
	u_long RoundTripTime;
	u_short DataSize;
	u_short Reserved;
	void* Data;
	IP_OPTION_INFORMATION Options;
} IP_ECHO_REPLY, *PIP_ECHO_REPLY;


//---------------------------------------------------------------------
// ping in ICMP.DLL
//
int icmp_ping(const char* host) {

	// ICMP.DLL �����[�h
	HINSTANCE hinstIcmp = LoadLibrary("ICMP.DLL");

	// �z�X�g������IP�A�h���X���擾
	hostent* phe = gethostbyname(host);

	// IcmpCreateFile, IcmpCloseHandle, IcmpSendEcho �̃A�h���X���擾����
	typedef HANDLE (WINAPI* ICMP_CF)(void);
	typedef BOOL (WINAPI* ICMP_CH)(HANDLE);
	typedef DWORD (WINAPI* ICMP_SE)(HANDLE, DWORD, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);

	ICMP_CF pfnIcmpCreateFile = (ICMP_CF)GetProcAddress(hinstIcmp, "IcmpCreateFile");
	ICMP_CH pfnIcmpCloseHandle = (ICMP_CH)GetProcAddress(hinstIcmp, "IcmpCloseHandle");
	ICMP_SE pfnIcmpSendEcho = (ICMP_SE)GetProcAddress(hinstIcmp, "IcmpSendEcho");

	// ping server ���J��
	HANDLE hIP = pfnIcmpCreateFile();

	// ping �p�P�b�g�̍쐬
	char acPingBuffer[64];
	memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
	IP_ECHO_REPLY* piper = (IP_ECHO_REPLY*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
	piper->Data = acPingBuffer;
	piper->DataSize = sizeof(acPingBuffer);

	// ping �̑��M
	DWORD dwStatus = pfnIcmpSendEcho(hIP, *(DWORD*)phe->h_addr_list[0], acPingBuffer, sizeof(acPingBuffer), NULL, piper, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);
	if (dwStatus != 0) {
		cout << "Addr: " << 
			(int)LOBYTE(LOWORD(piper->Address)) << "." <<
			(int)HIBYTE(LOWORD(piper->Address)) << "." <<
			(int)LOBYTE(HIWORD(piper->Address)) << "." <<
			(int)HIBYTE(HIWORD(piper->Address)) << ", " <<
			"RTT: " << (int)(piper->RoundTripTime) << "ms, " <<
			"TTL: " << (int)(piper->Options.Ttl) << endl;
	} else {
		cout << "ping error." << endl;
	}

	// �I������
	GlobalFree(piper);
	FreeLibrary(hinstIcmp);

	return 0;
}


//---------------------------------------------------------------------
// program start.
//
int main(int argc, char** argv) {

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	int ret = icmp_ping(argv[1]);

	WSACleanup();
	return ret;
}
