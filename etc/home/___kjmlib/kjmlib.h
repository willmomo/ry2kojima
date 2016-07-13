/**
	@file
	@brief	namespace kjm に属するクラスの宣言。
*/

#pragma once

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#include <string>
#include <vector>
#include <stdio.h>

/// 小島が作成するクラス用の namespace
namespace kjm
{
	/**
		WIN32APIをラップするためのクラス。
		@note
		static 関数ばかりで、インスタンス化を許可しない。
	*/
	class win32
	{
	public:
		static std::string ExpandEnvironmentStrings(const char* src);

		static std::string GetModuleFileName(HMODULE hModule = NULL);

	private:
		/// インスタンス化を許可しないため、コンストラクタは private
		win32() {};

		/// インスタンス化を許可しないため、デストラクタは private
		~win32() {};
	};

	/**
		InternetCrackUrl 関数をラップするためのクラス
	*/
	class UrlComponents
	{
	public:
		std::string m_szScheme;		///< URL_COMPONENTS::lpszScheme 相当
		INTERNET_SCHEME m_nScheme;	///< URL_COMPONENTS::nScheme 相当
		std::string m_szHostName;	///< URL_COMPONENTS::lpszHostName 相当
		INTERNET_PORT m_nPort;		///< URL_COMPONENTS::nPort 相当
		std::string m_szUserName;	///< URL_COMPONENTS::lpszUserName 相当
		std::string m_szPassword;	///< URL_COMPONENTS::lpszPassword 相当
		std::string m_szUrlPath;	///< URL_COMPONENTS::lpszUrlPath 相当
		std::string m_szExtraInfo;	///< URL_COMPONENTS::lpszExtraInfo 相当

		BOOL CrackUrl(const char* lpszUrl);
	};

	/**
		ハンドル HINTERNET を扱う関数をラップするためのクラス
	*/
	class Internet
	{
	public:
		Internet();

		/// デストラクタ
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
		/// コピーコンストラクタ。インスタンスのコピーを禁止する
		Internet(const Internet&) {};
		/// 代入演算子。インスタンスのコピーを禁止する
		void operator=(const Internet&) {};

		HINTERNET m_hInternet;	///< InetOpen, InetConnect, InetFtpOpenFile によって初期化されるハンドル
		DWORD m_lastError;		///< 内部でAPIを使用した直後の GetLastError 関数の戻り値
	};

	/**
		FILE 型を安全に使用するクラス
	*/
	class file
	{
	public:
		file();

		/// デストラクタ
		virtual ~file();

		bool fopen(const char* fname, const char* mode);

		void fclose();

		int fputs(const char* str);

		std::string fgets();

		void fprintf(const char* format, ...);

		void vfprintf(const char* format, va_list argptr);

	private:
		/// コピーコンストラクタ。インスタンスのコピーを禁止する
		file(const file&) {};
		/// 代入演算子。インスタンスのコピーを禁止する
		void operator=(const file&) {};

	private:
		FILE* m_fp;	///< ファイルポインタ
	};

	/**
		time_t 型を便利に使用するクラス
	*/
	class time
	{
	public:
		time();

		/// コピーコンストラクタ
		time(const time& src);

		/// time_t で初期化するコンストラクタ
		time(time_t t);

		/// デストラクタ
		virtual ~time();

		std::string strftime(const char* format);

		static time now();

		time& operator=(const time& rhs);

	private:
		time_t m_t;	///< 内部で保持する時間
	};

	/**
		コマンドラインの解析をサポートするクラス
		@note
		このクラスを派生して、parse_commandをオーバーライドしてください。
	*/
	class command_line_info
	{
	public:
		command_line_info();

		/// デストラクタ
		virtual ~command_line_info();

		void parse_command_line(int argc, char** argv);

	protected:
		/**
			各パラメータの解析を行うために、この関数をオーバーライドしてください。
			@param	flag	trueのとき、currentは、フラグ。falseのとき、currentは、パラメータ。
			@param	current	現在注目している引数の値
			@param	next	次に控えている引数の値
			@retval	true	nextをパラメータとして利用した
			@retval false	nextをパラメータとして利用していない
		*/
		virtual bool parse_command(bool flag, const char* current, const char* next) = 0;
	};

	/**
		汎用ログクラス
	*/
	class logfile
	{
	public:
		logfile();

		/// デストラクタ
		virtual ~logfile();

		void logsig();

		void log_f(const char* format, ...);

	private:
		std::string m_fname;	///< ログファイル名
	};
};

