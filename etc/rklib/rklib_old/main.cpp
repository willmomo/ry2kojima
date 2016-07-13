#include <iostream>
#include "rklib.h"

using namespace std;

///////////////////////////////////////////////////////////////////////
//	[概要]
//	プログラムスタート

int main(int argc, char **argv)
{
	cout << "rklib ver.1.0.0.0 - 2003/08/15" << endl;
	cout << "rklib.h と rklib.cpp をプロジェクトにコピーして使います。" << endl;
	cout << "\n";
	cout << "--- 関数 ---\n";
	cout << "int chomp(char *pStr);" << endl;
	cout << "DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize);" << endl;
	cout << "\n";
	cout << "--- マクロ ---\n";
	cout << "countof(T)" << endl;

	return 0;
}
