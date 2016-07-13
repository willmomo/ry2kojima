//---------------------------------------------------------------------
// use_date.cpp
//   VB�̓��t�^�A������^����������DLL�֐��̃T���v��
// �쐬��
//   �����Ǒ�
//---------------------------------------------------------------------
// 1. Date�^�������Ŏ󂯂�DOSDATETIME��Long�ɕϊ����Ė߂�
// 2. DOSDATETIME��Long�������Ŏ󂯂āADate�^�ɕϊ����Ė߂�
// 3. Date�^��String�^�ɕϊ����Ė߂�
// 4. Date�^�������œn���ꂽString������������

#include <windows.h>
#include <comdef.h>

//---------------------------------------------------------------------
// [�T�v]
//   VB�̓��t��DOS�̓��t�ɕϊ�
// [����]
//   vbDate	���t�^(Date�^)�̓��t
// [�߂�l]
//   DOSDATETIME�^(u_long�̃r�b�g�t�B�[���h)�ɕϊ������l
// [�@�\����]
//   VB��Date�^�������Ŏ󂯎���
//   VB�ł̒�`
//     Declare Function VbDate2DosDate(ByVal vbDate As Date) As Long
//---------------------------------------------------------------------
__declspec(dllexport) long __stdcall VbDate2DosDate(DATE vbDate)
{
	UDATE udate;
	u_long dwFlags = 0;

	memset(&udate, 0, sizeof(udate));

	// VB�̓��t�^(Date)��UDATE�\���̂ɕϊ�����
	HRESULT hres = VarUdateFromDate(vbDate, dwFlags, &udate);
	if (hres != S_OK) {
		return 0xffffffff;
	}

	return ((udate.st.wYear - 1980) << 25) |
		(udate.st.wMonth << 21) |
		(udate.st.wDay << 16) |
		(udate.st.wHour << 11) |
		(udate.st.wMinute << 5) |
		(udate.st.wSecond / 2);
}

//---------------------------------------------------------------------
// [�T�v]
//   DOS�̓��t��VB�̓��t�ɕϊ�
// [����]
//   dosDate	DOSDATETIME�^�̓��t
// [�߂�l]
//   ���t�^(Date�^)�ɕϊ������l
// [�@�\����]
//   VB��Date�^���쐬�����
//   VB�ł̒�`
//     Declare Function DosDate2VbDate(ByVal dosDate As Long) As Date
//---------------------------------------------------------------------
__declspec(dllexport) DATE __stdcall DosDate2VbDate(u_long dosDate)
{
	UDATE udate;
	u_long dwFlags = 0;
	DATE date;

	memset(&udate, 0, sizeof(udate));
	udate.st.wYear = (u_short)(((dosDate >> 25) & 0x7f) + 1980);
	udate.st.wMonth = (u_short)((dosDate >> 21) & 0x0f);
	udate.st.wDay = (u_short)((dosDate >> 16) & 0x1f);
	udate.st.wHour = (u_short)((dosDate >> 11) & 0x1f);
	udate.st.wMinute = (u_short)((dosDate >> 5) & 0x3f);
	udate.st.wSecond = (u_short)((dosDate & 0x1f) * 2);

	// UDATE�\���̂���VB�̓��t�^(Date�^)���쐬����
	HRESULT hres = VarDateFromUdate(&udate, dwFlags, &date);
	if (hres != S_OK) {
		return 0.0;
	}

	return date;
}

