//---------------------------------------------------------------------------------------
// kjmsub ver.0.0.0.0
//---------------------------------------------------------------------------------------
/*
	�yGetComputerNameEx�ɂ��āz
	
	Visual C++ 6�ł́AGetComputerNameEx�֐�����`����Ă��܂���B
	kjm::util::__GetComputerNameEx�̗p�Ɏ������邱�ƂŁA���p���邱�Ƃ��ł��܂��B

	�R���s���[�^���ɂ́ANetBIOS����DNS�z�X�g��������܂����AWindows2000�ȍ~�ł́ADNS�z�X
	�g�������C���ɂȂ�܂��B�܂��A�����I�ȃR���s���[�^���ƃN���X�^�@�\���g�����Ƃ�(���Ǝv
	��)�ɕt���A�_���I�ȃR���s���[�^��������܂��B

	��ʓI�ȃR���s���[�^�����擾����ɂ́A��������ComputerNamePhysicalDnsHostname ���w
	�肵�āAGetComputerNameEx�֐����Ăяo���܂��B

	�yVisual C++ �̃o�[�W��������ɂ��āz

	_MSC_VER == 1200	Visual C++ 6.0
	_MSC_VER == 1310	Visual C++ .NET 2003
	_MSC_VER == 1400	Visual C++ 2005
*/
#pragma once


#include <windows.h>
#include <windowsx.h>

#include <string>


// winbase.h �Ő錾����Ă���AGet/SetComputerNameEx�p�̈����̒l
// Visual C++ 6.0 �ȉ��̂Ƃ��ɗL���ɂ���B
#if _MSC_VER <= 1200
typedef enum _COMPUTER_NAME_FORMAT {
    ComputerNameNetBIOS,
    ComputerNameDnsHostname,
    ComputerNameDnsDomain,
    ComputerNameDnsFullyQualified,
    ComputerNamePhysicalNetBIOS,
    ComputerNamePhysicalDnsHostname,
    ComputerNamePhysicalDnsDomain,
    ComputerNamePhysicalDnsFullyQualified,
    ComputerNameMax
} COMPUTER_NAME_FORMAT ;
#endif


namespace kjm {
	// ���[�e�B���e�B�֐��p��static�N���X
	// [���l]
	// ���ׂĂ̊֐��́Astatic�Ő錾����܂��B
	class util {
	public:
		// �V�X�e���W���̃G���[���b�Z�[�W���쐬
		static std::basic_string<TCHAR> formatMessageBySystem(DWORD dwErr);

		// GetComputerNameEx�֐�(vc6�p)
		static BOOL __GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize);

		// string��Ԃ�GetComputerNameEx�֐�
		// [��O] throw ::GetLastError();
		static std::basic_string<TCHAR> getComputerNameEx(COMPUTER_NAME_FORMAT NameType);

		// string��Ԃ�GetDlgItemText�֐�
		// [����] hDlg        �_�C�A���O�{�b�N�X�̃n���h��
		// [����] nIDDlgItem  �R���g���[���̎��ʎq
		// [��O] throw ::GetLastError();
		static std::basic_string<TCHAR> getDlgItemText(HWND hDlg, int nIDDlgItem);

		// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�����
		static BOOL moveCenterWorkarea(HWND hwnd);

		// SetComputerNameEx�֐�(vc6�p)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);

	private:
		util() {};
		~util() {};
	};
}
