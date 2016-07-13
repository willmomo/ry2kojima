//---------------------------------------------------------------------------------------
// kjmsub ver.0.2.0.0
//---------------------------------------------------------------------------------------
/*
	�ykjm::util::exitWindows �ɂ��āz
	ver.0.2.0.0 ����ǉ��B

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
		static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);

		// Windows���V���b�g�_�E��������
		static int exitWindows();

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
