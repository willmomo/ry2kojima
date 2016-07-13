//---------------------------------------------------------------------
// [概要]
// スピードテスト・アプリケーション・クラス
//---------------------------------------------------------------------

#pragma warning(disable : 4786)	// 抑止:'identifier' : デバッグ情報で識別子が 'number' 文字に切り捨てられました。

//#define USE_STL
//#define USE_API

#include <windows.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <ctime>
#include "arguments.h"

using namespace std;

#define BUFSIZE			(1 * 1024 * 1024)
#define LOOP_10MBPS		32	
#define LOOP_100MBPS	320
#define LOOP_1GBPS		3200

char g_buffer[BUFSIZE];

//---------------------------------------------------------------------
// [概要]
// スピードテスト・アプリケーション・クラス
//---------------------------------------------------------------------

class CSpeedTest
{
private:

public:
	CSpeedTest(){};
	virtual ~CSpeedTest(){};

public:
	int SpeedTest(const char *pfname, int nFileSize);
	int domain(int argc, char **argv);
};

//---------------------------------------------------------------------
// [概要]
// スピードテスト
//---------------------------------------------------------------------

int CSpeedTest::SpeedTest(const char *pfname, int loop_count)
{
	char msgbuf[512];
	double dTime;
	clock_t s, e;
	int i;
#if defined(USE_STL)
	ifstream ifs;
	ofstream ofs;
#elif defined(USE_API)
	HANDLE hFile;
	DWORD written, read;
#else
	FILE *fp;
#endif

#if defined(USE_STL)
	ofs.open(pfname, ios::out | ios::binary | ios::trunc);
	if (ofs.is_open()) {
#elif defined(USE_API)
	hFile = CreateFile(pfname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
#else
	fp = fopen(pfname, "wb");
	if (fp) {
#endif
		cout << "書き込みサイズ：" << ((BUFSIZE / 1024 / 1024) * loop_count) << " MBytes\n";
		s = clock();
		for (i = 0; i < loop_count; i++) {
			cout << (i + 1) << "/" << loop_count << "\r";
#if defined(USE_STL)
			ofs.write(g_buffer, BUFSIZE);
#elif defined(USE_API)
			WriteFile(hFile, g_buffer, BUFSIZE, &written, NULL);
#else
			fwrite(g_buffer, sizeof(char), BUFSIZE, fp);
#endif
		}
		cout << "\n";
		e = clock();
#if defined(USE_STL)
		ofs.close();
#elif defined(USE_API)
		CloseHandle(hFile);
#else
		fclose(fp);
#endif

		dTime = (double)(e - s) / CLOCKS_PER_SEC;

		cout
			<< "所要時間：" << dTime << " 秒\n"
			<< "実効速度：" << ((double)(BUFSIZE / 1024 / 1024) * (double)loop_count / dTime * 8) << " Mbps\n";
	} else {
		sprintf(msgbuf, "ファイル '%s' が作成できません。", pfname);
		perror(msgbuf);
		return -1;
	}

	cout << endl;

#if defined(USE_STL)
	ifs.open(pfname, ios::in | ios::binary);
	if (ifs.is_open()) {
#elif defined(USE_API)
	hFile = CreateFile(pfname, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
#else
	fp = fopen(pfname, "rb");
	if (fp) {
#endif
		cout << "読み込みサイズ：" << ((BUFSIZE / 1024 / 1024) * loop_count) << " MBytes\n";
		s = clock();
		for (i = 0; i < loop_count; i++) {
			cout << (i + 1) << "/" << loop_count << "\r";
#if defined(USE_STL)
			ifs.read(g_buffer, BUFSIZE);
#elif defined(USE_API)
			ReadFile(hFile, g_buffer, BUFSIZE, &read, NULL);
#else
			fread(g_buffer, sizeof(char), BUFSIZE, fp);
#endif
		}
		cout << "\n";
		e = clock();
#if defined(USE_STL)
		ifs.close();
#elif defined(USE_API)
		CloseHandle(hFile);
#else
		fclose(fp);
#endif

		dTime = (double)(e - s) / CLOCKS_PER_SEC;

		cout
			<< "所要時間：" << dTime << " 秒\n"
			<< "実効速度：" << ((double)(BUFSIZE / 1024 / 1024) * (double)loop_count / dTime * 8) << " Mbps\n";
	} else {
		sprintf(msgbuf, "ファイル '%s' がオープンできません。", pfname);
		perror(msgbuf);
		return -1;
	}

	_unlink(pfname);

	return 0;
}

//---------------------------------------------------------------------
// [概要]
// アプリケーションのメイン処理
//---------------------------------------------------------------------

int CSpeedTest::domain(int argc, char **argv)
{
	int i, loop_count = LOOP_10MBPS;

	struct option long_options[] = {
		{"10mbps", no_argument, NULL, 'm'},
		{"100mbps", no_argument, NULL, 'M'},
		{"1gbps", no_argument, NULL, 'g'},
		{"help", no_argument, NULL, 0},
//		{"version", no_argument, NULL, 0},
		{NULL, 0, NULL, 0}
	};

	CArguments args(argc, argv, long_options);

	if (args.get_argument_count() != 1 || args.has_error() || args.has_option("help")) {
		cout 
			<< "使用法：spdtst [options] <file>\n"
			<< "\n"
			<< "  -m, --10mbps   10Mbps用の速度テスト(default)\n"
			<< "  -M, --100mbps  100Mbps用の速度テスト\n"
			<< "  -g, --1gbps    1Gbps用の速度テスト\n";

		return -1;
	}

	for (i = 0; i < BUFSIZE; i++) {
		g_buffer[i] = rand();
	}

	if (args.has_option("10mbps")) {
		// --10mbps option あり?
		loop_count = LOOP_10MBPS;
		cout << "リンク速度10Mbps用のテスト\n";
	} else if (args.has_option("100mbps")) {
		// --100mbps option あり?
		loop_count = LOOP_100MBPS;
		cout << "リンク速度100Mbps用のテスト\n";
	} else if (args.has_option("1gbps")) {
		// --1gbps optionあり?
		loop_count = LOOP_1GBPS;
		cout << "リンク速度1Gbps用のテスト\n";
	} else {
		// 速度指定が無いときは、 --10mbps扱い
		cout << "リンク速度10Mbps用のテスト\n";
	}

	SpeedTest(args.get_argument(0).c_str(), loop_count);

	return 0;
}

CSpeedTest theApp;	// アプリケーションのインスタンス

//---------------------------------------------------------------------
// [概要]
// プログラムスタート
//---------------------------------------------------------------------

int main(int argc, char **argv)
{
	return theApp.domain(argc, argv);
}
