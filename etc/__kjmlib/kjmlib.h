/**
	@file
	@brief	namespace kjm �ɑ�����N���X�̐錾�B
*/

#pragma once

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#include <string>
#include <vector>
#include <stdio.h>

/// �������쐬����N���X�p�� namespace
namespace kjm
{
	/**
		WIN32API�����b�v���邽�߂̃N���X�B
		@note
		static �֐��΂���ŁA�C���X�^���X���������Ȃ��B
	*/
	class win32
	{
	public:
		static std::string ExpandEnvironmentStrings(const char* src);

		static std::string GetModuleFileName(HMODULE hModule = NULL);

	private:
		/// �C���X�^���X���������Ȃ����߁A�R���X�g���N�^�� private
		win32() {};

		/// �C���X�^���X���������Ȃ����߁A�f�X�g���N�^�� private
		~win32() {};
	};

	/**
		InternetCrackUrl �֐������b�v���邽�߂̃N���X
	*/
	class UrlComponents
	{
	public:
		std::string m_szScheme;		///< URL_COMPONENTS::lpszScheme ����
		INTERNET_SCHEME m_nScheme;	///< URL_COMPONENTS::nScheme ����
		std::string m_szHostName;	///< URL_COMPONENTS::lpszHostName ����
		INTERNET_PORT m_nPort;		///< URL_COMPONENTS::nPort ����
		std::string m_szUserName;	///< URL_COMPONENTS::lpszUserName ����
		std::string m_szPassword;	///< URL_COMPONENTS::lpszPassword ����
		std::string m_szUrlPath;	///< URL_COMPONENTS::lpszUrlPath ����
		std::string m_szExtraInfo;	///< URL_COMPONENTS::lpszExtraInfo ����

		BOOL CrackUrl(const char* lpszUrl);
	};

	/**
		�n���h�� HINTERNET �������֐������b�v���邽�߂̃N���X
	*/
	class Internet
	{
	public:
		Internet();

		/// �f�X�g���N�^
		virtual ~Internet();

		BOOL InetOpen(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags);

		BOOL InetConnect(Internet& rInetSession, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD dwContext);

		BOOL InetFtpOpenFile(Internet& rFtpSession, LPCSTR lpszFileName, DWORD fdwAccess, DWORD dwFlags, DWORD dwContext);

		BOOL InetFtpGetFile(LPCSTR lpszRemoteFile, LPCSTR lpszNewFile, BOOL fFailIfExists, DWORD dwLocalFlagsAndAttributes, DWORD dwInternetFlags, DWORD dwContext);

		BOOL InetFtpPutFile(LPCSTR lpszLocalFile, LPCSTR lpszNewRemoteFile, DWORD dwFlags, DWORD dwContext);

		BOOL InetReadFile(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);

		BOOL InetReadFile(std::vector<char>& buffer);

		BOOL InetWriteFile(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten);

		BOOL InetQueryDataAvailable(LPDWORD lpdwNumberOfBytesAvailable, DWORD dwFlags, DWORD dwContext);

		BOOL InetCloseHandle();

		DWORD get_lastError();

	private:
		/// �R�s�[�R���X�g���N�^�B�C���X�^���X�̃R�s�[���֎~����
		Internet(const Internet&) {};
		/// ������Z�q�B�C���X�^���X�̃R�s�[���֎~����
		void operator=(const Internet&) {};

		HINTERNET m_hInternet;	///< InetOpen, InetConnect, InetFtpOpenFile �ɂ���ď����������n���h��
		DWORD m_lastError;		///< ������API���g�p��������� GetLastError �֐��̖߂�l
	};

	/**
		FILE �^�����S�Ɏg�p����N���X
	*/
	class file
	{
	public:
		file();

		/// �f�X�g���N�^
		virtual ~file();

		bool fopen(const char* fname, const char* mode);

		void fclose();

		int fputs(const char* str);

		std::string fgets();

		void fprintf(const char* format, ...);

		void vfprintf(const char* format, va_list argptr);

	private:
		/// �R�s�[�R���X�g���N�^�B�C���X�^���X�̃R�s�[���֎~����
		file(const file&) {};
		/// ������Z�q�B�C���X�^���X�̃R�s�[���֎~����
		void operator=(const file&) {};

	private:
		FILE* m_fp;	///< �t�@�C���|�C���^
	};

	/**
		time_t �^��֗��Ɏg�p����N���X
	*/
	class time
	{
	public:
		time();

		/// �R�s�[�R���X�g���N�^
		time(const time& src);

		/// time_t �ŏ���������R���X�g���N�^
		time(time_t t);

		/// �f�X�g���N�^
		virtual ~time();

		std::string strftime(const char* format);

		static time now();

		time& operator=(const time& rhs);

	private:
		time_t m_t;	///< �����ŕێ����鎞��
	};

	/**
		�R�}���h���C���̉�͂��T�|�[�g����N���X
		@note
		���̃N���X��h�����āAparse_command���I�[�o�[���C�h���Ă��������B
	*/
	class command_line_info
	{
	public:
		command_line_info();

		/// �f�X�g���N�^
		virtual ~command_line_info();

		void parse_command_line(int argc, char** argv);

	protected:
		/**
			�e�p�����[�^�̉�͂��s�����߂ɁA���̊֐����I�[�o�[���C�h���Ă��������B
			@param	flag	true�̂Ƃ��Acurrent�́A�t���O�Bfalse�̂Ƃ��Acurrent�́A�p�����[�^�B
			@param	current	���ݒ��ڂ��Ă�������̒l
			@param	next	���ɍT���Ă�������̒l
			@retval	true	next���p�����[�^�Ƃ��ė��p����
			@retval false	next���p�����[�^�Ƃ��ė��p���Ă��Ȃ�
		*/
		virtual bool parse_command(bool flag, const char* current, const char* next) = 0;
	};

	/**
		�ėp���O�N���X
	*/
	class logfile
	{
	public:
		logfile();

		/// �f�X�g���N�^
		virtual ~logfile();

		void logsig();

		void log_f(const char* format, ...);

	private:
		std::string m_fname;	///< ���O�t�@�C����
	};
};

