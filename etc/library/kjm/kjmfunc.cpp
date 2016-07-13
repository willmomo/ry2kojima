/*-------------------------------------------------------------------
    KJMFUNC.CPP -- ユーティリティ的関数
	               (c) Ryouta Kojima, 2004
  -------------------------------------------------------------------*/

#include "kjmfunc.h"

namespace kjm
{
	/*---------------------------------------------------------------
	    日付を進める(戻す) ... time_t型バージョン
	    
	    tSrc    変換前日付
	    nDays   進める(戻す)日数
	    tDst    変換後日付を保存するバッファへのポインタ
	-----------------------------------------------------------------*/
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

	/*---------------------------------------------------------------
	    日付を進める(戻す) ... tm型バージョン
	    
	    tSrc    変換前日付
	    nDays   進める(戻す)日数
	    tDst    変換後日付を保存するバッファへのポインタ
	-----------------------------------------------------------------*/
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

	/*---------------------------------------------------------------
	    日付を進める(戻す) ... "YYYYMMDD"バージョン

	    tSrc    変換前日付
	            文字列で"YYYYMMDD"の形になっているもの
	    nDays   進める(戻す)日数
	    tDst    変換後日付を保存するバッファへのポインタ
	            文字列で"YYYYMMDD"の形になります
	    nSize   nDstの文字数(countofマクロを使用してください)
	-----------------------------------------------------------------*/
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

	/*---------------------------------------------------------------
	    末尾の改行を取り除く
	    
	    lpBuffer    改行を取り除く文字列
	                バッファの中身を直接変更します
	  ---------------------------------------------------------------*/
	LPTSTR Chomp(LPTSTR lpBuffer)
	{
		for (int i = _tcslen(lpBuffer) - 1;
			i >= 0 && (lpBuffer[i] == TEXT('\r') || lpBuffer[i] == TEXT('\n')); i--) {

			lpBuffer[i] = TEXT('\0');
		}

		return lpBuffer;
	}

	/*---------------------------------------------------------------
		システムエラーを文字列にする
		
		dwError    システムエラーコード(GetLastErrorの戻り値)
		lpBuffer   エラーメッセージを保存するバッファへのポインタ
		nSize      lpBufferの文字数(countofマクロを使用してください)
	  ---------------------------------------------------------------*/
	DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize)
	{
		DWORD dwRet;
		
		dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
		Chomp(lpBuffer);

		return dwRet;
	}

	/*---------------------------------------------------------------
		ビットフィールド745型の日付を文字列に変換する
		745型とは、{yyyy yyym mmmd dddd}のビットフィールドで、
		年の開始日は1910から。月、日の開始日は1から。
		
		YMD745    日付をあらわすビットフィールド値
		lpBuffer  変換後の文字列を格納するバッファ(最低11文字分)
		          変換後の文字列は、"YYYY/MM/DD"の形になります
		nSize     lpBufferの文字数(countofマクロを使用してください)
	  ---------------------------------------------------------------*/
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
};
