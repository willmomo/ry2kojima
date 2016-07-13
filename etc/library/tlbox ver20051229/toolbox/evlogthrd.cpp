//---------------------------------------------------------------------
// イベントログ監視処理
//---------------------------------------------------------------------


#include "toolbox.h"


//---------------------------------------------------------------------
// ログを残す
//---------------------------------------------------------------------
static void logging( const char* message ) {

	// ログファイル名を作成
	kjm::path_string fname = g_toolboxini.get_EventlogLogFolder().c_str();
	if ( fname.empty() ) {
		fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
	}
	fname.Append( "evlog.log" );

	// メッセージをログに残す
	std::ofstream ofs;
	ofs.open( fname.c_str(), std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {
		ofs << "[" << kjm::time::GetCurrentTime().ctime().c_str() << "] " << message << std::endl;
		ofs.close();
	}
}


//---------------------------------------------------------------------
// イベントログを保存
//---------------------------------------------------------------------
static void SaveEventLog( const char* event_name, const char* pszFname ) {

	// 保存用のファイル名を作成
	kjm::path_string fname = g_toolboxini.get_EventlogLogFolder().c_str();
	if ( fname.c_str() ) {
		fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
	}
	fname.Append( pszFname );

	// 上書きできないため、ファイルが存在するときは、削除する。
	if ( fname.FileExists() ) {
		remove( fname.c_str() );
	}

	// システム イベントログをディスクに保存する。
	kjm::eventlog evlog;
	evlog.OpenEventLog( NULL, _T( event_name ) );
	evlog.BackupEventLog( fname.c_str() );
}


//---------------------------------------------------------------------
// 警告文字列を作成する
//---------------------------------------------------------------------
static void MakeWarningText( const char* filename) {

	// 警告をなしにする。
	g_evlog_warning = "";

	// System イベントログを取得
	kjm::eventlog evlog;
	evlog.OpenEventLog( NULL, _T( "System" ) );

	// イベントログから ID9 と51を探す。
	kjm::eventlog_record rec;
	while ( evlog.ReadEventLog( EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, rec ) ) {

		// 以下のエラーがあったとき、ディスクが危険と判断する。
		// ID9: デバイス \Device\Ide\IdePort0 はタイムアウト期間内に応答しませんでした。
		// ID51: ページング操作中にデバイス \Device\Harddisk0\DR0 上でエラーが検出されました。

		DWORD id1 = rec.EventID() & 0xffff;
		DWORD id2 = rec.EventID() & 0xffff;
		if ( id1 == 9 || id2 == 51 ) {
			g_evlog_warning = "ディスクの異常が検知されました。";
			break;
		}
	}

	// 警告があるときは、ログに残す
	if ( !g_evlog_warning.empty() ) {

		std::stringstream s;
		s << "*** " << g_evlog_warning << " ***";
		logging( s.str().c_str() );
	}
}


//---------------------------------------------------------------------
// イベントログ監視スレッド
//---------------------------------------------------------------------
unsigned __stdcall eventlog_thread( void* arg ) {

	static DWORD s_last_time = 0;

	// 開始メッセージ
	logging( "<<< イベントログ監視スタート >>>" );

	while ( !g_end_flag ) {

		// とりあえず休ませる
		Sleep( 500 );

		// 設定時間経過後にSMART情報をログに残す
		if ( ( s_last_time != 0 ) && ( abs( GetTickCount() - s_last_time ) < g_toolboxini.get_EventlogInterval() * 1000 ) )
			continue;

		// ログファイル名を作成
		kjm::path_string fname = g_toolboxini.get_EventlogLogFolder().c_str();
		if ( fname.empty() ) {
			fname = kjm::path_string::GetModuleFileName().RemoveFileSpec();
		}
		fname.Append( "evlog.log" );

		// システム/アプリケーション イベントログを保存
		SaveEventLog( "System", "system.evt" );
		SaveEventLog( "Application", "application.evt" );

		// 警告文字列を作成する
		MakeWarningText( fname.c_str() );

		// ログファイルのサイズが大きくなっているときは、一世代分のバックアップを取る
		BackupLog( fname.c_str() );

		s_last_time = GetTickCount();

	}

	// 終了メッセージ
	logging( "<<< イベントログ監視終了 >>>" );

	return 0;
}
