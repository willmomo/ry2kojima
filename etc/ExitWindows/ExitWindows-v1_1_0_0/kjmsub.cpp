//---------------------------------------------------------------------------------------
// kjmsub ver.0.2.0.0
//---------------------------------------------------------------------------------------
#include "kjmsub.h"

#include <vector>
#include <sstream>


//---------------------------------------------------------------------------------------
// CopyFile API�Ɠ��l�̓���������֐�(�G���[�ӏ����ڍׂɓ��肵�����Ƃ��Ɏg�p)
// [�߂�l]  0   ����I��
// [�߂�l] -1   �R�s�[���t�@�C���̃I�[�v���Ɏ��s
// [�߂�l] -2   �R�s�[��t�@�C���̃I�[�v���Ɏ��s
// [�߂�l] -3   �R�s�[���t�@�C���̓ǂݍ��݂Ɏ��s
// [�߂�l] -4   �R�s�[��t�@�C���̏������݂Ɏ��s
// [�߂�l] -5   �R�s�[��t�@�C���Ɏw��T�C�Y�������݂ł��Ȃ�
// [�߂�l] -6   �R�s�[���t�@�C���̃^�C���X�^���v�擾���s
// [�߂�l] -7   �R�s�[��t�@�C���̃^�C���X�^���v�X�V���s
// [�߂�l] -8   �R�s�[���t�@�C�������̂Ɏ��s
// [�߂�l] -9   �R�s�[��t�@�C�������̂Ɏ��s
// [�߂�l] -10  �R�s�[���t�@�C���̑����擾���s
// [�߂�l] -11  �R�s�[��t�@�C���̑����X�V���s
//---------------------------------------------------------------------------------------
int kjm::util::copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists) {

#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

	SetLastError(NOERROR);

	int result = 0;

	// �R�s�[���t�@�C�����J��
	HANDLE hFile1 = ::CreateFile(lpExistingFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile1 == INVALID_HANDLE_VALUE) {
		return -1;
	}

	// �R�s�[��t�@�C�����J��
	HANDLE hFile2 = ::CreateFile(lpNewFileName, GENERIC_WRITE, 0, NULL, (bFailIfExists) ? CREATE_NEW : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE) {
		::CloseHandle(hFile1);
		return -2;
	}

	SetLastError(NOERROR);

	char buffer[8192];
	DWORD dwNumberOfBytesRead;
	DWORD dwNumberOfBytesWritten;

	do {
		if (::ReadFile(hFile1, buffer, _countof(buffer), &dwNumberOfBytesRead, NULL) == FALSE) {
			result = -3;
			break;
		}

		if (dwNumberOfBytesRead > 0) {
			if (::WriteFile(hFile2, buffer, dwNumberOfBytesRead, &dwNumberOfBytesWritten, NULL)) {
				if (dwNumberOfBytesRead != dwNumberOfBytesWritten) {
					result = -5;
					break;
				}
			} else {
				result = -4;
				break;
			}
		}
	} while (dwNumberOfBytesRead == _countof(buffer));

	// �R�s�[������ɏI�������Ƃ��́A�^�C���X�^���v���R�s�[���ɍ��킹��
	if (result == 0) {
		FILETIME lastWriteTime;

		if (::GetFileTime(hFile1, NULL, NULL, &lastWriteTime)) {
			if (::SetFileTime(hFile2, NULL, NULL, &lastWriteTime)) {
				// �^�C���X�^���v�̕�������
			} else {
				result = -7;
			}
		} else {
			result = -6;
		}
	}

	if (::CloseHandle(hFile1) == FALSE) {
		result = -8;
	}
	if (::CloseHandle(hFile2) == FALSE) {
		result = -9;
	}

	// �R�s�[������ɐi��ł���Ƃ��́A�������R�s�[����
	if (result == 0) {
		DWORD dwAttributes = ::GetFileAttributes(lpExistingFileName);
		if (dwAttributes != 0xffffffff) {
			if (::SetFileAttributes(lpNewFileName, dwAttributes | FILE_ATTRIBUTE_ARCHIVE)) {
				// �����̕�������
			} else {
				result = -11;
			}
		} else {
			result = -10;
		}
	}

	return result;
}


//---------------------------------------------------------------------
// Windows���V���b�g�_�E��������
// [�߂�l]
// 0  ����I��
// -1 AdjustTokenPrivileges API�ŃG���[
// -2 ExitWindowsEx API�ŃG���[
//---------------------------------------------------------------------
int kjm::util::exitWindows() {
	int result = 0;

	// Windows ���V���b�g�_�E��
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT){
		// NT�Ή�
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
			//SE_SHUTDOWN_NAME��������������
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS) {
				return -1;
			}
			CloseHandle(hToken);
		}

		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT�n] �����s���܂��B\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT�n] �Ɏ��s���܂���(error %u)�B\n"), GetLastError());
			result = -2;
		}
		
	} else {
		// Windows95/98/Me �p
		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95�n] �����s���܂��B\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95�n] �Ɏ��s���܂���(error %u)�B\n"), GetLastError());
			result = -2;
		}
	}

	return result;
}


