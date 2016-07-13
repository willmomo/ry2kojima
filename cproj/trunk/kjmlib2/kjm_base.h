//
// kjmlib の基本となる定義を書く場所
//

#pragma once

#include <windows.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <sstream>

namespace kjm {
	typedef std::basic_string<TCHAR>	_tstring;
	typedef std::vector<_tstring>	_tstrings;
	typedef std::basic_ostringstream<TCHAR>	_otstringstream;

	//
	// m_doFree が true の時は free できる領域に文字列が確保されているという事だが、
	// 実際には未実装で、常に false に設定される。
	// 
	template<class T>
	class basic_exception {
	public:
		basic_exception() : m_what(NULL), m_doFree(false) {}
		basic_exception(const T *const& _what) : m_what(_what), m_doFree(false) {}
		basic_exception(const T *const& _what, int) : m_what(_what), m_doFree(false) {}
		basic_exception(const _tstring& _what) : m_what(_what.c_str()), m_doFree(false) {}
		basic_exception(const _tstring& _what, int) : m_what(_what.c_str()), m_doFree(false) {}
		basic_exception(const basic_exception& rhs) {
			*this = rhs;
		}
		basic_exception& operator=(const basic_exception& rhs) {
			if (this != &rhs) {
				dispose();
				if (m_doFree) {
					// copy_what(); ただし、実装予定なし
				} else {
					m_what = rhs.m_what;
				}
			}
			return *this;
		}
		virtual ~basic_exception() {
			dispose();
		}
		virtual const T *what() const {
			return m_what;
		}
	protected:
		virtual void dispose() {
			if (m_doFree) free(const_cast<T*>(m_what));
			m_what = NULL;
			m_doFree = false;
		}
	private:
		const T* m_what;
		bool m_doFree;
	};

	typedef basic_exception<TCHAR>	_texception;

	// エラーコードを保持できる例外

	class error_exception : public _texception {
	public:
		error_exception(DWORD error) : m_error(error), _texception() {}
		error_exception(DWORD error, const TCHAR* _what) : m_error(error), _texception(_what) {}
		error_exception(DWORD error, const _tstring& _what) : m_error(error), _texception(_what) {}
		error_exception(const error_exception& rhs) {
			*this = rhs;
		}
		error_exception& operator=(const error_exception& rhs) {
			if (this != &rhs) {
				_texception::operator =(rhs);
				m_error = rhs.m_error;
			}
			return *this;
		}
		virtual ~error_exception() {
			dispose();
		}
		DWORD error() {
			return m_error;
		}
	protected:
		virtual void dispose() {
			_texception::dispose();
			m_error = 0;
		}
	private:
		DWORD m_error;
	};
};
