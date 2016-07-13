//-----------------------------------------------------------------------------
// cron のメイン処理
//-----------------------------------------------------------------------------
#include "kjmlib-20130512/kjmlib.h"

#include "cron_core.h"

static cron_options* s_popt;

//-----------------------------------------------------------------------------
// mode
// 0:min 1:hour 2:day 3:month 4:week
//-----------------------------------------------------------------------------
static void parseCronExpression(int mode, const _tstring& expr, std::vector<int>& nums) {
	// 文字列に , が含まれる時は、token 分割して再帰呼び出し
	if (expr.find_first_of(_T(",")) != _tstring::npos) {
		std::vector<_tstring> tokens = kjm::util::split(expr, ',');
		for (std::vector<_tstring>::size_type i = 0; i < tokens.size(); i++) {
			parseCronExpression(mode, tokens[i], nums);
		}
	} else {
		// A/B > A部分で数値の範囲を B 部分で、間隔を指定するため
		// まず、 A と B に分割。スラッシュが無い時は、 B を 1 とする

		_tstring A;
		int B;
		int s = expr.find_first_of(_T("/"));	// スラッシュの有無
		if (s == _tstring::npos) {
			B = 1;
			A = expr;
		} else {
			B = _ttoi(expr.substr(s + 1).c_str());
			A = expr.substr(0, s);
		}
		if (B < 1) B = 1;

		// A の部分に - がある場合は、範囲の処理をする
		int start = -1, last = -1;
		int h = expr.find_first_of(_T("-"));	// ハイフンの有無
		if (h == _tstring::npos) {
			if (A != _T("*")) {
				start = last = _ttoi(A.c_str());
			}
		} else {
			int start = _ttoi(A.substr(0, h).c_str());
			int last = _ttoi(A.substr(h + 1).c_str());
		}

		// 範囲内に収まるように調整
		switch (mode) {
			case 0:	// min
				if (InRANGE(0, start, 59) == false)	start = 0;
				if (InRANGE(0, last,  59) == false) last  = 59;
				break;
			case 1:	// hour
				if (InRANGE(0, start, 23) == false)	start = 0;
				if (InRANGE(0, last,  23) == false) last  = 23;
				break;
			case 2:	// day
				if (InRANGE(1, start, 31) == false)	start = 1;
				if (InRANGE(1, last,  31) == false) last  = 31;
				break;
			case 3:	// month
				if (InRANGE(1, start, 12) == false)	start = 1;
				if (InRANGE(1, last,  12) == false) last  = 12;
				break;
			case 4:	// week
				if (InRANGE(0, start, 6) == false) start = 0;
				if (InRANGE(0, last,  6) == false) last  = 6;
				break;
		}

		for (int i = start; i <= last; i++) {
			nums.push_back(i);
		}
	}
}

class cronEntry {
public:
	cronEntry() {}
	cronEntry(const cronEntry& _Right) {
		min = _Right.min;
		hour = _Right.hour;
		day = _Right.day;
		month = _Right.month;
		week = _Right.week;
		cmd = _Right.cmd;
		parseEntry();
	}
	~cronEntry() {}

	cronEntry& operator=(const cronEntry& _Right) {
		if (this == &_Right) return *this;
		min = _Right.min;
		hour = _Right.hour;
		day = _Right.day;
		month = _Right.month;
		week = _Right.week;
		cmd = _Right.cmd;
		parseEntry();
		return *this;
	}
	_tstring min;
	_tstring hour;
	_tstring day;
	_tstring month;
	_tstring week;
	_tstring cmd;

	std::vector<int> vmin, vhour, vday, vmonth, vweek;
	
	void parseEntry() {
		vmin.clear(); vhour.clear(); vday.clear(); vmonth.clear(); vweek.clear();

		parseCronExpression(0, min, vmin);
		parseCronExpression(1, hour, vhour);
		parseCronExpression(2, day, vday);
		parseCronExpression(3, month, vmonth);
		parseCronExpression(4, week, vweek);
	}
};

static bool s_fLoop = true;