//---------------------------------------------------------------------
// [�T�v]
//   VB�̓��t��"yyyy/mm/dd hh:nn:ss"������ŕԂ�
// [����]
//   vbDate	���t�^(Date�^)�̓��t
// [�߂�l]
//   ������^(String�^)�ɕϊ��������t
// [�@�\����]
//   ������^(String)��߂��֐��̗�
//   VB�ł̒�`
//     Declare Function VbDate2String(ByVal vbDate As Date) As String
//---------------------------------------------------------------------
__declspec(dllexport) BSTR __stdcall VbDate2String(DATE vbDate)
{
	UDATE udate;
	u_long dwFlags = 0;

	memset(&udate, 0, sizeof(udate));

	// VB�̓��t�^(Date)��UDATE�\���̂ɕϊ�����
	HRESULT hres = VarUdateFromDate(vbDate, dwFlags, &udate);
	if (hres != S_OK) {
		return NULL;
	}

	char temp[64];
	int len = wsprintf(temp, "%04d/%02d/%02d %02d:%02d:%02d",
		udate.st.wYear, udate.st.wMonth, udate.st.wDay,
		udate.st.wHour, udate.st.wMinute, udate.st.wSecond);

	// return SysAllocString(temp);�ł��������ȋC�����邪�A
	// SysAllocString, SysAllocStringLen, SysReAllocString, SysReAllocStringLen
	// ��L4�̊֐��́AVB�ɐ��䂪�킽�����Ƃ��A����ɏ����ł��Ȃ��B
	// �����炭�A�����R�[�h�̎����ϊ�(Unicode->Ascii->Unicode)�̊֌W�ł�낵���Ȃ���
	// �v����B�B�ꐳ��ɓ���SysAllocStringByteLen���g�p�B

	return SysAllocStringByteLen(temp, len);
}

//---------------------------------------------------------------------
// [�T�v]
//   VB�̓��t��"yyyy/mm/dd" "hh:nn:ss"��̕�����ŕԂ��B
// [����]
//   vbDate	���t�^(Date�^)�̓��t
// [�߂�l]
//   0:		����I��
//   -1:	�ُ�I��(�ϊ����s)
// [�@�\����]
//   ������^(String)�������������
//   VB�ł̒�`
//   Declare Function VbDate2String2(ByVal vbDate As Date,
//     ByVal strYMD As String, ByVal strHMS As String) As Long
//---------------------------------------------------------------------
__declspec(dllexport) long __stdcall VbDate2String2(DATE vbDate, BSTR* ymd, BSTR* hms)
{
	UDATE udate;
	u_long dwFlags = 0;

	memset(&udate, 0, sizeof(udate));

	// VB�̓��t�^(Date)��UDATE�\���̂ɕϊ�����
	HRESULT hres = VarUdateFromDate(vbDate, dwFlags, &udate);
	if (hres != S_OK) {
		return -1;
	}

	// SysReSysAllocString(temp);�ł��������ȋC�����邪�A
	// SysAllocString, SysAllocStringLen, SysReAllocString, SysReAllocStringLen
	// ��L4�̊֐��́AVB�ɐ��䂪�킽�����Ƃ��A����ɏ����ł��Ȃ��B
	// �����炭�A�����R�[�h�̎����ϊ�(Unicode->Ascii->Unicode)�̊֌W�ł�낵���Ȃ���
	// �v����B�B�ꐳ��ɓ���SysAllocStringByteLen���g�p���邽�߁A
	// SysFreeString()���Ă���SysAllocStringByteLen()�ōĊm�ۂ���

	char temp[64];
	int len = wsprintf(temp, "%04d/%02d/%02d",
		udate.st.wYear, udate.st.wMonth, udate.st.wDay);
	SysFreeString(*ymd);
	*ymd = SysAllocStringByteLen(temp, len);

	len = wsprintf(temp, "%02d:%02d:%02d",
		udate.st.wHour, udate.st.wMinute, udate.st.wSecond);
	SysFreeString(*hms);
	*hms = SysAllocStringByteLen(temp, len);

	return 0;
}

//---------------------------------------------------------------------
// [�T�v]
//   DLL���C��
// [����]
//   hinstDLL		handle to DLL module
//   fdwReason		reason for calling fuction
//   lpvReserved	reserved
// [�߂�l]
//   0:		����I��
//   -1:	�ُ�I��(�ϊ����s)
// [�@�\����]
//   DLL�̃G���g���[
//---------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
