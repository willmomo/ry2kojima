
#include "UserInfo.h"
#include "KjmUtil.h"
#include <sstream>

// �����o�[�ϐ����N���A����
//
void UserInfo::ClearMember() {
	m_name.clear();		// ���̃����o�[�� empty �̂Ƃ��Anot available
	m_password.clear();
	m_password_age = 0;
	m_priv = 0;
	m_home_dir.clear();
	m_comment.clear();
	m_flags = 0;
	m_script_path.clear();
	m_auth_flags = 0;
	m_full_name.clear();
	m_usr_comment.clear();
	m_parms.clear();
	m_workstations.clear();
	m_last_logon = 0;
	m_last_logoff = 0;
	m_acct_expires = 0;
	m_max_storage = 0;
	m_units_per_week = 0;
	memset( m_logon_hours, 0, sizeof(m_logon_hours) );
	m_bad_pw_count = 0;
	m_num_logons = 0;
	m_logon_server.clear();
	m_country_code = 0;
	m_code_page = 0;
	m_user_id = 0;
	m_primary_group_id = 0;
	m_profile.clear();
	m_home_dir_drive.clear();
	m_password_expired = 0;
}


// ������Z�q
//
UserInfo& UserInfo::operator =( const UserInfo& src ) {
	// �������g�ɂ́A�R�s�[���Ȃ��B
	if ( this == &src ) {
		return *this;
	}

	// �����o�[�ϐ����ЂƂ��R�s�[
	m_name = src.m_name;
	m_password = src.m_password;
	m_password_age = src.m_password_age;
	m_priv = src.m_priv;
	m_home_dir = src.m_home_dir;
	m_comment = src.m_comment;
	m_flags = src.m_flags;
	m_script_path = src.m_script_path;
	m_auth_flags = src.m_auth_flags;
	m_full_name = src.m_full_name;
	m_usr_comment = src.m_usr_comment;
	m_parms = src.m_parms;
	m_workstations = src.m_workstations;
	m_last_logon = src.m_last_logon;
	m_last_logoff = src.m_last_logoff;
	m_acct_expires = src.m_acct_expires;
	m_max_storage = src.m_max_storage;
	m_units_per_week = src.m_units_per_week;
	memmove( m_logon_hours, src.m_logon_hours, sizeof(m_logon_hours) );
	m_bad_pw_count = src.m_bad_pw_count;
	m_num_logons = src.m_num_logons;
	m_logon_server = src.m_logon_server;
	m_country_code = src.m_country_code;
	m_code_page = src.m_code_page;
	m_user_id = src.m_user_id;
	m_primary_group_id = src.m_primary_group_id;
	m_profile = src.m_profile;
	m_home_dir_drive = src.m_home_dir_drive;
	m_password_expired = src.m_password_expired;

	return *this;
}


// ���[�U�[���̎擾
//
void UserInfo::GetInfo( const std::string& userName ) {
	// ���łɕێ����Ă�������N���A����
	ClearMember();

	// ���[�U�����擾
	USER_INFO_3 *pui = NULL;
	NET_API_STATUS stat = ::NetUserGetInfo( NULL, KjmUtil::StrToWStr( userName ).c_str(), 3, (LPBYTE*)&pui );

	switch ( stat ) {
		case NERR_Success:
			// ���[�U���������o�[�ϐ��ɓW�J����
			m_name = KjmUtil::WStrToStr(pui->usri3_name);
			m_password = KjmUtil::WStrToStr(pui->usri3_password);
			m_password_age = pui->usri3_password_age;
			m_priv = pui->usri3_priv;
			m_home_dir = KjmUtil::WStrToStr(pui->usri3_home_dir);
			m_comment = KjmUtil::WStrToStr(pui->usri3_comment);
			m_flags = pui->usri3_flags;
			m_script_path = KjmUtil::WStrToStr(pui->usri3_script_path);
			m_auth_flags = pui->usri3_auth_flags;
			m_full_name = KjmUtil::WStrToStr(pui->usri3_full_name);
			m_usr_comment = KjmUtil::WStrToStr(pui->usri3_usr_comment);
			m_parms = KjmUtil::WStrToStr(pui->usri3_parms);
			m_workstations = KjmUtil::WStrToStr(pui->usri3_workstations);
			m_last_logon = pui->usri3_last_logon;
			m_last_logoff = pui->usri3_last_logoff;
			m_acct_expires = pui->usri3_acct_expires;
			m_max_storage = pui->usri3_max_storage;
			m_units_per_week = pui->usri3_units_per_week;
			memmove( m_logon_hours, pui->usri3_logon_hours, sizeof(m_logon_hours) );
			m_bad_pw_count = pui->usri3_bad_pw_count;
			m_num_logons = pui->usri3_num_logons;
			m_logon_server = KjmUtil::WStrToStr(pui->usri3_logon_server);
			m_country_code = pui->usri3_country_code;
			m_code_page = pui->usri3_code_page;
			m_user_id = pui->usri3_user_id;
			m_primary_group_id = pui->usri3_primary_group_id;
			m_profile = KjmUtil::WStrToStr(pui->usri3_profile);
			m_home_dir_drive = KjmUtil::WStrToStr(pui->usri3_home_dir_drive);
			m_password_expired = pui->usri3_password_expired;

			// �o�b�t�@���J������
			::NetApiBufferFree(pui);
			break;

		case ERROR_ACCESS_DENIED:
			throw AccessDeniedError();
			break;
		case NERR_InvalidComputer:
			throw InvalidComputerError( "" );
			break;
		case NERR_UserNotFound:
			throw UserNotFoundError( userName );
			break;
		default:
			{
				std::ostringstream buf;
				buf << "NetUserGetInfo() returned " << stat;
				throw std::logic_error( buf.str() );
			}
			break;
	}
}


