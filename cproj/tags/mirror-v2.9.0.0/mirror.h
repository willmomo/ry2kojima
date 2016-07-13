/*	修正履歴
	v2.7.0.5	v2.7.0.5	2014/08/27	RECYCLER をデフォルト除外フォルダに追加。
	v2.7.0.3	v2.7.0.3	2014/07/14	Thumbs.db を常に除外するように修正。
	v2.7.0.2	v2.7.0.2	2014/06/30	テストモード(--test)のとき、ファイル名が出なかったのを修正。
	v2.7.0.0	v2.7.0.1	2014/06/18	コピーの進捗表示を抑え気味に変更。
	v2.7.0.0	v2.7.0.0	2014/06/18	$RECYCLE.BIN, System Volume Information 二つのフォルダを常に除外するように修正。
*/
#pragma once

#include <windows.h>

#include <iostream>
#include <string>
#include <deque>
#include <list>

using namespace std;

#include "kjmlib.h"
///#pragma comment(lib, "./kjmlib_20130801/wsslog.lib")

#include "resource.h"

#define WARNING_SEC			60		// ファイル/フォルダ列挙にこれ以上かかると警告する。

class BackupMirror {
public:

	// 動作モード
	enum modeOption {
		modeBackupOnly = 0,		// バックアップだけ
		modeMirrorOnly,			// ミラーリングだけ
		modeBackupMirror,		// バックアップしてからミラーリング
		modeMirrorBackup		// ミラーリングしてからバックアップ
	};

public:
	BackupMirror();
	virtual ~BackupMirror() {}

public:
	// バックアップとミラーリングを動作モードによって実行する
	int doMain();

private:
	// ファイルのコピー処理
	bool copy_file(const kjm::fileInfo& fiOrg, const _tstring& strMir, const _tstring& orgBase);

	// バックアップの実行
	bool StartBackup();

	// ミラーリングの実行
	bool StartMirror();

	// バックアップ処理
	bool backup(const _tstring& strOrg, const _tstring& strMir);

	// ミラーリング処理
	bool mirror(const _tstring& strOrg, const _tstring& strMir);
	
	// 除外判定を行う
	bool is_exclude(const _tstring& strFname);

public:
	// 原本パスの取得/設定
	_tstring get_OriginalPath() const { return m_strOrg; }
	void set_OriginalPath(const _tstring& newValue) { m_strOrg = newValue; }

	// 複製パスの取得/設定
	_tstring get_MirrorPath() const { return m_strMir; }
	void set_MirrorPath(const _tstring& newValue) { m_strMir = newValue; }

	// 動作モードの取得/設定
	modeOption get_Mode() const { return m_mode; }
	void set_Mode(modeOption newValue) { m_mode = newValue; }

	// テストモードの取得/設定
	bool get_TestMode() const { return m_fTest; }
	void set_TestMode(bool newValue) { m_fTest = newValue; }

	// アーカイブのみモード取得/設定
	bool get_ArchiveOnlyMode() const { return m_fArchive; }
	void set_ArchiveOnlyMode(bool newValue) { m_fArchive = newValue; }

	// 時差考慮モード取得/設定
	bool get_TimeGup() const { return m_fTimeGup; }
	void set_TimeGup(bool newValue) { m_fTimeGup = newValue; }

	// 空フォルダ作成しないモード
	bool get_NoEmptyFolder() const { return m_fNoEmptyFolder; }
	void set_NoEmptyFolder(bool newValue) { m_fNoEmptyFolder = newValue; }

	// おしゃべりモード取得/設定
	bool get_Verbose() const { return m_fVerbose; }
	void set_Verbose(bool newValue) { m_fVerbose = newValue; }

	// 警告表示秒数の取得/設定
	DWORD get_WarningSec() const { return m_warningSec; }
	void set_WarningSec(DWORD newValue) { m_warningSec = newValue; }

	// 除外ファイル一覧の取得/設定
	std::vector<_tstring> get_ExcludeList() const { return m_excludeList; }
	void set_ExcludeList(const std::vector<_tstring>& newValue) { m_excludeList = newValue; }

	// 逆順モード取得/設定
	bool get_Reverse() const { return m_fReverse; }
	void set_Reverse(bool newValue) { m_fReverse = newValue; }

	// 日付をチェック対象としない設定の取得/設定
	bool get_IgnoreDate() const { return m_fIgnoreDate; }
	void set_IgnoreDate(bool newValue) { m_fIgnoreDate = newValue; }

	// サイズをチェック対象としない設定の取得/設定
	bool get_IgnoreSize() const { return m_fIgnoreSize; }
	void set_IgnoreSize(bool newValue) { m_fIgnoreSize = newValue; }

	// 再帰処理をしない設定の取得/設定
	bool get_NoRecurse() const { return m_fNoRecurse; }
	void set_NoRecurse(bool newValue) { m_fNoRecurse = newValue; }

	// 共有違反コピーエラーをエラーとして扱う
	bool get_Strict() const { return m_fStrict; }
	void set_Strict(bool newValue) { m_fStrict = newValue; }

private:
	_tstring m_strOrg;					// 原本のパス
	_tstring m_strMir;					// 複製のパス
	modeOption m_mode;						// 動作モード
	bool m_fTest;							// テストモード時 true
	DWORD m_warningSec;						// 警告を表示する秒数
	std::vector<_tstring> m_excludeList;	// 除外ファイル一覧
	bool m_fArchive;						// アーカイブのみモード時 true
	bool m_fTimeGup;						// 時差考慮モード時 true
	bool m_fNoEmptyFolder;					// 空フォルダを作成しないモード時 true
	bool m_fVerbose;						// おしゃべりモード時 true
	bool m_fIgnoreDate;					// 日付をチェック対象としない時 true
	bool m_fIgnoreSize;					// サイズをチェック対象としない時 true

	bool m_fReverse;						// フォルダを名前逆順ソートで処理する true
		// フォルダ名が日付の場合、新しいフォルダから処理をすることができるようになる。

	bool m_fNoRecurse;						// (--norecurse)再帰的にコピーを行わないときに true
	bool m_fStrict;							// (--strict)共有違反コピーエラーをエラーとして扱うときに true

	kjm::_tstrings m_defaultExcludeFolders;	// 標準の除外フォルダリスト
	kjm::_tstrings m_defaultExcludeFiles;	// 標準の除外ファイルリスト
};
