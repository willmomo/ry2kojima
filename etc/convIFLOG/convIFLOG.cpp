#include <windows.h>
#include <stdio.h>

//---------------------------------------------------------------------
// グローバル変数

char g_szFname[MAX_PATH];	// 変換するファイル名

//---------------------------------------------------------------------
// [概要]
// 使用法の表示と終了
//
// [引数]
// int status	: 終了ステータス
//
// [戻り値]
// なし
//
// [説明]
// ヘルプを表示して終了します。引数はexit関数への引数
//
//---------------------------------------------------------------------

void usage(int status)
{
	puts("convIFLOG ver.2003.12.05 (r.kojima)");
	puts("");
	puts("使用法：convIFLOG <ファイル名>");
	puts("");
	puts("中継器の作成する 'IFLOG??.LOG'、サイズ <<<128>>> の電文を");
	puts("CSV形式で出力します。");
	puts("ファイルに保存する場合は、リダイレクトしてください。");

	exit(status);
}

//---------------------------------------------------------------------
// [概要]
// 引数チェック
//
// [引数]
// int argc		: 引数の数
// char** argv	: 引数の実態
//
// [戻り値]
// なし
//
// [説明]
// 引数が不適切な場合は、ヘルプを表示して終了
//
//---------------------------------------------------------------------

void chkarg(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			if (argv[i][1] == '?' && argv[i][2] == '\0') {
				usage(EXIT_SUCCESS);	// -?の時は、ヘルプを表示して正常終了
			} else {
				usage(EXIT_FAILURE);	// -?以外は、ヘルプを表示して異常終了
			}
		} else {
			if (g_szFname[0] != '\0') {
				usage(EXIT_FAILURE);	// すでにファイルは指定されている?
			} else {
				strcpy(g_szFname, argv[i]);
			}
		}
	}

	if (g_szFname[0] == '\0') {
		usage(EXIT_FAILURE);	// 最後までファイル名が未指定?
	}
}

//---------------------------------------------------------------------
// [概要]
// 行末の改行を削除
//
// [引数]
// char* buf	: 文字列
//
// [戻り値]
// なし
//
// [説明]
// 行末の改行 '\x0d' or '\x0a' をなくなるまで削除する。
//
//---------------------------------------------------------------------

void chomp(char* buf)
{
	for (int n = strlen(buf) - 1; n >= 0; n--) {
		if (buf[n] == '\x0d' || buf[n] == '\x0a') {
			buf[n] = '\0';
		}
	}
}

//---------------------------------------------------------------------
// [概要]
// プログラムスタート
//
// [引数]
// main関数のヘルプを参照
//
// [戻り値]
// main関数のヘルプを参照
//
// [説明]
// なし
//
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	char line[1024];
	int no = 1;

	chkarg(argc, argv);	// パラメータ解析

	printf("no,msg1,msg2,jhd_no,code3,choin0,choin1,choin2,cdhikisu,cdsu,update\n");
	FILE* fp = fopen(g_szFname, "r");
	if (fp) {
		while (fgets(line, sizeof(line), fp) != NULL) {
			chomp(line);	// 改行を取り除く
			if (strcmp("<<<128>>>", line) == 0) {

				// 128バイトの電文ログのみ処理する。
				// つねに、92 05 電文らしい。

				unsigned char buf[128];
				int idx = 0;

				// 続く8行分を処理する(1行16バイト * 8行 = 128バイト)
				for (int i = 0; i < 8; i++) {
					fgets(line, sizeof(line), fp);

					for (int c = 0; c < 16; c++) {
						buf[idx++] = (unsigned char)strtoul(&line[c*3], NULL, 16);
					}
				}

				printf("%d,%02X,%02X,%u,%d,%d,%d,%d,%d,%d,%c%c%c%c/%c%c/%c%c %c%c:%c%c:%c%c\n",
					no++,
					buf[0] & 0xff,
					buf[1] & 0xff,
					ntohl(*(long*)&buf[20]),
					
					buf[50] & 0xff,

					ntohl(*(long*)&buf[64]),
					ntohl(*(long*)&buf[68]),
					ntohl(*(long*)&buf[72]),

					ntohl(*(long*)&buf[88]),
					ntohl(*(long*)&buf[92]),

					buf[96],buf[97],buf[98],buf[99],
					buf[100],buf[101],buf[102],buf[103],
					
					buf[104],buf[105],buf[106],buf[107],buf[108],buf[109]);
			}
		}

		fclose(fp);
	}

	return 0;
}
