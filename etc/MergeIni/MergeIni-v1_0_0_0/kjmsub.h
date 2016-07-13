//---------------------------------------------------------------------------------------
// kjmsub ver.0.4.0.0
//---------------------------------------------------------------------------------------
/*
	【kjm::util::getFileSize について】
	ver.0.3.0.0 から追加。

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


#pragma warning(disable : 4786)

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <algorithm>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#pragma comment(lib, "wsslog.lib")
#endif

typedef std::basic_string<TCHAR>	_tstring;


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
	// 共通のログ処理
	class log {
	public:
		// デフォルトコンストラクタ
		log() : m_ident() {};

		// デストラクタ
		~log() {};

		// エラーログ
		void writeError(LPCTSTR pszFormat, ...);

		// 情報ログ
		void writeInfo(LPCTSTR pszFormat, ...);

		// 全ログ
		void writeAll(LPCTSTR pszFormat, ...);

		// 古いログの削除
		void deleteOldLog();

		// 識別子の取得
		_tstring get_Ident() { return m_ident; };

		// 識別氏の設定
		void set_Ident(const _tstring& value) { m_ident = value; };

	private:
		_tstring m_ident;
	};


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
		static _tstring formatMessageBySystem(DWORD dwErr);

		// GetComputerNameEx関数(vc6用)
		static BOOL __GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize);

		// stringを返すGetComputerNameEx関数
		// [例外] throw ::GetLastError();
		static _tstring getComputerNameEx(COMPUTER_NAME_FORMAT NameType);

		// stringを返すGetDlgItemText関数
		// [引数] hDlg        ダイアログボックスのハンドル
		// [引数] nIDDlgItem  コントロールの識別子
		// [例外] throw ::GetLastError();
		static _tstring getDlgItemText(HWND hDlg, int nIDDlgItem);

		// 指定ファイルのサイズを返す(GetFileSize APIのラッパー)
		// [引数]  lpszFileName  ファイル名
		// [戻り値] 64bitのファイルサイズ
		static __int64 getFileSize(LPCTSTR lpszFileName);

		// ワークエリアの中心にウィンドウを移動する
		static BOOL moveCenterWorkarea(HWND hwnd);

		// SetComputerNameEx関数(vc6用)
		static BOOL __SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer);


		// 安全な vsprintf
		static int vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr);

		// 安全な sprintf 
		//
		// [引数]
		// 一番目の引数が、vector<TCHAR>への参照になっている以外、
		// sprintf関数と同じです。
		static int sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...);

		// 内部とレースのためのログオブジェクトを指定する
		static void set_logObj(kjm::log* pLogObj);

	private:
		util() {};
		~util() {};
	};

	//-------------------------------------------------------------------------
	// iniファイルを処理するクラス
	//-------------------------------------------------------------------------
	class iniFile {
	public:
		// コンストラクタ
		iniFile() : m_fileName() {};

		// コピーコンストラクタ
		iniFile(const iniFile& src) : m_fileName(src.m_fileName) {};

		// ファイル名を与えるコンストラクタ
		iniFile(const _tstring& szFileName) : m_fileName(szFileName) {};

		// デストラクタ
		virtual ~iniFile() {};

		// 代入演算子
		kjm::iniFile& operator=(const iniFile& rhs) {
			if (this == &rhs) return *this;
			this->m_fileName = rhs.m_fileName;
			return *this;
		};

		// セクション一覧を取得
		std::vector<_tstring> getSectionNames() const;

		// セクション内のキー一覧を取得
		std::vector<_tstring> getKeyNames(const _tstring& strSectionName) const;

		// iniファイルから値を取得
		_tstring getString(const _tstring& strSection, const _tstring& strKey, const _tstring& strDefault, BOOL* pfUseDefault = NULL) const;

		// iniファイルに値を書き込む
		BOOL writeString(const _tstring& strSection, const _tstring& strKey, const _tstring& strValue);

		// 別のiniファイルの内容をマージする
		void mergeIniFile(const iniFile& newIniFile);

		// iniファイル名を取得
		_tstring get_fileName() const { return m_fileName; };

		// iniファイル名を設定
		void set_fileName(const _tstring& value) { m_fileName = value; };

	private:
		_tstring m_fileName;	// iniファイルの名前
	};
}

