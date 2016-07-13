
#pragma once


// 依存ファイル
#include <windows.h>
#include <lm.h>
#include <string>


// ユーザ情報管理の例外クラス
//


// アクセス権がない例外
//
class AccessDeniedError : public std::logic_error {
public:
	AccessDeniedError()
		: std::logic_error( "ユーザーには、要求した情報へのアクセス権がありません。" ) {};
};


// コンピュータ名が異常例外
//
class InvalidComputerError : public std::logic_error {
public:
	InvalidComputerError( const std::string& computerName )
		: std::logic_error( std::string( "コンピュータ名(" ) + computerName + ")が無効です。" ) {};
};


// ユーザーが見つからない例外
//
class UserNotFoundError : public std::logic_error {
public:
	UserNotFoundError( const std::string& userName )
		: std::logic_error( std::string( "指定されたユーザー名(" ) + userName + ")が見つかりませんでした。" ) {};
};


// グループが見つからない例外
//
class GroupNotFoundError : public std::logic_error {
public:
	GroupNotFoundError( const std::string& groupName )
		: std::logic_error( std::string( "指定されたローカルグループ(" ) + groupName + ")が存在しません。" ) {};
};


// 指定したメンバーのいくつかが見つからない
//
class NoSuchMemberError : public std::logic_error {
public:
	NoSuchMemberError()
		: std::logic_error( std::string( "新しいメンバのいずれかまたは複数が存在しません。"
										 "そのため、新しいメンバはまったく追加されませんでした。" ) ) {};
};


// すでにローカルグループのメンバーです例外
//
class MemberInAliasError : public std::logic_error {
public:
	MemberInAliasError()
		: std::logic_error( std::string( "指定されたメンバのいずれかまたは複数は、"
										 "既にローカルグループのメンバになっていました。"
										 "そのため、新しいメンバはまったく追加されませんでした。" ) ) {};
};


// 無効なメンバーエラー
//
class InvalidMemberError : public std::logic_error {
public:
	InvalidMemberError()
		: std::logic_error( std::string( "1 人または複数のメンバのアカウントタイプが無効なので、"
										 "それらのメンバを追加できません。そのため、"
										 "新しいメンバはまったく追加されませんでした。" ) ) {};
};


// ユーザ情報管理クラス
// USER_INFO_3相当品
//
class UserInfo {
private:
	// USER_INFO_3相当のメンバー変数群
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
	// メンバー変数をクリアする
	void ClearMember();

public:
	// デフォルト コンストラクタ
	UserInfo() {
		// メンバー変数を初期化
		ClearMember();
	};

	// コピー コンストラクタ
	UserInfo( const UserInfo& src ) {
		// 代入演算子を使ってコピーする
		*this = src;
	};

	// ユーザー情報初期化つきコンストラクタ
	UserInfo( const std::string& userName ) {
		try {
			// ユーザー情報の取得
			GetInfo( userName );
		} catch (...) {
			// コンストラクタで発生する例外は握りつぶす。
		}
	};

	// デストラクタ
	virtual ~UserInfo() {};

public:
	// インスタンスが有効な情報を保持しているかどうか返す
	bool IsAvailable() {
		return !m_name.empty();
	};

	// ユーザー情報の取得
	void GetInfo( const std::string& userName );

	// 代入演算子
	UserInfo& operator=( const UserInfo& src );

	// パスワードは無期限か否か
	bool DontExpirePassword() const {
		return ( m_flags & UF_DONT_EXPIRE_PASSWD ) ? true : false;
	};

	// ユーザーをグループに追加する
	void AddToGroup( const std::string& groupName );

	// パスワードを変更する
	bool ChangePassword( const std::string& oldPassword, const std::string& newPassword );

	// ログインする
	DWORD LoginCheck( const std::string& pass ) const;

	// ユーザーを追加する
	static UserInfo AddUser( const std::string& userName );
};
