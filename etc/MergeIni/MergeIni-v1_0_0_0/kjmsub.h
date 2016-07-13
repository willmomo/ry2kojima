//---------------------------------------------------------------------------------------
// kjmsub ver.0.4.0.0
//---------------------------------------------------------------------------------------
/*
	�ykjm::util::getFileSize �ɂ��āz
	ver.0.3.0.0 ����ǉ��B

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


#pragma warning(disable : 4786)

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <algorithm>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#pragma comment(lib, "wsslog.lib")
#endif

typedef std::basic_string<TCHAR>	_tstring;


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
	// ���ʂ̃��O����
	class log {
	public:
		// �f�t�H���g�R���X�g���N�^
		log() : m_ident() {};

		// �f�X�g���N�^
		~log() {};

		// �G���[���O
		void writeError(LPCTSTR pszFormat, ...);

		// ��񃍃O
		void writeInfo(LPCTSTR pszFormat, ...);

		// �S���O
		void writeAll(LPCTSTR pszFormat, ...);

		// �Â����O�̍폜
		void deleteOldLog();

		// ���ʎq�̎擾
		_tstring get_Ident() { return m_ident; };

		// ���ʎ��̐ݒ�
		void set_Ident(const _tstring& value) { m_ident = value; };

	private:
		_tstring m_ident;
	};


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
		static _tstring formatMessageBySystem(DWORD dwErr);

		// GetComputerNameEx�֐�(vc6�p)
		static BOOL __GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize);

		// string��Ԃ�GetComputerNameEx�֐�
		// [��O] throw ::GetLastError();
		static _tstring getComputerNameEx(COMPUTER_NAME_FORMAT NameType);

		// string��Ԃ�GetDlgItemText�֐�
		// [����] hDlg        �_�C�A���O�{�b�N�X�̃n���h��
		// [����] nIDDlgItem  �R���g���[���̎��ʎq
		// [��O] throw ::GetLastError();
		static _tstring getDlgItemText(HWND hDlg, int nIDDlgItem);

		// �w��t�@�C���̃T�C�Y��Ԃ�(GetFileSize API�̃��b�p�[)
		// [����]  lpszFileName  �t�@�C����
		// [�߂�l] 64bit�̃t�@�C���T�C�Y
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�����
		static BOOL moveCenterWorkarea(HWND hwnd);

		// SetComputerNameEx�֐�(vc6�p)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);


		// ���S�� vsprintf
		static int vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr);

		// ���S�� sprintf 
		//
		// [����]
		// ��Ԗڂ̈������Avector<TCHAR>�ւ̎Q�ƂɂȂ��Ă���ȊO�A
		// sprintf�֐��Ɠ����ł��B
		static int sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...);

		// �����ƃ��[�X�̂��߂̃��O�I�u�W�F�N�g���w�肷��
		static void set_logObj(kjm::log* pLogObj);

	private:
		util() {};
		~util() {};
	};

	//-------------------------------------------------------------------------
	// ini�t�@�C������������N���X
	//-------------------------------------------------------------------------
	class iniFile {
	public:
		// �R���X�g���N�^
		iniFile() : m_fileName() {};

		// �R�s�[�R���X�g���N�^
		iniFile(const iniFile& src) : m_fileName(src.m_fileName) {};

		// �t�@�C������^����R���X�g���N�^
		iniFile(const _tstring& szFileName) : m_fileName(szFileName) {};

		// �f�X�g���N�^
		virtual ~iniFile() {};

		// ������Z�q
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		};

		// �Z�N�V�����ꗗ���擾
		std::vector<_tstring> getSectionNames() const;

		// �Z�N�V�������̃L�[�ꗗ���擾
		std::vector<_tstring> getKeyNames(const _tstring& strSectionName) const;

		// ini�t�@�C������l���擾
		_tstring getString(const _tstring& strSection, const _tstring& strKey, const _tstring& strDefault, BOOL* pfUseDefault = NULL) const;

		// ini�t�@�C���ɒl����������
		BOOL writeString(const _tstring& strSection, const _tstring& strKey, const _tstring& strValue);

		// �ʂ�ini�t�@�C���̓��e���}�[�W����
		void mergeIniFile(const iniFile& newIniFile);

		// ini�t�@�C�������擾
		_tstring get_fileName() const { return m_fileName; };

		// ini�t�@�C������ݒ�
		void set_fileName(const _tstring& value) { m_fileName = value; };

	private:
		_tstring m_fileName;	// ini�t�@�C���̖��O
	};
}

