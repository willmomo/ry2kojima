//---------------------------------------------------------------------
// C++で使用できる、ユーティリティ関数(ver.0.2010.3.11)
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// INIファイルからすべてのセクション名を取得する。
//---------------------------------------------------------------------
int GetPrivateProfileSectionNames(std::vector<std::basic_string<TCHAR> >& names, LPCTSTR lpFileName) {
	std::vector<TCHAR> buf(512);
	do {
		if (GetPrivateProfileSectionNames(buf.begin(), buf.size(), lpFileName) != (buf.size() - 2)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);

	// \0 で区切りながら、names に追加していく。
	names.clear();
	for (int i = 0; buf[i] != '\0'; i += (lstrlen(&buf[i]) + 1)) {
		names.push_back(&buf[i]);
	}

	return names.size();
}


//---------------------------------------------------------------------
// INIファイルの値を取得する。
// デフォルト値を採用したかどうかの情報も取得できる。
//---------------------------------------------------------------------
std::basic_string<TCHAR> GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName, bool *pfUseDefault=NULL) {
	std::basic_string<TCHAR> pre_default = std::basic_string<TCHAR>(_T("pre_")) + lpDefault;

	std::vector<TCHAR> pre_buf(max(32, pre_default.length() * 2));
	do {
		if (GetPrivateProfileString(lpAppName, lpKeyName, pre_default.c_str(), pre_buf.begin(), pre_buf.size(), lpFileName) != (pre_buf.size() - 1)) {
			break;
		}
		pre_buf.resize(pre_buf.size() * 2);
	} while (1);

	std::vector<TCHAR> buf(pre_buf.size());
	do {
		if (GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf.begin(), buf.size(), lpFileName) != (buf.size() - 1)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);

	if (pfUseDefault) {
		*pfUseDefault = (lstrcmp(pre_buf.begin(), buf.begin()) != 0);
	}

	return buf.begin();
}


/**
	@brief	システム標準のエラーメッセージを作る
	@param error_code	GetLastError の戻り値
	@result	エラー文字列を返す。エラーメッセージが取得できないときは、空文字列を返す。
*/
std::string format_message(DWORD error_code) {
	std::string result;
	char* pBuffer = NULL;

	DWORD ret = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error_code,
		0,
		(LPTSTR)&pBuffer,
		0,
		NULL);

	// 関数の戻り値は見ずに、ポインタの有効性のみで判断する。
	if (pBuffer) {
		result = pBuffer;
		::LocalFree(pBuffer);
	}
	return result;
}


/**
	@brief	コンピュータ名をstd::stringで返す
	@return	コンピュータ名を返す。関数に失敗したときは、空文字列を返す。
*/

std::string get_computer_name() {
	char buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD nSize = sizeof(buffer);
	BOOL ret = ::GetComputerName(buffer, &nSize);
	return (ret) ? std::string(buffer) : std::string();
}


//---------------------------------------------------------------------
// 安全な sprintf 
//
// [引数]
// 一番目の引数が、vector<char>への参照になっている以外、
// sprintf関数と同じです。
//---------------------------------------------------------------------
int sprintf_vec(std::vector<char>& buf, const char* format, ...) {
	va_list marker;

	// バッファを指定されていないとき、256バイト割り当てておく
	if (buf.size() == 0) {
		buf.resize(256);
	}

	va_start(marker, format);
	while (1) {

		// vsnprintf で文字列を作成。
		// 不十分なバッファ上にデータを作成している間、ループを繰り返す。
		int n = _vsnprintf(buf.begin(), buf.size(), format, marker);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}
	va_end(marker);

	return strlen(buf.begin());
}
