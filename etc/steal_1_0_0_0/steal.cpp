/**********************************************************************

  stdin からの入力を表示しながらログに残すプログラム。

  <実行したいプログラム> | steal <出力ファイルのフルパス>

  出力ファイルのディレクトリがないときは、自動で作成します。
  出力ファイルのフルパス指定例:
  "%TEMP%\StealLog\StealLog%date:~0,4%%date:~5,2%%date:~8,2%.log"

  改行で終わらない出力や、入力を求める出力などは、期待通りに処理されま
  せん。

  なるべくシンプルに、無駄なコードを省いてあり、MBCSでのみ作成可能で
  す。

  History:
  v1.0.0.0
  新規作成

**********************************************************************/
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <stdio.h>


//---------------------------------------------------------------------
// フォルダの再帰的作成
//---------------------------------------------------------------------
bool create_directory(const char* pszPath) {
	if (PathFileExists(pszPath))	return true;
	if (PathIsRoot(pszPath))		return true;

	// 親ディレクトリの作成を試みる
	char pszParentPath[MAX_PATH];
	PathRemoveFileSpec(strcpy(pszParentPath, pszPath));
	if (create_directory(pszParentPath)) {
		if (CreateDirectory(pszPath, NULL)) {
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------
// 行末の改行 0x0d 0x0a を取り除く
//---------------------------------------------------------------------
void trim(char* ptop) {
	char* pend = ptop + strlen(ptop);

	while ((pend != ptop) && (*(pend - 1) == 0x0d || *(pend - 1) == 0x0a)) {
		*(--pend) = 0;
	}
}


//---------------------------------------------------------------------
// プログラム スタート
//---------------------------------------------------------------------
int main(int argc, char** argv) {
	char szParentPath[MAX_PATH];
	char buf[512];
	FILE* fp;

	// 引数のひとつ目をログファイルとして使用する。
	// 引数がないときはエラー終了
	if (argc != 2) {
		fprintf(stderr, "使用法：steal <出力ファイル名>\n");
		return -1;
	}

	// 親ディレクトリを作成する
	PathRemoveFileSpec(strcpy(szParentPath, argv[1]));
	create_directory(szParentPath);

	// 出力用ファイルを開く
	fp = fopen(argv[1], "a+");
	if (fp == NULL) {
		fprintf(stderr, "ファイル '%s' が開けません。\n", argv[1]);
		return -1;
	}

	// 標準入力が途切れるまで、画面とファイルに出力
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		trim(buf);
		puts(buf);
		fputs(buf, fp);
		fputs("\n", fp);
	}

	fclose(fp);

	return 0;
}
