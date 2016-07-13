
#include <iostream>
#include "UserInfo.h"


void CheckUserInfo( const UserInfo& ui ) {
	// ���[�U�[�̃p�X���[�h�͖��������H
	if ( ui.DontExpirePassword() ) {
	}

	// ���[�U�[�Ɏw��p�X���[�h�Ń��O�C���ł��邩�H
	if ( ui.LoginCheck( "mpcadmin" ) ) {
	}
}


void AddUser() {
	// ���݂��Ȃ��Ƃ��́A�V���ɍ쐬����B
	UserInfo ui = UserInfo::AddUser( "mpcadmin" );

	// ���[�U�[���w��O���[�v�ɓo�^����
	ui.AddToGroup( "Administrators" );

	// �p�X���[�h��ݒ肷��
	ui.ChangePassword( NULL, "mpcadmin" );
}

int main(int argc, char** argv) {

	UserInfo ui( "/mpcadmin" );

	// �w�胆�[�U�[���V�X�e���ɑ��݂��邩�H
	if ( ui.IsAvailable() ) {
		CheckUserInfo( ui );
	} else {
		AddUser();
	}

	return 0;
}
