#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>

#define READ_HANDLE		0
#define WRITE_HANDLE	1

void test()
{

   char   psBuffer[128];
   FILE   *chkdsk;

   /* DIR �����s���āA�o�͂��p�C�v�ɏ������܂��܂��B
    * ���̃p�C�v�́A�ǂݏo�������ƃe�L�X�g�������w�肵��
    * �J���A�e�L�X�g �t�@�C���Ɠ����悤�ɓǂݏo����悤�ɂ��܂��B
    */
   if( (chkdsk = _popen( "format a:", "rt" )) == NULL )
        exit( 1 );

   /* �p�C�v���I�[ (EOF) �܂œǂݏo���܂��BEOF �́ACHKDISK �� (����
    * �炭����I������) �W���o�͂�������Ƃ������܂��B
    */
   while( !feof( chkdsk ) )
   {
      if( fgets( psBuffer, 128, chkdsk ) != NULL )
         printf( psBuffer );
   }

   /* �p�C�v����āACHKDISK �̖߂�l��\�����܂��B */
   printf( "\n�v���Z�X����߂�܂���: %d\n", _pclose( chkdsk ) );
}

void popen_format()
{
	FILE* fpPipe = _popen("format a:", "wt");
	if (fpPipe) {
		while (!feof(fpPipe)) {
			char pszBuffer[512];

//			printf("%c", fgetc(fpPipe));
//			int nRead = fread(pszBuffer, sizeof(char), 512, fpPipe);
//			printf("[ %s ]\n", pszBuffer);
//			if (fgets(pszBuffer, sizeof(pszBuffer), fpPipe) != NULL) {
//				printf(pszBuffer);
//			}
		}

		_pclose(fpPipe);
	}
}

void pipe_format(int argc, char** argv)
{
	int hpipe[2];
	_pipe(hpipe, 256, _O_NOINHERIT|_O_TEXT);
	int dupStdout = _dup(_fileno(stdout));
	_dup2(hpipe[1], dupStdout);
	close(hpipe[1]);

	HANDLE hProcess = (HANDLE)_spawnvp(P_NOWAIT, argv[1], &argv[1]);

	_dup2(dupStdout, _fileno(stdout));
	close(dupStdout);

	if (hProcess) {
		DWORD nExitCode = STILL_ACTIVE;

		int nRead;
		char buffer[512];
		while (nExitCode == STILL_ACTIVE) {

			nRead = read(hpipe[0], buffer, sizeof(buffer));

			if (nRead >= 0) {
				char buf[512];
				sprintf(buf, "%s", buffer);
				OutputDebugString(buf);
			}

			if (!GetExitCodeProcess(hProcess, &nExitCode))
				return;
		}
	}
}

int main(int argc, char** argv)
{
	pipe_format(argc, argv);
	return 0;

	popen_format();
	return 0;

	test();
	return 0;


	return 0;
}