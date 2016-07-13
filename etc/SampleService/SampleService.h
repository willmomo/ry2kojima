#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <tchar.h>

#include "kjmlib.h"

#include "resource.h"

extern kjm::log g_log;	// グローバルなログ出力クラス

class ssthread : public kjm::thread {
public:
	// コンストラクタ
	ssthread() {};

	// デストラクタ
	virtual ~ssthread() {};

	// ワーカースレッド
	virtual unsigned worker();
};
