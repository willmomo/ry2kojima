/**********************************************************************

  stdin ����̓��͂�\�����Ȃ��烍�O�Ɏc���v���O�����B

  <���s�������v���O����> | steal <�o�̓t�@�C���̃t���p�X>

  �o�̓t�@�C���̃f�B���N�g�����Ȃ��Ƃ��́A�����ō쐬���܂��B
  �o�̓t�@�C���̃t���p�X�w���:
  "%TEMP%\StealLog\StealLog%date:~0,4%%date:~5,2%%date:~8,2%.log"

  ���s�ŏI���Ȃ��o�͂�A���͂����߂�o�͂Ȃǂ́A���Ғʂ�ɏ��������
  ����B

  �Ȃ�ׂ��V���v���ɁA���ʂȃR�[�h���Ȃ��Ă���AMBCS�ł̂ݍ쐬�\��
  ���B

  History:
  v1.0.0.0
  �V�K�쐬

**********************************************************************/
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <stdio.h>


//---------------------------------------------------------------------
// �t�H���_�̍ċA�I�쐬
//---------------------------------------------------------------------
bool create_directory(const char* pszPath) {
	if (PathFileExists(pszPath))	return true;
	if (PathIsRoot(pszPath))		return true;

	// �e�f�B���N�g���̍쐬�����݂�
	char pszParentPath[MAX_PATH];
	PathRemoveFileSpec(strcpy(pszParentPath, pszPath));
	if (create_directory(pszParentPath)) {
		if (CreateDirectory(pszPath, NULL)) {
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------
// �s���̉��s 0x0d 0x0a ����菜��
//---------------------------------------------------------------------
void trim(char* ptop) {
	char* pend = ptop + strlen(ptop);

	while ((pend != ptop) && (*(pend - 1) == 0x0d || *(pend - 1) == 0x0a)) {
		*(--pend) = 0;
	}
}


//---------------------------------------------------------------------
// �v���O���� �X�^�[�g
//---------------------------------------------------------------------
int main(int argc, char** argv) {
	char szParentPath[MAX_PATH];
	char buf[512];
	FILE* fp;

	// �����̂ЂƂڂ����O�t�@�C���Ƃ��Ďg�p����B
	// �������Ȃ��Ƃ��̓G���[�I��
	if (argc != 2) {
		fprintf(stderr, "�g�p�@�Fsteal <�o�̓t�@�C����>\n");
		return -1;
	}

	// �e�f�B���N�g�����쐬����
	PathRemoveFileSpec(strcpy(szParentPath, argv[1]));
	create_directory(szParentPath);

	// �o�͗p�t�@�C�����J��
	fp = fopen(argv[1], "a+");
	if (fp == NULL) {
		fprintf(stderr, "�t�@�C�� '%s' ���J���܂���B\n", argv[1]);
		return -1;
	}

	// �W�����͂��r�؂��܂ŁA��ʂƃt�@�C���ɏo��
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		trim(buf);
		puts(buf);
		fputs(buf, fp);
		fputs("\n", fp);
	}

	fclose(fp);

	return 0;
}
