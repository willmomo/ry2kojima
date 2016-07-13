
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

int main(int argc, char** argv) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	char myname[80];
	gethostname(myname, sizeof(myname));
	
	printf("my name is %s.\n", myname);

	struct hostent* myaddrs = gethostbyname(myname);

	for (int i = 0; myaddrs->h_addr_list[i] != NULL; i++) {
		struct in_addr addr;
		memcpy(&addr, myaddrs->h_addr_list[i], sizeof(addr));
		printf("my addr is %s.\n", inet_ntoa(addr));
	}


	WSACleanup();
	return 0;
}
