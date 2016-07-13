#include "kjmutil.h"
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <vector>
#include <share.h>


/* ������̍Ō�̉��s����菜�� */
void kjmutil::chomp(_tstring& str) {
	while (!str.empty()) {
		TCHAR c = *(str.begin() + str.length() - 1);
		if (c != '\x0d' && c != '\x0a')
			break;
		str.erase(str.begin() + str.length() - 1);
	}
}


/* kjmutil::rect�������ɂƂ�MoveWindow�֐� */
BOOL kjmutil::move_window(HWND hWnd, const kjmutil::rect& rc, BOOL bRepaint) {
	return MoveWindow(hWnd, rc.left, rc.top, rc.get_width(), rc.get_height(), bRepaint);
}


/****************************************************************************
  lpExistingFileName�̃t�@�C����lpNewFileName�ɃR�s�[���܂��B�R�s�[���s���ɂ�
  �w��񐔕����g���C���s���܂��B

  <�p�����[�^>
  lpExistingFileName  �R�s�[��
  lpNewFileName       �R�s�[��
  bFailIfExists       �㏑�����s����(FALSE�ŏ㏑�����s��)
  retry               �Ď��s������s����
  wait                �Ď��s���s���܂ŉ��b�҂̂�

  <�߂�l>
  �R�s�[�ɐ��������Ƃ�TRUE�A�R�s�[�Ɏ��s�����Ƃ�FALSE��Ԃ��B
****************************************************************************/
BOOL kjmutil::IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait) {
	BOOL bRet = FALSE;
	
	for (int i = 0; i <= retry; i++) {
		bRet = CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
		if (bRet) {
			break;
		}
		Sleep(wait * 1000);
	}

	return bRet;
}
/**********************************************************************
�Q�l�Ƃ��āA���̊֐���VB6�ł̎����������܂��B
FileSystemObject ��Sleep API���g�p�ł���悤�ɂ��Ă����Ă��������B
'----------------------------------------------------------------------------
' lpExistingFileName�̃t�@�C����lpNewFileName�ɃR�s�[���܂��B�R�s�[���s���ɂ�
' �w��񐔕����g���C���s���܂��B
'
' <�p�����[�^>
' lpExistingFileName  �R�s�[��
' lpNewFileName       �R�s�[��
' bFailIfExists       �㏑�����s����(False�ŏ㏑�����s��)
' retry               �Ď��s������s����
' wait                �Ď��s���s���܂ŉ��b�҂̂�
'
' <�߂�l>
' �R�s�[�ɐ��������Ƃ�True��R�s�[�Ɏ��s�����Ƃ�False��Ԃ��
'----------------------------------------------------------------------------
Private Function IOUtil__CopyFile(lpExistingFileName As String, lpNewFileName As String, bFailIfExists As Boolean, retry As Long, wait As Long) As Boolean
    Dim objFS As New FileSystemObject
    Dim i As Long
    
    On Error Resume Next
    
    For i = 0 To retry
        Err.Clear
        Call objFS.CopyFile(lpExistingFileName, lpNewFileName, Not bFailIfExists)
        If Err.Number = 0 Then
            IOUtil__CopyFile = True
            Exit For
        End If
        Call Sleep(wait * 1000)
    Next
End Function
**********************************************************************/


/* �G���[�̐������擾 */
_tstring kjmutil::err::get_description() {
	_tstring result;
	LPTSTR pszMsg;
	DWORD dwRet;
	LPCTSTR pArgs[] = {_T("%1"), _T("%2"), _T("%3"), NULL};

	dwRet = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		m_dwErrCode,
		0,
		(LPTSTR)&pszMsg,
		0,
		(va_list*)pArgs);

	if (dwRet == 0) {
		_tostringstream oss;
		oss << _T("�G���[ ") << m_dwErrCode << _T(" �̃��b�Z�[�W���擾���AFormatMessage �֐��ŃG���[ ") << GetLastError() << _T(" ���������܂����B");
		result = oss.str();
	} else {
		result = pszMsg;
		kjmutil::chomp(result);

		LocalFree(pszMsg);
	}

	return result;
}


