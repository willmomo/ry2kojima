//---------------------------------------------------------------------
// リンクに以下のファイルを追加します
// netapi32.lib shlwapi.lib
//---------------------------------------------------------------------

#include <windows.h>
#include <lm.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string>

//---------------------------------------------------------------------
// [概要]
// ASCII文字列をUnicode文字列に変換
//
// [引数]
// pszSrc	変換元ASCII文字列へのポインタ
//
// [戻り値]
// 変換後のUnicode文字列へのポインタ。変換できない場合は、空文字列を返す。
//---------------------------------------------------------------------

std::wstring StrToWStr(LPCSTR pszSrc)
{
	std::wstring wstr;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nSizeNeeded == 0) {
		return wstr;
	}

	// バッファを確保
	LPWSTR pwsz = (LPWSTR)malloc(sizeof(WCHAR) * nSizeNeeded);
	if (pwsz == NULL) {
		return wstr;
	}

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr = pwsz;
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
// [概要]
// Unicode文字列をASCII文字列に変換
//
// [引数]
// pwszSrc	変換元Unicode文字列へのポインタ
//
// [戻り値]
// 変換後のASCII文字列へのポインタ。変換できない場合は、空文字列を返す。
//---------------------------------------------------------------------

std::string WStrToStr(LPCWSTR pwszSrc)
{
	std::string str;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0) {
		return str;
	}

	// バッファを確保
	LPSTR psz = (LPSTR)malloc(sizeof(CHAR) * nSizeNeeded);
	if (psz == NULL) {
		return str;
	}

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str = psz;
	free(psz);

	return str;
}

//---------------------------------------------------------------------
// [概要]
// 指定フォルダを作成する
//---------------------------------------------------------------------

int CreateFolder(const char* pszFolder)
{
	// ルートフォルダは、必ず存在する(ドライブが無いかもだけど...)
	if (PathIsRoot(pszFolder)) {
		return 0;

	// 指定されたフォルダは存在する?
	} else if (PathFileExists(pszFolder)) {
		return 0;

	} else {
		char work[MAX_PATH];
		lstrcpy(work, pszFolder);
		PathAppend(work, "..");

		// 親フォルダを(無ければ)作成する
		if (CreateFolder(work) == 0) {
			if (!CreateDirectory(pszFolder, NULL)) {
				return -1;
			}
		} else {
			return -1;
		}
	}

	return 0;
}

//---------------------------------------------------------------------
// [概要]
// 指定の共有フォルダと共有パスの存在を保証する
//---------------------------------------------------------------------

int GuaranteeSharedFolder(const char* pszSharedFolder, const char* pszSharedPath)
{
	int nRet = 0;

	// 共有フォルダ名をUnicodeに変換
	std::wstring wstrSharedFolder = StrToWStr(pszSharedFolder);
	if (wstrSharedFolder.empty()) {
		// TODO: 共有フォルダの指定が間違っているときの処理

		return -1;
	}

	// 共有パス名をUnicodeに変換
	std::wstring wstrSharedPath = StrToWStr(pszSharedPath);
	if (wstrSharedPath.empty()) {
		// TODO: 共有パスの指定が間違っているときの処理

		return -1;
	}

	SHARE_INFO_2* pInfo;
	NET_API_STATUS ret;

	// ローカルに有る、指定の共有フォルダ情報を取得
	ret = NetShareGetInfo(NULL, (LPTSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
	if (ret == NERR_Success) {
	
		// 共有フォルダが指定の共有パスを指しているか確認する
		if (lstrcmpi(WStrToStr((LPCWSTR)pInfo->shi2_path).c_str(), pszSharedPath) != 0) {
			// TODO: 共有フォルダが共有パスと違うことを警告
		}

		NetApiBufferFree(pInfo);

		nRet = 0;	// 正常終了
	} else if (ret == NERR_NetNameNotFound) {
		// 共有フォルダが存在しないときの処理

		// 指定の共有パスを(無ければ)作成する
		if (CreateFolder(pszSharedPath) < 0) {
			// TODO: 共有パスが作成できない時の処理

			return -1;
		}

		SHARE_INFO_2 info;
		info.shi2_netname = (LPTSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPTSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPTSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPTSTR)L"";

		DWORD parm_err = 0;
		ret = NetShareAdd(NULL, 2, (LPBYTE)&info, &parm_err);
		if (ret == NERR_Success) {
			nRet = 0;	// 正常終了
		} else {
			// TODO: 共有フォルダが作成できなかったときの処理

			nRet = -1;	// 異常終了
		}
	} else {
		// TODO: 共有フォルダの情報が取得できない時の処理
		
		nRet = -1;	// 異常終了
	}

	return nRet;
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("GuaranteeSharedFolder <共有フォルダ> <共有パス>\n");
		return -1;
	}

	GuaranteeSharedFolder(argv[1], argv[2]);

	return 0;
}
