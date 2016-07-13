#pragma once


#include <windows.h>
#include <string>
#include <sstream>

#ifdef UNICODE
typedef std::wstring		_tstring;
typedef std::wostringstream	_tostringstream;
#else
typedef std::string			_tstring;
typedef std::ostringstream	_tostringstream;
#endif


namespace kjmutil {
	/* エラー情報を扱うクラス

	   [使用例]
	   kjmutil::err lastError;
	   _tprintf(_T("error %d: %s\n"), lastError.get_number(), lastError.get_description().c_str());
	*/
	class err {
	public:
		/* デフォルトコンストラクタ
		   オブジェクト作成時の GetLastError の値で初期化される。
		*/
		err() : m_dwErrCode(GetLastError()) {};

		/* コンストラクタ(エラー番号初期化付き) */
		err(DWORD dwErrCode) : m_dwErrCode(dwErrCode) {};

		/* デストラクタ */
		~err() {};

		/* エラー番号の取得 */
		DWORD get_number() { return m_dwErrCode; };

		/* エラー番号の設定 */
		void set_number(DWORD dwErrCode) { m_dwErrCode = dwErrCode; };

		/* エラーの説明を取得
		   FormatMessage 関数で取得できるエラーメッセージです。
		   通常、最後に付く改行を削除してあります。
		*/
		_tstring get_description();

	private:
		DWORD m_dwErrCode;
	};

	
	/* RECT を扱うクラス */
	class rect : public RECT {
	public:
		/* デフォルトコンストラクタ */
		rect() { SetRectEmpty(this); };

		/* デストラクタ */
		~rect() {};

		/* スクリーン座標をクライアント座標に変換 */
		rect& screen_to_client(HWND hwnd);

		/* 矩形をオフセットする */
		rect& offset(int dx, int dy);

		/* 幅 */
		int get_width() const { return right - left; };

		/* 高さ */
		int get_height() const { return bottom - top; };

		/* ウィンドウのクライアント座標を取得します */
		static rect get_client_rect(HWND hwnd);

		/* ウィンドウの座標を取得します */
		static rect get_window_rect(HWND hwnd);

		/* 指定された 2 つの長方形を完全に含む、最小の長方形を作成します */
		static rect union_rect(const RECT* lprcSrc1, const RECT* lprcSrc2);
	};


	/* 文字列の最後の改行を取り除く */
	void chomp(_tstring& str);

	/* kjmutil::rectを引数にとるMoveWindow関数 */
	BOOL move_window(HWND hWnd, const kjmutil::rect& rc, BOOL bRepaint);

	/****************************************************************************
	  lpExistingFileNameのファイルをlpNewFileNameにコピーします。コピー失敗時には
	  指定回数分リトライを行います。

	  <パラメータ>
	  lpExistingFileName  コピー元
	  lpNewFileName       コピー先
	  bFailIfExists       上書きを行うか(FALSEで上書きを行う)
	  retry               再試行を何回行うか
	  wait                再試行を行うまで何秒待つのか

	  <戻り値>
	  コピーに成功したときTRUE、コピーに失敗したときFALSEを返す。
	****************************************************************************/
	BOOL IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait);
};