/* �X�N���[�����W���N���C�A���g���W�ɕϊ� */
kjmutil::rect& kjmutil::rect::screen_to_client(HWND hwnd) {
	POINT pt;

	pt.x = left;
	pt.y = top;
	ScreenToClient(hwnd, &pt);
	left = pt.x;
	top = pt.y;

	pt.x = right;
	pt.y = bottom;
	ScreenToClient(hwnd, &pt);
	right = pt.x;
	bottom = pt.y;

	return *this;
}


/* ��`���I�t�Z�b�g���� */
kjmutil::rect& kjmutil::rect::offset(int dx, int dy) {
	OffsetRect(this, dx, dy);
	return *this;
}


/* �E�B���h�E�̃N���C�A���g���W���擾���܂� */
kjmutil::rect kjmutil::rect::get_client_rect(HWND hwnd) {
	kjmutil::rect rc;
	GetClientRect(hwnd, &rc);
	return rc;
}


/* �E�B���h�E�̍��W���擾���܂� */
kjmutil::rect kjmutil::rect::get_window_rect(HWND hwnd) {
	kjmutil::rect rc;
	GetWindowRect(hwnd, &rc);
	return rc;
}


/* �w�肳�ꂽ 2 �̒����`�����S�Ɋ܂ށA�ŏ��̒����`���쐬���܂� */
kjmutil::rect kjmutil::rect::union_rect(const RECT* lprcSrc1, const RECT* lprcSrc2) {
	kjmutil::rect rcDst;
	UnionRect(&rcDst, lprcSrc1, lprcSrc2);
	return rcDst;
}


//---------------------------------------------------------------------
// �ȈՃ��O�N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ���O������
//
// [����]
// format  �������䕶����
//---------------------------------------------------------------------
void kjmutil::log::writeLog(const char* format, ...) {
	std::vector<char> buf(8192);
	va_list marker;

	// ���O�t�@�C���̖��O
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::vector<char> work2(MAX_PATH);
	sprintf(work2.begin(), "%s%04d%02d%02d.log", 
		kjmutil::win32::pathFindFileName(kjmutil::win32::pathRemoveExtension(kjmutil::win32::getModuleFileName())).c_str(),
		st.wYear, st.wMonth, st.wDay);

	// ���O�t�@�C���̍쐬�������
	std::string fname = kjmutil::win32::pathAppend(kjmutil::win32::getEnvironmentVariable(_T("TEMP")), work2.begin());

	// �t�@�C�����J����܂ŁA500ms�҂�
	DWORD dw = GetTickCount();
	FILE* fp = NULL;
	while ((fp = _fsopen(fname.begin(), "a+", _SH_DENYWR)) == NULL) {
		if ((GetTickCount() - dw) > 500) break;
		Sleep(0);
	}

	if (fp) {
		fprintf(fp, "[%04d/%02d/%02d %02d:%02d:%02d.%03d][%04X] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, GetCurrentProcessId());
		va_start(marker, format);
		vfprintf(fp, format, marker);
		va_end(marker);
		fputs("\n", fp);

		fclose(fp);
	}
}


//---------------------------------------------------------------------
// WIN32�֐��̊ȈՌĂяo���N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// GetEnvironmentVariable�̃��b�p�[
//
// [����]
// lpName  ���ϐ��̖��O�ւ̃|�C���^
//
// [�߂�l]
// �w�肳�ꂽ���ϐ������l��Ԃ��B
//---------------------------------------------------------------------
std::string kjmutil::win32::getEnvironmentVariable(LPCTSTR lpName) {
	// �K�v�ȃo�b�t�@�̃T�C�Y���擾
	DWORD dwSizeNeeded = ::GetEnvironmentVariable(lpName, NULL, 0);
	if (dwSizeNeeded == 0) {
		return "";
	}

	std::vector<TCHAR> buf(dwSizeNeeded);
	::GetEnvironmentVariable(lpName, buf.begin(), buf.size());

	return buf.begin();
}


//---------------------------------------------------------------------
// GetModuleFileName�̃��b�p�[
//
// [����]
// hModule  ���W���[���̃n���h��
//
// [�߂�l]
// ���s�t�@�C���̖��O��Ԃ��B
//---------------------------------------------------------------------
std::string kjmutil::win32::getModuleFileName(HMODULE hModule) {
	std::vector<TCHAR> buf(MAX_PATH);
	while (::GetModuleFileName(hModule, buf.begin(), buf.size()) == buf.size()) {
		buf.resize(buf.size() * 2);
	}

	return buf.begin();
}


//---------------------------------------------------------------------
// PathAppend �̃��b�p�[
//
// [����]
// strPath  ���ɂȂ�t���p�X�̃t�@�C����
// strMore  ��������p�X�̖��O
//
// [�߂�l]
// �������ꂽ�V�����t�@�C����������
//---------------------------------------------------------------------
std::string kjmutil::win32::pathAppend(const std::string& strPath, const std::string& strMore) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// ���ɂȂ�t�@�C���������[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �p�X����������
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}

