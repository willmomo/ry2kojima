#include <windows.h>
#include <stdio.h>
#include <lm.h>

#define FMTSTR	"%14s: %s\n"
#define FMTNUM	"%14s: %d\n"

//---------------------------------------------------------------------
// [概要]
// ASCII文字列をUnicode文字列に変換
//
// [引数]
// pszSrc	変換元ASCII文字列へのポインタ
//
// [戻り値]
// 変換後のUnicode文字列へのポインタ。変換できない場合は、NULLを返す。
//
// [備考]
// 使用後は、free(戻り値)する必要あり。
//---------------------------------------------------------------------

LPWSTR StrToWStr(LPCSTR pszSrc)
{
	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nSizeNeeded == 0) {
		return NULL;
	}

	// バッファを確保
	LPWSTR pwsz = (LPWSTR)malloc(sizeof(WCHAR) * nSizeNeeded);
	if (pwsz == NULL) {
		return NULL;
	}

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return NULL;
	}

	return pwsz;
}

//---------------------------------------------------------------------
// [概要]
// Unicode文字列をASCII文字列に変換
//
// [引数]
// pwszSrc	変換元Unicode文字列へのポインタ
//
// [戻り値]
// 変換後のASCII文字列へのポインタ。変換できない場合は、NULLを返す。
//
// [備考]
// 使用後は、free(戻り値)する必要あり。
//---------------------------------------------------------------------

LPSTR WStrToStr(LPCWSTR pwszSrc)
{
	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0) {
		return NULL;
	}

	// バッファを確保
	LPSTR psz = (LPSTR)malloc(sizeof(CHAR) * nSizeNeeded);
	if (psz == NULL) {
		return NULL;
	}

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return NULL;
	}

	return psz;
}

//---------------------------------------------------------------------
// [概要]
// shi502_typeに対応した文字列を返す
//---------------------------------------------------------------------

static const char* shi502_type_to_string(DWORD shi502_type)
{
	static char share_work[512];

	switch (shi502_type & ~STYPE_SPECIAL) {
	case STYPE_DISKTREE:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "ディスク ドライブ (管理共有)");
		} else {
			strcpy(share_work, "ディスク ドライブ");
		}
		break;
	case STYPE_PRINTQ:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "プリントキュー (管理共有)");
		} else {
			strcpy(share_work, "プリントキュー");
		}
		break;
	case STYPE_DEVICE:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "通信デバイス (管理共有)");
		} else {
			strcpy(share_work, "通信デバイス");
		}
		break;
	case STYPE_IPC:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "IPC (管理共有)");
		} else {
			strcpy(share_work, "IPC");
		}
		break;
	default:
		sprintf(share_work, "(不明: 0x%08X)", shi502_type);
		break;
	}

	return share_work;
}

//---------------------------------------------------------------------
// [概要]
// プログラム・スタート
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	// 引数チェック
	if (argc != 3) {
		printf("error: NetShareGetInfo <サーバー名> <共有名>\n");
		return -1;
	}

	LPWSTR pwszServer = StrToWStr(argv[1]);	// サーバー名をUnicodeに変換
	if (!pwszServer) {
		printf("error: サーバー名がUnicodeに変換できません\n");
		return -1;
	}

	LPWSTR pwszShared = StrToWStr(argv[2]);	// 共有名をUnicodeに変換
	if (!pwszShared) {
		printf("error: 共有名がUnicodeに変換できません。\n");
		free(pwszServer);
		return -1;
	}

	SHARE_INFO_502* pInfo;
	NET_API_STATUS ret;

	ret = NetShareGetInfo((LPTSTR)pwszServer, (LPTSTR)pwszShared, 502, (LPBYTE*)&pInfo);
	if (ret == NERR_Success) {
		LPSTR p;
		
		p = WStrToStr((LPCWSTR)pInfo->shi502_netname);
		printf(FMTSTR, "共有フォルダ", p);
		free(p);

		printf(FMTSTR, "タイプ", shi502_type_to_string(pInfo->shi502_type));

		p = WStrToStr((LPCWSTR)pInfo->shi502_remark);
		printf(FMTSTR, "コメント", p);
		free(p);

		if (pInfo->shi502_max_uses == -1) {
			printf(FMTSTR, "ユーザー数制限", "無制限");
		} else {
			printf(FMTNUM, "ユーザー数制限", pInfo->shi502_max_uses);
		}

		printf(FMTNUM, "接続数", pInfo->shi502_current_uses);

		p = WStrToStr((LPCWSTR)pInfo->shi502_path);
		printf(FMTSTR, "共有パス", p);
		free(p);

		NetApiBufferFree(pInfo);
	} else {
		printf("error %d: NetShareGetInfoでエラーです。\n", ret);
	}

	// 後始末
	free(pwszServer);
	free(pwszShared);

	return 0;
}
