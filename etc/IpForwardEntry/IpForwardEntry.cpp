//---------------------------------------------------------------------
// [概要]
// コンピュータ名を取得/変更するプログラム

#include <windows.h>
#include <iomanip>
#include <iostream>
#include <strstream>
#include <vector>
#define __STDC__	1
#include "getopt.h"

using namespace std;


typedef struct _MIB_IPFORWARDROW {  
	DWORD dwForwardDest;  
	DWORD dwForwardMask;  
	DWORD dwForwardPolicy;  
	DWORD dwForwardNextHop;  
	DWORD dwForwardIfIndex;  
	DWORD dwForwardType;  
	DWORD dwForwardProto;  
	DWORD dwForwardAge;  
	DWORD dwForwardNextHopAS;  
	DWORD dwForwardMetric1;  
	DWORD dwForwardMetric2;  
	DWORD dwForwardMetric3;  
	DWORD dwForwardMetric4;  
	DWORD dwForwardMetric5;
} MIB_IPFORWARDROW, *PMIB_IPFORWARDROW;

#define ANY_SIZE	1

typedef struct _MIB_IPFORWARDTABLE {
	DWORD dwNumEntries;
	MIB_IPFORWARDROW table[ANY_SIZE];
} MIB_IPFORWARDTABLE, *PMIB_IPFORWARDTABLE;

typedef DWORD (WINAPI *GIPFT)(PMIB_IPFORWARDTABLE, PULONG, BOOL);
typedef DWORD (WINAPI *SIPFE)(PMIB_IPFORWARDROW);
typedef DWORD (WINAPI *DIPFE)(PMIB_IPFORWARDROW);

// routing table を取得する関数

DWORD __GetIpForwardTable(PMIB_IPFORWARDTABLE pIpForwardTable, PULONG pdwSize, BOOL bOrder)
{
	DWORD ret = FALSE;

	HMODULE mdl = LoadLibrary("iphlpapi.dll");
	if (mdl) {
		GIPFT fn = (GIPFT)GetProcAddress(mdl, "GetIpForwardTable");
		if (fn) {
			ret = fn(pIpForwardTable, pdwSize, bOrder);
		}

		FreeLibrary(mdl);
	}

	return ret;
}

// routing table を設定する関数

DWORD __SetIpForwardEntry(PMIB_IPFORWARDROW pRoute)
{
	DWORD ret = FALSE;

	HMODULE mdl = LoadLibrary("iphlpapi.dll");
	if (mdl) {
		SIPFE fn = (SIPFE)GetProcAddress(mdl, "SetIpForwardEntry");
		if (fn) {
			ret = fn(pRoute);
		}

		FreeLibrary(mdl);
	}

	return ret;
}

// routing table を削除する関数

DWORD __DeleteIpForwardEntry(PMIB_IPFORWARDROW pRoute)
{
	DWORD ret = FALSE;

	HMODULE mdl = LoadLibrary("iphlpapi.dll");
	if (mdl) {
		DIPFE fn = (DIPFE)GetProcAddress(mdl, "DeleteIpForwardEntry");
		if (fn) {
			ret = fn(pRoute);
		}

		FreeLibrary(mdl);
	}

	return ret;
}

string toIpString(DWORD dwIp)
{
	ostrstream os;

	os	<< (dwIp & 0x000000ff) << "." << ((dwIp & 0x0000ff00) >> 8) << "."
		<< ((dwIp & 0x00ff0000) >> 16) << "." << ((dwIp & 0xff000000) >> 24) << ends;
	return string(os.str());
}

//---------------------------------------------------------------------
// [概要]
// バージョン情報を取得
//---------------------------------------------------------------------

string Version()
{
	DWORD handle = 0;
	vector<char> fname(MAX_PATH);
	
	// バージョン情報リソースを取得
	GetModuleFileName(NULL, fname.begin(), fname.size());
	vector<char> buf(GetFileVersionInfoSize(fname.begin(), &handle));
	GetFileVersionInfo(fname.begin(), handle, buf.size(), buf.begin());

	VS_FIXEDFILEINFO* p;
	UINT size;
	VerQueryValue(buf.begin(), "\\", (LPVOID*)&p, &size);

	ostrstream os;
	os	<< ((p->dwFileVersionMS & 0xffff0000) >> 16) << "."
		<< (p->dwFileVersionMS & 0x0000ffff) << "."
		<< ((p->dwFileVersionLS & 0xffff0000) >> 16) << "."
		<< (p->dwFileVersionLS & 0x0000ffff);

	return string(os.str(), os.pcount());
}

//---------------------------------------------------------------------
// [概要]
// 使用法を表示して終了
//---------------------------------------------------------------------

void usage(int status)
{

	exit(status);
}

//---------------------------------------------------------------------
// [概要]
// プログラム・スタート
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	DWORD ret;
	
	DWORD usesize = 0;
	ret = __GetIpForwardTable(NULL, &usesize, FALSE);

	cout << ret << "," << usesize << endl;

	vector<char> buf(usesize);

	ret = __GetIpForwardTable((PMIB_IPFORWARDTABLE)buf.begin(), &usesize, FALSE);

	cout << ret << "," << usesize << endl;

	PMIB_IPFORWARDTABLE p = (PMIB_IPFORWARDTABLE)buf.begin();

	cout << "dwNumEntries: " << p->dwNumEntries << endl;

	for (int i = 0; i < p->dwNumEntries; i++) {
		cout	<< "entry number\t" << i << "\n"
				<< "dwForwardAge\t\t" << p->table[i].dwForwardAge << "\n"
				<< "dwForwardDest\t\t" << toIpString(p->table[i].dwForwardDest).c_str() << "\n"
				<< "dwForwardIfIndex\t" << p->table[i].dwForwardIfIndex << "\n"
				<< "dwForwardMask\t\t" << toIpString(p->table[i].dwForwardMask).c_str() << "\n"
				<< "dwForwardMetric1\t" << p->table[i].dwForwardMetric1 << "\n"
				<< "dwForwardMetric2\t" << p->table[i].dwForwardMetric2 << "\n"
				<< "dwForwardMetric3\t" << p->table[i].dwForwardMetric3 << "\n"
				<< "dwForwardMetric4\t" << p->table[i].dwForwardMetric4 << "\n"
				<< "dwForwardMetric5\t" << p->table[i].dwForwardMetric5 << "\n"
				<< "dwForwardNextHop\t" << toIpString(p->table[i].dwForwardNextHop).c_str() << "\n"
				<< "dwForwardNextHopAS\t" << p->table[i].dwForwardNextHopAS << "\n"
				<< "dwForwardPolicy\t\t" << p->table[i].dwForwardPolicy << "\n"
				<< "dwForwardProto\t\t" << p->table[i].dwForwardProto << "\n"
				<< "dwForwardType\t\t" << p->table[i].dwForwardType << "\n"
				<< endl;
	}

//	MIB_IPFORWARDROW newRoute;
//	memcpy(&newRoute, &p->table[0], sizeof(newRoute));
//	memset(&newRoute, 0, sizeof(newRoute));

//	ret = __DeleteIpForwardEntry(&p->table[0]);
//	cout << ret << endl;

//	newRoute.dwForwardDest = 0x0001a8c0;
//	newRoute.dwForwardMask = 0x00ffffff;
//	newRoute.dwForwardNextHop = 0x231811ac;

//	ret = __SetIpForwardEntry(&newRoute);

//	cout << ret << endl;

	return 0;
}
