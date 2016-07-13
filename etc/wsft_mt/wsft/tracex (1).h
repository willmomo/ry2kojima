#if !defined(__TRACEX_H)
#define __TRACEX_H

enum TRACE_LEVEL {
	TRACE_LEVEL_DEBUG = 0,	// �f�o�b�O�o��
	TRACE_LEVEL_INFO,		// ���o��
	TRACE_LEVEL_WARNING,	// �x��
	TRACE_LEVEL_ERROR,		// �G���[
	TRACE_LEVEL_FATAL		// �v����
};

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

typedef struct {
	char dir[_MAX_PATH];	// �o�͐�t�H���_
	char pre[_MAX_PATH];	// prefix
	char suf[_MAX_PATH];	// suffix
} TRACEX_OPT;

void TraceXStart(TRACEX_OPT* opt);
void TraceX(TRACE_LEVEL level, const char* format, ...);
void TraceD(const char* format, ...);
void TraceI(const char* format, ...);
void TraceW(const char* format, ...);
void TraceE(const char* format, ...);
void TraceF(const char* format, ...);

#endif
