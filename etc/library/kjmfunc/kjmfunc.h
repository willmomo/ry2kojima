//---------------------------------------------------------------------
// KJMFUNC.H -- ユーティリティ的関数
//              (c) Ryouta Kojima, 2004
//---------------------------------------------------------------------

#pragma once

#if !defined(__KJMFUNC_H)
#define __KJMFUNC_H

// 必要なヘッダファイル

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>

// マクロ

#if !defined(countof)
#define countof(a)	(sizeof((a))/sizeof((a)[0]))
#endif

// 関数プロトタイプ

namespace kjm
{
	// 時間の差を扱うクラス
	class KTimeSpan
	{
	private:
		double m_span;

	public:
		KTimeSpan() : m_span(0) {};
		KTimeSpan(const KTimeSpan& ts) : m_span(ts.GetTotalSeconds()) {};
		KTimeSpan(double ts) : m_span(ts) {};
		virtual ~KTimeSpan() {};

	public:
		double GetTotalSeconds() const
		{
			return m_span;
		};
		double GetDays() const
		{
			return m_span / (24 * 60 * 60);
		};

	public:
		const KTimeSpan& operator= (const KTimeSpan& ts)
		{
			m_span = ts.GetTotalSeconds();
			return *this;
		};
	};

	// time_tを扱うクラス
	class KTime
	{
	private:
		time_t m_time;

	public:
		KTime() : m_time((time_t)-1) {};
		KTime(const KTime& t) : m_time(t.GetTime()) {};
		KTime(time_t t) : m_time(t) {};
		KTime(FILETIME t);
		virtual ~KTime() {};

	public:
		time_t GetTime() const
		{
			return m_time;
		};
		std::string Format(LPCTSTR pFormat) const
		{
			TCHAR work[2048];
			_tcsftime(work, countof(work), pFormat, localtime(&m_time));
			return std::string(work);
		};

	public:
		const KTime& operator= (const KTime& t)
		{
			m_time = t.GetTime();
			return *this;
		};
		KTimeSpan operator- (KTime t) const
		{
			return KTimeSpan(difftime(m_time, t.GetTime()));
		};

	public:
		static KTime GetCurrentTime()
		{
			return KTime(time(NULL));
		};
	};

	// WNDCLASSを簡単に扱うクラス
	class KWndClass : public WNDCLASS
	{
	public:
		KWndClass()
		{
			// それとなく初期値を設定
			this->style = CS_HREDRAW | CS_VREDRAW;
			this->lpfnWndProc = NULL;
			this->cbClsExtra = 0;
			this->cbWndExtra = 0;
			this->hInstance = NULL;
			this->hIcon = LoadIcon(NULL, IDI_APPLICATION);
			this->hCursor = LoadCursor(NULL, IDC_ARROW);
			this->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			this->lpszMenuName = NULL;
			this->lpszClassName = NULL;
		};
		virtual ~KWndClass() {};

	public:
		ATOM RegisterClass(WNDPROC proc, HINSTANCE hinst, LPCTSTR pszClassName)
		{
			this->lpfnWndProc = proc;
			this->hInstance = hinst;
			this->lpszClassName = pszClassName;

			return ::RegisterClass(this);
		};
	};

	LPTSTR Chomp(LPTSTR lpBuffer);
	BOOL DeleteFolder(LPCTSTR);
	BOOL DeleteOldFile(LPCTSTR, int);
	BOOL DateAdd(time_t tSrc, int nDays, time_t* tDst);
	BOOL DateAdd(const tm* tSrc, int nDays, tm* tDst);
	BOOL DateAdd(LPCTSTR tSrc, int nDays, LPTSTR tDst, DWORD nSize);
	DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize);
	bool IsDots(LPCTSTR);
	LPTSTR ToStringYMD745(WORD YMD745, LPTSTR lpBuffer, DWORD nSize);
	time_t ToTimeYMD745(WORD YMD745);
	time_t ToTimePSTM(DWORD pstm);
};

#endif	// __KJMFUNC_H
