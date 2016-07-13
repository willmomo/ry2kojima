
#pragma once


// �ˑ��t�@�C��
#include <windows.h>
#include <lm.h>
#include <string>


// ���[�U���Ǘ��̗�O�N���X
//


// �A�N�Z�X�����Ȃ���O
//
class AccessDeniedError : public std::logic_error {
public:
	AccessDeniedError()
		: std::logic_error( "���[�U�[�ɂ́A�v���������ւ̃A�N�Z�X��������܂���B" ) {};
};


// �R���s���[�^�����ُ��O
//
class InvalidComputerError : public std::logic_error {
public:
	InvalidComputerError( const std::string& computerName )
		: std::logic_error( std::string( "�R���s���[�^��(" ) + computerName + ")�������ł��B" ) {};
};


// ���[�U�[��������Ȃ���O
//
class UserNotFoundError : public std::logic_error {
public:
	UserNotFoundError( const std::string& userName )
		: std::logic_error( std::string( "�w�肳�ꂽ���[�U�[��(" ) + userName + ")��������܂���ł����B" ) {};
};


// �O���[�v��������Ȃ���O
//
class GroupNotFoundError : public std::logic_error {
public:
	GroupNotFoundError( const std::string& groupName )
		: std::logic_error( std::string( "�w�肳�ꂽ���[�J���O���[�v(" ) + groupName + ")�����݂��܂���B" ) {};
};


// �w�肵�������o�[�̂�������������Ȃ�
//
class NoSuchMemberError : public std::logic_error {
public:
	NoSuchMemberError()
		: std::logic_error( std::string( "�V���������o�̂����ꂩ�܂��͕��������݂��܂���B"
										 "���̂��߁A�V���������o�͂܂������ǉ�����܂���ł����B" ) ) {};
};


// ���łɃ��[�J���O���[�v�̃����o�[�ł���O
//
class MemberInAliasError : public std::logic_error {
public:
	MemberInAliasError()
		: std::logic_error( std::string( "�w�肳�ꂽ�����o�̂����ꂩ�܂��͕����́A"
										 "���Ƀ��[�J���O���[�v�̃����o�ɂȂ��Ă��܂����B"
										 "���̂��߁A�V���������o�͂܂������ǉ�����܂���ł����B" ) ) {};
};


// �����ȃ����o�[�G���[
//
class InvalidMemberError : public std::logic_error {
public:
	InvalidMemberError()
		: std::logic_error( std::string( "1 �l�܂��͕����̃����o�̃A�J�E���g�^�C�v�������Ȃ̂ŁA"
										 "�����̃����o��ǉ��ł��܂���B���̂��߁A"
										 "�V���������o�͂܂������ǉ�����܂���ł����B" ) ) {};
};


// ���[�U���Ǘ��N���X
// USER_INFO_3�����i
//
class UserInfo {
private:
	// USER_INFO_3�����̃����o�[�ϐ��Q
	std::string m_name;
	std::string m_password;
	DWORD m_password_age;
	DWORD m_priv;
	std::string m_home_dir;
	std::string m_comment;
	DWORD m_flags;
	std::string m_script_path;
	DWORD m_auth_flags;
	std::string m_full_name;
	std::string m_usr_comment;
	std::string m_parms;
	std::string m_workstations;
	DWORD m_last_logon;
	DWORD m_last_logoff;
	DWORD m_acct_expires;
	DWORD m_max_storage;
	DWORD m_units_per_week;
	BYTE m_logon_hours[21];
	DWORD m_bad_pw_count;
	DWORD m_num_logons;
	std::string m_logon_server;
	DWORD m_country_code;
	DWORD m_code_page;
	DWORD m_user_id;
	DWORD m_primary_group_id;
	std::string m_profile;
	std::string m_home_dir_drive;
	DWORD m_password_expired;

private:
	// �����o�[�ϐ����N���A����
	void ClearMember();

public:
	// �f�t�H���g �R���X�g���N�^
	UserInfo() {
		// �����o�[�ϐ���������
		ClearMember();
	};

	// �R�s�[ �R���X�g���N�^
	UserInfo( const UserInfo& src ) {
		// ������Z�q���g���ăR�s�[����
		*this = src;
	};

	// ���[�U�[��񏉊������R���X�g���N�^
	UserInfo( const std::string& userName ) {
		try {
			// ���[�U�[���̎擾
			GetInfo( userName );
		} catch (...) {
			// �R���X�g���N�^�Ŕ��������O�͈���Ԃ��B
		}
	};

	// �f�X�g���N�^
	virtual ~UserInfo() {};

public:
	// �C���X�^���X���L���ȏ���ێ����Ă��邩�ǂ����Ԃ�
	bool IsAvailable() {
		return !m_name.empty();
	};

	// ���[�U�[���̎擾
	void GetInfo( const std::string& userName );

	// ������Z�q
	UserInfo& operator=( const UserInfo& src );

	// �p�X���[�h�͖��������ۂ�
	bool DontExpirePassword() const {
		return ( m_flags & UF_DONT_EXPIRE_PASSWD ) ? true : false;
	};

	// ���[�U�[���O���[�v�ɒǉ�����
	void AddToGroup( const std::string& groupName );

	// �p�X���[�h��ύX����
	bool ChangePassword( const std::string& oldPassword, const std::string& newPassword );

	// ���O�C������
	DWORD LoginCheck( const std::string& pass ) const;

	// ���[�U�[��ǉ�����
	static UserInfo AddUser( const std::string& userName );
};
