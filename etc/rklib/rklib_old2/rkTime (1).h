// rkTime.h : 時間処理クラス
// ヘッダのみです。

#pragma once

#include <windows.h>
#include <time.h>

// 時間間隔クラス : CTimeSpanのようなもの

class rkTimeSpan
{
private:
	double m_span;

public:
	rkTimeSpan() : m_span(0.0) {};
	rkTimeSpan(const rkTimeSpan& newSpan)
	{
		m_span = newSpan.GetTotalSeconds();
	};
	rkTimeSpan(double span) : m_span(span) {};
	rkTimeSpan(int days, int h, int m, int s)
	{
		m_span = days * 24 * 60 * 60;
		m_span += h * 60 * 60;
		m_span += m * 60;
		m_span += s;
	};

public:
	int GetDays()
	{
		return (int)(m_span / (24 * 60 * 60));
	};
	int GetTotalSeconds() const
	{
		return (int)(m_span);
	};

public:
	const rkTimeSpan& operator = (const rkTimeSpan& newTimeSpan)
	{
		m_span = newTimeSpan.GetTotalSeconds();
	};
	rkTimeSpan operator + (rkTimeSpan timeSpan) const
	{
		return rkTimeSpan(m_span + timeSpan.GetTotalSeconds());
	};
	rkTimeSpan operator - (rkTimeSpan timeSpan) const
	{
		return rkTimeSpan(m_span - timeSpan.GetTotalSeconds());
	};
	const rkTimeSpan& operator += (rkTimeSpan timeSpan)
	{
		m_span += timeSpan.GetTotalSeconds();
		return *this;
	};
	const rkTimeSpan& operator -= (rkTimeSpan timeSpan)
	{
		m_span -= timeSpan.GetTotalSeconds();
	};
	BOOL operator == (rkTimeSpan timeSpan) const
	{
		return (m_span == timeSpan.GetTotalSeconds()) ? TRUE : FALSE;
	};
	BOOL operator != (rkTimeSpan timeSpan) const
	{
		return (m_span != timeSpan.GetTotalSeconds()) ? TRUE : FALSE;
	};
	BOOL operator < (rkTimeSpan timeSpan) const
	{
		return (m_span < timeSpan.GetTotalSeconds()) ? TRUE : FALSE;
	};
	BOOL operator > (rkTimeSpan timeSpan) const
	{
		return (m_span > timeSpan.GetTotalSeconds()) ? TRUE : FALSE;
	};
	BOOL operator <= (rkTimeSpan timeSpan) const
	{
		return (m_span <= timeSpan.GetTotalSeconds()) ? TRUE : FALSE;
	};
	BOOL operator >= (rkTimeSpan timeSpan) const
	{
		return (m_span >= timeSpan.GetTotalSeconds()) ? TRUE : FALSE;
	};

};

// 時間クラス : CTimeのようなもの

class rkTime
{
private:
	time_t m_t;

public:
	rkTime() : m_t(0) {};
	rkTime(time_t now) : m_t(now) {};
	rkTime(const rkTime& newTime)
	{
		m_t = newTime.GetTime();
	};
	rkTime(int y, int m, int d, int h, int n, int s, int dst = -1)
	{
		struct tm ntm;
		memset(&ntm, 0, sizeof(ntm));
		ntm.tm_year = y - 1900;
		ntm.tm_mon = m - 1;
		ntm.tm_mday = d;
		ntm.tm_hour = h;
		ntm.tm_min = n;
		ntm.tm_sec = s;
		ntm.tm_isdst = dst;

		m_t = mktime(&ntm);
	};

public:
	static time_t GetCurrentTime()
	{
		return time(NULL);
	};
	time_t GetTime() const
	{
		return m_t;
	};
	struct tm* GetGmtTm(struct tm* ptm = NULL) const
	{
		static struct tm m_tm;
		m_tm = *gmtime(&m_t);
		if (ptm)	*ptm = m_tm;
		return &m_tm;
	};
	struct tm* GetLocalTm(struct tm* ptm = NULL) const
	{
		static struct tm m_tm;
		m_tm = *localtime(&m_t);
		if (ptm)	*ptm = m_tm;
		return &m_tm;
	};
	int GetYear() const
	{
		return GetLocalTm()->tm_year + 1900;
	};
	int GetMonth() const
	{
		return GetLocalTm()->tm_mon + 1;
	};
	int GetDay() const
	{
		return GetLocalTm()->tm_mday;
	};
	int GetHour() const
	{
		return GetLocalTm()->tm_hour;
	};
	int GetMinute() const
	{
		return GetLocalTm()->tm_min;
	};
	int GetSecond() const
	{
		return GetLocalTm()->tm_sec;
	};
	int GetDayOfWeek() const
	{
		return GetLocalTm()->tm_wday;
	};
	char* Format(char* buf, size_t maxsize, const char *format)
	{
		strftime(buf, maxsize, format, GetLocalTm());
		return buf;
	};

public:
	const rkTime& operator = (const rkTime& timeSec)
	{
		m_t = timeSec.GetTime();
		return *this;
	};
	const rkTime& operator = (time_t t)
	{
		m_t = t;
		return *this;
	};
	rkTime operator + (rkTimeSpan timeSpan) const
	{
		struct tm ltm = *localtime(&m_t);
		ltm.tm_sec += timeSpan.GetTotalSeconds();
		return rkTime(mktime(&ltm));
	};
	rkTime operator - (rkTimeSpan timeSpan) const
	{
		struct tm ltm = *localtime(&m_t);
		ltm.tm_sec -= timeSpan.GetTotalSeconds();
		return rkTime(mktime(&ltm));
	};
	rkTimeSpan operator - (rkTime t2) const
	{
		return rkTimeSpan(difftime(m_t, t2.GetTime()));
	};
	const rkTime& operator += (rkTimeSpan timeSpan)
	{
		struct tm ltm = *localtime(&m_t);
		ltm.tm_sec += timeSpan.GetTotalSeconds();
		m_t = mktime(&ltm);

		return *this;
	};
	const rkTime& operator -= (rkTimeSpan timeSpan)
	{
		struct tm ltm = *localtime(&m_t);
		ltm.tm_sec -= timeSpan.GetTotalSeconds();
		m_t = mktime(&ltm);

		return *this;
	};
	BOOL operator == (rkTime t) const
	{
		return (m_t == t.GetTime()) ? TRUE : FALSE;
	};
	BOOL operator != (rkTime t) const
	{
		return (m_t != t.GetTime()) ? TRUE : FALSE;
	};
	BOOL operator < (rkTime t) const
	{
		return (m_t < t.GetTime()) ? TRUE : FALSE;
	};
	BOOL operator > (rkTime t) const
	{
		return (m_t > t.GetTime()) ? TRUE : FALSE;
	};
	BOOL operator <= (rkTime t) const
	{
		return (m_t <= t.GetTime()) ? TRUE : FALSE;
	};
	BOOL operator >= (rkTime t) const
	{
		return (m_t >= t.GetTime()) ? TRUE : FALSE;
	};
};
