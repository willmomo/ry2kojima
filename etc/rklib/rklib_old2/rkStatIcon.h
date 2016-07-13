// rkStatIcon.h : ステータス状態をタスクトレイアイコンで表示するクラス

#pragma once

#include <windows.h>

// ステータスに対応するアイコン、文字列を格納する

typedef struct {
	int state;		// ステータスの番号
	HICON hicon;	// ステータスに対応するアイコン
	char tip[64];	// ステータスに対応するツールチップの文字列
} RKSTATINFO;

// ステータス管理クラス

#define RK_MAX_STATINFO	32	// 登録可能な最大ステータス数

class rkStatIcon
{
private:
	HWND m_hwnd;	// NOTIFYICONDATA::hWnd用データ
	UINT m_uid;		// NOTIFYICONDATA::uID用データ
	UINT m_uMsg;		// NOTIFYICONDATA::uCallbackMessage用データ

	int m_state;							// 現在のステータス
	RKSTATINFO m_statInfo[RK_MAX_STATINFO];	// ステータスに対応する情報
	bool m_useInfo[RK_MAX_STATINFO];		// m_statInfo[n]の使用中を示す

public:
	rkStatIcon()
	{
		m_hwnd = NULL;
		m_uid = m_uMsg = 0;
		m_state = -1;	// 初期状態(タスクトレイにアイコン未登録)
		memset(m_statInfo, 0, sizeof(m_statInfo));
		memset(m_useInfo, false, sizeof(m_useInfo));
	};
	virtual ~rkStatIcon()
	{
		if (m_state != -1)
			SetState(-1);	// アイコンの登録を解除
	};

public:
	BOOL AddState(int state, HICON hicon, const char* tip);		// ステータスの登録
	BOOL DeleteState(int state);								// ステータスの削除
	BOOL ModifyState(int state, HICON hicon, const char* tip);	// ステータス状態の変更

	VOID SetNotifyIconInfo(HWND hwnd, UINT uid, UINT uMsg);		// タスクトレイアイコン用のデータ登録
	BOOL GetStateInfo(int state, RKSTATINFO* info);

	BOOL SetState(int state);	// ステータスを設定する
	int GetState()				// ステータスを返す
	{
		return m_state;
	}
};
