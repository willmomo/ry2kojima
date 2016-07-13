#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <signal.h>

SOCKET g_listen_sock;

void send_string(SOCKET s, const char* p) {
	send(s, p, strlen(p), 0);
}

void doMain() {
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	g_listen_sock = listen_sock;

	sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	my_addr.sin_port = htons(7);
	bind(listen_sock, (sockaddr*)&my_addr, sizeof(my_addr));

	listen(listen_sock, SOMAXCONN);

	sockaddr_in cl_addr;
	int cl_addrlen = sizeof(cl_addr);
	SOCKET accept_sock;
	while ((accept_sock = accept(listen_sock, (sockaddr*)&cl_addr, &cl_addrlen)) != INVALID_SOCKET) {
		char buf[8192];
		int ret = recv(accept_sock, buf, sizeof(buf)-1, 0);
		send(accept_sock, buf, ret, 0);

		//closesocket(accept_sock);
	}

	closesocket(listen_sock);
}

void SignalHandler(int signal) {
    if (signal == SIGINT) {
		closesocket(g_listen_sock);
    }
}

int main() {
	typedef void (*SignalHandlerPointer)(int);

    SignalHandlerPointer previousHandler;
    previousHandler = signal(SIGINT, SignalHandler);

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	doMain();

	WSACleanup();
	return 0;
}
