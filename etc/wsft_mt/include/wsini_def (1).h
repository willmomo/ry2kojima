/**************************************************************************************/
/***																				***/
/***								wsini_def.h										***/
/***							設定ファイル用定数									***/
/**************************************************************************************/
#ifndef	_WSINI_DEF_H	//	{{
#define	_WSINI_DEF_H

//	セクション
#define INI_SEC_UPDATE_SYS	"WSSA_UPDATE_SYS"
#define INI_SEC_WSFT_SYS	"WSFT_SYS"
#define INI_SEC_LOG			"LOG"

//	キー
#define INI_KEY_PORT_READ	"Port_Read"
#define INI_KEY_PORT_WRITE	"Port_Write"
#define INI_KEY_TIMEOUTSEC	"timeoutsec"
#define INI_KEY_RETRYCOUNT	"retrycount"
#define INI_KEY_ADDR		"addr"
#define INI_KEY_STARTGO		"startgo"
#define INI_KEY_DISPMSG		"DispMsg"

#define INI_KEY_VER			"ver"
#define INI_KEY_MAINVER		"mainver"
#define INI_KEY_SUBVER	"subver"
#define INI_KEY_MAXSIZE		"maxsize"

#define INI_KEY_LOGPATH			"path"
#define INI_KEY_TRACELEVEL		"tracelevel"
#define INI_KEY_TRACESAVED		"tracesaved"
#define INI_KEY_TRACEMAXSIZE	"tracemaxsize"
#define INI_KEY_TRACEVER		"tracever"

//	デフォルト値
#define INI_DEF_PORT_READ	(20001)
#define INI_DEF_PORT_WRITE	(20002)
#define INI_DEF_TIMEOUTSEC	(3000)
#define INI_DEF_RETRYCOUNT	(3)
#define INI_DEF_ADDR		""
#define INI_DEF_STARTGO		"TRUE"
#define INI_DEF_DISPMSG		"FALSE"

#define INI_DEF_VER			"1.0"
#define INI_DEF_MAINVER		(1)
#define INI_DEF_SUBVER		(0)
#define INI_DEF_MAXSIZE		(4096)

#define INI_DEF_LOGPATH			""
#define INI_DEF_TRACELEVEL		(0)
#define INI_DEF_TRACESAVED		(0)
#define INI_DEF_TRACEMAXSIZE	(0)
#define INI_DEF_TRACEVER		(2)


#endif	//	_WSINI_DEF_H	}}
