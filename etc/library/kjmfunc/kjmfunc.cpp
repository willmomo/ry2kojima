//---------------------------------------------------------------------
// KJMFUNC.CPP -- ユーティリティ的関数
//                (c) Ryouta Kojima, 2004
//---------------------------------------------------------------------

#include "kjmfunc.h"

namespace kjm
{
	//-----------------------------------------------------------------
	// [概要]
	// KTimeコンストラクタ
	//
	// [備考]
	// FILETIME型を使ったコンストラクタ
	//-----------------------------------------------------------------

	KTime::KTime(FILETIME t)
	{
		FILETIME lft;
		FileTimeToLocalFileTime(&t, &lft);
		SYSTEMTIME st;
		FileTimeToSystemTime(&lft, &st);

		tm stm;
		memset(&stm, 0, sizeof(stm));
		stm.tm_year = st.wYear - 1900;
		stm.tm_mon = st.wMonth - 1;
		stm.tm_mday = st.wDay;
		stm.tm_hour = st.wHour;
		stm.tm_min = st.wMinute;
		stm.tm_sec = st.wSecond;

		m_time = mktime(&stm);
	}

	//-----------------------------------------------------------------
	// [概要]
	// フォルダを削除します。
	//-----------------------------------------------------------------

	BOOL DeleteFolder(LPCTSTR lpszPathName)
	{
		BOOL bRet = TRUE;

		// 検索用の文字列を作成
		std::vector<TCHAR> szFileSpec(MAX_PATH);
		lstrcpy(szFileSpec.begin(), lpszPathName);
		PathAppend(szFileSpec.begin(), _T("*.*"));

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(szFileSpec.begin(), &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// "." or ".." は、スキップ
				if (kjm::IsDots(data.cFileName))	continue;

				std::vector<TCHAR> szFullName(MAX_PATH);
				lstrcpy(szFullName.begin(), lpszPathName);
				PathAppend(szFullName.begin(), data.cFileName);

				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if ((bRet = DeleteFolder(szFullName.begin())) == FALSE) {
						break;
					}
				} else {
					if ((bRet = DeleteFile(szFullName.begin())) == FALSE) {
						break;
					}
				}
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);

