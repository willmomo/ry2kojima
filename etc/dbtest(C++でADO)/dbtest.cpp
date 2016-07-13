#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")

int main(int argc, char** argv) {
	CoInitialize(NULL);

	_ConnectionPtr pCn = NULL;
	_RecordsetPtr pRs = NULL;

	HRESULT hr = pCn.CreateInstance(__uuidof(Connection));
	if (SUCCEEDED(hr)) {
		hr = pCn->Open("Driver={Microsoft Access Driver (*.mdb)}; DBQ=E:\\work\\RecUpdate\\RecUpdate.mdb", "", "", 0);
		if (SUCCEEDED(hr)) {
			pCn->Execute("UPDATE T_CACHE INNER JOIN T_MASTER ON T_CACHE.RecID = T_MASTER.RecID SET T_MASTER.Data1 = T_CACHE.Data1;", NULL, 0);
			pCn->Close();
		}
	}

	CoUninitialize();
	return 0;
}
