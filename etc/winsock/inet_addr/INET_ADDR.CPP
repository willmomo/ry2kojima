// inet_addr.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
	u_short wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;

	if (WSAStartup(wVersionRequested, &wsaData)) {
		cout << "WSAStartup error" << endl;
		return 1;
	}

//	const char* ip_string = "192.168.100.100";
	const char* ip_string = "192.168.100.2";
//	const char* ip_string = "localhost";
	u_long ip_address;

	ip_address = inet_addr(ip_string);
	if (ip_address == INADDR_NONE) {
		struct hostent* phostent;

		phostent = gethostbyname(ip_string);
		if (phostent) {
			ip_address = *(u_long*)(phostent->h_addr_list[0]);
		} else {
			cout << "cannot get host infomation." << endl;
			goto finish;
		}
	} else {
		struct hostent* phostent;
		struct in_addr ia;
		ia.s_addr = ip_address;
		phostent = gethostbyaddr((const char*)&ia, sizeof(ia), AF_INET);
		if (phostent) {
			ip_address = *(u_long*)(phostent->h_addr_list[0]);
		} else {
			cout << "cannot get host infomation." << endl;
			goto finish;
		}
	}

	cout << "ip address = 0x" << hex << ip_address << endl;

finish:
	WSACleanup();

	return 0;
}
