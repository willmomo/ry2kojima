// BeepFilter.Cpp
/* 必要なコンパイル オプション: なし
   実行方法: BeepFilter.exe <path>Beeper.exe
*/
#include <windows.h>
#include <process.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <errno.h>

#define   OUT_BUFF_SIZE 512
#define   READ_HANDLE 0
#define   WRITE_HANDLE 1
#define   BEEP_CHAR 7

char szBuffer[OUT_BUFF_SIZE];

int Filter(char* szBuff, ULONG nSize, int nChar)
{
   char* szPos = szBuff + nSize -1;
   char* szEnd = szPos;
   int nRet = nSize;

   while (szPos > szBuff)
   {
      if (*szPos == nChar)
         {
            memmove(szPos, szPos+1, szEnd - szPos);
            --nRet;
         }
      --szPos;
   }
   return nRet;
}

void test3(int argc, char** argv)
{
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	int hpipe_stdout[2];	// 子プロセスのstdoutに接続するパイプ
	int hpipe_stdin[2];		// 子プロセスのstdinに接続するパイプ

	_pipe(hpipe_stdout, 128, O_TEXT|O_NOINHERIT);
	_pipe(hpipe_stdin, 128, O_TEXT|O_NOINHERIT);

	int save_stdout = _dup(_fileno(stdout));
	_dup2(hpipe_stdout[WRITE], _fileno(stdout));
	_close(hpipe_stdout[WRITE]);
	int save_stdin = _dup(_fileno(stdin));
	_dup2(hpipe_stdin[READ], _fileno(stdin));
	_close(hpipe_stdin[READ]);
	
	hprocess = (HANDLE)spawnvp(P_NOWAIT, argv[1], (const char* const*)&argv[1]);

	_dup2(save_stdout, _fileno(stdout));
	_close(save_stdout);
	_dup2(save_stdin, _fileno(stdin));
	_close(save_stdin);

#define KEYWORD0	"準備ができたら Enter キーを押してください... "
#define KEYWORD1	"パーセント終了しました。"
#define KEYWORD2	"(Y/N)? "

	int stat = 0;
	int n = 0;
	char buffer[1024];
	FILE* fp = _fdopen(hpipe_stdout[READ], "r");
	while (!feof(fp)) {
		buffer[n++] = fgetc(fp);
		buffer[n] = '\0';
		
		if (stat == 0) {
			char* p = strstr(buffer, KEYWORD0);
			if (p) {
				strcpy(buffer, p + strlen(KEYWORD0));
				n = strlen(buffer);
				_write(hpipe_stdin[WRITE], "\n", 1);
				stat = 1;
			}
		} else if (stat == 1) {
			char* p = strstr(buffer, KEYWORD1);
			if (p) {
				int i;
				printf("%d %%\n", i = atoi(buffer));
				strcpy(buffer, p + strlen(KEYWORD1));
				n = strlen(buffer);
				if (i == 100)
					stat = 2;
			} else if (buffer[n-1] == '\n') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			} else if (buffer[n-1] == '\x0d') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			}
		} else if (stat == 2) {
			char* p = strstr(buffer, KEYWORD2);
			if (p) {
				_write(hpipe_stdin[WRITE], "N", 1);
				_write(hpipe_stdin[WRITE], "\n", 1);
			} else if (buffer[n-1] == '\n') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			}
		} else {
			printf(buffer);
		}
	}

	fclose(fp);
	_close(hpipe_stdin[WRITE]);
}

void test2(int argc, char** argv)
{
	HANDLE hprocess;
	enum PIPE { READ, WRITE };
	int hpipeR[2];		// 子プロセスのstdoutをWRITEに接続してREADを使用するパイプ
	int hpipeW[2];	// 子プロセスのstdinをREADに接続してWRITEを使用するパイプ

	// 子stdout->READパイプの作成
	if (_pipe(hpipeR, 512, O_BINARY|O_NOINHERIT) != 0) {
		return;
	}

	// WRITE->子stdinパイプの作成
	if (_pipe(hpipeW, 512, O_BINARY|O_NOINHERIT) != 0) {
		return;
	}

	int saveStdout = _dup(_fileno(stdout));	// 自stdoutをコピー
	_dup2(hpipeR[WRITE], _fileno(stdout));
	_close(hpipeR[WRITE]);

	int saveStdin = _dup(_fileno(stdin));	// 自stdinをコピー
	_dup2(hpipeW[READ], _fileno(stdin));
	_close(hpipeW[READ]);

	hprocess = (HANDLE)spawnvp(P_NOWAIT, argv[1], (const char* const*)&argv[1]);

	_dup2(saveStdout, _fileno(stdout));
	_close(saveStdout);

	_dup2(saveStdin, _fileno(stdin));
	_close(saveStdin);

	FILE* fpRead = _fdopen(hpipeR[READ], "rb");

	while (!feof(fpRead)) {
		fgetc(fpRead);
	}

	_dup2(saveStdout, _fileno(stdout));
	_dup2(saveStdin, _fileno(stdin));
}

