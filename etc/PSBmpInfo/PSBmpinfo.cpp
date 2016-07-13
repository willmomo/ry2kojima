/////////////////////////////////////////////////////////////////////////
// BMPINFO.CPP

#include <windows.h>
#include <iostream.h>
#include <objbase.h>
#include <shlobj.h>

#include "resource.h"
#include "Iface.h"      // Interface declarations

/////////////////////////////////////////////////////////////////////////
// グローバル変数

static HINSTANCE g_hInst = NULL;	// DLL モジュール インスタンス
static long g_cComponents = 0;		// アクティブ コンポーネント カウンタ
static long g_cServerLocks = 0;		// ロック カウンタ
static char g_szFile[MAX_PATH];


// Friendly name of component
const char g_szFriendlyName[] = "Property Sheet Bitmap Info";

// Version-independent ProgID
const char g_szVerIndProgID[] = "RKojima.PSBmpInfo";

// ProgID
const char g_szProgID[] = "RKojima.PSBmpInfo.1";


/////////////////////////////////////////////////////////////////////////
// 拡張プロパティシート クラス
// IShellExtInit, IShellPropSheetExt, IUnknown のコンポーネント

class CPSBmpInfo : public IShellExtInit, public IShellPropSheetExt
{
private:
	long m_cRef;	// 参照カウンタ

public:
	// コンストラクタ/デストラクタ
	CPSBmpInfo();
	~CPSBmpInfo() ;

	// IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	// IShellExtInit
	virtual HRESULT __stdcall Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID);

	// IShellPropSheetExt
	virtual HRESULT __stdcall AddPages(LPFNADDPROPSHEETPAGE lpfnAddPage,LPARAM lParam);	
	virtual HRESULT __stdcall ReplacePage(UINT uPageID, LPFNADDPROPSHEETPAGE lpfnReplacePage, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////
// CPSBmpInfo コンポーネントの実装

/////////////////////////////////////////////////////////////////////////
// コンストラクタ/デストラクタ

CPSBmpInfo::CPSBmpInfo() : m_cRef(1)
{ 
	InterlockedIncrement(&g_cComponents); 
}

CPSBmpInfo::~CPSBmpInfo() 
{ 
	InterlockedDecrement(&g_cComponents); 
}

/////////////////////////////////////////////////////////////////////////
// IUnknown インタフェース

HRESULT __stdcall CPSBmpInfo::QueryInterface(REFIID iid, void** ppv)
{    
	if (iid == IID_IUnknown) {
		*ppv = static_cast<IShellExtInit*>(this) ; 
	} else if (iid == IID_IShellExtInit) {
		*ppv = static_cast<IShellExtInit*>(this);
	} else if (iid == IID_IShellPropSheetExt) {
		*ppv = static_cast<IShellPropSheetExt*>(this);
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK ;
}

ULONG __stdcall CPSBmpInfo::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CPSBmpInfo::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}

	return m_cRef;
}

/////////////////////////////////////////////////////////////////////////
// IShellExtInit インタフェース