// �w�胆�[�U�[�Ń��O�C���\���ǂ����̃`�F�b�N
//
// [����]
// password	���O�I���Ɏg�p����p�X���[�h
//
// [�߂�l]
// GetLastError()�̖߂�l�B0�̏ꍇ�͐���I��(���O�I������)�B
//
DWORD UserInfo::LoginCheck( const std::string& password ) const {
	// �w��p�X���[�h�Ń��O�I�����Ă݂�
	HANDLE hToken = NULL;
	BOOL ret = ::LogonUser( (LPTSTR)m_name.c_str(), NULL, 
		(LPTSTR)password.c_str(), LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken );
	if ( ret ) {
		::CloseHandle( hToken );
	}

	return ::GetLastError();
}


// �p�X���[�h��ύX����
//
// [����]
// oldPassword	�Â��p�X���[�h
// newPassword	�V�����p�X���[�h
//
bool UserInfo::ChangePassword( const std::string& oldPassword, const std::string& newPassword ) {
	bool ret = false;

	// ���p�X���[�h���w�肳��Ă���Ƃ��́A
	// �p�X���[�h�̕ύX
	if ( oldPassword.empty() ) {
		// �V�E���p�X���[�h�����j�R�[�h�ɕϊ�
		std::wstring userNameW = KjmUtil::StrToWStr( m_name );
		std::wstring oldPasswordW = KjmUtil::StrToWStr( oldPassword );
		std::wstring newPasswordW = KjmUtil::StrToWStr( newPassword );

		NET_API_STATUS nerr = ::NetUserChangePassword(
			NULL, userNameW.c_str(), oldPasswordW.c_str(), newPasswordW.c_str() );
		if ( ret == NERR_Success ) {
			ret = true;
		}
	} else {
		// �w�肳��Ă��Ȃ��Ƃ��́A�p�X���[�h�̐ݒ�
		// �V�E���p�X���[�h�����j�R�[�h�ɕϊ�
		std::wstring userNameW = KjmUtil::StrToWStr( m_name );
		std::wstring newPasswordW = KjmUtil::StrToWStr( newPassword );

		USER_INFO_1003 ui = { (LPWSTR)newPasswordW.c_str() };
		DWORD parm_err = 0;
		
		NET_API_STATUS nerr = ::NetUserSetInfo( NULL, (LPWSTR)userNameW.c_str(), 1003, (LPBYTE)&ui, &parm_err );
		if ( nerr == NERR_Success ) {
			ret = true;
		}
	}

	return ret;
}


// ���[�U�[���O���[�v�ɒǉ�����
//
// [����]
// groupName	���̃��[�U�[���ǉ������O���[�v��
//
// [��O]
// GroupNotFoundError
// NoSuchMemberError
// MemberInAliasError
// InvalidMemberError
// logic_error
//
void UserInfo::AddToGroup( const std::string& groupName ) {
	// �O���[�v���́AUnicode�ŕێ�����B
	std::wstring groupNameW = KjmUtil::StrToWStr( groupName );

	// ���[�U�[����Unicode�ŕێ�����B
	std::wstring userNameW = KjmUtil::StrToWStr( m_name );

	// �O���[�v�ɒǉ�����B
	LOCALGROUP_MEMBERS_INFO_3 memInfo;
	memset( &memInfo, 0, sizeof(memInfo) );
	memInfo.lgrmi3_domainandname = (LPWSTR)userNameW.c_str();
	NET_API_STATUS stat = ::NetLocalGroupAddMembers( NULL, groupNameW.c_str(), 3, (LPBYTE)&memInfo, 1 );

	switch ( stat ) {
		case NERR_GroupNotFound:
			throw GroupNotFoundError( groupName );
			break;
		case ERROR_NO_SUCH_MEMBER:
			throw NoSuchMemberError();
			break;
		case ERROR_MEMBER_IN_ALIAS:
			throw MemberInAliasError();
			break;
		case ERROR_INVALID_MEMBER:
			throw InvalidMemberError();
			break;
		default:
			{
				std::ostringstream buf;
				buf << "NetLocalGroupAddMembers() returned " << stat;
				throw std::logic_error( buf.str() );
			}
			break;
	}
}


// ���[�U�[��ǉ�����
//
UserInfo UserInfo::AddUser( const std::string& userName ) {
	// �ǉ����郆�[�U�[�����쐬
	USER_INFO_1 ui;

	memset(&ui, 0, sizeof(ui));
	std::wstring userNameW = KjmUtil::StrToWStr(userName);
	ui.usri1_name = (LPWSTR)userNameW.c_str();
	ui.usri1_password = L"";
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_flags = UF_DONT_EXPIRE_PASSWD;

	DWORD parm_err = 0;
	NET_API_STATUS ret = ::NetUserAdd(NULL, 1, (LPBYTE)&ui, &parm_err);
	if ( ret == NERR_Success ) {
		return UserInfo( userName );
	} else {
		switch ( ret ) {
			case ERROR_ACCESS_DENIED:
				break;
			case NERR_InvalidComputer:
				break;
			case NERR_NotPrimary:
				break;
			case NERR_GroupExists:
				break;
			case NERR_UserExists:
				break;
			case NERR_PasswordTooShort:
				break;
			default:
				break;
		}
	}

	return UserInfo();
}
