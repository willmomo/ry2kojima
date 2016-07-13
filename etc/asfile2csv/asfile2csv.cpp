#include "asfile2csv.h"

using namespace std;

ASFILE2CSV_OPTION g_option = {false, -1, 0};

CONV_FUNC g_funcs[] = {
	{1001, conv1001},
	{1002, conv1002},
	{1103, conv1103},
	{1106, conv1106},
	{4103, conv4103},
	{0, NULL}
};

short ntoh(short n) { return ntohs(n); }
unsigned short ntoh(unsigned short n) { return ntohs(n); }
int ntoh(int n) { return ntohl(n); }
unsigned int ntoh(unsigned int n) { return ntohl(n); }
long ntoh(long n) { return ntohl(n); }
unsigned long ntoh(unsigned long n) { return ntohl(n); }

int asfile2csv(const char* fullpath)
{
	int ret = 0;

	char fname[_MAX_PATH], ext[_MAX_EXT];
	_splitpath(fullpath, NULL, NULL, fname, ext);
	_makepath(fname, NULL, NULL, fname, ext);

	regex_t reg;
	if (regcomp(&reg, "^(m?)f([0-9]{4})(_[1-3])?.bin$", REG_ICASE|REG_EXTENDED) == 0) {
		regmatch_t match[3];
		if (regexec(&reg, fname, countof(match), match, 0) == 0) {
			// 'm'の有無でエンディアンを決定
			g_option.endian = (match[1].rm_so == match[1].rm_eo) ? 0 : 1;

			// ファイル番号はファイル名部分から取得
			char temp[96];
			memset(temp, 0, sizeof(temp));
			g_option.fno = atoi(strncpy(temp, &fname[match[2].rm_so], match[2].rm_eo - match[2].rm_so));

			// ファイル番号に対応する変換ルーチンで変換
			for (int i = 0; g_funcs[i].fno != 0; i++) {
				if (g_funcs[i].fno == g_option.fno) {
					ret = (g_funcs[i].conv_csv)(fullpath, g_option.raw, g_option.endian);
					break;
				}
			}

			if (g_funcs[i].fno == 0) {
				cerr << "ファイル番号: " << g_option.fno << " の変換は、サポートしていません。" << endl;
			}
		}

		regfree(&reg);
	}

	return ret;
}

void usage()
{
	cout << "asfile2csv ver.1.0.0.0 - 2003/08/15" << endl;
	cout << "support file: ";
	for (int i = 0; g_funcs[i].fno != 0; i++) {
		cout << g_funcs[i].fno << " ";
	}
	cout << endl;

	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	int c, option_index = 0;
	struct option long_options[] = {
		{"raw", no_argument, NULL, 'r'},
		{NULL, 0, NULL, 0}
	};

	while ((c = getopt_long(argc, argv, "r", long_options, &option_index)) != EOF) {
		switch (c) {
		case 'r':
			g_option.raw = true;
			break;
		default:
			usage();
			break;
		}
	}

	for (; optind < argc; ++optind) {
		asfile2csv(argv[optind]);
	}

	return 0;
}
