//---------------------------------------------------------------------
// use_date.cpp
//   VBの日付型、文字列型を処理するDLL関数のサンプル
// 作成者
//   小島良太
//---------------------------------------------------------------------
// 1. Date型を引数で受けてDOSDATETIMEのLongに変換して戻す
// 2. DOSDATETIMEのLongを引数で受けて、Date型に変換して戻す
// 3. Date型をString型に変換して戻す
// 4. Date型を引数で渡されたStringを書き換える

#include <windows.h>
#include <comdef.h>

//---------------------------------------------------------------------
// [概要]
//   VBの日付をDOSの日付に変換
// [引数]
//   vbDate	日付型(Date型)の日付
// [戻り値]
//   DOSDATETIME型(u_longのビットフィールド)に変換した値
// [機能説明]
//   VBのDate型を引数で受け取る例
//   VBでの定義
//     Declare Function VbDate2DosDate(ByVal vbDate As Date) As Long
//---------------------------------------------------------------------
__declspec(dllexport) long __stdcall VbDate2DosDate(DATE vbDate)
{
	UDATE udate;
	u_long dwFlags = 0;

	memset(&udate, 0, sizeof(udate));

	// VBの日付型(Date)をUDATE構造体に変換する
	HRESULT hres = VarUdateFromDate(vbDate, dwFlags, &udate);
	if (hres != S_OK) {
		return 0xffffffff;
	}

	return ((udate.st.wYear - 1980) << 25) |
		(udate.st.wMonth << 21) |
		(udate.st.wDay << 16) |
		(udate.st.wHour << 11) |
		(udate.st.wMinute << 5) |
		(udate.st.wSecond / 2);
}

//---------------------------------------------------------------------
// [概要]
//   DOSの日付をVBの日付に変換
// [引数]
//   dosDate	DOSDATETIME型の日付
// [戻り値]
//   日付型(Date型)に変換した値
// [機能説明]
//   VBのDate型を作成する例
//   VBでの定義
//     Declare Function DosDate2VbDate(ByVal dosDate As Long) As Date
//---------------------------------------------------------------------
__declspec(dllexport) DATE __stdcall DosDate2VbDate(u_long dosDate)
{
	UDATE udate;
	u_long dwFlags = 0;
	DATE date;

	memset(&udate, 0, sizeof(udate));
	udate.st.wYear = (u_short)(((dosDate >> 25) & 0x7f) + 1980);
	udate.st.wMonth = (u_short)((dosDate >> 21) & 0x0f);
	udate.st.wDay = (u_short)((dosDate >> 16) & 0x1f);
	udate.st.wHour = (u_short)((dosDate >> 11) & 0x1f);
	udate.st.wMinute = (u_short)((dosDate >> 5) & 0x3f);
	udate.st.wSecond = (u_short)((dosDate & 0x1f) * 2);

	// UDATE構造体からVBの日付型(Date型)を作成する
	HRESULT hres = VarDateFromUdate(&udate, dwFlags, &date);
	if (hres != S_OK) {
		return 0.0;
	}

	return date;
}

//---------------------------------------------------------------------
// [概要]
//   VBの日付を"yyyy/mm/dd hh:nn:ss"文字列で返す
// [引数]
//   vbDate	日付型(Date型)の日付
// [戻り値]
//   文字列型(String型)に変換した日付
// [機能説明]
//   文字列型(String)を戻す関数の例
//   VBでの定義
//     Declare Function VbDate2String(ByVal vbDate As Date) As String
//---------------------------------------------------------------------
__declspec(dllexport) BSTR __stdcall VbDate2String(DATE vbDate)
{
	UDATE udate;
	u_long dwFlags = 0;

	memset(&udate, 0, sizeof(udate));

	// VBの日付型(Date)をUDATE構造体に変換する
	HRESULT hres = VarUdateFromDate(vbDate, dwFlags, &udate);
	if (hres != S_OK) {
		return NULL;
	}

	char temp[64];
	int len = wsprintf(temp, "%04d/%02d/%02d %02d:%02d:%02d",
		udate.st.wYear, udate.st.wMonth, udate.st.wDay,
		udate.st.wHour, udate.st.wMinute, udate.st.wSecond);

	// return SysAllocString(temp);でいけそうな気がするが、
	// SysAllocString, SysAllocStringLen, SysReAllocString, SysReAllocStringLen
	// 上記4つの関数は、VBに制御がわたったとき、正常に処理できない。
	// おそらく、文字コードの自動変換(Unicode->Ascii->Unicode)の関係でよろしくないと
	// 思われり。唯一正常に動くSysAllocStringByteLenを使用。

	return SysAllocStringByteLen(temp, len);
}

//---------------------------------------------------------------------
// [概要]
//   VBの日付を"yyyy/mm/dd" "hh:nn:ss"二つの文字列で返す。
// [引数]
//   vbDate	日付型(Date型)の日付
// [戻り値]
//   0:		正常終了
//   -1:	異常終了(変換失敗)
// [機能説明]
//   文字列型(String)を書き換える例
//   VBでの定義
//   Declare Function VbDate2String2(ByVal vbDate As Date,
//     ByVal strYMD As String, ByVal strHMS As String) As Long
//---------------------------------------------------------------------
__declspec(dllexport) long __stdcall VbDate2String2(DATE vbDate, BSTR* ymd, BSTR* hms)
{
	UDATE udate;
	u_long dwFlags = 0;

	memset(&udate, 0, sizeof(udate));

	// VBの日付型(Date)をUDATE構造体に変換する
	HRESULT hres = VarUdateFromDate(vbDate, dwFlags, &udate);
	if (hres != S_OK) {
		return -1;
	}

	// SysReSysAllocString(temp);でいけそうな気がするが、
	// SysAllocString, SysAllocStringLen, SysReAllocString, SysReAllocStringLen
	// 上記4つの関数は、VBに制御がわたったとき、正常に処理できない。
	// おそらく、文字コードの自動変換(Unicode->Ascii->Unicode)の関係でよろしくないと
	// 思われり。唯一正常に動くSysAllocStringByteLenを使用するため、
	// SysFreeString()してからSysAllocStringByteLen()で再確保する

	char temp[64];
	int len = wsprintf(temp, "%04d/%02d/%02d",
		udate.st.wYear, udate.st.wMonth, udate.st.wDay);
	SysFreeString(*ymd);
	*ymd = SysAllocStringByteLen(temp, len);

	len = wsprintf(temp, "%02d:%02d:%02d",
		udate.st.wHour, udate.st.wMinute, udate.st.wSecond);
	SysFreeString(*hms);
	*hms = SysAllocStringByteLen(temp, len);

	return 0;
}

//---------------------------------------------------------------------
// [概要]
//   DLLメイン
// [引数]
//   hinstDLL		handle to DLL module
//   fdwReason		reason for calling fuction
//   lpvReserved	reserved
// [戻り値]
//   0:		正常終了
//   -1:	異常終了(変換失敗)
// [機能説明]
//   DLLのエントリー
//---------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
