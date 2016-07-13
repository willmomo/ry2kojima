//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
#pragma once

#include "./kjmlib-20130318/kjmlib.h"

class patcherIni : public kjm::iniFile {
public:
	patcherIni() {}
	patcherIni(const _tstring& iniFile) : kjm::iniFile(iniFile) {}

	virtual ~patcherIni() {}

public:
	// Startup セクションにエントリーを「追加」する
	void appendStartupSection(const _tstring& title, const _tstring& commandLine, const _tstring& currentDirectory);

	// タイトルから Startup セクションを探す
	int findStartupSection(const _tstring& title) const;

	// 最大の startup セクションを取得
	int getMaxStartupSection() const;

private:
	// コピーと代入をブロック
	patcherIni(const patcherIni& src) {}
	patcherIni& operator=(const patcherIni& rhs) {}
};