			// 最後にフォルダを削除(上で削除に失敗していると、ここも常に失敗)
			bRet = RemoveDirectory(lpszPathName);
		}

		return bRet;
	}

	//-----------------------------------------------------------------
	// [概要]
	// 古いファイル/フォルダを削除
	//
	// [備考]
	// lpszPathNameで指定したフォルダの中のファイル/フォルダで、最終更
	// 新日時が、nDaysよりも古いものを削除する。
	//
	// lpszPathName以下のフォルダが削除対象になった場合、その下のファイ
	// ル/フォルダの最終更新日時は、チェックされない。
	//-----------------------------------------------------------------

	BOOL DeleteOldFile(LPCTSTR lpszPathName, int nDays)
	{
		BOOL bRet = TRUE;

		// 検索用の文字列を作成
		std::vector<TCHAR> szFileSpec(MAX_PATH);
		lstrcpy(szFileSpec.begin(), lpszPathName);
		PathAppend(szFileSpec.begin(), _T("*.*"));

		// 現在日時を取得
		kjm::KTime now = kjm::KTime::GetCurrentTime();

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(szFileSpec.begin(), &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// "." or ".." は、スキップ
				if (kjm::IsDots(data.cFileName))	continue;

				// ファイルの日付が指定日数より古いときは削除する
				kjm::KTime ft(data.ftLastWriteTime);
				if ((now - ft).GetDays() > nDays) {
					std::vector<TCHAR> szFullName(MAX_PATH);
					lstrcpy(szFullName.begin(), lpszPathName);
					PathAppend(szFullName.begin(), data.cFileName);

					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						if ((bRet = DeleteFolder(szFullName.begin())) == FALSE) {
							break;
						}
					} else {
						if ((bRet = DeleteFile(szFullName.begin())) == FALSE) {
							break;
						}
					}
				}
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}

		return bRet;
	}

	//-----------------------------------------------------------------
	// 日付を進める(戻す) ... time_t型バージョン
	//
	// tSrc		変換前日付
	// nDays	進める(戻す)日数
	// tDst		変換後日付を保存するバッファへのポインタ
	//-----------------------------------------------------------------
	BOOL DateAdd(time_t tSrc, int nDays, time_t* tDst)
	{
		// time_t型の日付をtm型に変換
		tm* ptm = localtime(&tSrc);
		if (ptm == NULL) {
			return FALSE;
		}

		tm tmBuf = *ptm;
		tmBuf.tm_mday += nDays;		// nDays進める

		*tDst = mktime(&tmBuf);		// 日付を作る

		return (*tDst == (time_t)-1) ? FALSE : TRUE;
	}

	//-----------------------------------------------------------------
	// 日付を進める(戻す) ... tm型バージョン
	//
	// tSrc		変換前日付
	// nDays	進める(戻す)日数
	// tDst		変換後日付を保存するバッファへのポインタ
	//-----------------------------------------------------------------
	BOOL DateAdd(const tm* tSrc, int nDays, tm* tDst)
	{
		tm tmBuf = *tSrc;
		tmBuf.tm_mday += nDays;		// nDays進める

		time_t t = mktime(&tmBuf);		// 日付を作る
		if (t == (time_t)-1) {
			return FALSE;
		}

		tm* ptm = localtime(&t);
		if (ptm == NULL) {
			return FALSE;
		}

		*tDst = *ptm;

		return TRUE;
	}

	//-----------------------------------------------------------------
	// 日付を進める(戻す) ... "YYYYMMDD"バージョン
	//
	// tSrc		変換前日付
	//			文字列で"YYYYMMDD"の形になっているもの
	// nDays	進める(戻す)日数
	// tDst		変換後日付を保存するバッファへのポインタ
	//			文字列で"YYYYMMDD"の形になります
	// nSize	nDstの文字数(countofマクロを使用してください)
	//-----------------------------------------------------------------
	BOOL DateAdd(LPCTSTR tSrc, int nDays, LPTSTR tDst, DWORD nSize)
	{
		int y, m, d;
		tm tmBuf;
		memset(&tmBuf, 0, sizeof(tmBuf));
		_stscanf(tSrc, TEXT("%04d%02d%02d"), &y, &m, &d);
		tmBuf.tm_year = y - 1900;
		tmBuf.tm_mon = m - 1;
		tmBuf.tm_mday = d + nDays;

		time_t t = mktime(&tmBuf);		// 日付を作る
		if (t == (time_t)-1) {
			return FALSE;
		}

		tm* ptm = localtime(&t);
		if (ptm == NULL) {
			return FALSE;
		}

		_sntprintf(tDst, nSize, TEXT("%04d%02d%02d"), ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);

		return TRUE;
	}

	//---------------------------------------------------------------
	// 末尾の改行を取り除く
	//
	// lpBuffer	改行を取り除く文字列
	//			バッファの中身を直接変更します。
	//---------------------------------------------------------------
	LPTSTR Chomp(LPTSTR lpBuffer)
	{
		for (int i = _tcslen(lpBuffer) - 1;
			i >= 0 && (lpBuffer[i] == TEXT('\r') || lpBuffer[i] == TEXT('\n')); i--) {

			lpBuffer[i] = TEXT('\0');
		}

		return lpBuffer;
	}

	//-----------------------------------------------------------------
	// システムエラーを文字列にする
	//
	// dwError	システムエラーコード(GetLastErrorの戻り値)
	// lpBuffer	エラーメッセージを保存するバッファへのポインタ
	// nSize	lpBufferの文字数(countofマクロを使用してください)
	//-----------------------------------------------------------------
	DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize)
	{
		DWORD dwRet;
		
		dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
		Chomp(lpBuffer);

		return dwRet;
	}

	//-----------------------------------------------------------------
	// [概要]
	// 指定された文字列が "." or ".." かを調べる
	//
	// [戻り値]
	// 指定された文字列が "." or ".." のとき true、違うとき falseを戻す
	//-----------------------------------------------------------------

	bool IsDots(LPCTSTR psz)
	{
		return (lstrcmp(psz, _T(".")) == 0) || (lstrcmp(psz, _T("..")) == 0);
	}

	//-----------------------------------------------------------------
	// ビットフィールド745型の日付を文字列に変換する
	// 745型とは、{yyyy yyym mmmd dddd}のビットフィールドで、
	// 年の開始日は1910から。月、日の開始日は1から。
	//
	// YMD745	日付をあらわすビットフィールド値
	// lpBuffer	変換後の文字列を格納するバッファ(最低11文字分)
	//			変換後の文字列は、"YYYY/MM/DD"の形になります。
	// nSize	lpBufferの文字数(countofマクロを使用してください)
	//-----------------------------------------------------------------
	LPTSTR ToStringYMD745(WORD YMD745, LPTSTR lpBuffer, DWORD nSize)
	{
		struct _ymd745 {
			unsigned d:5;
			unsigned m:4;
			unsigned y:7;
		} *p = reinterpret_cast<struct _ymd745*>(&YMD745);

		_sntprintf(lpBuffer, nSize, TEXT("%04d/%02d/%02d"), p->y + 1910, p->m, p->d);

		return lpBuffer;
	}

	//---------------------------------------------------------------
	// ビットフィールド745型の日付をtime_t型に変換する
	// 745型とは、{yyyy yyym mmmd dddd}のビットフィールドで、
	// 年の開始日は1910から。月、日の開始日は1から。
	//
	// YMD745	日付をあらわすビットフィールド値
	//
	// [戻り値]
	// エラーのときは、time_t型にキャストした-1を返します。
	// (mktime関数のエラーです)
	//---------------------------------------------------------------
	time_t ToTimeYMD745(WORD YMD745)
	{
		struct _ymd745 {
			unsigned d:5;
			unsigned m:4;
			unsigned y:7;
		} *p = reinterpret_cast<struct _ymd745*>(&YMD745);

		tm tmBuf;

		memset(&tmBuf, 0, sizeof(tmBuf));
		tmBuf.tm_year = (p->y + 1910) - 1900;
		tmBuf.tm_mon = p->m - 1;
		tmBuf.tm_mday = p->d;

		return mktime(&tmBuf);
	}

	//-----------------------------------------------------------------
	// ps_tm型の日付をtime_t型に変換する
	// ps_tm型とは、{yyyy yyym mmmd dddd hhhh hnnn nnns ssss}のビット
	// フィールドで、年の開始日は1910から。月、日の開始日は1から。
	// 秒は、1/2。
	//
	// pstm		日付をあらわすビットフィールド値
	//
	// [戻り値]
	// エラーのときは、time_t型にキャストした-1を返します。
	// (mktime関数のエラーです)
	//-----------------------------------------------------------------
	time_t ToTimePSTM(DWORD pstm)
	{
		struct _pstm {
			unsigned s:5;
			unsigned n:6;
			unsigned h:5;
			unsigned d:5;
			unsigned m:4;
			unsigned y:7;
		} *p = reinterpret_cast<struct _pstm*>(&pstm);

		tm tmBuf;

		memset(&tmBuf, 0, sizeof(tmBuf));
		tmBuf.tm_year = (p->y + 1910) - 1900;
		tmBuf.tm_mon = p->m - 1;
		tmBuf.tm_mday = p->d;
		tmBuf.tm_hour = p->h;
		tmBuf.tm_min = p->n;
		tmBuf.tm_sec = p->s * 2;

		return mktime(&tmBuf);
	}
};
