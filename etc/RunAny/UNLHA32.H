/* UNLHA32.H, Micco,  Jun.17,2002
**
** LH 2.02a
** Copyright (c) 1988-95   by Haruyasu Yoshizaki  All rights reserved.
**
** Win/WinNT-SFX
** Copyright (c) 1995-96   by mH        All rights reserved.
**
** UNLHA32.DLL
** Copyright (c) 1995-2002 by Micco     All rights reserved.
*/


#if !defined(UNLHA32_H)
#define UNLHA32_H

#define UNLHA32_VERSION		186

#ifndef FNAME_MAX32
#define FNAME_MAX32		512
#endif

#ifndef ARC_DECSTRACT
#define ARC_DECSTRACT

#if defined(__BORLANDC__)
#pragma option -a-
#else
#pragma pack(1)
#endif

#if !defined(__BORLANDC__) || __BORLANDC__ >= 0x550
typedef LONGLONG	ULHA_INT64;
#else
typedef struct {
	DWORD	LowPart;
	LONG	HighPart;
} ULHA_INT64, *LPULHA_INT64;
#endif

typedef	HGLOBAL	HARC;

typedef struct {
	DWORD 			dwOriginalSize;		/* ファイルのサイズ */
 	DWORD 			dwCompressedSize;	/* 圧縮後のサイズ */
	DWORD			dwCRC;				/* 格納ファイルのチェックサム */
	UINT			uFlag;				/* 処理結果 */
										/* Status flag */
	UINT			uOSType;			/* 書庫作成に使われた OS */
	WORD			wRatio;				/* 圧縮率 */
	WORD			wDate;				/* 格納ファイルの日付(DOS 形式) */
	WORD 			wTime;				/* 格納ファイルの時刻(〃) */
	char			szFileName[FNAME_MAX32 + 1];	/* 書庫名 */
	char			dummy1[3];
	char			szAttribute[8];		/* 格納ファイルの属性(書庫固有) */
	char			szMode[8];			/* 格納ファイルの格納モード(〃) */
}	INDIVIDUALINFO, *LPINDIVIDUALINFO;

typedef struct {
	DWORD 			dwFileSize;		/* 格納ファイルのサイズ */
	DWORD			dwWriteSize;	/* 書き込みサイズ */
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
									/* 解凍先または圧縮元パス名 */
	char			dummy[3];
}	EXTRACTINGINFO, *LPEXTRACTINGINFO;

typedef struct {
	EXTRACTINGINFO exinfo;
	DWORD dwCompressedSize;
	DWORD dwCRC;
	UINT  uOSType;
	WORD  wRatio;
	WORD  wDate;
	WORD  wTime;
	char  szAttribute[8];
	char  szMode[8];
} EXTRACTINGINFOEX, *LPEXTRACTINGINFOEX;

typedef struct {
	DWORD			dwStructSize;
	EXTRACTINGINFO	exinfo;
	DWORD			dwFileSize;			/* 格納ファイルのサイズ */
	DWORD			dwCompressedSize;
	DWORD			dwWriteSize;		/* 書き込みサイズ */
	DWORD			dwAttributes;
	DWORD 			dwCRC;
	UINT  			uOSType;
	WORD  			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char  			szMode[8];
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
										/* 解凍先または圧縮元パス名 */
	char			dummy2[3];
} EXTRACTINGINFOEX32, *LPEXTRACTINGINFOEX32;

typedef struct {
	DWORD			dwStructSize;
	EXTRACTINGINFO	exinfo;
	ULHA_INT64		llFileSize;			/* 格納ファイルのサイズ */
	ULHA_INT64		llCompressedSize;
	ULHA_INT64		llWriteSize;		/* 書き込みサイズ */
	DWORD			dwAttributes;
	DWORD 			dwCRC;
	UINT  			uOSType;
	WORD  			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char  			szMode[8];
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
										/* 解凍先または圧縮元パス名 */
	char			dummy2[3];
} EXTRACTINGINFOEX64, *LPEXTRACTINGINFOEX64;

typedef BOOL CALLBACK ARCHIVERPROC(HWND _hwnd, UINT _uMsg,
						UINT _nState, LPVOID _lpEis);
