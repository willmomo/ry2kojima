#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "eventlog.h"

/* �����ݕ�����̎擾 */
char **GetArgs(const EVENTLOGRECORD *pBuf)
{
	char *cp;
	WORD ArgCount;
	char **Args = NULL;

	if(pBuf->NumStrings == 0) return NULL;

	/* �������X�g���擾 */
	Args = (char **)GlobalAlloc(GMEM_FIXED, sizeof(char *) * pBuf->NumStrings);
	cp = (char *)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		Args[ArgCount] = cp;
		cp += strlen(cp) + 1;
	}
	return Args;
}

/* �\�[�X�����烂�W���[�������擾 */
BOOL GetModuleNameFromSourceName(
	const char *SourceName, 
	const char *EntryName, 
	char *ExpandedName /* 1000�o�C�g�̃o�b�t�@ */)
{
	DWORD lResult;
	DWORD ModuleNameSize;
	char ModuleName[1000];
	HKEY hAppKey = NULL;
	HKEY hSourceKey = NULL;
	BOOL bReturn = FALSE;

	/* Application���O�p�̃��W�X�g���L�[���I�[�v�� */
	lResult = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application",
		0,
		KEY_READ,
		&hAppKey);

	if(lResult != ERROR_SUCCESS) {
		printf("registry can not open.\n");
		goto Exit;
	}

	/* �\�[�X�̏�񂪊i�[����Ă��郌�W�X�g�����I�[�v�� */
	lResult = RegOpenKeyEx(
		hAppKey,
		SourceName,
		0,
		KEY_READ,
		&hSourceKey);

	if(lResult != ERROR_SUCCESS) goto Exit;

	ModuleNameSize = 1000;

	/* �\�[�X���W���[�������擾 */
	lResult = RegQueryValueEx(
		hSourceKey,
		"EventMessageFile",
		NULL,
		NULL,
		(unsigned char *)ModuleName,
		&ModuleNameSize);

	if(lResult != ERROR_SUCCESS) goto Exit;

	/* ���ϐ���W�J */
	ExpandEnvironmentStrings(ModuleName, ExpandedName, 1000);

	/* ����I�� */
	bReturn = TRUE;

Exit: /* �㏈�� */
	if(hSourceKey != NULL) RegCloseKey(hSourceKey);
	if(hAppKey != NULL) RegCloseKey(hAppKey);

	return bReturn;
}

/* ���b�Z�[�W�̕\�� */
BOOL DispMessage(
	const char *SourceName, 
	const char *EntryName,
	const char **Args, 
	DWORD MessageId)
{
	BOOL bResult;
	BOOL bReturn = FALSE;
	HANDLE hSourceModule = NULL;
	char SourceModuleName[1000];
	char *pMessage = NULL;

	/* �\�[�X���W���[�������擾 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) goto Exit;

	/* �\�[�X���W���[�������[�h */
	hSourceModule = LoadLibraryEx(
		SourceModuleName,
		NULL,
		DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

	if(hSourceModule == NULL) goto Exit;

	/* ���b�Z�[�W���쐬 */
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		hSourceModule,
		MessageId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&pMessage,
		0,
		(va_list *)Args);

	/* ����I�� */
	bReturn = TRUE;

Exit: /* �㏈�� */
	if(pMessage != NULL) printf("%s", pMessage);
	else printf("(%d)\n", MessageId);

	if(hSourceModule != NULL) FreeLibrary((HINSTANCE)hSourceModule);
	if(pMessage != NULL) LocalFree(pMessage);

	return bReturn;
}

/* �C�x���g���O�̓ǂݎ�� */
void ReadLog(void)
{
	DWORD BufSize;
	DWORD ReadBytes;
	DWORD NextSize;
	BOOL bResult;
	DWORD i;
	char *cp;
	char *pSourceName;
	char *pComputerName;
	HANDLE hEventLog = NULL;
	EVENTLOGRECORD *pBuf = NULL;
	char **Args = NULL;

	/* �C�x���g���O�̃I�[�v�� */
	hEventLog = OpenEventLog(NULL, "Application");

	if(hEventLog == NULL) {
		printf("event log can not open.\n");
		goto Exit;
	}

	for(;;) {
		/* �C�x���g���O�̃T�C�Y�擾 */
		BufSize = 1;
		pBuf = (EVENTLOGRECORD *)GlobalAlloc(GMEM_FIXED, BufSize);

		bResult = ReadEventLog(
			hEventLog,
			EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
			0,
			pBuf,
			BufSize,
			&ReadBytes,
			&NextSize);

		if(!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) break;

		GlobalFree(pBuf);
		pBuf = NULL;

		/* �o�b�t�@���蓖�� */
		BufSize = NextSize;
		pBuf = (EVENTLOGRECORD *)GlobalAlloc(GMEM_FIXED, BufSize);

		/* �C�x���g���O�̓ǂݎ�� */
		bResult = ReadEventLog(
			hEventLog,
			EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
			0,
			pBuf,
			BufSize,
			&ReadBytes,
			&NextSize);

		if(!bResult) break;

		eventlog_record rec( pBuf );

		/* �ǂݎ�����C�x���g�̕\�� */
		printf("���R�[�h�ԍ�: %d\n", rec.RecordNumber() );
		time_t t = rec.TimeGenerated();
		printf("��������: %s", ctime( &t ) );
		t = rec.TimeWritten();
		printf("�������ݎ���: %s", ctime( &t ) );
		printf("�C�x���gID: %08x\n", rec.EventID() );

		printf( "�C�x���g�̎��: %s\n", rec.EventTypeText().c_str() );

		cp = (char *)pBuf;
		cp += sizeof(EVENTLOGRECORD);

		pSourceName = cp;
		cp += strlen(cp)+1;

		pComputerName = cp;
		cp += strlen(cp)+1;

		printf("�\�[�X��: %s\n", rec.SourceName().c_str() );
		printf("�R���s���[�^��: %s\n", rec.ComputerName().c_str() );

		/* �J�e�S���̕\�� */
		printf("�񎟃J�e�S��: ", pBuf->EventCategory);
		DispMessage(pSourceName, "CategoryMessageFile", NULL, pBuf->EventCategory);

		/* ���b�Z�[�W�̕\�� */
		Args = GetArgs(pBuf);

		printf("���b�Z�[�W: ");
		DispMessage(pSourceName, "EventMessageFile", (const char **)Args, pBuf->EventID);

		if(Args != NULL) {
			GlobalFree(Args);
			Args = NULL;
		}

		/* �ŗL�f�[�^�̕\�� */
		if(pBuf->DataLength > 0) {
			printf("�ŗL�f�[�^: ");
			for(i=0; i<pBuf->DataLength; i++) printf("%02x ", *(((unsigned char *)pBuf)+(pBuf->DataOffset)+i));
			printf("\n");
		}

		printf("\n");

		/* �o�b�t�@��� */
		GlobalFree(pBuf);
		pBuf = NULL;
	}

Exit: /* �㏈�� */
	if(pBuf != NULL) GlobalFree(pBuf);
	if(Args != NULL) GlobalFree(Args);
	if(hEventLog != NULL) CloseEventLog(hEventLog);
}

void main() {
	ReadLog();
}
