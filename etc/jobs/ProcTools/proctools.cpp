
#include "proctools.h"

// �R�[���o�b�N�֐��ɓn���p�����[�^
typedef struct
{
	DWORD processId;	// ��������v���Z�XID
	int count;			// ���������E�B���h�E�n���h���̐�
	HWND* hwnds;		// ���������E�B���h�E�n���h���ւ̔z��
} EnumParam;

// �v���Z�XID����E�B���h�E�n���h���������o���R�[���o�b�N�֐�
//
static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	EnumParam* param = (EnumParam*)lParam;

	// �E�B���h�E�n���h������v���Z�XID���擾���āA
	// �������Ă���v���Z�XID�ƈ�v���邩�ǂ������ׂ�B
	if (WindowHandleToProcessId(hwnd) == param->processId)
	{
		// ����Ăяo��
		if (param->hwnds == NULL)
		{
			// �������m��
			param->hwnds = (HWND*)GlobalAlloc(GPTR, sizeof(HANDLE));

			// �������m�ۂɎ��s�����Ƃ��́A�񋓒��~
			if (param->hwnds == NULL)
			{
				param->count = -1;
				return FALSE;
			}

			param->count = 1;
		}
		else
		{
			// ���������g��
			HGLOBAL temp = GlobalReAlloc(param->hwnds, sizeof(HANDLE) * (param->count + 1), 0);

			// �������g���Ɏ��s�����Ƃ��́A�񋓒��~
			if (temp == NULL)
			{
				// ��n��
				param->hwnds = (HWND*)GlobalFree(param->hwnds);
				param->count = -1;
				return FALSE;
			}

			// �n���h����V�������āA�������ЂƂ��₷
			param->hwnds = (HWND*)temp;
			param->count++;
		}

		// �m�ۂ����������̍Ō�ɃE�B���h�E�n���h����ۑ�����B
		param->hwnds[param->count-1] = hwnd;
	}

	return TRUE;
}

// �v���Z�XID����v���Z�X�n���h�����擾����
//
// [����]
// processId	�v���Z�X�n���h�����擾�������v���Z�X��ID
//
// [�߂�l]
// �v���Z�X�n���h���B�֐������s�����Ƃ��́ANULL��Ԃ��B
// �n���h�����s�v�ɂȂ����Ƃ��́ACloseHandle���g���ăN���[�Y����B
//
// [���l]
// OpenProcess API �̃��b�p�[
//
HANDLE ProcessIdToProcessHandle(DWORD processId)
{
	return OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
}

// �E�B���h�E�n���h������v���Z�XID���擾����
//
// [����]
// hwnd		�v���Z�XID���擾�������E�B���h�E�ւ̃n���h��
//
// [�߂�l]
// �v���Z�XID�B�֐������s�����Ƃ���0��Ԃ��B
//
// [���l]
// GetWindowThreadProcessId API �̃��b�p�[
//
DWORD WindowHandleToProcessId(HWND hwnd)
{
	DWORD processId;
	if (GetWindowThreadProcessId(hwnd, &processId) == 0)
		processId = 0;

	return processId;
}

// �E�B���h�E�n���h������v���Z�X�n���h�����擾����
//
// [����]
// hwnd		�v���Z�X�n���h�����擾�������E�B���h�E�ւ̃n���h��
//
// [�߂�l]
// �v���Z�X�n���h���B�֐������s�����Ƃ��́ANULL��Ԃ��B
// �n���h�����s�v�ɂȂ����Ƃ��́ACloseHandle���g���ăN���[�Y����B
//
HANDLE WindowHandleToProcessHandle(HWND hwnd)
{
	DWORD processId = WindowHandleToProcessId(hwnd);

	return (processId == 0) ? NULL : ProcessIdToProcessHandle(processId);
}

// �v���Z�XID����E�B���h�E�n���h�����擾����
//
// [����]
// processId	�E�B���h�E�n���h�����擾�������v���Z�X��ID
// buffer		HWND�z��ւ̃|�C���^(�������E�B���h�E�n���h�������ׂĕۑ�����)
// count		buffer�̗v�f��
//
// [�߂�l]
// �������E�B���h�E�n���h���̐��B�߂�l > count �̎��́Abuffer�T�C�Y��
// ���₵�āA�ēx�֐����ĂԁB
// �֐������s�����Ƃ��́A-1��Ԃ��B
//
int ProcessIdToWindowHandle(DWORD processId, HWND* buffer, int count)
{
	EnumParam param = {processId, 0, NULL};

	// processId�Ɋ֘A����E�B���h�E�n���h����񋓂���
	// �֐����s or �񋓎��s(�������m�ۃG���[)�̎��́A�G���[���^�[��
	if (!EnumWindows(EnumWndProc, (LPARAM)&param) || param.count < 0)
	{
		return -1;
	}

	// �������E�B���h�E�n���h�����o�b�t�@�ɕۑ�����B
	// �������A�I�[�o�[�t���[���Ȃ��悤�ɂ���B
	for (int i = 0; i < min(count, param.count); i++)
	{
		buffer[i] = param.hwnds[i];
	}

	// �񋓊֐��Ŋm�ۂ��ꂽ��������j������B
	GlobalFree(param.hwnds);

	return param.count;
}

// �v���Z�X�n���h������v���Z�XID���擾����
// (WinXPsp1�ȍ~�ŗL��)
//
// [����]
// hproc	�v���Z�XID���擾�������v���Z�X�ւ̃n���h��
//
// [�߂�l]
// �v���Z�XID�B�֐������s�����ꍇ�́A0��Ԃ��B
//
// [���l]
// Windows XP sp1 �ȍ~�Ɏ�������Ă���GetProcessId API ���g�p����ȊO�A
// �v���Z�X�n���h������v���Z�XID���擾���邱�Ƃ́A�ł��Ȃ��͗l�B
//
DWORD ProcessHandleToProcessId(HANDLE hproc)
{
	return GetProcessId(hproc);
}

// �v���Z�X�n���h������E�B���h�E�n���h�����擾����
// (WinXPsp1�ȍ~�ŗL��)
//
// [����]
// hproc	�E�B���h�E�n���h�����擾�������v���Z�X�ւ̃n���h��
// buffer	HWND�z��ւ̃|�C���^(�������E�B���h�E�n���h�������ׂĕۑ�����)
// count	buffer�̗v�f��
//
// [�߂�l]
// �������E�B���h�E�n���h���̐��B�߂�l > count �̎��́Abuffer�T�C�Y��
// ���₵�āA�ēx�֐����ĂԁB
// �֐������s�����Ƃ��́A-1��Ԃ��B
//
// [���l]
// �����ŁAProcessHandleToProcessId�֐����g�p���邽�߁AWindows XP sp1 �ȍ~�̂ݑΉ��B
//
int ProcessHandleToWindowHandle(HANDLE hproc, HWND* buffer, int count)
{
	// �v���Z�XID���擾
	DWORD processId = ProcessHandleToProcessId(hproc);
	if (processId == 0)
	{
		return -1;
	}

	// �v���Z�XID����E�B���h�E�n���h�����擾����
	return ProcessIdToWindowHandle(processId, buffer, count);
}
