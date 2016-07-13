#pragma once

#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


//---------------------------------------------------------------------
// とても簡単な配列テンプレート
//
// [備考]
// メモリの確保にremallocを使うので、Tは基本型のみで使用してください。
//---------------------------------------------------------------------
template<class T> class array {
public:
	//-----------------------------------------------------------------
	// デフォルトコンストラクタ
	//-----------------------------------------------------------------
	array() : m_pt(NULL), m_size(0) {};

	//-----------------------------------------------------------------
	// メモリ確保付きコンストラクタ
	//-----------------------------------------------------------------
	array(int size) : m_pt(NULL), m_size(0) { resize(size); };

	//-----------------------------------------------------------------
	// デストラクタ
	//-----------------------------------------------------------------
	~array() { resize(0); };

	//-----------------------------------------------------------------
	// メモリの(再)割り当て
	//
	// [引数]
	// size  新しい要素数
	//
	// [備考]
	// sizeを0にすると、解放処理になります。
	//-----------------------------------------------------------------
	bool resize(int size) {
		bool result = true;

		if (size == 0) {
			free(m_pt);
			m_pt = NULL;
			m_size = 0;
		} else {
			m_pt = (T*)realloc(m_pt, sizeof(T) * size);
			m_size = size;
			if (m_pt == NULL) {
				m_size = 0;
				result = false;
			}
		}

		return result;
	};

	//-----------------------------------------------------------------
	// 配列の要素数を返す
	//-----------------------------------------------------------------
	int size() const { return m_size; };

	//-----------------------------------------------------------------
	// 配列の先頭アドレスを取得
	//-----------------------------------------------------------------
	T* top() { return m_pt; };
	const T* top() const { return m_pt; };
	operator T*() { return m_pt; };
	operator const T* () const { return m_pt; };

private:
	T* m_pt;		// 配列の先頭アドレス
	int m_size;		// 要素数
};


bool remove_folderW(const wchar_t* pszPath);
bool remove_folderA(const char* pszPath);

#if defined(UNICODE)
#define remove_folder	remove_folderW
#else
#define remove_folder	remove_folderA
#endif
