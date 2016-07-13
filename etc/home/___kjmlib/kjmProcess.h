//---------------------------------------------------------------------
// プロセスを扱うクラス
//
// 2009/01/24 小島  新規作成
//---------------------------------------------------------------------
#pragma once

#include <windows.h>

namespace kjm {
	// プロセスを扱うクラス
	class Process {
	private:
		PROCESS_INFORMATION m_pi;
		STARTUPINFO m_si;

	public:
		Process();
		virtual ~Process() { CloseProcessHandle(); };

		bool IsStillActive();		// プロセスが動作中かどうか調べる
		bool Run(LPCTSTR cmdLine);	// プロセスを実行する
		void CloseProcessHandle();
	};
};