typedef ARCHIVERPROC *LPARCHIVERPROC;

typedef struct {
	DWORD			dwStructSize;
	UINT			uCommand;
	DWORD			dwOriginalSize;
	DWORD			dwCompressedSize;
	DWORD			dwAttributes;
	DWORD			dwCRC;
	UINT			uOSType;
	WORD			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAddFileName[FNAME_MAX32 + 1];
	char			dummy2[3];
}	UNLHA_ENUM_MEMBER_INFO, *LPUNLHA_ENUM_MEMBER_INFO;

typedef struct {
	DWORD			dwStructSize;
	UINT			uCommand;
	ULHA_INT64		llOriginalSize;
	ULHA_INT64		llCompressedSize;
	DWORD			dwAttributes;
	DWORD			dwCRC;
	UINT			uOSType;
	WORD			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAddFileName[FNAME_MAX32 + 1];
	char			dummy2[3];
}	UNLHA_ENUM_MEMBER_INFO64, *LPUNLHA_ENUM_MEMBER_INFO64;

typedef BOOL (CALLBACK *UNLHA_WND_ENUMMEMBPROC)(LPVOID);

#if !defined(__BORLANDC__)
#pragma pack()
#else
#pragma option -a.
#endif

#endif /* ARC_DECSTRACT */

#if !defined(__BORLANDC__)
#define	_export
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ### LHA.DLL Ver 1.1 と互換性のある API 群です。### */
/* LHA.DLL compatible API */
WORD WINAPI _export UnlhaGetVersion(VOID);

BOOL WINAPI _export UnlhaGetRunning(VOID);

BOOL WINAPI _export UnlhaGetBackGroundMode(VOID);
BOOL WINAPI _export UnlhaSetBackGroundMode(const BOOL _BackGroundMode);
BOOL WINAPI _export UnlhaGetCursorMode(VOID);
BOOL WINAPI _export UnlhaSetCursorMode(const BOOL _CursorMode);
WORD WINAPI _export UnlhaGetCursorInterval(VOID);
BOOL WINAPI _export UnlhaSetCursorInterval(const WORD _Interval);

int WINAPI _export Unlha(const HWND _hwnd, LPCSTR _szCmdLine,
						LPSTR _szOutput, const DWORD _dwSize);

/* ###『統合アーカイバ API』共通の API です。### */
/* API of Common Archiver Project */
#if !defined(CHECKARCHIVE_RAPID)
#define	CHECKARCHIVE_RAPID		0	/* 簡易(最初の３ヘッダまで) */
#define	CHECKARCHIVE_BASIC		1	/* 標準(全てのヘッダ) */
#define	CHECKARCHIVE_FULLCRC	2	/* 完全(格納ファイルの CRC チェック) */

/* 以下のフラグは上記と組み合わせて使用。*/
#define CHECKARCHIVE_RECOVERY	4   /* 破損ヘッダを読み飛ばして処理 */
#define CHECKARCHIVE_SFX		8	/* SFX かどうかを返す */
#define CHECKARCHIVE_ALL		16	/* ファイルの最後まで検索する */
#define CHECKARCHIVE_ENDDATA	32	/* 書庫より後ろの余剰データを許可 */
#endif

BOOL WINAPI _export UnlhaCheckArchive(LPCSTR _szFileName, const int _iMode);

int WINAPI _export UnlhaGetFileCount(LPCSTR _szArcFile);

#if !defined(UNPACK_CONFIG_MODE)
#define	UNPACK_CONFIG_MODE		1	/* 解凍系の設定 */
#define PACK_CONFIG_MODE		2	/* 圧縮系の設定 */
#endif

BOOL WINAPI _export UnlhaConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,
						const int _iMode);

#if !defined(ISARC_FUNCTION_START)
#define ISARC_FUNCTION_START			0
#define ISARC							0	/* Unlha */
#define ISARC_GET_VERSION				1	/* UnlhaGetVersion */
#define ISARC_GET_CURSOR_INTERVAL		2	/* UnlhaGetCursorInterval */
#define ISARC_SET_CURSOR_INTERVAL		3	/* UnlhaSetCursorInterval */
#define ISARC_GET_BACK_GROUND_MODE		4	/* UnlhaGetBackGroundMode */
#define ISARC_SET_BACK_GROUND_MODE		5	/* UnlhaSetBackGroundMode */
#define ISARC_GET_CURSOR_MODE			6	/* UnlhaGetCursorMode */
#define ISARC_SET_CURSOR_MODE			7	/* UnlhaSetCursorMode */
#define ISARC_GET_RUNNING				8	/* UnlhaGetRunning */

