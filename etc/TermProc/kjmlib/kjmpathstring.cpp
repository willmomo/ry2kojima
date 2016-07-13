//---------------------------------------------------------------------
// kjm::PathString の実装ファイル
//---------------------------------------------------------------------
#include "kjmlib.h"
#include <shlwapi.h>


#if !defined(UNICODE)
#include <mbctype.h>
#endif


//---------------------------------------------------------------------
// \\?\ \\?\UNC を追加した文字列を返す
//---------------------------------------------------------------------
kjm::_tstring kjm::pathString::GetOverMaxPath() {
	kjm::_tstring result;

	if (PathIsUNC(m_str.c_str())) {
		result = _T("\\\\?\\UNC\\") + kjm::_tstring(&m_str[2]);
	} else {
		result = _T("\\\\?\\") + m_str;
	}

	return result;
}


//---------------------------------------------------------------------
// パス文字列の後ろに '\' を追加する
//---------------------------------------------------------------------
kjm::pathString& kjm::pathString::AddBackslash() {
	// 空文字列の時は、何もしない
	if (m_str.empty()) {
		return *this;
	}

	// 1文字の時は、'\' と比較して単純に追加処理を行う
	if (m_str.length() == 1) {
		if (m_str[0] != '\\') {
			m_str += '\\';
		}
		return *this;
	}

	// 2文字以上の時
#if defined(UNICODE)
	// UNICODEの時は、単純に最後の一文字をチェックすればよい
	if (m_str[m_str.length() - 1] != '\\') {
		m_str += '\\';
	}
#else
	if (_ismbstrail((unsigned char*)&m_str[0], (unsigned char*)&m_str[m_str.length() - 1]) != 0) {
		// 最後の一文字が、マルチバイトの後続バイトのときは、有無を言わさず '\' をくっつけても良い
		m_str += '\\';
	} else if (m_str[m_str.length() - 1] != '\\') {
		// 最後の一文字が、マルチバイトの後続バイトでなければ、1バイト文字なので、
		// '\' でなければ、'\' をくっつける。
		m_str += '\\';
	}
#endif

	return *this;
}


//---------------------------------------------------------------------
// パス文字列の結合
//---------------------------------------------------------------------
kjm::pathString& kjm::pathString::Append(const kjm::_tstring& pMore) {
	// まず、連結のための '\' を追加する。
	AddBackslash();

	// 次に与えられた文字列をくっつける
	m_str += pMore;

	return *this;
}
