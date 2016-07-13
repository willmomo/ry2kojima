//---------------------------------------------------------------------------------------
// kjmsub ver.0.0.0.0
//---------------------------------------------------------------------------------------
/*
	【GetComputerNameExについて】
	
	Visual C++ 6では、GetComputerNameEx関数が定義されていません。
	kjm::util::__GetComputerNameExの用に実装することで、利用することができます。

	コンピュータ名には、NetBIOS名とDNSホスト名がありますが、Windows2000以降では、DNSホス
	ト名がメインになります。また、物理的なコンピュータ名とクラスタ機能を使ったとき(だと思
	う)に付く、論理的なコンピュータ名があります。

	一般的なコンピュータ名を取得するには、第一引数にComputerNamePhysicalDnsHostname を指
	定して、GetComputerNameEx関数を呼び出します。

	【Visual C++ のバージョン判定について】

	_MSC_VER == 1200	Visual C++ 6.0
	_MSC_VER == 1310	Visual C++ .NET 2003
	_MSC_VER == 1400	Visual C++ 2005
*/
#pragma once


#include <windows.h>
#include <windowsx.h>

#include <string>


// winbase.h で宣言されている、Get/SetComputerNameEx用の引数の値
// Visual C++ 6.0 以下のときに有効にする。
#if _MSC_VER <= 1200
typedef enum _COMPUTER_NAME_FORMAT {
    ComputerNameNetBIOS,
    ComputerNameDnsHostname,
    ComputerNameDnsDomain,
    ComputerNameDnsFullyQualified,
    ComputerNamePhysicalNetBIOS,
    ComputerNamePhysicalDnsHostname,
    ComputerNamePhysicalDnsDomain,
    ComputerNamePhysicalDnsFullyQualified,
    ComputerNameMax
} COMPUTER_NAME_FORMAT ;
#endif


namespace kjm {
	// ユーティリティ関数用のstaticクラス
	// [備考]
	// すべての関数は、staticで宣言されます。
	class util {
	public:
		// システム標準のエラーメッセージを作成
		static std::basic_string<TCHAR> formatMessageBySystem(DWORD dwErr);

		// GetComputerNameEx関数(vc6用)
		static BOOL __GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize);

		// stringを返すGetComputerNameEx関数
		// [例外] throw ::GetLastError();
		static std::basic_string<TCHAR> getComputerNameEx(COMPUTER_NAME_FORMAT NameType);

		// stringを返すGetDlgItemText関数
		// [引数] hDlg        ダイアログボックスのハンドル
		// [引数] nIDDlgItem  コントロールの識別子
		// [例外] throw ::GetLastError();
		static std::basic_string<TCHAR> getDlgItemText(HWND hDlg, int nIDDlgItem);

		// ワークエリアの中心にウィンドウを移動する
		static BOOL moveCenterWorkarea(HWND hwnd);

		// SetComputerNameEx関数(vc6用)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);

	private:
		util() {};
		~util() {};
	};
}