#define ISARC_CHECK_ARCHIVE				16	/* UnlhaCheckArchive */
#define ISARC_CONFIG_DIALOG				17	/* UnlhaConfigDialog */
#define ISARC_GET_FILE_COUNT			18	/* UnlhaGetFileCount */
#define ISARC_QUERY_FUNCTION_LIST		19	/* UnlhaQueryFunctionList */
#define ISARC_HOUT						20	/* (UnlhaHOut) */
#define ISARC_STRUCTOUT					21	/* (UnlhaStructOut) */
#define ISARC_GET_ARC_FILE_INFO			22	/* UnlhaGetArcFileInfo */

#define ISARC_OPEN_ARCHIVE				23	/* UnlhaOpenArchive */
#define ISARC_CLOSE_ARCHIVE				24	/* UnlhaCloseArchive */
#define ISARC_FIND_FIRST				25	/* UnlhaFindFirst */
#define ISARC_FIND_NEXT					26	/* UnlhaFindNext */
#define ISARC_EXTRACT					27	/* (UnlhaExtract) */
#define ISARC_ADD						28	/* (UnlhaAdd) */
#define ISARC_MOVE						29	/* (UnlhaMove) */
#define ISARC_DELETE					30	/* (UnlhaDelete) */
#define ISARC_SETOWNERWINDOW			31	/* UnlhaSetOwnerWindow */
#define ISARC_CLEAROWNERWINDOW			32	/* UnlhaClearOwnerWindow */
#define ISARC_SETOWNERWINDOWEX			33	/* UnlhaSetOwnerWindowEx */
#define ISARC_KILLOWNERWINDOWEX			34	/* UnlhaKillOwnerWindowEx */

