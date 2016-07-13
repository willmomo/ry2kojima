
#include <iostream>
#include "UserInfo.h"


void CheckUserInfo( const UserInfo& ui ) {
	// ユーザーのパスワードは無期限か？
	if ( ui.DontExpirePassword() ) {
	}

	// ユーザーに指定パスワードでログインできるか？
	if ( ui.LoginCheck( "mpcadmin" ) ) {
	}
}


void AddUser() {
	// 存在しないときは、新たに作成する。
	UserInfo ui = UserInfo::AddUser( "mpcadmin" );

	// ユーザーを指定グループに登録する
	ui.AddToGroup( "Administrators" );

	// パスワードを設定する
	ui.ChangePassword( NULL, "mpcadmin" );
}

int main(int argc, char** argv) {

	UserInfo ui( "/mpcadmin" );

	// 指定ユーザーがシステムに存在するか？
	if ( ui.IsAvailable() ) {
		CheckUserInfo( ui );
	} else {
		AddUser();
	}

	return 0;
}