//-----------------------------------------------------------------------------
// cronw.txt のエントリーを解析する
//-----------------------------------------------------------------------------
static int parseCronText(const _tstring& str, std::vector<cronEntry>& entries) {
	_tstring s = str;

	cronEntry ce;

	s = kjm::util::trimLeft(s);
	if (s.empty())   return 0;  // 空行
	if (s[0] == '#') return 0;	// コメント行

	int b;

	if ((b = s.find_first_of(_T(" \t"))) == _tstring::npos) return 0;	// 異常エントリー
	ce.min = _tstring(s, 0, b);
	s = kjm::util::trimLeft(_tstring(s, b));

	if ((b = s.find_first_of(_T(" \t"))) == _tstring::npos) return 0;	// 異常エントリー
	ce.hour = _tstring(s, 0, b);
	s = kjm::util::trimLeft(_tstring(s, b));

	if ((b = s.find_first_of(_T(" \t"))) == _tstring::npos) return 0;	// 異常エントリー
	ce.day = _tstring(s, 0, b);
	s = kjm::util::trimLeft(_tstring(s, b));

	if ((b = s.find_first_of(_T(" \t"))) == _tstring::npos) return 0;	// 異常エントリー
	ce.month = _tstring(s, 0, b);
	s = kjm::util::trimLeft(_tstring(s, b));

	if ((b = s.find_first_of(_T(" \t"))) == _tstring::npos) return 0;	// 異常エントリー
	ce.week = _tstring(s, 0, b);
	s = kjm::util::trimLeft(_tstring(s, b));

	if (s.empty()) return 0; // 異常エントリー
	ce.cmd = s;

	ce.parseEntry();
	entries.push_back(ce);

	return 0;
}

//-----------------------------------------------------------------------------
// cronw.txt を読込む
//-----------------------------------------------------------------------------
static int readCronText(std::vector<cronEntry>& cronEntries) {
	_tstring cronText = kjm::util::renameExtension(_T(".txt"));

	FILE* fp = _tfopen(cronText.c_str(), _T("r"));
	if (fp != NULL) {
		int ret;
		do {
			_tstring str;
			ret = kjm::util::getline(fp, str);
			if (!str.empty()) {
				parseCronText(str, cronEntries);
			}
		} while (ret == 0);
		fclose(fp);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// エントリーの実行
//-----------------------------------------------------------------------------
static void doAction(const cronEntry& entry) {
	kjm::GetLog().write(_T("$ action: %s (NotRun = %d)"), entry.cmd.c_str(), s_popt->m_debug_notrun);

	if (s_popt->m_debug_notrun == 0) {
		kjm::process proc;
		if (proc.createProcess(entry.cmd)) {
			proc.waitForInputIdle(3000);
			kjm::GetLog().write(_T("  プロセス起動成功"));
		} else {
			kjm::GetLog().write(_T("  プロセス起動失敗: error %d"), GetLastError());
		}
	}
}

//-----------------------------------------------------------------------------
// エントリー実行（必要なら）
//-----------------------------------------------------------------------------
static void actionEntries(const kjm::dateTime& dt, const std::vector<cronEntry>& entries) {
	for (std::vector<cronEntry>::size_type i = 0; i < entries.size(); i++) {
		if (std::find(entries[i].vmin.begin(), entries[i].vmin.end(), dt.get_Minute()) == entries[i].vmin.end()) continue;
		if (std::find(entries[i].vhour.begin(), entries[i].vhour.end(), dt.get_Hour()) == entries[i].vhour.end()) continue;
		if (std::find(entries[i].vday.begin(), entries[i].vday.end(), dt.get_Day()) == entries[i].vday.end()) continue;
		if (std::find(entries[i].vmonth.begin(), entries[i].vmonth.end(), dt.get_Month()) == entries[i].vmonth.end()) continue;
		if (std::find(entries[i].vweek.begin(), entries[i].vweek.end(), dt.get_Week()) == entries[i].vweek.end()) continue;

		doAction(entries[i]);
		Sleep(0);
	}
}

//-----------------------------------------------------------------------------
// crond thread
//-----------------------------------------------------------------------------
unsigned __stdcall crond_thread(void *parg) {
	kjm::GetLog().write(_T("cron thread: start"));

	s_fLoop = true;
	s_popt = (cron_options*)parg;

	std::vector<cronEntry> cronEntries;

	kjm::GetLog().write(_T("cron thread: readCronText start"));
	readCronText(cronEntries);
	kjm::GetLog().write(_T("cron thread: readCronText end"));

	for (std::vector<cronEntry>::size_type i = 0; i < cronEntries.size(); i++) {
		cronEntry& ce = cronEntries[i];
		kjm::GetLog().write(_T("cron [%d] min[%s] hour[%s] day[%s] month[%s] week[%s] cmd[%s]"),
			i, ce.min.c_str(), ce.hour.c_str(), ce.day.c_str(), ce.month.c_str(), ce.week.c_str(), ce.cmd.c_str());
	}

	int prevMin = -1;	// 前回実行した分(分が変化した時点で実行する)
	while (s_fLoop) {
		Sleep(1000);

		kjm::dateTime now = kjm::dateTime::now();

		if (prevMin != now.get_Minute()) {
			prevMin = now.get_Minute();

			actionEntries(now, cronEntries);
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// スレッドの終了指示
//-----------------------------------------------------------------------------
void crond_thread_end() {
	s_fLoop = false;
}