#define ISARC_GET_ARC_FILE_NAME			40	/* UnlhaGetArcFileName */
#define ISARC_GET_ARC_FILE_SIZE			41	/* UnlhaGetArcFileSize */
#define ISARC_GET_ARC_ORIGINAL_SIZE		42	/* UnlhaArcOriginalSize */
#define ISARC_GET_ARC_COMPRESSED_SIZE	43	/* UnlhaGetArcCompressedSize */
#define ISARC_GET_ARC_RATIO				44	/* UnlhaGetArcRatio */
#define ISARC_GET_ARC_DATE				45	/* UnlhaGetArcDate */
#define ISARC_GET_ARC_TIME				46	/* UnlhaGetArcTime */
#define ISARC_GET_ARC_OS_TYPE			47	/* UnlhaGetArcOSType */
#define ISARC_GET_ARC_IS_SFX_FILE		48	/* UnlhaGetArcIsSFXFile */
#define ISARC_GET_ARC_WRITE_TIME_EX		49	/* UnlhaGetArcWriteTimeEx */
#define ISARC_GET_ARC_CREATE_TIME_EX	50	/* UnlhaGetArcCreateTimeEx */
#define	ISARC_GET_ARC_ACCESS_TIME_EX	51	/* UnlhaGetArcAccessTimeEx */
#define	ISARC_GET_ARC_CREATE_TIME_EX2	52	/* UnlhaGetArcCreateTimeEx2 */
#define ISARC_GET_ARC_WRITE_TIME_EX2	53	/* UnlhaGetArcWriteTimeEx2 */
#define ISARC_GET_FILE_NAME				57	/* UnlhaGetFileName */
#define ISARC_GET_ORIGINAL_SIZE			58	/* UnlhaGetOriginalSize */
#define ISARC_GET_COMPRESSED_SIZE		59	/* UnlhaGetCompressedSize */
#define ISARC_GET_RATIO					60	/* UnlhaGetRatio */
#define ISARC_GET_DATE					61	/* UnlhaGetDate */
#define ISARC_GET_TIME					62	/* UnlhaGetTime */
#define ISARC_GET_CRC					63	/* UnlhaGetCRC */
#define ISARC_GET_ATTRIBUTE				64	/* UnlhaGetAttribute */
#define ISARC_GET_OS_TYPE				65	/* UnlhaGetOSType */
#define ISARC_GET_METHOD				66	/* UnlhaGetMethod */
#define ISARC_GET_WRITE_TIME			67	/* UnlhaGetWriteTime */
#define ISARC_GET_CREATE_TIME			68	/* UnlhaGetCreateTime */
#define ISARC_GET_ACCESS_TIME			69	/* UnlhaGetAccessTime */
#define ISARC_GET_WRITE_TIME_EX			70	/* UnlhaGetWriteTimeEx */
#define ISARC_GET_CREATE_TIME_EX		71	/* UnlhaGetCreateTimeEx */
#define ISARC_GET_ACCESS_TIME_EX		72	/* UnlhaGetAccessTimeEx */
#define ISARC_SET_ENUM_MEMBERS_PROC		80  /* UnlhaSetEnumMembersProc */
#define ISARC_CLEAR_ENUM_MEMBERS_PROC	81	/* UnlhaClearEnumMembersProc */
#define ISARC_GET_ARC_FILE_SIZE_EX		82	/* UnlhaGetArcFileSizeEx */
#define ISARC_GET_ARC_ORIGINAL_SIZE_EX	83	/* UnlhaArcOriginalSizeEx */
#define ISARC_GET_ARC_COMPRESSED_SIZE_EX	84	/* UnlhaGetArcCompressedSizeEx */
#define ISARC_GET_ORIGINAL_SIZE_EX		85	/* UnlhaGetOriginalSizeEx */
#define ISARC_GET_COMPRESSED_SIZE_EX	86	/* UnlhaGetCompressedSizeEx */
#define ISARC_SETOWNERWINDOWEX64		87	/* UnlhaSetOwnerWindowEx64 */
#define ISARC_KILLOWNERWINDOWEX64		88	/* UnlhaKillOwnerWindowEx64 */
#define ISARC_SET_ENUM_MEMBERS_PROC64	89  /* UnlhaSetEnumMembersProc64 */
#define ISARC_CLEAR_ENUM_MEMBERS_PROC64	90	/* UnlhaClearEnumMembersProc64 */
#define ISARC_OPEN_ARCHIVE2				91	/* UnlhaOpenArchive2 */
#define ISARC_GET_ARC_READ_SIZE			92	/* UnlhaGetArcReadSize */
#define ISARC_GET_ARC_READ_SIZE_EX		93	/* UnlhaGetArcReadSizeEx*/
#define ISARC_FUNCTION_END				ISARC_GET_ARC_READ_SIZE_EX


#endif	/* ISARC_FUNCTION_START */

BOOL WINAPI _export UnlhaQueryFunctionList(const int _iFunction);

#ifndef WM_ARCEXTRACT
#define	WM_ARCEXTRACT	"wm_arcextract"

#define	ARCEXTRACT_BEGIN		0	/* 該当ファイルの処理の開始 */
#define	ARCEXTRACT_INPROCESS	1	/* 該当ファイルの展開中 */
#define	ARCEXTRACT_END			2	/* 処理終了、関連メモリを開放 */
#define ARCEXTRACT_OPEN			3	/* 該当書庫の処理の開始 */
#define ARCEXTRACT_COPY			4	/* ワークファイルの書き戻し */

BOOL WINAPI _export UnlhaSetOwnerWindow(const HWND _hwnd);
BOOL WINAPI _export UnlhaClearOwnerWindow(VOID);

BOOL WINAPI _export UnlhaSetOwnerWindowEx(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc);
BOOL WINAPI _export UnlhaKillOwnerWindowEx(const HWND _hwnd);
BOOL WINAPI _export UnlhaSetOwnerWindowEx64(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize);
BOOL WINAPI _export UnlhaKillOwnerWindowEx64(const HWND _hwnd);
#endif

/* OpenArchive 系 API です。 */

