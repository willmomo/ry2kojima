//---------------------------------------------------------------------
//	WssLog���ʃw�b�_(VB�p��WssLog.bas)
//
//	[�T�v]
//	WssLog.dll���g�p����l�́A������C���N���[�h

#pragma once


// INI�t�@�C���̃f�t�H���g�l

#define DEFAULT_LOG_SAVEDAYS	90		// [Log] SaveDays= 90 ��


//�@���O�̃��x����`

#define LOGLV_DEBUG			0			// �f�o�b�O�p
#define	LOGLV_TRACE			1			// �g���[�X
#define	LOGLV_WARNING		2			// ���[�j���O(�ʏ�͂��̃��x���ȏ���o��)
#define	LOGLV_ERR			3			// �G���[
#define	LOGLV_FATALERR		4			// �v���I�G���[
#define LOGLV_INFO			5			// ���
#define	LOGLV_ALWAYS		9			// �K�����O���o��
										// (��).�p�ɂɋN������̂Ɋւ��Ă͎g�p�֎~
// ���O���ʃR�[�h(6�����ȓ��Œ�`)

#define	LOGC_APPCTRL		"APPCTL"	// WssAppCtrl.exe
#define LOGC_COM			"COM"		// WssCom.exe
#define LOGC_SYUUKEI		"SYUKEI"	// Syukei.exe
#define LOGC_TNIO			"TNIO"		// Tnio.exe
#define	LOGC_UNSET			"UNYOU"		// Unyou.exe
#define	LOGC_GYOSET			"GYOSET"	// GyoSet.exe
#define	LOGC_SYSSET			"SYSSET"	// SysSet.exe
#define	LOGC_TOUINJI		"TOUKA"		// Toukainji.exe
#define LOGC_WINKT			"WINKT"		// WinKT.exe
#define LOGC_MPCPROC		"MPCPRC"	// mpcproc.exe
#define	LOGC_CALDLL			"CALDLL"	// WinSousaCalc.dll
#define	LOGC_FILDLL			"FILDLL"	// Wsft.dll(GN/HT�\�[�X�݊��� GN�ł̂ݎg�p)
#define	LOGC_WSFT			"WSFT"		// Wsft.dll
#define	LOGC_UNDLL			"OPEDLL"	// Opesend.dll
#define	LOGC_GFTP			"GFTP"		// GFTP.exe
#define	LOGC_DLG			"WSSDLG"	// WssDialog.exe
#define	LOGC_WSSHUT			"WSSHUT"	// WssShutter.exe
#define LOGC_WSSASUP		"SUPDLL"	// Wssasup.dll
#define LOGC_TNPROC			"TNPROC"	// TnProc.exe
#define LOGC_RIREKI			"RIREKI"	// rireki.dll
#define LOGC_HSCP			"HSCP"		// hscp.dll
#define LOGC_ITA			"ITA"		// ITA.dll

// export ���ꂽ�֐��̒�`

#if defined(WSSLOG_EXPORTS)
#define WSSLOG_API	__declspec(dllexport) __stdcall
#else
#define WSSLOG_API	__declspec(dllimport) __stdcall
#endif

#define	DllExport	__declspec(dllexport)
#if defined(__cplusplus)
extern	"C" {
#endif
DllExport	void WINAPI Winssa_Log(const char *sikibetu,long loglevel,const char * format, ...);
DllExport	void WINAPI Winssa_Loglevel(long loglevel);
DllExport	void WINAPI Winssa_Dump(const char *sikibetu,long loglevel,const void *buffer, long size);
#if defined(__cplusplus)
}
#endif
void WSSLOG_API WLSetLogLevel(long nNewLogLevel);
long WSSLOG_API WLGetLogLevel();
void WSSLOG_API WLFlushLog();
BOOL WSSLOG_API WLDeleteOldLog();

int WSSLOG_API WLGetLogPath(char* pszBuffer, int nSize);
void WSSLOG_API WLSetLogPath(const char* pszPath);

