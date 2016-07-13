#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <vector>

#include <stdio.h>

class winsock_exception : public exception {
public:
	winsock_exception() {}
	virtual ~winsock_exception() {}
};

class winsock {
public:
	winsock() {}
	~winsock() {}
	static std::string get_hostname() {
		std::vector<char> name(1);
		gethostname(&name[0], name.size());
		int nErr = WSAGetLastError();

		return &name[0];
	}
};

int do_main() {
	std::string name = winsock::get_hostname();

	struct hostent* phe = gethostbyname(name.c_str());

	if (phe != NULL) {
		for (int i = 0; phe->h_addr_list[i] != 0; i++) {
			struct in_addr addr = *(struct in_addr*)phe->h_addr_list[i];

			printf("[%s]\n", inet_ntoa(addr));
		}
	}

	return 0;
}

int main(int argc, char** argv) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	do_main();

	WSACleanup();
	return 0;
}
