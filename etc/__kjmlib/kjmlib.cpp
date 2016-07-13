/**
	@file
	@brief	kjmlib.h �Ő錾���Ă���N���X�̎����B
*/

#include "kjmlib.h"
#include <vector>
#include <stdarg.h>
#include <time.h>
#include <assert.h>

#include <shlwapi.h>


//-------------------------------------------------------------------------------
// kjm::file
//-------------------------------------------------------------------------------

/**
	�f�t�H���g�R���X�g���N�^
*/
kjm::file::file() : m_fp(NULL)
{
}

/**
	�f�X�g���N�^
*/
kjm::file::~file()
{
	this->fclose();
}

/**
	�t�@�C�����J��

	@param	fname	�t�@�C�����ւ̃|�C���^�B�ڍׂ́Afopen �֐����Q��
	@param	mode	�t�@�C�����J�����[�h�ւ̃|�C���^�B�ڍׂ́Afopen �֐����Q��
	@retval	true	����Ƀt�@�C���I�[�v��
	@retval	false	�t�@�C���I�[�v���Ɏ��s
*/
bool kjm::file::fopen(const char* fname, const char* mode)
{
	assert(m_fp == NULL);

	m_fp = ::fopen(fname, mode);

	return (m_fp != NULL);
}

/**
	�t�@�C�������

	@note	�t�@�C�����I�[�v�����Ă��Ȃ��Ă��Ăяo����B
*/
void kjm::file::fclose()
{
	if (m_fp) {
		::fclose(m_fp);
		m_fp = NULL;
	}
}


/**
	��s��������

	@param	str	�������ޕ�����ւ̃|�C���^
	@retval	fputs �֐����Q��
*/
int kjm::file::fputs(const char* str)
{
	assert(m_fp != NULL);

	int ret;

	ret = ::fputs(str, m_fp);
	if (ret < 0)	return ret;

	return ret;
}


/**
	��s�ǂݍ���

	@retval	���s�A�܂��́AEOF�܂ŁA�ǂݍ��񂾕������Ԃ��B
*/
std::string kjm::file::fgets()
{
	std::string result;
	char buf[14];

	while (1) {
		// ���s or EOF�܂ŁA�ǂݍ���
		char* p = ::fgets(buf, sizeof(buf), m_fp);
		if (p == NULL)	return result;

		// �ǂݍ��񂾃f�[�^�̍Ōオ���s�Ȃ�A��s�ǂݍ��񂾂��ƂɂȂ�̂�
		// �ǂݍ��ݏI���B
		if (buf[strlen(p) - 1] == '\n') {
			result += buf;
			break;
		}

		// ���̈ꕶ�����ǂ�
		int c = ::fgetc(m_fp);
		if (feof(m_fp)) {
			// �t�@�C���I�[�܂œǂݍ���ł���̂ŁA��s�ǂݍ��񂾂Ƃ��ďI��
			result += buf;
			break;
		} else {
			::ungetc(c, m_fp);
			result += buf;
		}
	}

	return result;
}


/**
	�t�H�[�}�b�g��������

	@param	format	printf �֐����Q��
*/
void kjm::file::fprintf(const char* format, ...)
{
	assert(m_fp != NULL);

	::va_list marker;
	va_start(marker, format);
	this->vfprintf(format, marker);
	va_end(marker);
}


/**
	�t�H�[�}�b�g��������
	@param format	vfprintf�֐����Q��
	@param argptr	vfprintf�֐����Q��
*/
void kjm::file::vfprintf(const char* format, va_list argptr)
{
	assert(m_fp != NULL);

	::vfprintf(m_fp, format, argptr);
}


//-------------------------------------------------------------------------------
// kjm::time
//-------------------------------------------------------------------------------

/**
	�f�t�H���g�R���X�g���N�^
*/
kjm::time::time() : m_t(0)
{
}


/**
	�R�s�[�R���X�g���N�^
*/
kjm::time::time(const kjm::time& src) : m_t(src.m_t)
{
}


/**
	time_t �ŏ���������R���X�g���N�^
*/
kjm::time::time(time_t t) : m_t(t)
{
}


/**
	�f�X�g���N�^
*/
kjm::time::~time()
{
}


