// GuaranteeUser.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

// �����p���[�e�B���e�B�N���X
//
class KjmUtil {
public:

	// char�ȕ������wchar_t������ɕϊ�
	static std::wstring StrToWStr(const char *src);
};

std::wstring KjmUtil::StrToWStr(const char *src)
{
	size_t needed = mbstowcs(NULL, src, strlen(src));

	std::vector<std::wstring::value_type> dst(needed);

	mbstowcs( &dst[0], src, strlen(src) );

	return std::wstring(dst.begin(), dst.end());
}

// ���[�U�[���Ǘ��N���X
//
class UserInfo {
private:
	USER_INFO_3* m_pui;

	// �m�ۍς݂̃o�b�t�@��j������
	// �o�b�t�@���m�ۂ̏ꍇ�́A�������Ȃ�
	void ReleaseBuffer() {
		if (m_pui) {
			::NetApiBufferFree(m_pui);
			m_pui = NULL;
		}
	};

public:
	// �f�t�H���g�R���X�g���N�^
	UserInfo() : m_pui(NULL) {};

	// ���[�U�[�����w�肵�Ă̏������t���R���X�g���N�^
	UserInfo(const _TCHAR* userName) {
		m_pui = NULL;

		// ���[�U�[���̎擾
		GetInfo(userName);
	};

	// �f�X�g���N�^
	virtual ~UserInfo() {
		// �o�b�t�@���J���B
		ReleaseBuffer();
	};

	// ���̃C���X�^���X���L���ȃ��[�U�[����ێ����Ă��邩�ǂ����̔���
	bool IsAvailable() {
		return (m_pui) ? true : false;
	};

	// �w�肵�����[�U�[�̏����擾
	bool GetInfo(const _TCHAR* userName);

	// �p�X���[�h�����������ǂ������ׂ�
	bool IsDontExpirePassword() {
		return (m_pui->usri3_flags & UF_DONT_EXPIRE_PASSWD) ? true : false;
	};
};

// �w�肵�����[�U�[�̏����擾
//
bool UserInfo::GetInfo(const _TCHAR* userName) {

	// ���łɕʂ̃��[�U�[����ێ����Ă���Ƃ��́A�o�b�t�@���J���B
	ReleaseBuffer();

	// ���[�U�[�����擾
	NET_API_STATUS ret = ::NetUserGetInfo( NULL, KjmUtil::StrToWStr(userName).c_str(), 3, (LPBYTE*)&m_pui );

	return (ret == NERR_Success) ? true : false;
}

// �v���O�����X�^�[�g
//
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1) {
		UserInfo ui(argv[1]);
		std::cout << ui.IsDontExpirePassword() << std::endl;
	}

	return 0;
}

