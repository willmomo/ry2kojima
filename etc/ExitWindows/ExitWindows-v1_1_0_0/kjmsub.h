//---------------------------------------------------------------------------------------
// kjmsub ver.0.2.0.0
//---------------------------------------------------------------------------------------
/*
	【kjm::util::exitWindows について】
	ver.0.2.0.0 から追加。

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
		// CopyFile APIと同様の動きをする関数(エラー箇所を詳細に特定したいときに使用)
		// [戻り値]  0   正常終了
		// [戻り値] -1   コピー元ファイルのオープンに失敗
		// [戻り値] -2   コピー先ファイルのオープンに失敗
		// [戻り値] -3   コピー元ファイルの読み込みに失敗
		// [戻り値] -4   コピー先ファイルの書き込みに失敗
		// [戻り値] -5   コピー先ファイルに指定サイズ書き込みできない
		// [戻り値] -6   コピー元ファイルのタイムスタンプ取得失敗
		// [戻り値] -7   コピー先ファイルのタイムスタンプ更新失敗
		// [戻り値] -8   コピー元ファイルを閉じるのに失敗
		// [戻り値] -9   コピー先ファイルを閉じるのに失敗
		// [戻り値] -10  コピー元ファイルの属性取得失敗
		// [戻り値] -11  コピー先ファイルの属性更新失敗
		static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);

		// Windowsをシャットダウンさせる
		static int exitWindows();

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
