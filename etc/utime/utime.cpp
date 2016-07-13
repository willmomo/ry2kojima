#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/utime.h>


using namespace std;


int main(int argc, char** argv) {

	struct _stat buffer;

	int ret = _stat(argv[1], &buffer);
	if (ret == 0) {
		cout << argv[1] << " - " << ctime(&buffer.st_mtime);
	} else {
		cout << argv[1] << " の情報が取得できません。\n";
	}

	if (argc == 3) {
		if (_stricmp(argv[2], "now") == 0) {	// 現在時間に設定する
			ret = _utime(argv[1], NULL);
		} else {
		}

		if (ret == 0) {
			int ret = _stat(argv[1], &buffer);
			if (ret == 0) {
				cout << argv[1] << " - " << ctime(&buffer.st_mtime);
			} else {
				cout << argv[1] << " の情報が取得できません。\n";
			}
		} else {
			switch (errno) {
			case EACCES:
				cerr << "パス名にディレクトリまたは読み出し専用ファイルを指定した。\n";
				break;
			case EINVAL:
				cerr << "引数が無効。\n";
				break;
			case EMFILE:
				cerr << "開いているファイルが多すぎる (ファイルを開かないと更新時刻を変更できません)。\n";
				break;
			case ENOENT:
				cerr << "ファイル名またはパス名が見つからない。\n";
				break;
			default:
				cerr << "未知のエラー(errno:" << errno << ")\n";
				break;
			}
		}
	}

	return 0;
}