#if !defined(EXTRACT_FOUND_FILE)
/* MODE (for UnarjOpenArchive) */
#define M_INIT_FILE_USE			0x00000001L	/* レジストリの設定を使用 */
#define M_REGARDLESS_INIT_FILE	0x00000002L	/* 〃 を使用しない */
#define M_NO_BACKGROUND_MODE	0x00000004L	/* バックグラウンドを禁止 */
#define M_NOT_USE_TIME_STAMP	0x00000008L
#define M_EXTRACT_REPLACE_FILE	0x00000010L
#define M_EXTRACT_NEW_FILE		0x00000020L
#define M_EXTRACT_UPDATE_FILE	0x00000040L
#define M_CHECK_ALL_PATH		0x00000100L	/* 厳格なファイル名サーチ */
#define M_CHECK_FILENAME_ONLY	0x00000200L	/* 〃を行わない */
#define M_CHECK_DISK_SIZE		0x00000400L
#define M_REGARDLESS_DISK_SIZE	0x00000800L
#define M_USE_DRIVE_LETTER		0x00001000L	/* ドライブ名から格納 */
#define M_NOT_USE_DRIVE_LETTER	0x00002000L	/* 〃 を格納しない */
#define M_INQUIRE_DIRECTORY		0x00004000L
#define M_NOT_INQUIRE_DIRECTORY 0x00008000L
#define M_INQUIRE_WRITE			0x00010000L
#define M_NOT_INQUIRE_WRITE		0x00020000L
#define M_CHECK_READONLY		0x00040000L
#define M_REGARDLESS_READONLY	0x00080000L
#define M_REGARD_E_COMMAND		0x00100000L
#define M_REGARD_X_COMMAND		0x00200000L
#define M_ERROR_MESSAGE_ON		0x00400000L	/* エラーメッセージを表示 */
#define M_ERROR_MESSAGE_OFF		0x00800000L	/* 〃を表示しない */
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L
#define M_CHECK_PATH			0x04000000L
#define M_RECOVERY_ON			0x08000000L /* 破損ヘッダの読み飛ばし */

#define M_MAKE_INDEX_FILE		0x10000000L
#define M_NOT_MAKE_INDEX_FILE	0x20000000L
#define EXTRACT_FOUND_FILE		0x40000000L	/* 検索されたファイルを解凍 */
#define EXTRACT_NAMED_FILE		0x80000000L	/* 指定したファイルを解凍 */
#endif /* EXTRACT_FOUND_FILE */

#if !defined(SFX_NOT)
#define SFX_NOT					0	/* 通常の書庫 */

#define SFX_DOS_S				1		/* LHA's SFX 系 (small) */
#define SFX_DOS_204S			1		/* LHA's SFX 2.04S 以降 */
#define SFX_DOS_213S			1		/* LHA's SFX 2.04S 以降 */
#define SFX_DOS_250S			2		/* LHA's SFX 2.50S 以降 */
#define SFX_DOS_260S			3		/* LHA's SFX 2.60S 以降 */
#define SFX_DOS_265S			3		/* LHA's SFX 2.60S 以降 */

#define SFX_DOS_L				51		/* LHA's SFX 系 (large) */
#define SFX_DOS_204L			51		/* LHA's SFX 2.04L 以降 */
#define SFX_DOS_213L			51		/* LHA's SFX 2.04L 以降 */
#define SFX_DOS_250L			52		/* LHA's SFX 2.50L 以降 */
#define SFX_DOS_260L			53		/* LHA's SFX 2.60L 以降 */
#define SFX_DOS_265L			53		/* LHA's SFX 2.60L 以降 */

#define SFX_DOS_LARC			201		/* SFX by LARC 系 */
#define SFX_DOS_LARC_S			201		/* SFX by LARC (small) */
#define SFX_DOS_LARC_L			202		/* SFX by LARC (large) */

#define SFX_DOS_LHARC			301		/* LHarc's SFX 系 */
#define SFX_DOS_LHARC_S			301		/* LHarc's SFX (small) */
#define SFX_DOS_LHARC_L			351		/* LHarc's SFX (large) */

#define SFX_WIN16_213			1001	/* LHA's SFX 2.13.w16 系 */
#define SFX_WIN16_213_1			1001	/* WinSFX 2.13.w16.1 */
#define SFX_WIN16_213_2			1002	/* WinSFX 2.13.w16.2 */
#define SFX_WIN16_213_3			1003	/* WinSFX 2.13.w16.3 以降 */

