
#pragma once


#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include <string>
#include <vector>
#include <assert.h>

#include <time.h>

/**
	win32������O�p�N���X
*/
class kjm_w32err_exception {
public:
	DWORD m_dwErr;

	kjm_w32err_exception(DWORD dwError) : m_dwErr(dwError) {};
	virtual ~kjm_w32err_exception() {};
};


class kjm_UrlComponents {
public:
	std::string m_szScheme;
	INTERNET_SCHEME m_nScheme;
	std::string m_szHostName;
	INTERNET_PORT m_nPort;
	std::string m_szUserName;
	std::string m_szPassword;
	std::string m_szUrlPath;
	std::string m_szExtraInfo;

	BOOL CrackUrl(LPCSTR lpszUrl);
};


/**
	�n���h��HINTERNET���Ǘ�����N���X
*/
class kjm_Internet {
public:
	/**
		�f�t�H���g�R���X�g���N�^
	*/
	kjm_Internet() : m_hInternet(NULL), m_lastError(ERROR_SUCCESS) {};

	/**
		�f�X�g���N�^
	*/
	virtual ~kjm_Internet() { InetCloseHandle(); };

	/**
		InternetOpen�֐��̃��b�p�[
	*/
	BOOL InetOpen(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags) {
		assert(m_hInternet == NULL);
		if  (!(m_hInternet == NULL))	return FALSE;

		m_hInternet = ::InternetOpen(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
		m_lastError = ::GetLastError();
		return (m_hInternet != NULL);
	};

	/**
		InternetConnect�֐��̃��b�p�[
	*/
	BOOL InetConnect(HINTERNET hInternetSession, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD dwContext) {
		assert(m_hInternet == NULL);
		if  (!(m_hInternet == NULL))	return FALSE;

		m_hInternet = ::InternetConnect(hInternetSession, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
		m_lastError = ::GetLastError();
		return (m_hInternet != NULL);
	};

	/**
		FtpOpenFile�֐��̃��b�p�[
	*/
	BOOL InetFtpOpenFile(HINTERNET hFtpSession, LPCSTR lpszFileName, DWORD fdwAccess, DWORD dwFlags, DWORD dwContext) {
		assert(m_hInternet == NULL);
		if  (!(m_hInternet == NULL))	return FALSE;

		m_hInternet = ::FtpOpenFile(hFtpSession, lpszFileName, fdwAccess, dwFlags, dwContext);
		m_lastError = ::GetLastError();
		return (m_hInternet != NULL);
	};

	/**
		InternetReadFile�֐��̃��b�p�[
	*/
	BOOL InetReadFile(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead) {
		assert(m_hInternet != NULL);
		if  (!(m_hInternet != NULL))	return FALSE;

		BOOL ret = ::InternetReadFile(m_hInternet, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead);
		m_lastError = ::GetLastError();
		return ret;
	};

	/**
		InternetReadFile�֐��̃��b�p�[�B�w�肳�ꂽ�Avector<char>�̃o�b�t�@�ɂ��ׂēǂݍ��ށB
	*/
	BOOL InetReadFile(std::vector<char>& buffer) {
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
	};

	/**
		InternetWriteFile�֐��̃��b�p�[
	*/
	BOOL InetWriteFile(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten) {
		assert(m_hInternet != NULL);
		if  (!(m_hInternet != NULL))	return FALSE;

		BOOL ret = ::InternetWriteFile(m_hInternet, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
		m_lastError = ::GetLastError();
		return ret;
	};

	/**
		InternetQueryDataAvailable�֐��̃��b�p�[
	*/
	BOOL InetQueryDataAvailable(LPDWORD lpdwNumberOfBytesAvailable, DWORD dwFlags, DWORD dwContext) {
		assert(m_hInternet != NULL);
		if  (!(m_hInternet != NULL))	return FALSE;

		BOOL ret = ::InternetQueryDataAvailable(m_hInternet, lpdwNumberOfBytesAvailable, dwFlags, dwContext);
		m_lastError = ::GetLastError();
		return ret;
	};

	/**
		InternetCloseHandle�֐��̃��b�p�[
	*/
	BOOL InetCloseHandle() {
		BOOL ret = TRUE;
		if (m_hInternet) {
			ret = ::InternetCloseHandle(m_hInternet);
			m_hInternet = NULL;
		}
		return ret;
	};

	HINTERNET get_handle() { return m_hInternet; };
	DWORD get_lastError() { return m_lastError; };

private:
	kjm_Internet(const kjm_Internet& src) {};	// �R�s�[�֎~
	void operator=(const kjm_Internet& src) {};	// �R�s�[�֎~

	HINTERNET m_hInternet;
	DWORD m_lastError;
};


std::string kjm_ExpandEnvironmentStrings(LPCTSTR lpSrc);


namespace kjm {


	class time {
	public:
		time() : m_t(0) {};

		time(const time& src) { m_t = src.m_t; };

		time(time_t t) : m_t(t) {};

		virtual ~time() {};

		std::string strftime(const char* format) {

			char buf[512];

			::strftime(buf, sizeof(buf), format, ::localtime(&m_t));

			return std::string(buf);
		};

		time& operator=(const time& src) {

			if (this == &src) return *this;

			this->m_t = src.m_t;
			return *this;
		};

		static time now() {

			return time(::time(NULL));
		};

	private:
		time_t m_t;
	};


	class file {
	public:
		file() : m_fp(NULL) {};

		virtual ~file() { this->fclose(); };

		bool fopen(const char* fname, const char* mode) {

			this->fclose();

			m_fp = ::fopen(fname, mode);

			return (m_fp != NULL);
		};

		void fclose() {

			if (m_fp) {
				::fclose(m_fp);
				m_fp = NULL;
			}
		};

		std::string read_line() {

			std::string result;

			if (m_fp == NULL) return result;

			while (1) {
				int c = fgetc(m_fp);
				if (c == EOF) {
					break;
				} else if (c == '\n') {
					break;
				} else {
					result += c;
				}
			}

			return result;
		};

		void fprintf(const char* format, ...) {

			if (m_fp == NULL)	return;

			::va_list marker;
			va_start(marker, format);
			::vfprintf(m_fp, format, marker);
			va_end(marker);
		};

	private:
		file(const file&) {};			// �R�s�[�R���X�g���N�^
		void operator=(const file&) {};	// ������Z�q

	private:
		FILE* m_fp;
	};
};
