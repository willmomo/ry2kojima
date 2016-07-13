#include "kjmutil.h"
#include <tchar.h>


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