#if defined(__cplusplus)
extern "C" {
#endif

DllExport int WINAPI TraceLogStart(unsigned char mainver, unsigned char minnerver, DWORD stTraceInfo);

#if defined(__cplusplus)
DllExport int WINAPI TraceLogDeleteOldFile(short index = 0);
#else
DllExport int WINAPI TraceLogDeleteOldFile(short index);
#endif

DllExport void WINAPI TraceLog(short tracestatus, const char *format, ...);

#if defined(__cplusplus)
}
#endif

// Trace.dll �� replace ���邽�߂̒�`��
// �ʏ�͕K�v�Ȃ��̂ŁAREPLACE_TRACELOG_DLL ����`����Ă���Ƃ������L���ɂȂ�B

#if defined(REPLACE_TRACELOG_DLL)

/*	�\�����x��	*/
typedef enum {
	TRACE_OUTPUT_LOG_MAX = 0,	/*	�g���[�X��p���O(�ő�)		*/
	TRACE_OUTPUT_LOG     = 1,	/*	�g���[�X��p���O(����)		*/
	TRACE_OUTPUT_LOG_MIN = 2,	/*	�g���[�X��p���O(�ŏ�)		*/
	TRACE_DEBUG_MAX      = 3,	/*	�f�o�b�O�p���O(�ő�)		*/
	TRACE_DEBUG          = 4,	/*	�f�o�b�O�p���O(����)		*/
	TRACE_DEBUG_MIN      = 5,	/*	�f�o�b�O�p���O(�ŏ�)		*/
	TRACE_WARNING_MAX    = 6,	/*	�x�����O(�ő�)				*/
	TRACE_WARNING        = 7,	/*	�f�o�b�O�p���O(����)		*/
	TRACE_WARNING_MIN    = 8,	/*	�f�o�b�O�p���O(�ŏ�)		*/
	TRACE_ERROR_SLIGHT   = 9,	/*	�G���[���O(�y��)			*/
	TRACE_ERROR_FATAL    = 10,	/*	�G���[���O(�d��)			*/
	TRACE_ERROR_SYSTEM   = 11,	/*	�G���[���O(�V�X�e������)	*/
	TRACE_INFO_MAX       = 12,	/*	���\��(��ɕ\��)			*/
	TRACE_INFO           = 13,	/*	���\��(��ɕ\��)			*/
	TRACE_INFO_MIN       = 14,	/*	���\��(��ɕ\��)			*/
	TRACE_SYSTEM_INFO	 = 15,	/*	�����J�n�ƏI���������ނƂ� (������ς������̂�)	*/
	TRACE_NO_OUTPUT      = 17,	/*	�_�~�[(�Ō�=����ŁA��\�����������Ă���)	*/
	TRACE_ERROR_DATABASE = 18,	/*	�f�[�^�x�[�X�ɋN��������	*/
	TRACE_SYSTEM_BUG     = 19,	/*	�o�O�Ȃ̂ŕK���\��			*/
} ETraceStatus ;

/*	�g���[�X���x���ݒ�	*/
#define	TRACELEVEL_ALL_MAX		TRACE_OUTPUT_LOG_MAX	/*	�S�ĕ\������							*/
#define	TRACELEVEL_LOG_MAX		TRACE_OUTPUT_LOG_MAX	/*	���O���ȏ��\������					*/
#define	TRACELEVEL_DEBUG_MAX	TRACE_DEBUG_MAX			/*	�f�o�b�O���ȏ��\������				*/
#define	TRACELEVEL_WARNING_MAX	TRACE_WARNING_MAX		/*	�x�����ȏ��\������					*/
#define	TRACELEVEL_ERROR_SLIGHT	TRACE_ERROR_SLIGHT		/*	�G���[���(�y��)�ȏ��\������			*/
#define	TRACELEVEL_ERROR_FATAL	TRACE_ERROR_FATAL		/*	�G���[���(�d��)�ȏ��\������			*/
#define	TRACELEVEL_ERROR_SYSTEM	TRACE_ERROR_SYSTEM		/*	�G���[���(�V�X�e������)�ȏ��\������	*/
#define	TRACELEVEL_INFO			TRACE_INFO				/*	���ȏ��\������						*/
#define	TRACELEVEL_NO_OUTPUT	TRACE_NO_OUTPUT			/*	�����\�����Ȃ�							*/

#endif