#pragma once


#include <windows.h>
#include <string>
#include <sstream>

#ifdef UNICODE
typedef std::wstring		_tstring;
typedef std::wostringstream	_tostringstream;
#else
typedef std::string			_tstring;
typedef std::ostringstream	_tostringstream;
#endif


namespace kjmutil {
	/* �G���[���������N���X

	   [�g�p��]
	   kjmutil::err lastError;
	   _tprintf(_T("error %d: %s\n"), lastError.get_number(), lastError.get_description().c_str());
	*/
	class err {
	public:
		/* �f�t�H���g�R���X�g���N�^
		   �I�u�W�F�N�g�쐬���� GetLastError �̒l�ŏ����������B
		*/
		err() : m_dwErrCode(GetLastError()) {};

		/* �R���X�g���N�^(�G���[�ԍ��������t��) */
		err(DWORD dwErrCode) : m_dwErrCode(dwErrCode) {};

		/* �f�X�g���N�^ */
		~err() {};

		/* �G���[�ԍ��̎擾 */
		DWORD get_number() { return m_dwErrCode; };

		/* �G���[�ԍ��̐ݒ� */
		void set_number(DWORD dwErrCode) { m_dwErrCode = dwErrCode; };

		/* �G���[�̐������擾
		   FormatMessage �֐��Ŏ擾�ł���G���[���b�Z�[�W�ł��B
		   �ʏ�A�Ō�ɕt�����s���폜���Ă���܂��B
		*/
		_tstring get_description();

	private:
		DWORD m_dwErrCode;
	};

	
	/* RECT �������N���X */
	class rect : public RECT {
	public:
		/* �f�t�H���g�R���X�g���N�^ */
		rect() { SetRectEmpty(this); };

		/* �f�X�g���N�^ */
		~rect() {};

		/* �X�N���[�����W���N���C�A���g���W�ɕϊ� */
		rect& screen_to_client(HWND hwnd);

		/* ��`���I�t�Z�b�g���� */
		rect& offset(int dx, int dy);

		/* �� */
		int get_width() const { return right - left; };

		/* ���� */
		int get_height() const { return bottom - top; };

		/* �E�B���h�E�̃N���C�A���g���W���擾���܂� */
		static rect get_client_rect(HWND hwnd);

		/* �E�B���h�E�̍��W���擾���܂� */
		static rect get_window_rect(HWND hwnd);

		/* �w�肳�ꂽ 2 �̒����`�����S�Ɋ܂ށA�ŏ��̒����`���쐬���܂� */
		static rect union_rect(const RECT* lprcSrc1, const RECT* lprcSrc2);
	};


	/* ������̍Ō�̉��s����菜�� */
	void chomp(_tstring& str);

	/* kjmutil::rect�������ɂƂ�MoveWindow�֐� */
	BOOL move_window(HWND hWnd, const kjmutil::rect& rc, BOOL bRepaint);

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
	BOOL IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait);
};
