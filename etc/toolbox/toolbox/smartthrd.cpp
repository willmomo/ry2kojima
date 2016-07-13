//---------------------------------------------------------------------
// SMART情報監視処理
//---------------------------------------------------------------------


#include "toolbox.h"


//---------------------------------------------------------------------
// ログを残す
//---------------------------------------------------------------------
static void logging( const char* message ) {

	// ログファイル名を作成
	kjm::path_string fname = g_toolboxini.get_SmartLogFolder().c_str();
	if ( fname.empty() ) {
		fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
	}
	fname.Append( "smart.log" );

	// メッセージをログに残す
	std::ofstream ofs;
	ofs.open( fname.c_str(), std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {
		ofs << "[" << kjm::time::GetCurrentTime().ctime().c_str() << "] " << message << std::endl;
		ofs.close();
	}
}


//---------------------------------------------------------------------
// SMART情報をログファイルに追記する
//---------------------------------------------------------------------
static void AppendSmartLog( const char* filename ) {

	std::ofstream ofs;

	ofs.open( filename, std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {

		kjm::physical_drive drive;
		if ( drive.OpenDrive( "\\\\.\\PhysicalDrive0" ) ) {

			ofs << "[" << kjm::time::GetCurrentTime().ctime().c_str() << "]" << std::endl;

			if ( !drive.isSupportedSMART() ) {
				ofs << "S.M.A.R.T未対応" << std::endl;
			} else {
				ofs << "状態：" << drive.smartReturnStatus() << std::endl;
				ofs << drive.toCSVString() << std::endl;
			}
		}
		ofs.close();
	}
}


//---------------------------------------------------------------------
// 警告文字列を作成する
//---------------------------------------------------------------------
static void MakeWarningText( const char* filename) {

	// 警告をなしにする。
	g_smart_warning = "";

	kjm::physical_drive drive;
	if ( drive.OpenDrive( "\\\\.\\PhysicalDrive0" ) ) {
		if ( drive.isSupportedSMART() ) {

			if ( drive.smartReturnStatus() != 0 ) {
				// SMARTがサポートする信頼度チェックを使う。

				g_smart_warning = "ディスクの信頼度が、低下しています。";

			} else {
				// 正常のときは、個別の閾値をチェックする。

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

						// 現在値が閾値を下回っているときは、警告を出す。
						if ( ( WORD )attrData.AttributeData[ index ].attributeValue
							< ( WORD )thData.ThresholdData[index].attributeThreshold ) {

							g_smart_warning = "ディスクの信頼度が、低下しています。";
						}
					}
				}
			}
		}
	}

	// 警告内容が合った時は、ログに残す。
	if ( !g_smart_warning.empty() ) {

		std::stringstream s;
		s << "*** " << g_smart_warning << " ***";
		logging( s.str().c_str() );
	}
}


//---------------------------------------------------------------------
// SMART情報監視スレッド
//---------------------------------------------------------------------
unsigned __stdcall smart_thread( void* arg ) {

	static DWORD s_last_time = 0;

	// 開始メッセージ
	logging( "<<< SMART情報監視スタート >>>" );

	while ( !g_end_flag ) {

		// とりあえず休ませる
		Sleep( 500 );

		// 設定時間経過後にSMART情報をログに残す
		if ( ( s_last_time != 0 ) && ( abs( GetTickCount() - s_last_time ) < g_toolboxini.get_SmartInterval() * 1000 ) )
			continue;

		// ログファイル名を作成
		kjm::path_string fname = g_toolboxini.get_SmartLogFolder().c_str();
		if ( fname.empty() ) {
			fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
		}
		fname.Append( "smart.log" );

		// SMART情報をログファイルに追記する
		AppendSmartLog( fname.c_str() );

		// 警告文字列を作成する
		MakeWarningText( fname.c_str() );

		// ログファイルのサイズが大きくなっているときは、一世代分のバックアップを取る
		BackupLog( fname.c_str() );

		// 処理が終わってから、指定時間待つようにする。
		s_last_time = GetTickCount();
	}

	// 終了メッセージ
	logging( "<<< SMART情報監視終了 >>>" );

	return 0;
}