void test(int argc, char** argv)
{
	int nExitCode = STILL_ACTIVE;

#if 1
	enum PIPE { READ, WRITE };
	int hpipe[2];
	_pipe(hpipe, 512, O_BINARY|O_NOINHERIT);
	int saveStdout = _dup(_fileno(stdout));
	_dup2(hpipe[WRITE], _fileno(stdout));
	_close(hpipe[WRITE]);
	HANDLE hProcess = (HANDLE)spawnvp(P_NOWAIT, argv[1], (const char* const*)&argv[1]);
	_dup2(saveStdout, _fileno(stdout));
	_close(saveStdout);
	FILE* fp = _fdopen(hpipe[READ], "rb");
#endif
#if 0
	int saveStdout;
	enum PIPE { READ, WRITE };
	int hpipe[2];
	_pipe(hpipe, 512, O_BINARY|O_NOINHERIT);
	int saveStdin = _dup(_fileno(stdin));
	_dup2(hpipe[READ], _fileno(stdin));
	_close(hpipe[READ]);
	HANDLE hProcess = (HANDLE)spawnvp(P_NOWAIT, argv[1], (const char* const*)&argv[1]);
	_dup2(saveStdin, _fileno(stdin));
	_close(saveStdin);
	FILE* fp = _fdopen(hpipe[WRITE], "wb");

#if 1
	char buf[] = "\x0a\x0aN\x0a";
	for (int ii = 0; ii < 4; ii++) {
		while (!_eof(hpipe[WRITE]));
		_write(hpipe[WRITE], &buf[ii], 1);
	}
#else
	char buf[] = "\x0a\x0aN\x0a";
	for (int ii = 0; ii < 4; ii++) {
		fflush(fp);
		while (!feof(fp));
		fputc(buf[ii], fp);
	}
	fwrite("\x0a\x0a", sizeof(char), 2, fp);
//	fwrite("\x0a\x0aN\x0a", sizeof(char), 4, fp);
	fflush(fp);
	Sleep(10 * 1000);
	fwrite("N", sizeof(char), 1, fp);
	fflush(fp);
	Sleep(1 * 1000);
	fwrite("\x0a", sizeof(char), 1, fp);
	fflush(fp);
#endif
	DWORD dw = STILL_ACTIVE;
	while (dw == STILL_ACTIVE)
		GetExitCodeProcess(hProcess, &dw);
	return;
#endif

	int nn = 0;
	char szBuf[512];

#if 0
	memset(szBuf, 0, sizeof(szBuf));
	szBuf[511] = 0;
	char temp[1024];
	while (!feof(fp)) {
		szBuf[nn] = fgetc(fp);
		nn += 1;
//		int nRead = fread(&szBuf[nn], sizeof(char), sizeof(szBuf), fp);
//		nn += nRead;

		if (1) {
//			szBuf[nn] = '\0';
			sprintf(temp, "-- %d ------------------------------------------------------\n", nn);
			OutputDebugString(temp);
			sprintf(temp, "%s\n", szBuf);
			OutputDebugString(temp);
			sprintf(temp, "-- %d ------------------------------------------------------\n", nn);
			OutputDebugString(temp);
		}
	}

   getchar();
	return;
#endif

	while (_eof(hpipe[READ]) && nExitCode == STILL_ACTIVE) {
//	while (1) {
		int nRead = _read(hpipe[READ], &szBuf[nn], sizeof(szBuf));
		nn += nRead;

		if (nRead) {
			szBuf[nn] = '\0';
			char temp[1024];
			sprintf(temp, "%s\n", szBuf);
			OutputDebugString(temp);
		}

//		if (_eof(hpipe[READ]))
//			break;

		if (!GetExitCodeProcess(hProcess, (unsigned long *)&nExitCode))
			break;
	}

	_dup2(saveStdout, _fileno(stdout));
   printf("\n\'ENTER\' キーを押すと続行します... ");
   getchar();
}

int main(int argc, char** argv)
{
	test3(argc, argv);
	return 0;

   int nExitCode = STILL_ACTIVE;
   if (argc >= 2)
   {
      HANDLE hProcess;
      int hStdOut;
      int hStdOutPipe[2];

      // パイプを作成する。
      if(_pipe(hStdOutPipe, 512, O_BINARY | O_NOINHERIT) == -1)
         return   1;

      // 標準出力ハンドルを複製する(次の行で元のハンドルをクローズする)。
      hStdOut = _dup(_fileno(stdout));

      // 書き込みパイプを標準出力ハンドルに結合する。
      if(_dup2(hStdOutPipe[WRITE_HANDLE], _fileno(stdout)) != 0)
         return   2;

      // 元の書き込みパイプを閉じる。
      close(hStdOutPipe[WRITE_HANDLE]);

      // プロセスを呼び出す。
      hProcess = (HANDLE)spawnvp(P_NOWAIT, argv[1], 
       (const char* const*)&argv[1]);

      // 元の標準出力の複製を標準出力に結合し直す。
      if(_dup2(hStdOut, _fileno(stdout)) != 0)
         return   3;

      // 元の標準出力の複製を閉じる。
      close(hStdOut);

      if(hProcess)
      {
         int nOutRead;
         while   (nExitCode == STILL_ACTIVE)
         {
            nOutRead = read(hStdOutPipe[READ_HANDLE], 
             szBuffer, OUT_BUFF_SIZE);
            if(nOutRead)
            {
               nOutRead = Filter(szBuffer, nOutRead, BEEP_CHAR);
               fwrite(szBuffer, 1, nOutRead, stdout);
            }

            if(!GetExitCodeProcess(hProcess,(unsigned long*)&nExitCode))
               return 4;
         }
      }
   }

   printf("\n\'ENTER\' キーを押すと続行します... ");
   getchar();
   return nExitCode;
}
