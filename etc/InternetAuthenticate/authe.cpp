#include <windows.h>
#include <wininet.h>
#include <stdio.h>

int main()
{
	HINTERNET hSession;
	HINTERNET hService;
	char szUrl[1024];
	char szUsr[1024];
	char szPas[1024];
	char buffer[1024 + 1];

	hSession = InternetOpen("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hSession == NULL) {
		fprintf(stderr, "InternetOpen : error %u\n", GetLastError());
		return -1;
	}

	printf("URLを入力してください: ");
	//fgets(szUrl, sizeof(szUrl), stdin);
	scanf("%s", szUrl);

	hService = InternetOpenUrl(hSession, szUrl, NULL, 0, 0, 0);
	if (hService == NULL) {
		fprintf(stderr, "InternetOpenUrl : error %u\n", GetLastError());
		InternetCloseHandle(hSession);
		return -1;
	}

	printf("ユーザ名を入力してください: ");
	//fgets(szUsr, sizeof(szUsr), stdin);
	scanf("%s", szUsr);

	printf("パスワード入力してください: ");
	//fgets(szPas, sizeof(szPas), stdin);
	scanf("%s", szPas);

	if (InternetSetOption(hService, INTERNET_OPTION_USERNAME, szUsr, strlen(szUsr)) == FALSE) {
		fprintf(stderr, "InternetSetOption (username) : error %u\n", GetLastError());
		InternetCloseHandle(hService);
		InternetCloseHandle(hSession);
		return -1;
	}

	if (InternetSetOption(hService, INTERNET_OPTION_PASSWORD, szPas, strlen(szPas)) == FALSE) {
		fprintf(stderr, "InternetSetOption (password) : error %u\n", GetLastError());
		InternetCloseHandle(hService);
		InternetCloseHandle(hSession);
		return -1;
	}

	if (HttpSendRequest(hService, NULL, 0, NULL, 0) == FALSE) {
		fprintf(stderr, "HttpSendRequest : error %u\n", GetLastError());
		InternetCloseHandle(hService);
		InternetCloseHandle(hSession);
		return -1;
	}

	while (1) {
		DWORD dwBytesRead = sizeof(buffer) - 1;

		if (InternetReadFile(hService, buffer, sizeof(buffer) - 1, &dwBytesRead) == FALSE) {
			fprintf(stderr, "InternetReadFile : error %u\n", GetLastError());
			break;
		}

		if (dwBytesRead == 0) {
			break;
		}

		buffer[dwBytesRead] = 0;
		puts(buffer);
	}

	InternetCloseHandle(hService);
	InternetCloseHandle(hSession);

	return 0;
}
