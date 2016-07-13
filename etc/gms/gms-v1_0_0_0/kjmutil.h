/**
 * 各種ユーティリティ
 */

#pragma once

// 警告レベルを4に引き上げる
#pragma warning(push, 4)

// 警告 '引数が関数の本体で 1 度も参照されません。' を無効化する
#pragma warning(disable: 4100)

// ユーティリティが必要とするヘッダをインクルード
#include <windows.h>

// namespace kjm の定義
namespace kjm {
	
	/**
	 * Rectangle構造体(.NET風)
	 */

	struct Rectangle : public tagRECT
	{
		/**
		 * このRectanlge構造体の下端のy座標を取得します。
		 */

		int get_Bottom() {
			return tagRECT::bottom;
		};

		/**
		 * このRectangle構造体の高さを取得または設定します。
		 */

		int get_Height() {
			return tagRECT::bottom - tagRECT::top;
		};
		void set_Height(int value) {
			tagRECT::bottom = value - tagRECT::top;
		};

		/**
		 * このRectangleのすべての数値プロパティに値ゼロがあるかどうかテストします。
		 */

		bool get_IsEmpty() {
			return ::IsRectEmpty(this) != FALSE;
		};

		/**
		 * このRectangleの構造体の左端のx座標を取得します。
		 */

		int get_Left() {
			return tagRECT::left;
		};

		/**
		 * このRectangle構造体の右端のx座標を取得します。
		 */

		int get_Right() {
			return tagRECT::right;
		};

		/**
		 * このRectangle構造体の上端のy座標を取得します。
		 */

		int get_Top() {
			return tagRECT::top;
		};

		/**
		 * このRectangle構造体の幅を取得または設定します。
		 */

		int get_Width() {
			return tagRECT::right - tagRECT::left;
		};
		void set_Width(int value) {
			tagRECT::right = value - tagRECT::left;
		};

		/**
		 * このRectangle構造体の左上隅のx座標を取得または設定します。
		 */

		int get_X() {
			return tagRECT::left;
		};
		void set_X(int value) {
			tagRECT::left = value;
		};

		/**
		 * このRectangle構造体の左上隅のy座標を取得または設定します。
		 */

		int get_Y() {
			return tagRECT::top;
		};
		void set_Y(int value) {
			tagRECT::top = value;
		};

		/**
		 * このRectangleを指定の量だけ膨らませます。
		 * @param width 水平方向の膨張量
		 * @param height 垂直方向の膨張量
		 */

		void Inflate(int width, int height) {
			::InflateRect(this, width, height);
		};
	};

	// regfunc.cpp
	DWORD RegGetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPTSTR lpValueName, DWORD dwDefault);
	BOOL RegSetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData);
}
