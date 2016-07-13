//
// モニター情報に関するコード
//
// monitor : Version 0.2015.12.23
//
// r.kojima
//
#pragma once

#include <Windows.h>
#include <vector>

namespace kjm {
	//
	// モニター情報を保持するクラス
	//
	class monitorInfo {
	public:
		MONITORINFOEX m_info;

	public:
		monitorInfo(){}
		monitorInfo(const monitorInfo& src) : m_info(src.m_info) {}
		monitorInfo& operator=(const monitorInfo& src);
		~monitorInfo() {}

	public:
		BOOL getMonitorInfo(HMONITOR hMonitor);
	};

	//
	// モニター一覧を保持するクラス
	//
	class monitors {
	public:
		typedef std::vector<monitorInfo> MONITORS;
		MONITORS m_monitors;

	public:
		monitors(){}
		monitors(const monitors& src) : m_monitors(src.m_monitors) {}
		monitors& operator=(const monitors& src);
		~monitors(){}

	public:
		BOOL enumDisplayMonitors(HDC hdc=NULL, LPCRECT lprcClip=NULL);

	public:
		int get_MonitorCount() const { return m_monitors.size(); }

		// プライマリーディスプレイモニターのインデックスを取得
		// 見つからない場合は、-1を返す
		int findPrimaryDisplayMonitor() const;

	private:
		// enumDisplayMonitors で使用する列挙関数
		// dwData に this を渡している
		static BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
	};
};