//---------------------------------------------------------------------
// PathFindFileName �̃��b�p�[
//
// [����]
// strPath  �t�@�C��������������t���p�X�̃t�@�C����
//
// [�߂�l]
// �t�@�C���������̕�����
//---------------------------------------------------------------------
std::string kjmutil::win32::pathFindFileName(const std::string& strPath) {
	return PathFindFileName(strPath.c_str());
}


//---------------------------------------------------------------------
// PathRemoveExtension �̃��b�p�[
//
// [����]
// strPath  �g���q����菜�������t�@�C����
//
// [�߂�l]
// �g���q����菜�����V����������
//---------------------------------------------------------------------
std::string kjmutil::win32::pathRemoveExtension(const std::string& strPath) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length());

	// ���ɂȂ�t�@�C���������[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �g���q����菜��
	PathRemoveExtension(work.begin());

	return work.begin();
}


//---------------------------------------------------------------------
// PathRenameExtension �̃��b�p�[
//
// [����]
// strPath  ���ɂӂ���t���p�X�̃t�@�C����
// strExt   �u��������g���q
//
// [�߂�l]
// �g���q��u���������V�����t�@�C����������
//---------------------------------------------------------------------
std::string kjmutil::win32::pathRenameExtension(const std::string& strPath, const std::string& strExt) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strExt.length());

	// ���ɂȂ�t�@�C���������[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �g���q�u������
	PathRenameExtension(work.begin(), strExt.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// �t�@�C������������菜���ĐV�����t�@�C��������������
// PathRemoveFileSpec �� PathAppend �����������悤�Ȋ֐�
//
// [����]
// strPath  ���ɂȂ�t���p�X�̃t�@�C����
// strMore  �ύX���� FileSpec
//
// [�߂�l]
// �V�����t���p�X�̃t�@�C�����B
//---------------------------------------------------------------------
std::string kjmutil::win32::pathRenameFileSpec(const std::string& strPath, const std::string& strMore) {
	// �K�v�\���ȃo�b�t�@������
	std::vector<TCHAR> work(MAX_PATH + strPath.length() + strMore.length());

	// ���ɂȂ�t���p�X�����[�N�ɃR�s�[
	lstrcpy(work.begin(), strPath.c_str());

	// �t�@�C������������菜��
	PathRemoveFileSpec(work.begin());

	// �V�����t�@�C��������������
	PathAppend(work.begin(), strMore.c_str());

	return work.begin();
}


//---------------------------------------------------------------------
// ini�t�@�C���A�N�Z�X�N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//
// [����]
// ���s�t�@�C����.ini ���f�t�H���g�ŃA�N�Z�X����悤�ɒ��������B
//---------------------------------------------------------------------
kjmutil::iniFile::iniFile() {
	this->m_strIniFile = kjmutil::win32::pathRenameExtension(kjmutil::win32::getModuleFileName(), _T(".ini"));
}


//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjmutil::iniFile& kjmutil::iniFile::operator =(const kjmutil::iniFile& rhs) {
	if (this == &rhs)	return *this;	// �����I�u�W�F�N�g���w���Ă���

	this->m_strIniFile = rhs.m_strIniFile;
	return *this;
}