#define SFX_WIN16_250			1011	/* LHA's SFX 2.50.w16 系 */
#define SFX_WIN16_250_1			1011	/* WinSFXM 2.50.w16.0001 以降 */
#define SFX_WIN16_255_1			1021	/* WinSFXM 2.55.w16.0001 以降 */

#define SFX_WIN32_213			2001	/* LHA's SFX 2.13.w32 系 */
#define SFX_WIN32_213_1			2001	/* WinSFX32 2.13.w32.1 以降 */
#define SFX_WIN32_213_3			2002	/* WinSFX32 2.13.w32.3 以降 */

#define SFX_WIN32_214_1			2003	/* WinSFX32 2.14.w32.0001 以降 */

#define SFX_WIN32_250			2011	/* LHA's SFX 2.50.w32 系 */
#define SFX_WIN32_250_1			2011	/* WinSFX32M 2.50.w32.0001 以降 */
#define SFX_WIN32_250_6			2012	/* WinSFX32M 2.50.w32.0006 以降 */

#define SFX_LZHSFX				2051	/* LZHSFX 系 */
#define SFX_LZHSFX_1002			2051	/* LZHSFX 1.0.0.2 以降 */
#define SFX_LZHSFX_1100			2052	/* LZHSFX 1.1.0.0 以降 */

#define SFX_LZHAUTO				2101	/* LZHAUTO 系 */
#define SFX_LZHAUTO_0002		2101	/* LZHAUTO 0.0.0.2 以降 */
#define SFX_LZHAUTO_1000		2102	/* LZHAUTO 1.0.0.0 以降 */
#define SFX_LZHAUTO_1002		2102	/* LZHAUTO 1.0.0.0 以降 */
#define SFX_LZHAUTO_1100		2103	/* LZHAUTO 1.1.0.0 以降 */

#define SFX_WIN32_LHASA			3001	/* Lhasa インストーラ */

#define SFX_DOS_UNKNOWN			9901	/* 認識できない DOS SFX */
#define SFX_WIN16_UNKNOWN		9911	/* 認識できない Win16 SFX */
#define SFX_WIN32_UNKNOWN		9921	/* 認識できない Win32 SFX */
#endif

