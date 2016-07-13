#pragma warning(disable: 4786)

#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

map<string, string> g_querystring;

void makequerystring()
{
	g_querystring["file"] = "";
	g_querystring["size"] = "";

	string env(getenv("QUERY_STRING"));
	if (!env.empty()) {

		vector<char> work(env.begin(), env.end());
		work.push_back('\0');

		char *p = strtok(work.begin(), "=");
		while (1) {
			if (p != NULL) {

				string mapkey(p);

				p = strtok(NULL, "&");
				if (p != NULL) {
					string mapval(p);

					g_querystring[mapkey] = mapval;
				} else {
					break;
				}
			} else {
				break;
			}

			p = strtok(NULL, "=");
		}
	}
}

int main()
{
	//cout << "content-type: text/html";
	makequerystring();

	HANDLE h = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, g_querystring["file"].c_str());
	char* p = (char*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	cout << "Content-Type: application/octet-stream\nContent-Disposition: attachment; filename=\"" << g_querystring["file"].c_str() << "\"";
	cout << "\n\n";

	int size = atoi(g_querystring["size"].c_str());
	char *buf = (char *)malloc(size);

	try
	{
		memmove(buf, p, size);
		_setmode(fileno(stdout), _O_BINARY);
		cout.write(p, size);
	}
	catch(...)
	{
		cout << "ERROR ERROR ERROR" << endl;
	}

	UnmapViewOfFile(p);
	CloseHandle(h);

	free(buf);

	return 0;
}
