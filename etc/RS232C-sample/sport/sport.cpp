#include <windows.h>
#include <stdio.h>

#include <conio.h>

int main(int argc, char** argv) {

retry_open:
	HANDLE hFile = CreateFile("COM8", GENERIC_READ|GENERIC_WRITE,
		0, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) {	// オープンに失敗？
		printf("COM8オープンエラー (%d)\n", GetLastError());
		Sleep(1000);
		goto retry_open;
	} else {
		printf("COM8オープン\n");
	}

	DCB dcb;
	if (GetCommState(hFile, &dcb) == FALSE) {
		printf("GetCommState error %d\n", GetLastError());
	}

	dcb.BaudRate = CBR_19200;
	dcb.ByteSize = 8;
	dcb.Parity = EVENPARITY;
	dcb.StopBits = ONESTOPBIT;
	
	if (SetCommState(hFile, &dcb) == FALSE) {
		printf("SetCommState error %d\n", GetLastError());
	}

	if (GetCommState(hFile, &dcb) == FALSE) {
		printf("GetCommState error %d \n", GetLastError());
	}

	DWORD dwEvMask = EV_RXCHAR;
	SetCommMask(hFile, dwEvMask);
	SetupComm( hFile, 4096, 4096 ) ;
	PurgeComm( hFile, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
	EscapeCommFunction(hFile, SETDTR);

	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout = 0xFFFFFFFF ;
	cto.ReadTotalTimeoutMultiplier = 0 ;
	cto.ReadTotalTimeoutConstant = 1000 ;
	cto.WriteTotalTimeoutMultiplier = 0 ;
	cto.WriteTotalTimeoutConstant = 1000 ;
	SetCommTimeouts(hFile, &cto);

	while (!kbhit()) {
		DWORD dwNumberOfBytesWritten = 0;
		BOOL fRtn;
		char buf[512];

		buf[0] = 0xfe;
		buf[1] = 0x0c;
		buf[2] = 0x08;
		buf[3] = 0x45;
		buf[4] = 0x77;
		buf[5] = 0x01;
		buf[6] = 0x31;
		buf[7] = 0x32;
		buf[8] = 0x33;
		buf[9] = 0x34;
		buf[10] = 0x33;
		buf[11] = 0xff;
		buf[12] = 0;

		fRtn = WriteFile(hFile, buf, 12, &dwNumberOfBytesWritten, NULL);
		if (fRtn == FALSE) {
			printf("WriteFileError = %d\n", GetLastError());
			CloseHandle(hFile);
			goto retry_open;
		}
		printf("送信 %d\n", fRtn);

		DWORD bytesRead = 0;
		char readBuf[32];
		fRtn = ReadFile(hFile, readBuf, sizeof(readBuf), &bytesRead, NULL);
		if (fRtn != FALSE) {
			printf("受信 %d\n", bytesRead);
		} else {
			printf("受信 error %d\n", GetLastError());
			CloseHandle(hFile);
			goto retry_open;
		}

		Sleep(1000);
	}

	CloseHandle(hFile);

	return 0;
}