HRESULT __stdcall CPSBmpInfo::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID)
{
	// IDataObject::GetDataで取得するデータの種類を設定
	FORMATETC fe;
	ZeroMemory(&fe, sizeof(fe));
	fe.cfFormat = CF_HDROP;
	fe.ptd = NULL;
	fe.dwAspect = DVASPECT_CONTENT;
	fe.lindex = -1;
	fe.tymed = TYMED_HGLOBAL;

	// 選択されたファイルの情報を取得
	STGMEDIUM medium;
	lpdobj->GetData(&fe, &medium);

	if (DragQueryFile(medium.hGlobal, 0xFFFFFFFF, NULL, 0) == 1) {
		DragQueryFile(medium.hGlobal, 0, g_szFile, sizeof(g_szFile));
	} else {
		// 複数ファイルの選択は、サポートしない
		return E_FAIL;
	}

	ReleaseStgMedium(&medium);

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////
// IShellPropSheetExt インタフェース

BOOL CALLBACK pspProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BITMAPFILEHEADER s_bfh;
	static BITMAPINFOHEADER s_bih;

	switch (uMsg) {
	case WM_INITDIALOG:
		{
			SetDlgItemText(hwnd, IDC_FNAME, g_szFile);
			HANDLE hFile = CreateFile(g_szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				DWORD dwNumberOfBytesRead;

				ReadFile(hFile, &s_bfh, sizeof(s_bfh), &dwNumberOfBytesRead, NULL);
				ReadFile(hFile, &s_bih, sizeof(s_bih), &dwNumberOfBytesRead, NULL);

				CloseHandle(hFile);
			}

			if (s_bfh.bfType != 'MB')
				return TRUE;

			char buf[128];
			HWND hwndList = GetDlgItem(hwnd, IDC_LIST);

			LVCOLUMN column;
			ZeroMemory(&column, sizeof(column));
			column.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
			column.fmt = LVCFMT_LEFT;
			column.cx = 100;
			column.pszText = "項目";
			ListView_InsertColumn(hwndList, 0, &column);

			column.cx = 150;
			column.pszText = "値";
			ListView_InsertColumn(hwndList, 1, &column);

			LVITEM item;
			ZeroMemory(&item, sizeof(item));
			item.mask = LVIF_TEXT;
			item.iItem = 0;
			item.iSubItem = 0;
			item.pszText = "サイズ(幅×高さ)";
			ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d×%d", s_bih.biWidth, s_bih.biHeight);
			ListView_SetItemText(hwndList, 0, 1, buf);

			item.iItem = 1;
			item.iSubItem = 0;
			item.pszText = "色数";
			item.iItem = ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%dビット カラー", s_bih.biBitCount);
			ListView_SetItemText(hwndList, 1, 1, buf);

			item.iItem = 2;
			item.iSubItem = 0;
			item.pszText = "圧縮";
			item.iItem = ListView_InsertItem(hwndList, &item);

			switch (s_bih.biCompression) {
			case BI_RGB:	lstrcpy(buf, "無圧縮");	break;
			case BI_RLE8:	lstrcpy(buf, "8ビット ランレングス");	break;
			case BI_RLE4:	lstrcpy(buf, "4ビット ランレングス");	break;
			default:
				wsprintf(buf, "未知の圧縮 (値=%d)", s_bih.biCompression);
				break;
			}
			ListView_SetItemText(hwndList, 2, 1, buf);

			item.iItem = 3;
			item.iSubItem = 0;
			item.pszText = "画像データサイズ";
			ListView_InsertItem(hwndList, &item);

			if (s_bih.biCompression == BI_RGB) {
				lstrcpy(buf, "*");
			} else {
				wsprintf(buf, "%d バイト", s_bih.biSizeImage);
			}
			ListView_SetItemText(hwndList, 3, 1, buf);

			item.iItem = 4;
			item.iSubItem = 0;
			item.pszText = "水平方向解像度";
			ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d ピクセル/メートル", s_bih.biXPelsPerMeter);
			ListView_SetItemText(hwndList, 4, 1, buf);

			item.iItem = 5;
			item.iSubItem = 0;
			item.pszText = "垂直方向解像度";
			ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d ピクセル/メートル", s_bih.biYPelsPerMeter);
			ListView_SetItemText(hwndList, 5, 1, buf);

			item.iItem = 6;
			item.iSubItem = 0;
			item.pszText = "パレット数";
			ListView_InsertItem(hwndList, &item);

			if (s_bih.biBitCount >= 16) {
				lstrcpy(buf, "*");
			} else {
				wsprintf(buf, "%d", s_bih.biClrUsed);
			}
			ListView_SetItemText(hwndList, 6, 1, buf);

			item.iItem = 7;
			item.iSubItem = 0;
			item.pszText = "重要視する色";
			ListView_InsertItem(hwndList, &item);
			
			wsprintf(buf, "%d", s_bih.biClrImportant);
			ListView_SetItemText(hwndList, 7, 1, buf);
		}
		return TRUE;
	case WM_NOTIFY:
		SetWindowLong(hwnd, DWL_MSGRESULT, FALSE);
		return TRUE;
	}

	return FALSE;
}

HRESULT __stdcall CPSBmpInfo::AddPages(LPFNADDPROPSHEETPAGE lpfnAddPage,LPARAM lParam)
{
	// プロパティシート情報セット
	PROPSHEETPAGE psp;
	ZeroMemory(&psp, sizeof(psp));
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = g_hInst;
	psp.pszTitle = "BMP情報";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_OLE_PROPPAGE_SMALL);
	psp.pfnDlgProc = (DLGPROC)pspProc;

	// プロパティシートを作成して、成功したらlpfnAddPageを呼ぶ
	HPROPSHEETPAGE handle;
	if (handle = CreatePropertySheetPage(&psp)) {
		if (!(*lpfnAddPage)(handle, lParam)) {
			DestroyPropertySheetPage(handle);

			return S_FALSE;
		}
	}

	return NOERROR;
}

HRESULT __stdcall CPSBmpInfo::ReplacePage(UINT uPageID, LPFNADDPROPSHEETPAGE lpfnReplacePage, LPARAM lParam)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////
// ファクトリクラス
// IClassFactory, IUknown のコンポーネント

class CFactory : public IClassFactory
{
private:
	long m_cRef;	// 参照カウンタ

public:
	// コンストラクタ/デストラクタ
	CFactory() : m_cRef(1) {}
	~CFactory() {}

	// IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	// IClassFactory
	virtual HRESULT __stdcall CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj);
	virtual HRESULT __stdcall LockServer(int fLock) ;
};

/////////////////////////////////////////////////////////////////////////
// クラス ファクトリの実装

/////////////////////////////////////////////////////////////////////////
// IUnknown インタフェース

HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void** ppv)
{    
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory)) {
		*ppv = static_cast<IClassFactory*>(this);
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK ;
}

ULONG __stdcall CFactory::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CFactory::Release() 
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}
	return m_cRef;
}

/////////////////////////////////////////////////////////////////////////
// IClassFactory インタフェース

HRESULT __stdcall CFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
	// 集約と包含は、サポートしていない
	if (pUnkOuter != NULL) {
		return CLASS_E_NOAGGREGATION;
	}

	// コンポーネントを作成
	CPSBmpInfo* pPsbi = new CPSBmpInfo;
	if (pPsbi == NULL) {
		return E_OUTOFMEMORY;
	}

	// コンポーネントから要求されたインタフェースを取得
	HRESULT hr = pPsbi->QueryInterface(riid, ppvObj);

	// もし、QueryInterfaceに失敗しているなら、コンポーネントは、削除されます
	pPsbi->Release();
	
	return hr;
}

HRESULT __stdcall CFactory::LockServer(int fLock) 
{
	if (fLock) {
		InterlockedIncrement(&g_cServerLocks); 
	} else {
		InterlockedDecrement(&g_cServerLocks);
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// exportされた関数

/////////////////////////////////////////////////////////////////////////
// DLLがアンロードできるかどうか確認する

STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0)) {
		return S_OK;
	} else {
		return S_FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////
// クラス・ファクトリ・インタフェースの取得

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	// 自分が作成できるコンポーネントか？
	if (rclsid != CLSID_Component1) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	// まず、クラス ファクトリを作成する
	CFactory* pFactory = new CFactory;	// (コンストラクタでAddRefされている)
	if (pFactory == NULL) {
		return E_OUTOFMEMORY ;
	}

	// 要求されたインタフェースを取得する
	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release();

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// レジストリへ登録

STDAPI DllRegisterServer()
{
	LPOLESTR psz;
	HRESULT hr = StringFromCLSID(CLSID_Component1, &psz);
	if (FAILED(hr)) {
		return S_FALSE;
	}

	char szCLSID[64];
	wcstombs(szCLSID, psz, 64) ;

	CoTaskMemFree(psz);

	char szSubKey[MAX_PATH];
	wsprintf(szSubKey, "CLSID\\%s", szCLSID);
	HKEY hkey;
	RegCreateKeyEx(HKEY_CLASSES_ROOT, szSubKey, NULL, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL);

	char szFriendName[] = "Property Sheet Bitmap Information";
	RegSetValueEx(hkey, NULL, 0, REG_SZ, (CONST BYTE*)szFriendName, sizeof(szFriendName));
	RegCloseKey(hkey);

	lstrcat(szSubKey, "\\InprocServer32");
	RegCreateKeyEx(HKEY_CLASSES_ROOT, szSubKey, NULL, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL);

	char szModule[512] ;
	DWORD dwResult = GetModuleFileName(g_hInst, szModule, sizeof(szModule)/sizeof(char)) ;
	RegSetValueEx(hkey, NULL, 0, REG_SZ, (CONST BYTE*)szModule, sizeof(szModule));
	RegSetValueEx(hkey, "ThreadingModel", 0, REG_SZ, (CONST BYTE*)"Apartment", sizeof("Apartment"));

	RegCloseKey(hkey);

	wsprintf(szSubKey, ".bmp\\ShellEx\\PropertySheetHandlers\\%s", szCLSID);
	RegCreateKeyEx(HKEY_CLASSES_ROOT, szSubKey, NULL, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL);
	RegCloseKey(hkey);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// レジストリから登録解除

STDAPI DllUnregisterServer()
{
	LPOLESTR psz;
	HRESULT hr = StringFromCLSID(CLSID_Component1, &psz);
	if (FAILED(hr)) {
		return S_FALSE;
	}

	char szCLSID[64];
	wcstombs(szCLSID, psz, 64) ;

	CoTaskMemFree(psz);

	char szSubKey[MAX_PATH];
	HKEY hkey;

	wsprintf(szSubKey, "CLSID\\%s\\InprocServer32", szCLSID);
	RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey);

	wsprintf(szSubKey, "CLSID\\%s", szCLSID);
	RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey);

	wsprintf(szSubKey, ".bmp\\ShellEx\\PropertySheetHandlers\\%s", szCLSID);
	RegDeleteKey(HKEY_CLASSES_ROOT, szSubKey);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// DLL モジュール スタート

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		g_hInst = hInst;
	}

	return TRUE ;
}
