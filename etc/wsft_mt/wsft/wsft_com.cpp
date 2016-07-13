/**************************************************************************************/
/***																				***/
/***								wsft.cpp										***/
/***																				***/
/**************************************************************************************/
#include <afx.h>
#include <assert.h>
#include <winsock2.h>


#include "wsft.h"
#include "wsft_def.h"
#include "wsft_err.h"
#include "tracex.h"
//#include "../wssasup/wssasup.h"

#include <string>
std::string GetExeName();	// adhoc プロトタイプ

__declspec(thread) DWORD g_dwTOCount[2];		//[0]:全受信発行回数 [1]:タイムアウト発生回数

extern LONG g_Suspended;

//	2001/09/27日 追加 --->
/*---------------------------------------------------------------------------------/
								IPアドレスを取得する

	[関数名]
		getIPAddr

	[引数]
		LPCSTR		*lpName		:	ホスト名

	[戻り値]
		DWORD		:	ＩＰアドレス

	[備考]
		ネットワークバイトオーダでの値を戻り値とする
/---------------------------------------------------------------------------------*/
DWORD getIPAddr( LPCSTR lpName )
{
	if (lpName[0] == '\0')	return INADDR_ANY;

	// addrを0x00000000で初期化　**FUMIYA
	DWORD	addr = INADDR_ANY;
	// この関数で使うポインタ変数に引数のアドレスを格納
	LPCSTR trg_name = lpName;
	struct hostent	*hent;
	// バイナリのインタネットアドレスに変換　**FUMIYA
	addr = inet_addr( trg_name );
	// trg_nameが"192.16.255.255"のような数値文字列ではなかったとき**FUMIYA
	if( addr == INADDR_NONE ){
		// trg_nameが"www.idgbooks.com"のようなｷｬﾗｸﾀ文字列のとき**FUMIYA
		hent = gethostbyname( trg_name );
		if( hent != NULL ){
			// ネットワークアドレスを取得(先頭のポインタの中身に格納)**FUMIYA
			addr = *((DWORD *)(hent->h_addr_list[0]));
		}
		else{
			// addrを0x00000000で初期化しなおす　**FUMIYA
			addr = INADDR_ANY;
		}
	}
	//return	ntohl( addr );	//osako
	return	addr;
}
//	<--- ここまで

//サーバー検索
//	指定されたIPアドレス値からのサーバー検索
// **FUMIYA　>>
//	機能：該当サーバーの有無を調べる関数
//	該当サーバーあり：ret　=　0
//	該当サーバーなし：ret　=　WSFTERR_NOT_SERVER
// **FUMIYA　<<
int GetHost( unsigned long addr ){
	LPHOSTENT lpHostEnt;		//hostent構造体のtypedef　**FUMIYA
	struct in_addr iaHC;		//アドレス構造体　**FUMIYA
	char	logstr[256] ;

	//変更されたserverAddrの値を送信先アドレスとして設定する
	iaHC.s_addr = addr ;
	// 引数で受け取ったアドレスからhostent構造体取得**FUMIYA
	lpHostEnt = gethostbyaddr( (const char * )&iaHC, sizeof( struct in_addr ), AF_INET );

	if( lpHostEnt == NULL ){
		//サーバの情報が取得できなかった
		wsprintf( logstr, "GetHost() ： 指定されたサーバーが見つかりません。アドレス：%s", inet_ntoa( iaHC ) );
		TraceF(logstr );
		return WSFTERR_NOT_SERVER ;
	}

	return 0;
}

/*-----------------------------------------------------------------------------------------/
										ソケットを閉じる
/-----------------------------------------------------------------------------------------*/
void CloseSocket(St_SocketInfo *lpInfo)
{
	//	ソケットを閉じる
	// **FUMIYA >>
	// 相手側のソケットに通知を出してソケットを閉じる
	// SD_RECEIVE:ソケットに対する受信を無効にする*FUMIYA
	// **FUMIYA <<
	shutdown( lpInfo->sock, SD_RECEIVE );
	//ソケットもう一度閉じる　**FUMIYA
	closesocket( lpInfo->sock );
}

//---------------------------------------------------------------------
// ソケットの作成

