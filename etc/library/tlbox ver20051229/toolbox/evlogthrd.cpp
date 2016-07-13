//---------------------------------------------------------------------
// �C�x���g���O�Ď�����
//---------------------------------------------------------------------


#include "toolbox.h"


//---------------------------------------------------------------------
// ���O���c��
//---------------------------------------------------------------------
static void logging( const char* message ) {

	// ���O�t�@�C�������쐬
	kjm::path_string fname = g_toolboxini.get_EventlogLogFolder().c_str();
	if ( fname.empty() ) {
		fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
	}
	fname.Append( "evlog.log" );

	// ���b�Z�[�W�����O�Ɏc��
	std::ofstream ofs;
	ofs.open( fname.c_str(), std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {
		ofs << "[" << kjm::time::GetCurrentTime().ctime().c_str() << "] " << message << std::endl;
		ofs.close();
	}
}


//---------------------------------------------------------------------
// �C�x���g���O��ۑ�
//---------------------------------------------------------------------
static void SaveEventLog( const char* event_name, const char* pszFname ) {

	// �ۑ��p�̃t�@�C�������쐬
	kjm::path_string fname = g_toolboxini.get_EventlogLogFolder().c_str();
	if ( fname.c_str() ) {
		fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
	}
	fname.Append( pszFname );

	// �㏑���ł��Ȃ����߁A�t�@�C�������݂���Ƃ��́A�폜����B
	if ( fname.FileExists() ) {
		remove( fname.c_str() );
	}

	// �V�X�e�� �C�x���g���O���f�B�X�N�ɕۑ�����B
	kjm::eventlog evlog;
	evlog.OpenEventLog( NULL, _T( event_name ) );
	evlog.BackupEventLog( fname.c_str() );
}


//---------------------------------------------------------------------
// �x����������쐬����
//---------------------------------------------------------------------
static void MakeWarningText( const char* filename) {

	// �x�����Ȃ��ɂ���B
	g_evlog_warning = "";

	// System �C�x���g���O���擾
	kjm::eventlog evlog;
	evlog.OpenEventLog( NULL, _T( "System" ) );

	// �C�x���g���O���� ID9 ��51��T���B
	kjm::eventlog_record rec;
	while ( evlog.ReadEventLog( EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, rec ) ) {

		// �ȉ��̃G���[���������Ƃ��A�f�B�X�N���댯�Ɣ��f����B
		// ID9: �f�o�C�X \Device\Ide\IdePort0 �̓^�C���A�E�g���ԓ��ɉ������܂���ł����B
		// ID51: �y�[�W���O���쒆�Ƀf�o�C�X \Device\Harddisk0\DR0 ��ŃG���[�����o����܂����B

		DWORD id1 = rec.EventID() & 0xffff;
		DWORD id2 = rec.EventID() & 0xffff;
		if ( id1 == 9 || id2 == 51 ) {
			g_evlog_warning = "�f�B�X�N�ُ̈킪���m����܂����B";
			break;
		}
	}

	// �x��������Ƃ��́A���O�Ɏc��
	if ( !g_evlog_warning.empty() ) {

		std::stringstream s;
		s << "*** " << g_evlog_warning << " ***";
		logging( s.str().c_str() );
	}
}


//---------------------------------------------------------------------
// �C�x���g���O�Ď��X���b�h
//---------------------------------------------------------------------
unsigned __stdcall eventlog_thread( void* arg ) {

	static DWORD s_last_time = 0;

	// �J�n���b�Z�[�W
	logging( "<<< �C�x���g���O�Ď��X�^�[�g >>>" );

	while ( !g_end_flag ) {

		// �Ƃ肠�����x�܂���
		Sleep( 500 );

		// �ݒ莞�Ԍo�ߌ��SMART�������O�Ɏc��
		if ( ( s_last_time != 0 ) && ( abs( GetTickCount() - s_last_time ) < g_toolboxini.get_EventlogInterval() * 1000 ) )
			continue;

		// ���O�t�@�C�������쐬
		kjm::path_string fname = g_toolboxini.get_EventlogLogFolder().c_str();
		if ( fname.empty() ) {
			fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
		}
		fname.Append( "evlog.log" );

		// �V�X�e��/�A�v���P�[�V���� �C�x���g���O��ۑ�
		SaveEventLog( "System", "system.evt" );
		SaveEventLog( "Application", "application.evt" );

		// �x����������쐬����
		MakeWarningText( fname.c_str() );

		// ���O�t�@�C���̃T�C�Y���傫���Ȃ��Ă���Ƃ��́A�ꐢ�㕪�̃o�b�N�A�b�v�����
		BackupLog( fname.c_str() );

		s_last_time = GetTickCount();

	}

	// �I�����b�Z�[�W
	logging( "<<< �C�x���g���O�Ď��I�� >>>" );

	return 0;
}