/**
	���ݎ��Ԃ�ێ����� time ��Ԃ��֐�
*/
kjm::time kjm::time::now()
{
	return kjm::time(::time(NULL));
}


/**
	�������t�H�[�}�b�g����
	@param	format	strftime �֐��̃w���v���Q�ƁB
	@return	strftime �֐��ɂ���ăt�H�[�}�b�g���ꂽ�������Ԃ��B
*/
std::string kjm::time::strftime(const char* format)
{
	std::vector<char> buf;

	do {
		buf.resize(buf.size() + 256);
	} while (::strftime(buf.begin(), buf.size(), format, ::localtime(&m_t)) == 0);

	return std::string(buf.begin());
}


/**
	������Z�q
	@param	rhs	�R�s�[��(Right Hand Side)
	@return	lhs(Left Hand Side)(�������g)��Ԃ��B
*/
kjm::time& kjm::time::operator =(const kjm::time& rhs)
{
	if (this == &rhs)	return *this;	// ����I�u�W�F�N�g�R�s�[�̉��
	this->m_t = rhs.m_t;
	return *this;
}


//-------------------------------------------------------------------------------
// kjm::win32
//-------------------------------------------------------------------------------

/**
	ExpandEnvironmentStrings �̃��b�p�[

	@param	src	���ϐ����܂񂾕�����ւ̃|�C���^
	@retval	���ϐ���W�J������̕�����
*/
std::string kjm::win32::ExpandEnvironmentStrings(const char* src)
{
	DWORD dwNeeded = ::ExpandEnvironmentStrings(src, NULL, 0);

	std::vector<char> dst(dwNeeded);

	::ExpandEnvironmentStrings(src, dst.begin(), dst.size());

	return std::string(dst.begin());
}


/**
	GetModuleFileName �̃��b�p�[
	@param hModule	GetModuleFileName ���Q�ƁB�f�t�H���gNULL�B
	@retval	���W���[���̃t�@�C����
*/
std::string kjm::win32::GetModuleFileName(HMODULE hModule)
{
	DWORD ret;
	std::vector<char> work;
	do {
		work.resize(work.size() + MAX_PATH);
		ret = ::GetModuleFileName(hModule, work.begin(), work.size());
	} while (ret == work.size());

	return work.begin();
}


//-------------------------------------------------------------------------------
// kjm::UrlComponents
//-------------------------------------------------------------------------------

/**
	InternetCrackUrl �̃��b�p�[

	@param	lpszUrl	URL�`���̕�����ւ̃|�C���^
	@retval	TRUE	�ϊ��ɐ���
	@retval FALSE	�ϊ��Ɏ��s
	@note	TRUE ��߂����ꍇ�̂݁A�����o�ϐ��ɈӖ��̂���l������B
	@warning �������ꂽ������́A���̂���1024�o�C�g�ȓ��ł���K�v������B
*/
BOOL kjm::UrlComponents::CrackUrl(const char* lpszUrl)
{
	char bufScheme[1024];
	char bufHostName[1024];
	char bufUserName[1024];
	char bufPassword[1024];
	char bufUrlPath[1024];
	char bufExtraInfo[1024];

	URL_COMPONENTS uc;

	memset(&uc, 0, sizeof(uc));

	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = bufScheme;
	uc.dwSchemeLength = sizeof(bufScheme);
	uc.lpszHostName = bufHostName;
	uc.dwHostNameLength = sizeof(bufHostName);
	uc.lpszUserName = bufUserName;
	uc.dwUserNameLength = sizeof(bufUserName);
	uc.lpszPassword = bufPassword;
	uc.dwPasswordLength = sizeof(bufPassword);
	uc.lpszUrlPath = bufUrlPath;
	uc.dwUrlPathLength = sizeof(bufUrlPath);
	uc.lpszExtraInfo = bufExtraInfo;
	uc.dwExtraInfoLength = sizeof(bufExtraInfo);

	BOOL ret = ::InternetCrackUrl(lpszUrl, 0, 0, &uc);
	if (ret) {
		m_szScheme = uc.lpszScheme;
		m_nScheme = uc.nScheme;
		m_szHostName = uc.lpszHostName;
		m_nPort = uc.nPort;
		m_szUserName = uc.lpszUserName;
		m_szPassword = uc.lpszPassword;
		m_szUrlPath = uc.lpszUrlPath;
		m_szExtraInfo = uc.lpszExtraInfo;
	}

	return ret;
}


