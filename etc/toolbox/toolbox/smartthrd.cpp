//---------------------------------------------------------------------
// SMART���Ď�����
//---------------------------------------------------------------------


#include "toolbox.h"


//---------------------------------------------------------------------
// ���O���c��
//---------------------------------------------------------------------
static void logging( const char* message ) {

	// ���O�t�@�C�������쐬
	kjm::path_string fname = g_toolboxini.get_SmartLogFolder().c_str();
	if ( fname.empty() ) {
		fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
	}
	fname.Append( "smart.log" );

	// ���b�Z�[�W�����O�Ɏc��
	std::ofstream ofs;
	ofs.open( fname.c_str(), std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {
		ofs << "[" << kjm::time::GetCurrentTime().ctime().c_str() << "] " << message << std::endl;
		ofs.close();
	}
}


//---------------------------------------------------------------------
// SMART�������O�t�@�C���ɒǋL����
//---------------------------------------------------------------------
static void AppendSmartLog( const char* filename ) {

	std::ofstream ofs;

	ofs.open( filename, std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {

		kjm::physical_drive drive;
		if ( drive.OpenDrive( "\\\\.\\PhysicalDrive0" ) ) {

			ofs << "[" << kjm::time::GetCurrentTime().ctime().c_str() << "]" << std::endl;

			if ( !drive.isSupportedSMART() ) {
				ofs << "S.M.A.R.T���Ή�" << std::endl;
			} else {
				ofs << "��ԁF" << drive.smartReturnStatus() << std::endl;
				ofs << drive.toCSVString() << std::endl;
			}
		}
		ofs.close();
	}
}


//---------------------------------------------------------------------
// �x����������쐬����
//---------------------------------------------------------------------
static void MakeWarningText( const char* filename) {

	// �x�����Ȃ��ɂ���B
	g_smart_warning = "";

	kjm::physical_drive drive;
	if ( drive.OpenDrive( "\\\\.\\PhysicalDrive0" ) ) {
		if ( drive.isSupportedSMART() ) {

			if ( drive.smartReturnStatus() != 0 ) {
				// SMART���T�|�[�g����M���x�`�F�b�N���g���B

				g_smart_warning = "�f�B�X�N�̐M���x���A�ቺ���Ă��܂��B";

			} else {
				// ����̂Ƃ��́A�ʂ�臒l���`�F�b�N����B

				SMARTATTRIBUTESDATA	attrData;
				SMARTHRESHOLDSDATA thData;

				memset( &attrData, 0, sizeof( attrData ) );
				memset( &thData, 0, sizeof( thData ) );

				drive.smartReadAttributeValues( &attrData );
				drive.smartReadAttributeThresholds( &thData );

				for ( int index = 0; index < NUM_ATTRIBUTE_STRUCTS; ++index ) {
					int attrID = attrData.AttributeData[ index ].attributeID;
					if ( 0 != attrID ) {
						char*	attrName;
						int		arrayIndex = 0;
						while ( 0 != kjm::attrValueAndNames[arrayIndex].value ) {
							if ( attrID == kjm::attrValueAndNames[arrayIndex].value ) {
								break;
							}
							++arrayIndex;
						}
						
						if ( 0 != kjm::attrValueAndNames[arrayIndex].value ) {
							attrName = kjm::attrValueAndNames[arrayIndex].name;
						} else {
							attrName = "Unknown Attribute";
						}

						// ���ݒl��臒l��������Ă���Ƃ��́A�x�����o���B
						if ( ( WORD )attrData.AttributeData[ index ].attributeValue
							< ( WORD )thData.ThresholdData[index].attributeThreshold ) {

							g_smart_warning = "�f�B�X�N�̐M���x���A�ቺ���Ă��܂��B";
						}
					}
				}
			}
		}
	}

	// �x�����e�����������́A���O�Ɏc���B
	if ( !g_smart_warning.empty() ) {

		std::stringstream s;
		s << "*** " << g_smart_warning << " ***";
		logging( s.str().c_str() );
	}
}


//---------------------------------------------------------------------
// SMART���Ď��X���b�h
//---------------------------------------------------------------------
unsigned __stdcall smart_thread( void* arg ) {

	static DWORD s_last_time = 0;

	// �J�n���b�Z�[�W
	logging( "<<< SMART���Ď��X�^�[�g >>>" );

	while ( !g_end_flag ) {

		// �Ƃ肠�����x�܂���
		Sleep( 500 );

		// �ݒ莞�Ԍo�ߌ��SMART�������O�Ɏc��
		if ( ( s_last_time != 0 ) && ( abs( GetTickCount() - s_last_time ) < g_toolboxini.get_SmartInterval() * 1000 ) )
			continue;

		// ���O�t�@�C�������쐬
		kjm::path_string fname = g_toolboxini.get_SmartLogFolder().c_str();
		if ( fname.empty() ) {
			fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
		}
		fname.Append( "smart.log" );

		// SMART�������O�t�@�C���ɒǋL����
		AppendSmartLog( fname.c_str() );

		// �x����������쐬����
		MakeWarningText( fname.c_str() );

		// ���O�t�@�C���̃T�C�Y���傫���Ȃ��Ă���Ƃ��́A�ꐢ�㕪�̃o�b�N�A�b�v�����
		BackupLog( fname.c_str() );

		// �������I����Ă���A�w�莞�ԑ҂悤�ɂ���B
		s_last_time = GetTickCount();
	}

	// �I�����b�Z�[�W
	logging( "<<< SMART���Ď��I�� >>>" );

	return 0;
}
