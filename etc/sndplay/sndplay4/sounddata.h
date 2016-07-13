
#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>

class CSoundData
{
public:
	// コンストラクタ
	CSoundData(HWND hwnd, const char* soundFile);

	// デストラクタ
	~CSoundData();

	// 再生
	bool Play();

	// 停止
	bool Stop();

	// 閉じる
	bool Close();

private:
	// デフォルトコンストラクタ(空オブジェクト作成禁止)
	CSoundData(){};

	// コピーコンストラクタ(コピー禁止)
	CSoundData(const CSoundData&){};

	// 代入演算子(コピー禁止)
	operator=(const CSoundData&){};

private:
	// 通知を受けるウィンドウへのハンドル
	HWND m_hwnd;

	// 再生するサウンドファイル名
	std::string m_soundFile;

	// デバイスID
	WORD m_deviceID;

	// MCI関数で発生した最後のエラーの文字列
	std::string m_mciLastErrorText;
};