//-------------------------------------------------------------------------------
// kjm::Internet
//-------------------------------------------------------------------------------

/**
	�f�t�H���g�R���X�g���N�^
*/
kjm::Internet::Internet() : m_hInternet(NULL), m_lastError(ERROR_SUCCESS)
{
}


/**
	�f�X�g���N�^
*/
kjm::Internet::~Internet()
{
	InetCloseHandle();
}


/**
	InternetOpen �̃��b�p�[
	@param lpszAgent InternetOpen �֐��̃w���v���Q�ƁB
	@param dwAccessType InternetOpen �֐��̃w���v���Q�ƁB
	@param lpszProxyName InternetOpen �֐��̃w���v���Q�ƁB
	@param lpszProxyBypass InternetOpen �֐��̃w���v���Q�ƁB
	@param dwFlags InternetOpen �֐��̃w���v���Q�ƁB
	@retval TRUE ����I��
	@retval FALSE �G���[�I���B�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ��
*/
BOOL kjm::Internet::InetOpen(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags)
{
	assert(m_hInternet == NULL);
	if  (!(m_hInternet == NULL))	return FALSE;

	m_hInternet = ::InternetOpen(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
	m_lastError = ::GetLastError();
	return (m_hInternet != NULL);
}


/**
	InternetConnect �̃��b�p�[
	@param rInetSession		InetOpen ���g���ď��������ꂽ�Akjm::Internet �ւ̎Q�ƁB
	@param lpszServerName	InternetConnect �֐��̃w���v���Q�ƁB
	@param nServerPort		InternetConnect �֐��̃w���v���Q�ƁB
	@param lpszUsername		InternetConnect �֐��̃w���v���Q�ƁB
	@param lpszPassword		InternetConnect �֐��̃w���v���Q�ƁB
	@param dwService		InternetConnect �֐��̃w���v���Q�ƁB
	@param dwFlags			InternetConnect �֐��̃w���v���Q�ƁB
	@param dwContext		InternetConnect �֐��̃w���v���Q�ƁB
	@retval TRUE ����I��
	@retval FALSE �G���[�I���B�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ��
*/
BOOL kjm::Internet::InetConnect(kjm::Internet& rInetSession, LPCSTR lpszServerName, INTERNET_PORT nServerPort,
								LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet == NULL);
	if  (!(m_hInternet == NULL))	return FALSE;

	m_hInternet = ::InternetConnect(rInetSession.m_hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return (m_hInternet != NULL);
}


/**
	FtpOpenFile�֐��̃��b�p�[
	@param rFtpSession	InetConnect ���g���ď��������ꂽ�Akjm::Internet �ւ̎Q�ƁB
	@param lpszFileName	FtpOpenFile �֐��̃w���v���Q�ƁB
	@param fdwAccess	FtpOpenFile �֐��̃w���v���Q�ƁB
	@param dwFlags		FtpOpenFile �֐��̃w���v���Q�ƁB
	@param dwContext	FtpOpenFile �֐��̃w���v���Q�ƁB
	@retval TRUE ����I��
	@retval FALSE �G���[�I���B�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ��
*/
BOOL kjm::Internet::InetFtpOpenFile(kjm::Internet& rFtpSession, LPCSTR lpszFileName, DWORD fdwAccess, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet == NULL);
	if  (!(m_hInternet == NULL))	return FALSE;

	m_hInternet = ::FtpOpenFile(rFtpSession.m_hInternet, lpszFileName, fdwAccess, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return (m_hInternet != NULL);
}


/**
	FtpGetFile�֐��̃��b�p�[
	@param	lpszRemoteFile				FtpGetFile�֐��̃w���v���Q�ƁB
	@param	lpszNewFile					FtpGetFile�֐��̃w���v���Q�ƁB
	@param	fFailIfExists				FtpGetFile�֐��̃w���v���Q�ƁB
	@param	dwLocalFlagsAndAttributes	FtpGetFile�֐��̃w���v���Q�ƁB
	@param	dwInternetFlags				FtpGetFile�֐��̃w���v���Q�ƁB
	@param	dwContext					FtpGetFile�֐��̃w���v���Q�ƁB
	@return	FtpGetFile�֐��̖߂�l���Q�ƁB�G���[�̏ڍׂ́Aget_lastError�����o�ŎQ�Ɖ\�B
	@warning	InetConnect�����o�ŏ����������A�C���X�^���X�݂̂Ŏg�p�\�B
*/
BOOL kjm::Internet::InetFtpGetFile(LPCSTR lpszRemoteFile, LPCSTR lpszNewFile, BOOL fFailIfExists, DWORD dwLocalFlagsAndAttributes, DWORD dwInternetFlags, DWORD dwContext)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::FtpGetFile(m_hInternet, lpszRemoteFile, lpszNewFile, fFailIfExists, dwLocalFlagsAndAttributes, dwInternetFlags, dwContext);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	FtpPutFile�֐��̃��b�p�[
	@param	lpszLocalFile		FtpPutFile�֐��̃w���v���Q�ƁB
	@param	lpszNewRemoteFile	FtpPutFile�֐��̃w���v���Q�ƁB
	@param	dwFlags				FtpPutFile�֐��̃w���v���Q�ƁB
	@param	dwContext			FtpPutFile�֐��̃w���v���Q�ƁB
	@return	FtpPutFile�֐��̖߂�l���Q�ƁB�G���[�̏ڍׂ́Aget_lastError�����o�ŎQ�Ɖ\�B
	@warning	InetConnect�����o�ŏ����������A�C���X�^���X�݂̂Ŏg�p�\�B
*/
BOOL kjm::Internet::InetFtpPutFile(LPCSTR lpszLocalFile, LPCSTR lpszNewRemoteFile, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::FtpPutFile(m_hInternet, lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetReadFile�֐��̃��b�p�[
	@param lpBuffer					InternetReadFile �֐��̃w���v���Q�ƁB
	@param dwNumberOfBytesToRead	InternetReadFile �֐��̃w���v���Q�ƁB
	@param lpNumberOfBytesRead		InternetReadFile �֐��̃w���v���Q�ƁB
	@return InternetReadFile �֐��̖߂�l���Q�ƁB�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ�ƁB
	@warning InetFtpOpenFile �����o�ŏ����������A�C���X�^���X�݂̂Ŏg�p�\�B
*/
BOOL kjm::Internet::InetReadFile(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead) 
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::InternetReadFile(m_hInternet, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetReadFile�֐��̃��b�p�[�B�w�肳�ꂽ�Avector@<char@>�̃o�b�t�@�ɂ��ׂēǂݍ��ށB
	@param buffer	�ǂݍ��ނ��߂̃o�b�t�@
	@return InternetReadFile �֐��̖߂�l���Q�ƁB�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ�ƁB
	@warning InetFtpOpenFile �����o�ŏ����������A�C���X�^���X�݂̂Ŏg�p�\�B
	@note
	������ buffer �́A�N���A����܂��B
*/
BOOL kjm::Internet::InetReadFile(std::vector<char>& buffer)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	char temp[9];
	buffer.clear();

	DWORD dwRead;
	BOOL ret;
	do {
		dwRead = 0;
		ret = InetReadFile(temp, sizeof(temp), &dwRead);

		if (ret && dwRead > 0) {
			buffer.insert(buffer.end(), &temp[0], &temp[dwRead]);
		}
	} while (ret && dwRead != 0);

	return ret;
}


/**
	InternetWriteFile�֐��̃��b�p�[
	@param	lpBuffer					InternetWriteFile �֐��̃w���v���Q�ƁB
	@param	dwNumberOfBytesToWrite		InternetWriteFile �֐��̃w���v���Q�ƁB
	@param	lpdwNumberOfBytesWritten	InternetWriteFile �֐��̃w���v���Q�ƁB
	@return InternetWriteFile �֐��̖߂�l���Q�ƁB�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ�ƁB
*/
BOOL kjm::Internet::InetWriteFile(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::InternetWriteFile(m_hInternet, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetQueryDataAvailable�֐��̃��b�p�[
	@param	lpdwNumberOfBytesAvailable	InternetQueryDataAvailable �֐��̃w���v���Q�ƁB
	@param	dwFlags						InternetQueryDataAvailable �֐��̃w���v���Q�ƁB
	@param	dwContext					InternetQueryDataAvailable �֐��̃w���v���Q�ƁB
	@return InetQueryDataAvailable �֐��̖߂�l���Q�ƁB�G���[�̏ڍׂ́Aget_lastError �����o�ŎQ�ƁB
*/
BOOL kjm::Internet::InetQueryDataAvailable(LPDWORD lpdwNumberOfBytesAvailable, DWORD dwFlags, DWORD dwContext)
{
	assert(m_hInternet != NULL);
	if  (!(m_hInternet != NULL))	return FALSE;

	BOOL ret = ::InternetQueryDataAvailable(m_hInternet, lpdwNumberOfBytesAvailable, dwFlags, dwContext);
	m_lastError = ::GetLastError();
	return ret;
}


/**
	InternetCloseHandle�֐��̃��b�p�[
	@retval	TRUE	����I��
	@retval FALSE	�G���[�I��
	@note
	�n���h�����J���Ă��Ȃ���ԂŌĂяo���Ă��A����ɓ��삷��B
*/
BOOL kjm::Internet::InetCloseHandle()
{
	BOOL ret = TRUE;
	if (m_hInternet) {
		ret = ::InternetCloseHandle(m_hInternet);
		m_lastError = ::GetLastError();
		m_hInternet = NULL;
	}
	return ret;
}


/**
	������API���g�p��������� GetLastError �֐��̖߂�l��Ԃ�
	@return �Ō�Ɏ��s���� API �́AGetLastError() �֐��̒l�B
*/
DWORD kjm::Internet::get_lastError()
{
	return m_lastError;
}


//-------------------------------------------------------------------------------
// kjm::command_line_info
//-------------------------------------------------------------------------------

/// �f�t�H���g�R���X�g���N�^
kjm::command_line_info::command_line_info()
{
}


/// �f�X�g���N�^
kjm::command_line_info::~command_line_info()
{
}

/**
	�R�}���h���C���̉��
	@param	argc	main�֐���argc, WinMain�֐���__argc��n��
	@param	argv	main�֐���argv, WinMain�֐���__argv��n��
*/
void kjm::command_line_info::parse_command_line(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		bool flag = (argv[i][0] == '/' || argv[i][0] == '-') ? true : false;

		if (i + 1 < argc) {
			if (parse_command(flag, &argv[i][(flag) ? 1 : 0], argv[i + 1])) {
				++i;
			}
		} else {
			parse_command(flag, &argv[i][(flag) ? 1 : 0], NULL);
		}
	}
}


//-------------------------------------------------------------------------------
// kjm::logfile
//-------------------------------------------------------------------------------

/**
	�f�t�H���g�R���X�g���N�^
	@note
	�f�t�H���g�Ń��O�t�@�C���̖��O���A%temp%\<���s�t�@�C����>.log�ŏ���������B
*/
kjm::logfile::logfile()
{
	std::vector<char> path(MAX_PATH);

	::PathAppend(
		lstrcpy(path.begin(), kjm::win32::ExpandEnvironmentStrings("%temp%").c_str()),
		::PathFindFileName(kjm::win32::GetModuleFileName().c_str()));

	::PathRenameExtension(path.begin(), ".log");

	this->m_fname = path.begin();
}


/// �f�X�g���N�^
kjm::logfile::~logfile()
{
}


/**
	�V�O�l�`�����o�͂���
*/
void kjm::logfile::logsig()
{
	log_f("%s", kjm::win32::GetModuleFileName().c_str());
}


/**
	�t�H�[�}�b�g�t�����O�o��
*/
void kjm::logfile::log_f(const char* format, ...)
{
	kjm::file f;
	if (f.fopen(m_fname.c_str(), "a+")) {
		f.fprintf("[%s][p%04X][t%04X] ", kjm::time::now().strftime("%Y/%m/%d %H:%M:%S").c_str(), ::GetCurrentProcessId(), ::GetCurrentThreadId());

		::va_list marker;
		va_start(marker, format);
		f.vfprintf(format, marker);
		va_end(marker);

		f.fputs("\n");
	}
}