int SetServer(St_SocketInfo	*lpInfo, u_short port)
{
	//UDPソケット記述子を作成する。
	lpInfo->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//ソケット作成失敗！ add comment by FUMIYA 02.01.17
	if ( lpInfo->sock == INVALID_SOCKET ){
		TraceE("ConnectHost() : UDPソケットの作成に失敗しました。" ) ;
		return WSFTERR_INIT_SOCKET ;
	}
	//アドレス構造体分長さを格納 add comment by FUMIYA 02.01.17
	lpInfo->len = sizeof(SOCKADDR);

//	TraceD("SetServer() : サーバー [%ld] ポート[%ld] に接続します。",
//		lpInfo->saAddr, port) ;
	TraceD("SetServer() : サーバー [%d.%d.%d.%d] ポート[%ld] に接続します。",
		(lpInfo->saAddr & 0xff), ((lpInfo->saAddr >> 8) & 0xff),
		((lpInfo->saAddr >> 16) & 0xff), ((lpInfo->saAddr >> 24) & 0xff),
		port) ;

	//	サーバーの設定
	lpInfo->saServer.sin_family = AF_INET;
	lpInfo->saServer.sin_addr.S_un.S_addr = lpInfo->saAddr ;
	lpInfo->saServer.sin_port = htons(port);

	// タイムアウトの設定
	int to = (lpInfo->timeout > 500) ? lpInfo->timeout - 500 : 1;
	int ret = setsockopt(lpInfo->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
	if (ret == SOCKET_ERROR) {
		TraceF("%s(%d): SetServer(): タイムアウトの設定でエラーが発生しました。(ERR: %u)",
			__FILE__, __LINE__, WSAGetLastError());
		return WSFTERR_OPTION_TIMEOUT;
	}

	return ret;
}

/*-----------------------------------------------------------------------------------------/
						受信したエラー電文の内容を出力する
/-----------------------------------------------------------------------------------------*/
int OutputErrorLog(ST_DATA req, ST_COMMAND *anscmdheader)
{

	int		ret = htons(anscmdheader->info[0]) ;

	switch( htons(anscmdheader->info[0]) ){
	case WSFTERR_BUSY_I :	//	他機器からの書き込み中
		/*TraceF("CheckCommonHeader():他機器から書込み排他中。ファイル番号=%ld, 書込み機器IPアドレス=%d.%d.*.*",
			htons(anscmdheader->info[1]),
			(htons(anscmdheader->info[2]) & 0xff00) >> 8,
			htons(anscmdheader->info[2]) & 0x00ff) ;*/
		TraceF("CheckCommonHeader():他機器から書込み排他中。info[0]=0x%04x, info[1]=0x%04x, info[2]=0x%04x, info[3]=0x%04x",
			htons(anscmdheader->info[0]), htons(anscmdheader->info[1]), htons(anscmdheader->info[2]),
			htons(anscmdheader->info[3]));
		//	送信したコマンドにより戻り値を変更する
		switch( req.cmdheader.command ){
		case 'I' :	//	ファイル情報取得要求後
			ret = WSFTERR_BUSY_I ;
			break ;
		case 'R' :	//	読み込み要求後
			ret = WSFTERR_BUSY_R ;
			break ;
		case 'w' :	//	核込み準備要求後
			ret = WSFTERR_BUSY_S ;
			break ;
		case 'D' :	//	書き込み要求後
			ret = WSFTERR_BUSY_W ;
			break ;
		case 'W' :	//	書き込み完了要求後
			ret = WSFTERR_BUSY_E ;
			break ;
		}
		break ;
	case WSFTERR_BUFF :	//	バッファ確保失敗
		TraceF("CheckCommonHeader():書き込み準備要求後のバッファ確保エラー。" ) ;
		break ;
	case WSFTERR_FRAME_SKIP :	//	フレーム抜け
		TraceF("CheckCommonHeader():フレーム抜けエラー。 %ld, %ld, %ld", htons(anscmdheader->info[1]), htons(anscmdheader->info[2]), htons(anscmdheader->info[3]) ) ;
		break ;
	case WSFTERR_REC_TIMEOUT :	//	書き込み時タイムアウト
		TraceF("CheckCommonHeader():書き込み受信タイムアウト。 最終受信フレーム=%ld, 受信フレーム合計=%ld, 必要フレーム合計=%ld", htons(anscmdheader->info[1]), htons(anscmdheader->info[2]), htons(anscmdheader->info[3]) ) ;
		break ;
	case WSFTERR_NO_SUPPORT_SMF :	//	ＳＭＦに無いファイル番号
		TraceF("CheckCommonHeader():SMFに無いファイル番号です。" ) ;
		break ;
	case WSFTERR_ADDRESS :			//	アドレス
		TraceF("CheckCommonHeader():アクセス禁止領域のアドレスを指定されました。" ) ;
		break ;
	case WSFTERR_RECORD :			//	レコード
		TraceF("CheckCommonHeader():範囲外のレコードを指定されました。" ) ;
		break ;
	case WSFTERR_FRAME :			//	フレーム
		TraceF("CheckCommonHeader():ありえないフレーム番号でアクセスされました。" ) ;
		break ;
	case WSFTERR_COMMAND :			//	コマンド
		TraceF("CheckCommonHeader():規定コマンド以外が指定されました。 コマンド=%ld", htons(anscmdheader->info[1]) ) ;
		break ;
	case WSFTERR_VERSION :			//	バージョン
		TraceF("CheckCommonHeader():プロトコルバージョンが間違っています。" ) ;
		break ;
	default :						//	その他
		TraceF("CheckCommonHeader():サポートされていないエラーコードです。 エラーコード=0x%04X", htons(anscmdheader->info[0]) ) ;
		break ;
	}

	return ret ;
}

/*-----------------------------------------------------------------------------------------/
									電文共通部のチェック

	[引数]
		ST_DATA			req				:	要求電文
		ST_APP_HEADER	*ansappheader	:	応答電文のヘッダー部
		ST_COMMAND		*anscmdheader	:	応答電文のヘッダー部

	[戻り値]
		== 0							:	エラーなし
		!= 0							:	エラーコード

/-----------------------------------------------------------------------------------------*/
inline int CheckCommonHeader(	ST_DATA	req	,ST_APP_HEADER *ansappheader	,ST_COMMAND *anscmdheader	)
{
	char	anscmd ;
	short	i = 0 ;

	//	エラーが帰ってきたとき
	if( anscmdheader->command == 'E' ){
		return OutputErrorLog( req, anscmdheader ) ;
	}

	//	要求した電文に該当する応答コマンドの設定
	switch( req.cmdheader.command ){
	case 'R' :
		anscmd = 'D' ;
		break ;
	case 'f' :
		anscmd = 'D' ;
		break ;
	case 'D' :
		anscmd = 'A' ;
		break ;
	default :
		//	残りは要求電文と同じコマンドで返ってくる
		anscmd = req.cmdheader.command ;
		break ;
	}

	//	要求したものと違う電文を受信したときは無視する
	if( req.appheader.seqnum != ansappheader->seqnum ){
		TraceE("CheckCommonHeader() : 異なるシーケンス番号のデータを受信しました。要求=%u, 応答=%u",
			req.appheader.seqnum & 0xff, ansappheader->seqnum & 0xff ) ;
		return -2 ;
	}
	//	応答電文のコマンドが期待しているものかどうかのチェック
	if( anscmd != anscmdheader->command ){
		TraceE("CheckCommonHeader() : 異なるコマンドのデータを受信しました。要求=%c, 応答=%c", req.cmdheader.command, anscmdheader->command ) ;
		return -1 ;
	}
	//	送信したファイル番号またはアドレスがそのまま返ってきているか
	if( req.cmdheader.addr != anscmdheader->addr ){
		TraceE("CheckCommonHeader() : 異なるファイル番号のデータを受信しました。要求=%d, 応答=%d", htonl(req.cmdheader.addr), htonl(anscmdheader->addr) ) ;
		return -1 ;
	}
	//	要求したレコード番号の値が返ってきているか
	if( req.cmdheader.rec != anscmdheader->rec ){
		TraceE("CheckCommonHeader() : 異なるレコードのデータを受信しました。要求=%d, 応答=%d", htonl(req.cmdheader.rec), htonl(anscmdheader->rec) ) ;
		return -1 ;
	}

	//	送信した電文によりさらにチェックする項目があるとき
	switch( req.cmdheader.command ){
	case 'D' :
	case 'w' :
	case 'W' :
		for( i=0 ; i<4 ; i++ ){
			if( req.cmdheader.info[i] != anscmdheader->info[i] ){
				TraceE("CheckCommonHeader() : Info[%ld] の値が一致しません。要求=%ld, 応答=%ld", i, htons(req.cmdheader.info[i]), htons(anscmdheader->info[i]) ) ;
				return -1 ;
			}
		}
		break ;
	}

	return 0 ;
}

/////////////////////// KOJIMA //////////////////////////////

int SendAndRecv(
	St_SocketInfo *lpInfo
	,char *req
	,unsigned short reqsize
	,char *recvbuff
	,unsigned short anssize
	)
{
	ST_DATA		req1 ;			//	要求電文用構造体
	int			ret = 0 ;		//	戻り値用
	int			cnt = 0 ;		//	リトライ回数カウント用
	int			size = 0 ;		//	送受信用戻り値
	long		stime ;			//	受信開始時刻
	long		etime ;			//	受信終了時刻
	ST_APP_HEADER	*app ;		//	ヘッダー構造体
	ST_COMMAND		*cmd ;		//	ヘッダー構造体

	// g_Suspended == FALSE の時、電文を送信しないで終了する
	if (g_Suspended != TRUE) {
		Winssa_Log(LOGC_WSFT, LOGLV_INFO, "(%s)SendAndRecv(): g_Suspended = FALSE のため、送信しません。",
			GetExeName().c_str());
		return -1;
	}

	sockaddr_in sock_addr;	//**02.02.27 misako**
	int iAddress = sizeof(sock_addr);	//**02.02.27 misako**
	//	2001/09/26日 追加 --->
//	if( GetHost( lpInfo->saAddr ) != 0 ){
		//	サーバーが見つからない
//		TraceF("設定されているサーバー[%ld]が見つかりません。", lpInfo->saAddr ) ;
//		return WSFTERR_NOT_SERVER ;
//	}
	//	<--- ここまで

	//	設定回数リトライする
	//	2001/09/20日 修正 --->
	//	リトライ回数が１回多かった
	//for( cnt=0 ; cnt<=lpInfo->retrycount; cnt++ ){
	for( cnt=0 ; cnt<lpInfo->retrycount; cnt++ ){
	//	<--- ここまで
		//	要求電文用バッファのクリア
//		memset( &req1, 0, sizeof(req1) ) ;
		//	応答電文用バッファのクリア
//		memset( recvbuff, 0, anssize ) ;
		//	シーケンス番号の加算
		if( lpInfo->seqnum >= 255 ){
			lpInfo->seqnum = 1 ;
		}else{
			lpInfo->seqnum++ ;
		}
		//	ヘッダを取得
		memcpy( &req1, req, sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ) ;
		//	シーケンス番号を取得
		req1.appheader.seqnum = lpInfo->seqnum ;
		memcpy( req, &req1, sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ) ;
		//	再度送信するときは戻り値を正常終了の値にしておく
		ret = 0 ;
		//	引数で渡されたの要求電文を送信
		//size = sendto( lpInfo->sock, (char*)&req1, reqsize, 0, (LPSOCKADDR)&lpInfo->saServer, lpInfo->len ) ;
		size = sendto( lpInfo->sock, req, reqsize, 0, (LPSOCKADDR)&lpInfo->saServer, lpInfo->len ) ;

		// ログを残すのみで、今回は送信エラー時の処理は特に追加しない
		if( size == SOCKET_ERROR ){
			ret = WSAGetLastError();
			if( ret == WSAETIMEDOUT ){
				TraceE( "SendAndRecv()：試行%d：タイムアウト時間内に送信できませんでした。(WSAE:%d)", cnt+1, ret );
			}
			else{
				TraceE( "SendAndRecv()：試行%d：電文の送信に失敗しました。(WSAE:%d)", cnt+1, ret );
			}
		}
		else if( size != reqsize ){
			TraceE( "SendAndRecv()：試行%d：電文を一度に送信し切れませんでした。", cnt+1 );
		}

		//	受信
		stime = GetTickCount() ;
		do{
recv_retry:
			ret = 0 ;

			//	受信
			//size = recvfrom( lpInfo->sock, recvbuff, anssize, 0, (LPSOCKADDR)&lpInfo->saServer, &lpInfo->len );
			g_dwTOCount[0]++;
			size = recvfrom( lpInfo->sock, recvbuff, anssize, 0, (LPSOCKADDR)&sock_addr, &iAddress ); //**02.02.27 misako**

			//	エラー発生時は即受信終了
			if( size == SOCKET_ERROR ){
				break ;
			}

			//	受信データがあるとき
			if ( size != 0 ){
				app = (ST_APP_HEADER*)recvbuff ;
				cmd = (ST_COMMAND*)&recvbuff[sizeof(ST_APP_HEADER)] ;

				//	電文共通部のチェック
				ret = CheckCommonHeader( req1, app, cmd ) ;
				if (ret == -2)			// シーケンスNo不一致のときは
					goto recv_retry;	// 受信のみリトライ

				break;
			}

			//	既に受信開始からタイムアウト秒経過しているときは無条件で出る
			//  ここではソケットのタイムアウトとプログラムでのタイムアウトを
			//　取っている*****02.01.21 FUMIYA*****
			etime = GetTickCount() ;
			if( etime - stime > (lpInfo->timeout) ){
				break ;
			}

			//	共通電文の不一致のときは受信を続ける
			if( ret != -1 ){
				//	エラーコマンドを受信したときは受信は終了(共通電文の不一致のときは受信を続ける)
				if( ret != 0 || size != 0 ){
					break ;
				}
			}
		}while(1) ;

		// 正常な電文が受信できなかったとき
		if( ret != 0 ){
			//	ファイル取得のとき以外はエラーコマンドが返ってきてもリトライする
			//	フレーム抜けのときは中断
			//	フレーム抜けエラーのときは中断
			if( ret != WSFTERR_FRAME_SKIP ){
				Sleep(150);	// リトライするので150msまってから実行
				continue ;
			//	ファイル取得でエラー電文が返ってきたときは中断する
			}else{
				break ;
			}
		}

		//	受信成功
		if( size != 0 && size != SOCKET_ERROR ){
			break ;
		}else{
			if( WSAGetLastError() == WSAETIMEDOUT ){
				g_dwTOCount[1]++;
				switch( req1.cmdheader.command ){
				case 'I' :
					ret = WSFTERR_TIMEOUT_I ;
					break ;
				case 'R' :
					ret = WSFTERR_TIMEOUT_R ;
					break ;
				case 'w' :
					ret = WSFTERR_TIMEOUT_S ;
					break ;
				case 'D' :
					ret = WSFTERR_TIMEOUT_W ;
					break ;
				case 'W' :
					ret = WSFTERR_TIMEOUT_E ;
					break ;
				default :
					//	エラーコード設定
					//	サポートしていないコマンドのとき
					ret = WSFTERR_TIMEOUT_I ;
					break ;
				}

				continue ;
			}else{
				//	受信に失敗しました。
				TraceF("SendAndRecv() : 受信に失敗しました。 戻り値 %d (%d) ", size, WSAGetLastError() ) ;
				//	受信エラーのときのエラーコードを設定
				switch( req1.cmdheader.command ){
				case 'I' :
					ret = WSFTERR_REC_I ;
					break ;
				case 'R' :
					ret = WSFTERR_REC_R ;
					break ;
				case 'w' :
					ret = WSFTERR_REC_S ;
					break ;
				case 'D' :
					ret = WSFTERR_REC_W ;
					break ;
				case 'W' :
					ret = WSFTERR_REC_E ;
					break ;
				default :
					//	サポートしていないコマンドのとき
					ret = WSFTERR_REC_I ;
					break ;
				}
				//	2001/09/26日 追加 --->
				//	タイムアウト秒待つ
				Sleep( lpInfo->timeout ) ;
				//	<--- ここまで
				continue ;
			}
		}
	}

	return ret ;

}


/*-----------------------------------------------------------------------------------------/
									ファイル情報取得

	[関数名]
		com_infofile

	[引数]
		St_SocketInfo	*lpInfo		:	通信の設定
		unsigned long	fno			:	情報を取得するファイルのsmfファイル番号
		unsigned short	rec			:	情報を取得するレコード番号(0のときはファイル)
		unsigned long	*fsize		:	ファイルサイズ
		unsigned long	*fr			:	フレーム数
		unsigned long	*flag		:	フラグ	0x0001:書き込み中 0x0002:リザーブ中
		char			FileorAddr	:	'F' or 'A'

	[戻り値]
		== 0						:	正常
		!= 0						:	エラー(エラーコード)

/-----------------------------------------------------------------------------------------*/
int com_infofile(St_SocketInfo *lpInfo, u_long fno, u_long rec,
				 u_long *fsize, u_short *fr, u_short *flag, char FileorAddr)
{
	ST_DATA		req ;			//	要求電文用構造体
	ST_DATA		ans ;			//	応答電文受信用構造体
	int			ret = 0 ;		//	戻り値
	unsigned short	reqsize ;	//	要求電文サイズ

	memset(&req, 0, sizeof(req));

	//	アプリヘッダを設定
	//	プロトコルバージョン
	req.appheader.ver = htons(lpInfo->ver);
	//	要求電文サイズ
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	req.appheader.size = htons(reqsize) ;

	//	コマンドを設定
	req.cmdheader.command = 'I' ;
	//	ファイル番号 or アドレス の指定 ( 'F' or 'A' )
	req.cmdheader.fileoraddr = FileorAddr ;
	//	ファイル番号 or アドレス
	req.cmdheader.addr = htonl(fno) ;
	//	レコード番号 or サイズ
	req.cmdheader.rec = htonl(rec) ;

	//	送受信
	ret = SendAndRecv( lpInfo, (char*)&req, reqsize, (char*)&ans, reqsize ) ;

	if (ret == 0) {
		if (fsize) {	// ファイルサイズ
			*fsize = ntohl(*(u_long*)ans.cmdheader.info);
		}
		if (flag) {		// フラグ
			*flag = ntohs(ans.cmdheader.info[2]);
		}
		if (fr) {		// フレーム数
			*fr = ntohs(ans.cmdheader.info[3]);
		}
	}

	TraceD("com_infofile() : com_infofile関数を戻り値 (%d) で終了します。", ret ) ;

	return ret ;

}


/*-----------------------------------------------------------------------------------------/
									ファイル読み込み

	[関数名]
		com_readfile

	[引数]
		St_SocketInfo	*lpInfo		:	通信設定
		unsigned short	addr		:	取得するファイルのsmfファイル番号またはアドレス
		unsigned short	rec			;	取得するレコード(０のときはファイル)
		char			*buf		:	取得したファイルのデータを格納するバッファ
		unsigned long	bufsize		:	バッファサイズ
		char			FileorAddr	:	'F' : ファイル番号で取得。 'A' : アドレスで取得

	[戻り値]
		== 0						:	正常
		!= 1						:	エラー(エラーコード)

/-----------------------------------------------------------------------------------------*/
int com_readfile(
	St_SocketInfo *lpInfo
	,unsigned long	addr
	,unsigned long rec
	,char *buf
	,unsigned long bufsize
	,char FileorAddr
	)
{
	ST_DATA		req ;			//	要求電文バッファ
	int			ret = 0 ;		//	戻り値
	unsigned long fsize ;		//	ファイルサイズ
	unsigned short fr ;			//	フレーム数
	unsigned short flag ;		//	フラグ
	unsigned short reqsize ;	//	要求電文バッファサイズ
	unsigned short anssize ;	//	応答電文バッファサイズ
	unsigned short frno ;		//	フレーム番号
	unsigned long total ;		//	トータルデータサイズ
	char *tmpbuff ;				//	テンポラリバッファ
	char *back ;
	unsigned short rsize = 0 ;	//	受信データサイズ
	char *rbuf ;				//	受信バッファ
	unsigned short maxdatasize = 0 ;	//	１回で受信できる最大バッファサイズ(データ部のみ)

	//	ファイル情報を取得する
	ret = com_infofile( lpInfo, addr, rec, &fsize, &fr, &flag, FileorAddr ) ;
	if( ret != 0 ){
		TraceD("com_readfile() : com_readfile関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	TraceD("com_readfile() : fno=%ld, fsize=%ld, framecount=%ld, flag=%ld", addr, fsize, fr, flag ) ;

	//	ファイル情報のフラグを見て処理を行う場合は以下のコードを有効にする
	switch( flag ){
	case WSFT_INFO_FILE_FLAG_WRITE :
		TraceW("com_readfile() : 指定されたファイルは現在書き込み中です。 ファイル番号=%ld", addr ) ;
		break ;
	case WSFT_INFO_FILE_FLAG_RESERVE :
		TraceW("com_readfile() : 指定されたファイルは現在リザーブ中です。 ファイル番号=%ld", addr ) ;
		break ;
	}

	//	バッファサイズとファイルサイズの比較
	//if( fsize != bufsize ){
	//	引数でわたされたバッファのサイズが通信するファイルのサイズより大きいときにエラーとして中断する
	//↑仕様ｴﾗｰのため、引数で渡されたﾊﾞｯﾌｧとﾌｧｲﾙのｻｲｽﾞが一致しないとｴﾗｰにする。**02.03.08 misako**
	if( bufsize != fsize ){
		TraceF("com_readfile() : 通信ファイルサイズと一致しないバッファが指定されています。 受信バッファサイズ=%ld、通信ファイルサイズ=%ld", bufsize, fsize ) ;
		ret = WSFTERR_REC_SIZE ;
		TraceD("com_readfile() : com_readfile関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	//	取得するファイルサイズ分だけ確保する(バッファサイズではない)
	tmpbuff = (char*)malloc(fsize) ;
	if( tmpbuff == NULL ){
		TraceF("com_readfile() : メモリの確保に失敗しました。" ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceD("com_readfile() : com_readfile関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	//	１回の通信で送受信できる最大のバッファ分確保する
	rbuf = (char*)malloc(lpInfo->maxsize) ;
	if( rbuf == NULL ){
		TraceF("com_readfile() : メモリの確保に失敗しました。" ) ;
		free( tmpbuff ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceI("com_readfile() : com_readfile関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	//	あとで戻すために先頭アドレスを覚えておく
	back = tmpbuff ;

	//	要求電文のバッファサイズを計算する
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	//	トータルの電文サイズ
	total = fsize ;
	// １回で受け取るデータ部分の最大バッファサイズ
	maxdatasize = lpInfo->maxsize - reqsize ;

	//	１フレームずつ(フレームＮｏは１から)
	for( frno=1 ; frno<=fr ; frno++ ){
		if( total > maxdatasize ){
			rsize = maxdatasize ;
		}else{
			rsize = (unsigned short)total ;
		}
		total = total - rsize ;

		//	プロトコルバージョン
		req.appheader.ver = htons(lpInfo->ver);
		//	要求電文サイズ
		req.appheader.size = htons(reqsize) ;
		//	要求コマンド
		req.cmdheader.command = 'R' ;
		//	指定方法 ( 'F' or 'A' )
		req.cmdheader.fileoraddr = FileorAddr ;
		//	ファイル番号 or アドレス
		req.cmdheader.addr = htonl(addr) ;
		//	指定レコード or サイズ
		req.cmdheader.rec = htonl(rec) ;
		//	フレームＮｏ
		req.cmdheader.info[0] = htons(frno) ;
		//	受信データサイズ
		anssize = reqsize + rsize ;

		//	取得データ
		TraceD("com_readfile() : ファイル番号(アドレス)=%ld, レコード(サイズ)=%ld, フレームNo=%ld", addr, rec, frno ) ;

		//	ファイルの読み込み
		ret = SendAndRecv( lpInfo, (char*)&req, reqsize, rbuf, anssize ) ;

	 	//	正常に通信が行われたときはバッファに格納
		if( ret == 0 ){
			//	受信サイズ分コピー
			memcpy( tmpbuff, &rbuf[reqsize], rsize ) ;
			//	次の書き込み位置に移動
			tmpbuff += rsize ;
		}else{
			break ;
		}
	}

	//	先頭に戻る
	tmpbuff = back ;

	//	テンポラリバッファからコピー
	if( ret == 0 ){
		memcpy( buf, tmpbuff, bufsize ) ;
	}

	//	開放
	free( tmpbuff ) ;
	free( rbuf ) ;

	TraceD("com_readfile() : com_readfile関数を戻り値 (%d) で終了します。", ret ) ;

	return ret ;

}

/////////////////////////////////////////////////////////////////////////////
// ファイル読み込み2
//
// [備考]
// 指定レコードから指定レコード数読み込む。ファイル全体を読み込むときは、
// com_readfileを使用する。

int com_readfile2(
	St_SocketInfo *lpInfo
	,unsigned long	addr
	,unsigned long rec
	,size_t rec_size
	,size_t rec_count
	,char *buf
	,unsigned long bufsize
	,char FileorAddr
	)
{
	ST_DATA		req ;			//	要求電文バッファ
	int			ret = 0 ;		//	戻り値
	unsigned long fsize ;		//	ファイルサイズ
	unsigned short fr ;			//	フレーム数
	unsigned short flag ;		//	フラグ
	unsigned short reqsize ;	//	要求電文バッファサイズ
	unsigned short anssize ;	//	応答電文バッファサイズ
	unsigned short frno ;		//	フレーム番号
	unsigned long total ;		//	トータルデータサイズ
	char *tmpbuff ;				//	テンポラリバッファ
	char *back ;
	unsigned short rsize = 0 ;	//	受信データサイズ
	char *rbuf ;				//	受信バッファ
	unsigned short maxdatasize = 0 ;	//	１回で受信できる最大バッファサイズ(データ部のみ)

	//	ファイル情報を取得する
	ret = com_infofile( lpInfo, addr, 0, &fsize, &fr, &flag, FileorAddr ) ;
	if( ret != 0 ){
		TraceD("com_readfile2() : com_readfile2関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	TraceD("com_readfile2() : fno=%ld, fsize=%ld, framecount=%ld, flag=%ld", addr, fsize, fr, flag ) ;

	//	ファイル情報のフラグを見て処理を行う場合は以下のコードを有効にする
	switch( flag ){
	case WSFT_INFO_FILE_FLAG_WRITE :
		TraceW("com_readfile2() : 指定されたファイルは現在書き込み中です。 ファイル番号=%ld", addr ) ;
		break ;
	case WSFT_INFO_FILE_FLAG_RESERVE :
		TraceW("com_readfile2() : 指定されたファイルは現在リザーブ中です。 ファイル番号=%ld", addr ) ;
		break ;
	}

	//	バッファサイズとファイルサイズの比較
	//if( fsize != bufsize ){
	//	引数でわたされたバッファのサイズが通信するファイルのサイズより大きいときにエラーとして中断する
	//↑仕様ｴﾗｰのため、引数で渡されたﾊﾞｯﾌｧとﾌｧｲﾙのｻｲｽﾞが一致しないとｴﾗｰにする。**02.03.08 misako**
//	if( bufsize != fsize ){
//		TraceF("com_readfile2() : 通信ファイルサイズと一致しないバッファが指定されています。 受信バッファサイズ=%ld、通信ファイルサイズ=%ld", bufsize, fsize ) ;
//		ret = WSFTERR_REC_SIZE ;
//		TraceD("com_readfile2() : com_readfile2関数を戻り値 (%d) で終了します。", ret ) ;
//		return ret ;
//	}

	//	取得するファイルサイズ分だけ確保する(バッファサイズではない)
	tmpbuff = (char*)malloc(fsize) ;
	if( tmpbuff == NULL ){
		TraceF("com_readfile2() : メモリの確保に失敗しました。" ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceD("com_readfile2() : com_readfile2関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	//	１回の通信で送受信できる最大のバッファ分確保する
	rbuf = (char*)malloc(lpInfo->maxsize) ;
	if( rbuf == NULL ){
		TraceF("com_readfile2() : メモリの確保に失敗しました。" ) ;
		free( tmpbuff ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceI("com_readfile2() : com_readfile2関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	//	あとで戻すために先頭アドレスを覚えておく
	back = tmpbuff ;

	//	要求電文のバッファサイズを計算する
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	//	トータルの電文サイズ
	total = fsize ;
	// １回で受け取るデータ部分の最大バッファサイズ
	maxdatasize = lpInfo->maxsize - reqsize ;

	int start_frame = ((rec_size * (rec - 1)) / maxdatasize) + 1;				// 開始フレーム
	int end_frame = ((rec_size * (rec + rec_count - 1) - 1) / maxdatasize) + 1;	// 終了フレーム
	total -= maxdatasize * (start_frame - 1);	// 開始フレームまで読み込んだ状態でスタート

	//	１フレームずつ(フレームＮｏは１から)
	for( frno=start_frame ; frno<=end_frame ; frno++ ){
		if( total > maxdatasize ){
			rsize = maxdatasize ;
		}else{
			rsize = (unsigned short)total ;
		}
		total = total - rsize ;

		//	プロトコルバージョン
		req.appheader.ver = htons(lpInfo->ver);
		//	要求電文サイズ
		req.appheader.size = htons(reqsize) ;
		//	要求コマンド
		req.cmdheader.command = 'R' ;
		//	指定方法 ( 'F' or 'A' )
		req.cmdheader.fileoraddr = FileorAddr ;
		//	ファイル番号 or アドレス
		req.cmdheader.addr = htonl(addr) ;
		//	指定レコード or サイズ
		req.cmdheader.rec = htonl(0) ;
		//	フレームＮｏ
		req.cmdheader.info[0] = htons(frno) ;
		//	受信データサイズ
		anssize = reqsize + rsize ;

		//	取得データ
		TraceD("com_readfile2() : ファイル番号(アドレス)=%ld, レコード(サイズ)=%ld, フレームNo=%ld", addr, rec, frno ) ;

		//	ファイルの読み込み
		ret = SendAndRecv( lpInfo, (char*)&req, reqsize, rbuf, anssize ) ;

	 	//	正常に通信が行われたときはバッファに格納
		if( ret == 0 ){
			if (frno == start_frame) {	// 要求先頭フレームのとき
				int ofs = ((rec - 1) * rec_size) - ((frno - 1) * maxdatasize);
				memmove(tmpbuff, &rbuf[reqsize + ofs], rsize - ofs);
				tmpbuff += rsize - ofs;
			} else if (frno == end_frame) {	// 要求最終フレームのとき
				memmove(tmpbuff, &rbuf[reqsize],
					((rec + rec_count - 1) * rec_size) - ((frno - 1) * maxdatasize));
				tmpbuff += ((rec + rec_count - 1) * rec_size) - ((frno - 1) * maxdatasize);
			} else {
				//	受信サイズ分コピー
				memcpy( tmpbuff, &rbuf[reqsize], rsize ) ;
				//	次の書き込み位置に移動
				tmpbuff += rsize ;
			}
		}else{
			break ;
		}
	}

	//	先頭に戻る
	tmpbuff = back ;

	//	テンポラリバッファからコピー
	if( ret == 0 ){
		memcpy( buf, tmpbuff, bufsize ) ;
	}

	//	開放
	free( tmpbuff ) ;
	free( rbuf ) ;

	TraceD("com_readfile2() : com_readfile2関数を戻り値 (%d) で終了します。", ret ) ;

	return ret ;

}

/*-----------------------------------------------------------------------------------------/
									ファイル書き込み

	[関数名]
		com_writefile

	[引数]
		St_SocketInfo	*lpInfo		:	通信設定
		unsigned long	fno			:	取得するファイルのsmfファイル番号
		unsigned short	rec			:	書き込みを行うレコード番号(0はファイル)
		char			*buf		:	取得したファイルのデータを格納するバッファ
		unsigned long			bufsize		:	バッファサイズ
		char			FileorAddr	:	'F' : ファイル番号で書き込み 'A' : アドレスで書き込み

	[戻り値]
		== 0						:	正常
		!= 1						:	エラー(エラーコード)

/-----------------------------------------------------------------------------------------*/
int com_writefile(
	St_SocketInfo *lpInfo
	,unsigned long	fno
	,unsigned long rec
	,char *buf
	,unsigned long bufsize
	,char FileorAddr
	)
{
	ST_DATA			req ;				//	要求電文用構造体
	ST_DATA			ans ;				//	応答電文受信用構造体
	int				ret = 0 ;			//	戻り値
	unsigned long	fsize ;				//	ファイルサイズ
	unsigned short	fr ;				//	フレーム数
	unsigned short	flag ;				//	フラグ
	unsigned short	reqsize ;			//	要求電文サイズ
	unsigned short	anssize ;			//	応答電文サイズ
	unsigned short	frno ;				//	フレームNo
	unsigned long	total ;				//	トータルデータサイズ
	unsigned long	p ;					//	書き込みデータ位置
	char			*reqbuf ;			//	要求電文用バッファ
	unsigned long	maxdatasize = 0 ;	//	１回で送受信を行える最大バッファサイズ(データ部のみ)
	short			retrycount = 0 ;	//	リトライ回数(フレーム抜けの再書き込み時)
	int				i = 0 ;				//	ループ用カウンタ
	int				j = 0 ;				//	ループ用
	//	2001/09/27日 修正 --->
	//	unsigned にしてしまっていたため
	//unsigned short			thisskipfr[3] ;		//	フレーム抜け電文で返ってきたフレームNo
	short			thisskipfr[3] ;		//	フレーム抜け電文で返ってきたフレームNo
	//unsigned short			skipfr[3] ;			//	前回のフレーム抜けで帰ってきたフレームNo
	short			skipfr[3] ;			//	前回のフレーム抜けで帰ってきたフレームNo
	//	<--- ここまで
	short			f_retrycount[3] ;	//	フレーム抜けの再書き込み回数(フレームNoごと)
	short			thisretrycount[3] ;	//

	//	ファイル情報を取得する
	ret = com_infofile( lpInfo, fno, rec, &fsize, &fr, &flag, FileorAddr ) ;
	if( ret != 0 ){
		return ret ;
	}

	//	バッファサイズが一致しないといけません
	if( bufsize != fsize ){
		TraceF("com_writefile() : 書き込むバッファのサイズとファイルのデータサイズが一致しません。 バッファサイズ=%ld、ファイルサイズ=%ld", bufsize, fsize ) ;
		ret = WSFTERR_BUFFSIZE ;
		TraceD("com_writefile() : com_writefile関数を戻り値 (%d) で終了します。", ret ) ;
		return ret ;
	}

	//	要求電文と応答電文のサイズ
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;

	//	要求電文用バッファの確保
	reqbuf = (char*)malloc(lpInfo->maxsize) ;
	if( reqbuf == NULL ){
		ret = WSFTERR_INIT_MALLOC ;
		return ret ;
	}

	//	書き込み準備送受信
	//	ヘッダーの設定
	//	プロトコルバージョン
	req.appheader.ver = htons(lpInfo->ver);
	//	要求電文サイズ
	req.appheader.size = htons(reqsize) ;
	//	コマンド
	req.cmdheader.command = 'w' ;
	//	ファイル番号 or アドレス の指定 ( 'F' or 'A' )
	req.cmdheader.fileoraddr = FileorAddr ;
	//	ファイル番号 or アドレス
	req.cmdheader.addr = htonl(fno) ;
	//	レコード番号 or サイズ
	req.cmdheader.rec = htonl(rec) ;
	//	フレーム数
	req.cmdheader.info[0] = htons(fr) ;
	//	送受信
	ret = SendAndRecv( lpInfo, (char*)&req, reqsize, (char*)&ans, anssize ) ;
	//	全送信サイズ
	total = bufsize ;
	//	１回で送信するサイズのデータ部分のサイズ
	maxdatasize = lpInfo->maxsize - reqsize ;

	p = 0 ;
	if( ret == 0 ){
		//	フレームＮｏは１から
		for( frno=1 ; frno<=fr ; frno++ ){
			//	今回の送信サイズの計算
			if( total > maxdatasize ){
				fsize = maxdatasize ;
			}else{
				fsize = total ;
			}
			total = total - fsize ;
			//	書き込み
			reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) + fsize ;
			anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
			//	ヘッダーの設定
			//	プロトコルバージョン
			req.appheader.ver = htons(lpInfo->ver);
			//	要求電文サイズ
			req.appheader.size = htons(reqsize) ;
			//	コマンド
			req.cmdheader.command = 'D' ;
			//	ファイル番号 or アドレス の指定 ( 'F' or 'A' )
			req.cmdheader.fileoraddr = FileorAddr ;
			//	ファイル番号 or アドレス
			req.cmdheader.addr = htonl(fno) ;
			//	レコード番号 or サイズ
			req.cmdheader.rec = htonl(rec) ;
			//	フレームNo
			req.cmdheader.info[0] = htons(frno) ;
			//	初期化
			memset( reqbuf, 0, lpInfo->maxsize ) ;
			//	ヘッダ部をコピー
			memcpy( reqbuf, &req, anssize ) ;
			//	書き込みデータをコピー
			memcpy( &reqbuf[anssize], &buf[p], fsize ) ;
			//	送受信
			ret = SendAndRecv( lpInfo, reqbuf, reqsize, (char*)&ans, anssize ) ;

			//	失敗したときは即出る
			if( ret != 0 ){
				break ;
			}
			//	次の書き込みデータ位置に移動
			p = p + fsize ;
		}
	}

	//	書き込み完了
	if( ret == 0 ){
		do{
			//	要求電文と受信電文のサイズを計算
			reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
			anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
			//	ヘッダーの設定
			//	プロトコルバージョン
			req.appheader.ver = htons(lpInfo->ver);
			//	要求電文サイズ
			req.appheader.size = htons(reqsize) ;
			//	コマンド
			req.cmdheader.command = 'W' ;
			//	ファイル番号 or アドレス の指定 ( 'F' or 'A' )
			req.cmdheader.fileoraddr = FileorAddr ;
			//	ファイル番号 or アドレス
			req.cmdheader.addr = htonl(fno) ;
			//	レコード番号 or サイズ
			req.cmdheader.rec = htonl(rec) ;
			//	フレーム数
			req.cmdheader.info[0] = htons(fr) ;
			//	送受信
			// FROMアクセスの可能性があるのでタイムアウトに15sec上乗せ
			// 2097152バイトのFROMファイル書き込みに約10sec必要。
			lpInfo->timeout += 15000;
			int to = (lpInfo->timeout > 500) ? lpInfo->timeout - 500 : 1;
			int ret = setsockopt(lpInfo->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
			// SetServerを呼ぶと、ソケットを破棄せずに再作成してしまうため削除。
//			SetServer(lpInfo, lpInfo->wPort);
			ret = SendAndRecv( lpInfo, (char*)&req, reqsize, (char*)&ans, anssize ) ;
			lpInfo->timeout -= 15000;	// 次に備えてさっさと戻しておく
			// SetServerを呼ぶと、ソケットを破棄せずに再作成してしまうため削除。
//			SetServer(lpInfo, lpInfo->wPort);
			to = (lpInfo->timeout > 500) ? lpInfo->timeout - 500 : 1;
			ret = setsockopt(lpInfo->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));

			//	フレーム抜けのエラーが返ってきたとき
			if( ret == WSFTERR_FRAME_SKIP ){
				//	エラーコードを戻します
				ret = 0 ;

				//	抜けていたフレーム番号を取得する(最大３つ)
				for( i=1 ; i<4 ; i++ ){
					thisskipfr[i-1] = -1 ;
					//	有効なフレームＮｏのとき
					if( htons(ans.cmdheader.info[i]) > 0 && htons(ans.cmdheader.info[i]) <= fr ){
						thisskipfr[i-1] = htons(ans.cmdheader.info[i]) ;
					}else{
						break ;
					}
				}

				//	抜けていたフレームの内容を再度送信します
				for( i=0 ; i<3 ; i++ ){
					//	有効なフレームＮｏのとき
					if( thisskipfr[i] != -1 ){
						//	抜けていたフレームNoの取得
						//frno = htons( ans.cmdheader.info[i] ) ;
						frno = thisskipfr[i] ;
						//	今回の送信サイズの計算
						if( (frno * maxdatasize) > bufsize ){
							fsize = bufsize - ((frno-1)*maxdatasize) ;
						}else{
							fsize = maxdatasize ;
						}
						//	送信するフレームのデータ位置
						p = (frno-1)* maxdatasize ;

						//	書き込み用の要求電文と応答電文のサイズを計算
						reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) + fsize ;
						anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
						//	ヘッダーの設定
						//	プロトコルバージョン
						req.appheader.ver = htons(lpInfo->ver);
						//	要求電文サイズ
						req.appheader.size = htons(reqsize) ;
						//	コマンド
						req.cmdheader.command = 'D' ;
						//	ファイル番号 or アドレス の指定 ( 'F' or 'A' )
						req.cmdheader.fileoraddr = FileorAddr ;
						//	ファイル番号 or アドレス
						req.cmdheader.addr = htonl(fno) ;
						//	レコード番号 or サイズ
						req.cmdheader.rec = htonl(rec) ;
						//	フレームNo
						req.cmdheader.info[0] = htons(frno) ;
						//	初期化
						memset( reqbuf, 0, lpInfo->maxsize ) ;
						//	ヘッダ部をコピー
						memcpy( reqbuf, &req, anssize ) ;
						//	書き込みデータをコピー
						memcpy( &reqbuf[anssize], &buf[p], fsize ) ;
						//	送受信
						ret = SendAndRecv( lpInfo, reqbuf, reqsize, (char*)&ans, anssize ) ;

						//	失敗したときは即出る
						if( ret != 0 ){
							break ;
						}
					}else{
						break ;
					}
				}

				if( ret == 0 ){
					//	今回のフレーム抜けで
					for( i=0 ; i<3 ; i++ ){
						thisretrycount[i] = 0 ;
						for( j=0 ; j<3 ; j++ ){
							if( skipfr[j] == thisskipfr[i] ){
								thisretrycount[i] = f_retrycount[j] ;
							}
						}
						thisretrycount[i]++ ;
					}

					retrycount = 0 ;
					for( i=0 ; i<3 ; i++ ){
						skipfr[i] = thisskipfr[i] ;
						f_retrycount[i] = thisretrycount[i] ;
						if( retrycount < f_retrycount[i] ){
							retrycount = f_retrycount[i] ;
						}
					}
					ret = WSFTERR_FRAME_SKIP ;
					//	まだリトライ回数が設定回数以下のときは戻り値を戻す
					//	2001/09/20日 修正 --->
					//	リトライ回数が１回多かった
					//if( retrycount <= lpInfo->retrycount ){
					if( retrycount < lpInfo->retrycount ){
					//	<--- ここまで
						ret = 0 ;
					}
				}
			//	フレーム抜けが無い場合は終了
			}else{
				break ;
			}
		}while( ret == 0 ) ;
	}

	//	確保したメモリの解放
	free( reqbuf ) ;

	TraceD("com_writefile() : com_writefile関数を戻り値 (%d) で終了します。", ret ) ;

	return ret ;

}