// �V�X�e���W���̃G���[���b�Z�[�W���쐬
std::basic_string<TCHAR> kjm::util::formatMessageBySystem(DWORD dwErr) {
	std::basic_string<TCHAR> ret;
	PVOID pBuffer;

	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, (LPTSTR)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		std::basic_stringstream<TCHAR> ss;
		ss << TEXT("FormatMessage(") << dwErr << TEXT(") failed: error ") << dwErr;
		ret = ss.str();
	} else {
		ret = (LPTSTR)pBuffer;
		LocalFree(pBuffer);
	}

	return ret;
}

// GetComputerNameEx�֐�(vc6�p)
BOOL kjm::util::__GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize) {
	typedef BOOL (WINAPI *GCNEX)(COMPUTER_NAME_FORMAT, LPTSTR, LPDWORD);

	BOOL bRet = FALSE;
	HMODULE hModule = LoadLibrary(TEXT("kernel32.dll"));
	
	if (hModule != NULL) {
#if defined(_MBCS)
		GCNEX pFunc = (GCNEX)GetProcAddress(hModule, "GetComputerNameExA");
#else
		GCNEX pFunc = (GCNEX)GetProcAddress(hModule, "GetComputerNameExW");
#endif
		if (pFunc != NULL) {
			bRet = pFunc(NameType, lpBuffer, nSize);
		}
		FreeLibrary(hModule);
	}

	return bRet;
}

// string��Ԃ�GetComputerNameEx�֐�
std::basic_string<TCHAR> kjm::util::getComputerNameEx(COMPUTER_NAME_FORMAT NameType) {
	DWORD dwSize = 0;

	// �K�v�ȃo�b�t�@�T�C�Y���擾(�G���[�`�F�b�N�Ȃ�)
	kjm::util::__GetComputerNameEx(NameType, NULL, &dwSize);

	// �o�b�t�@���m��
	std::vector<TCHAR> buf(dwSize);
	if (!kjm::util::__GetComputerNameEx(NameType, &buf[0], &dwSize))
		throw ::GetLastError();

	return &buf[0];
}

// string��Ԃ�GetDlgItemText�֐�
// [����] hDlg        �_�C�A���O�{�b�N�X�̃n���h��
// [����] nIDDlgItem  �R���g���[���̎��ʎq
// [��O] throw ::GetLastError();
std::basic_string<TCHAR> kjm::util::getDlgItemText(HWND hDlg, int nIDDlgItem) {
	std::vector<TCHAR> work(32);

	do {
		UINT ret = ::GetDlgItemText(hDlg, nIDDlgItem, &work[0], work.size());
		DWORD dwErr = ::GetLastError();

		if ((ret == 0) && (dwErr != 0))
			throw dwErr;

		if ((ret == 0) || (ret + 1 < work.size()))
			break;

		work.resize(work.size() * 2);
	} while (1);

	return &work[0];
}


// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�����
BOOL kjm::util::moveCenterWorkarea(HWND hwnd) {
	// ���[�N�X�G���A�̋�`���擾
	RECT rcWorkarea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkarea, 0);

	// �ړ�����E�B���h�E�̋�`���擾
	RECT rcClient;
	::GetWindowRect(hwnd, &rcClient);

	int nNewLeft = ((rcWorkarea.right - rcWorkarea.left) - (rcClient.right - rcClient.left)) / 2;
	int nNewTop = ((rcWorkarea.bottom - rcWorkarea.top) - (rcClient.bottom - rcClient.top)) / 2;

	// �ړ��\��̍��オWorkarea����͂ݏo�Ă���ꍇ�́A���W��0�ɂ���
	if (nNewLeft < 0)	nNewLeft = 0;
	if (nNewTop < 0)	nNewTop = 0;

	return ::MoveWindow(hwnd, nNewLeft, nNewTop, 
		(rcClient.right - rcClient.left), (rcClient.bottom - rcClient.left), FALSE);
}


// SetComputerNameEx�֐�(vc6�p)
BOOL kjm::util::__SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer) {
	typedef BOOL (WINAPI *SCNEx)(COMPUTER_NAME_FORMAT, LPCTSTR);

	BOOL bRet = FALSE;

	HMODULE hModule = LoadLibrary(TEXT("kernel32.dll"));
	if (hModule) {
#if defined(_MBCS)
		SCNEx pFunc = (SCNEx)GetProcAddress(hModule, "SetComputerNameExA");
#else
		SCNEx pFunc = (SCNEx)GetProcAddress(hModule, "SetComputerNameExW");
#endif
		if (pFunc) {
			bRet = pFunc(NameType, lpBuffer);
		}

		FreeLibrary(hModule);
	}

	return bRet;
}
