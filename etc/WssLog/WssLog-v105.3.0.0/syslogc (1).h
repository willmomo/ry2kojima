//---------------------------------------------------------------------
// $Header: /GLw3.0.0.x/WssLog/syslogc.h 2     07/09/07 18:23 Kojima $ 
//---------------------------------------------------------------------
#pragma once


#include "kjmlib.h"


// syslog client の thread クラス
//
class syslog_client : public kjm::thread {
private:
	bool m_loop;	// 終了するためのフラグ(worker内で見る)

public:
	syslog_client() : m_loop(true) {};	// コンストラクタ
	virtual ~syslog_client() {};		// デストラクタ

	unsigned worker();					// ワーカスレッド(実装は、WssLog.cppにあり)
	void end() { m_loop = false; };		// 終了指示関数
};


/**

$History: syslogc.h $ 
 * 
 * *****************  Version 2  *****************
 * User: Kojima       Date: 07/09/07   Time: 18:23
 * Updated in $/GLw3.0.0.x/WssLog
 * Embedded対応。スレッドを利用した遅延書き込み型に変更。

**/
