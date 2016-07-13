
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>

char *g_hostname = "localhost";
char *g_user = "kojima";
char *g_pass = "Kojima";

int Connect(SOCKET s, const char* pHostname) {

	struct servent *pse;
	struct hostent *phe;
	struct sockaddr_in to;
	int hostaddr;
	int nRecv;
	char msg[1024];

	/* ホスト名からIPアドレスを取得 */
	phe = gethostbyname(pHostname);
	if (phe == NULL) {
		printf("gethostbyname error. (%d)\n", WSAGetLastError());
		return -1;
	}
	memmove(&hostaddr, phe->h_addr, sizeof(hostaddr));

	/* FTPの制御ポートを取得 */
	pse = getservbyname("ftp", "tcp");
	if (pse == NULL) {
		printf("getservbyname error. (%d)\n", WSAGetLastError());
		return -1;
	}

	/* 接続 */
	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
	to.sin_port = pse->s_port;
	to.sin_addr.S_un.S_addr = hostaddr;
	if (connect(s, (struct sockaddr*)&to, sizeof(to)) != 0) {
		printf("connect error. (%d)\n", WSAGetLastError());
		return -1;
	}

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send USER error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendUSER(SOCKET s, const char* pUser) {
	char msg[1024];
	int nRecv;

	sprintf(msg, "USER %s\r\n", pUser);
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send USER error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send USER error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendPASS(SOCKET s, const char* pPass) {
	char msg[1024];
	int nRecv;

	sprintf(msg, "PASS %s\r\n", pPass);
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send PASS error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send PASS error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendCWD(SOCKET s, const char* pDir) {
	char msg[1024];
	int nRecv;

	sprintf(msg, "CWD %s\r\n", pDir);
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send CWD error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send CWD error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendTYPE(SOCKET s, char c) {
	char msg[1024];
	int nRecv;

	sprintf(msg, "TYPE %c\r\n", c);
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send TYPE error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send TYPE error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendQUIT(SOCKET s) {
	char msg[1024];
	int nRecv;

	sprintf(msg, "QUIT\r\n");
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send QUIT error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send QUIT error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendLIST(SOCKET s, SOCKET sDataWait) {
	SOCKET sData;
	char msg[2048];
	int nRecv;
	struct sockaddr_in from;
	int fromlen;

	sprintf(msg, "LIST\r\n");
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send LIST error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send LIST error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	memset(&from, 0, sizeof(from));
	fromlen = sizeof(from);
	sData = accept(sDataWait, (struct sockaddr*)&from, &fromlen);
	printf("---------------------------------------------------\n");
	while (1) {
		nRecv = recv(sData, msg, sizeof(msg) - 1, 0);
		if (nRecv > 0) {
			msg[nRecv] = '\0';
			printf("%s", msg);
		} else {
			break;
		}
	}
	printf("---------------------------------------------------\n");
	closesocket(sData);
	closesocket(sDataWait);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send LIST error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendRETR(SOCKET s, SOCKET sDataWait, const char* fname) {
	SOCKET sData;
	char msg[2048];
	int nRecv;
	struct sockaddr_in from;
	int fromlen;
	int nTotal = 0;

	sprintf(msg, "RETR %s\r\n", fname);
	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send LIST error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send LIST error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	memset(&from, 0, sizeof(from));
	fromlen = sizeof(from);
	sData = accept(sDataWait, (struct sockaddr*)&from, &fromlen);
	printf("---------------------------------------------------\n");
	nTotal = 0;
	while (1) {
		nRecv = recv(sData, msg, sizeof(msg) - 1, 0);
		if (nRecv > 0) {
			nTotal += nRecv;
			msg[nRecv] = '\0';
			printf("size = %d (%d) \n", nTotal, nRecv);
		} else {
			WSAESHUTDOWN;
			printf("recv error %d\n", WSAGetLastError());
			break;
		}
	}
	printf("---------------------------------------------------\n");
	closesocket(sData);
	closesocket(sDataWait);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send LIST error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

int SendPORT(SOCKET s, SOCKET* psData) {

	char msg[1024];
	int nRecv;
	int i;
	struct sockaddr_in me;
	struct sockaddr_in myname;
	int mynamelen;
	BOOL optval = 1;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.S_un.S_addr = INADDR_ANY;

	for (i = 5000 + (time(NULL) % 100); i < 65536; i++) {

		/* データ受信用ソケット作成 */
		*psData = socket(AF_INET, SOCK_STREAM, 0);
		if (*psData == INVALID_SOCKET) {
			printf("socket error. (%d)\n", WSAGetLastError());
			return -1;
		}

		/* ポートを割り当てる */
		me.sin_port = htons(i);
		if (bind(*psData, (struct sockaddr*)&me, sizeof(me)) == 0) {
			break;
		}
	}

	if (i == 65536) {
		printf("bind error %d\n", WSAGetLastError());
		closesocket(*psData);
		return -1;
	}

	if (listen(*psData, 5) != 0) {
		printf("listen error %d\n", WSAGetLastError());
		closesocket(*psData);
		return -1;
	}

	memset(&myname, 0, sizeof(myname));
	mynamelen = sizeof(myname);
	if (getsockname(s, (struct sockaddr*)&myname, &mynamelen) != 0) {
		printf("getsockname error %d\n", WSAGetLastError());
		closesocket(*psData);
		return -1;
	}

	sprintf(msg, "PORT %d,%d,%d,%d,%d,%d\r\n",
		myname.sin_addr.S_un.S_un_b.s_b1,
		myname.sin_addr.S_un.S_un_b.s_b2,
		myname.sin_addr.S_un.S_un_b.s_b3,
		myname.sin_addr.S_un.S_un_b.s_b4,
		i / 256, i % 256);

	if (send(s, msg, strlen(msg), 0) != strlen(msg)) {
		printf("send QUIT error. (%d)\n", WSAGetLastError());
		return -1;
	}

	printf("C: >%s", msg);

	nRecv = recv(s, msg, sizeof(msg) - 1, 0);
	if (nRecv < 0) {
		printf("send QUIT error. (%d)\n", WSAGetLastError());
		return -1;
	}
	msg[nRecv] = '\0';

	printf("S: >%s", msg);

	return 0;
}

void DoMain() {

	char msg[512];
	SOCKET s;
	SOCKET sData = INVALID_SOCKET;

	/* ソケットを開く */
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("socket error. (%d)\n", WSAGetLastError());
		return;
	}

	/* 接続 */
	if (Connect(s, g_hostname) != 0) {
		goto end;
	}

	/* USER 送信 */
	if (SendUSER(s, g_user) != 0) {
		goto end;
	}

	/* PASS 送信 */
	if (SendPASS(s, g_pass) != 0) {
		goto end;
	}

	/* CWD 送信 */
	if (SendCWD(s, "/work") != 0) {
		goto end;
	}

	/* PORT 送信 */
	if (SendPORT(s, &sData) != 0) {
		goto end;
	}

	/* LIST 送信 */
	if (SendLIST(s, sData) != 0) {
		goto end;
	}

	/* TYPE 送信 */
	if (SendTYPE(s, 'I') != 0) {
		goto end;
	}

	Sleep(1500);

	/* PORT 送信 */
	if (SendPORT(s, &sData) != 0) {
		goto end;
	}

	/* RETR 送信 */
	if (SendRETR(s, sData, "a_rec2.lzh") != 0) {
		goto end;
	}

	/* QUIT 送信 */
	if (SendQUIT(s) != 0) {
		goto end;
	}
	
end:
	/* ソケットを閉じる */
	if (closesocket(s) != 0) {
		printf("closesocket error. (%d)\n", WSAGetLastError());
	}
}


int main(int argc, char** argv) {

	WORD wVersion;
	WSADATA wsaData;

	/* winsockの初期化 */
	wVersion = MAKEWORD(2, 0);
	if (WSAStartup(wVersion, &wsaData) != 0) {
		return -1;
	}
	if (wsaData.wVersion != wVersion) {
		return -1;
	}

	srand(time(NULL));

	/* メイン処理 */
	DoMain();

	/* winsockの終了 */
	WSACleanup();

	return 0;
}