HARC WINAPI _export UnlhaOpenArchive(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode);
HARC WINAPI _export UnlhaOpenArchive2(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode, LPCSTR _szOption);
int WINAPI _export UnlhaCloseArchive(HARC _harc);
int WINAPI _export UnlhaFindFirst(HARC _harc, LPCSTR _szWildName,
							INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export UnlhaFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export UnlhaGetArcFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export UnlhaGetArcFileSize(HARC _harc);
DWORD WINAPI _export UnlhaGetArcOriginalSize(HARC _harc);
DWORD WINAPI _export UnlhaGetArcCompressedSize(HARC _harc);
BOOL WINAPI _export UnlhaGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export UnlhaGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export UnlhaGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
DWORD WINAPI _export UnlhaGetArcReadSize(HARC _harc);
BOOL WINAPI _export UnlhaGetArcReadSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export UnlhaGetArcRatio(HARC _harc);
WORD WINAPI _export UnlhaGetArcDate(HARC _harc);
WORD WINAPI _export UnlhaGetArcTime(HARC _harc);
BOOL WINAPI _export UnlhaGetArcWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export UnlhaGetArcCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export UnlhaGetArcAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
UINT WINAPI _export UnlhaGetArcOSType(HARC _harc);
int WINAPI _export UnlhaIsSFXFile(HARC _harc);
int WINAPI _export UnlhaGetFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
int WINAPI _export UnlhaGetMethod(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export UnlhaGetOriginalSize(HARC _harc);
DWORD WINAPI _export UnlhaGetCompressedSize(HARC _harc);
BOOL WINAPI _export UnlhaGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export UnlhaGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export UnlhaGetRatio(HARC _harc);
WORD WINAPI _export UnlhaGetDate(HARC _harc);
WORD WINAPI _export UnlhaGetTime(HARC _harc);
DWORD WINAPI _export UnlhaGetWriteTime(HARC _harc);
DWORD WINAPI _export UnlhaGetAccessTime(HARC _harc);
DWORD WINAPI _export UnlhaGetCreateTime(HARC _harc);
BOOL WINAPI _export UnlhaGetWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export UnlhaGetCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export UnlhaGetAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
DWORD WINAPI _export UnlhaGetCRC(HARC _harc);
int WINAPI _export UnlhaGetAttribute(HARC _harc);
UINT WINAPI _export UnlhaGetOSType(HARC _harc);

#ifndef FA_RDONLY
/* Attribute */
#define FA_RDONLY       0x01            /* 書き込み保護属性 */
#define FA_HIDDEN       0x02            /* 隠し属性 */
#define FA_SYSTEM       0x04            /* システム属性 */
#define FA_LABEL        0x08            /* ボリューム・ラベル */
#define FA_DIREC        0x10            /* ディレクトリ */
#define FA_ARCH         0x20            /* アーカイブ属性 */
#endif

#if !defined(SFXFLAG_AUTO)
#define SFXFLAG_AUTO			0x0001
#define SFXFLAG_OVERWRITE		0x0002
#define SFXFLAG_CHECKCRC		0x0004
#define SFXFLAG_CHECKTIME		0x0008
#define SFXFLAG_ATTRIBUTE		0x0010
#define SFXFLAG_NO_AUTO			0x0100
#define SFXFLAG_NO_OVERWRITE	0x0200
#define SFXFLAG_NO_CHECKCRC		0x0400
#define SFXFLAG_NO_CHECKTIME	0x0800
#define SFXFLAG_NO_ATTRIBUTE	0x1000
#define SFXFLAG_NO_WAIT			0x2000
#endif

#if !defined(ERROR_START)
#define ERROR_START				0x8000
/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010

#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#if !defined(ERROR_ALREADY_RUNNING)
#define ERROR_ALREADY_RUNNING	0x801F
#endif
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f
#define ERROR_MAKE_ARCHIVE		0x8040
#define ERROR_NOT_CONFIRM_NAME	0x8041
#define ERROR_UNEXPECTED_EOF	0x8042
#define ERROR_INVALID_END_MARK	0x8043
#define ERROR_INVOLVED_LZH		0x8044
#define ERROR_NO_END_MARK		0x8045
#define ERROR_HDR_INVALID_SIZE	0x8046
#define ERROR_UNKNOWN_LEVEL		0x8047
#define ERROR_BROKEN_DATA		0x8048

#define ERROR_END	ERROR_BROKEN_DATA
#endif /* ERROR_START */

/* ### UNLHA32.DLL 独自の API です。### */
#define UNLHA_LIST_COMMAND		1
#define UNLHA_ADD_COMMAND		2
#define UNLHA_FRESH_COMMAND		3
#define UNLHA_DELETE_COMMAND	4
#define UNLHA_EXTRACT_COMMAND	5
#define UNLHA_PRINT_COMMAND		6
#define UNLHA_TEST_COMMAND		7
#define UNLHA_MAKESFX_COMMAND	8
#define UNLHA_JOINT_COMMAND		9
#define UNLHA_CONVERT_COMMAND	10
#define UNLHA_RENAME_COMMAND	11

WORD WINAPI _export UnlhaGetSubVersion(VOID);

int WINAPI _export UnlhaExtractMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,
		time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);
int WINAPI _export UnlhaCompressMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,
		const time_t *_lpTime, const LPWORD _lpwAttr,
		LPDWORD _lpdwWriteSize);

BOOL WINAPI _export UnlhaSetEnumMembersProc(UNLHA_WND_ENUMMEMBPROC
		_lpEnumProc);
BOOL WINAPI _export UnlhaClearEnumMembersProc(VOID);
BOOL WINAPI _export UnlhaSetEnumMembersProc64(UNLHA_WND_ENUMMEMBPROC
		_lpEnumProc, DWORD _dwStructSize);
BOOL WINAPI _export UnlhaClearEnumMembersProc64(VOID);

VOID WINAPI _export SetLangueJapanese(VOID);
VOID WINAPI _export SetLangueEnglish(VOID);

#ifdef __cplusplus
}
#endif

#endif	/* UNLHA32_H */
