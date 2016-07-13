/////////////////////////////////////////////////////////////////////////
// BMPINFO.CPP

#include <windows.h>
#include <iostream.h>
#include <objbase.h>
#include <shlobj.h>

#include "resource.h"
#include "Iface.h"      // Interface declarations

/////////////////////////////////////////////////////////////////////////
// �O���[�o���ϐ�

static HINSTANCE g_hInst = NULL;	// DLL ���W���[�� �C���X�^���X
static long g_cComponents = 0;		// �A�N�e�B�u �R���|�[�l���g �J�E���^
static long g_cServerLocks = 0;		// ���b�N �J�E���^
static char g_szFile[MAX_PATH];


// Friendly name of component
const char g_szFriendlyName[] = "Property Sheet Bitmap Info";

// Version-independent ProgID
const char g_szVerIndProgID[] = "RKojima.PSBmpInfo";

// ProgID
const char g_szProgID[] = "RKojima.PSBmpInfo.1";


/////////////////////////////////////////////////////////////////////////
// �g���v���p�e�B�V�[�g �N���X
// IShellExtInit, IShellPropSheetExt, IUnknown �̃R���|�[�l���g

class CPSBmpInfo : public IShellExtInit, public IShellPropSheetExt
{
private:
	long m_cRef;	// �Q�ƃJ�E���^

public:
	// �R���X�g���N�^/�f�X�g���N�^
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
// CPSBmpInfo �R���|�[�l���g�̎���

/////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^/�f�X�g���N�^

CPSBmpInfo::CPSBmpInfo() : m_cRef(1)
{ 
	InterlockedIncrement(&g_cComponents); 
}

CPSBmpInfo::~CPSBmpInfo() 
{ 
	InterlockedDecrement(&g_cComponents); 
}

/////////////////////////////////////////////////////////////////////////
// IUnknown �C���^�t�F�[�X

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
// IShellExtInit �C���^�t�F�[�X

HRESULT __stdcall CPSBmpInfo::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID)
{
	// IDataObject::GetData�Ŏ擾����f�[�^�̎�ނ�ݒ�
	FORMATETC fe;
	ZeroMemory(&fe, sizeof(fe));
	fe.cfFormat = CF_HDROP;
	fe.ptd = NULL;
	fe.dwAspect = DVASPECT_CONTENT;
	fe.lindex = -1;
	fe.tymed = TYMED_HGLOBAL;

	// �I�����ꂽ�t�@�C���̏����擾
	STGMEDIUM medium;
	lpdobj->GetData(&fe, &medium);

	if (DragQueryFile(medium.hGlobal, 0xFFFFFFFF, NULL, 0) == 1) {
		DragQueryFile(medium.hGlobal, 0, g_szFile, sizeof(g_szFile));
	} else {
		// �����t�@�C���̑I���́A�T�|�[�g���Ȃ�
		return E_FAIL;
	}

	ReleaseStgMedium(&medium);

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////
// IShellPropSheetExt �C���^�t�F�[�X

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
			column.pszText = "����";
			ListView_InsertColumn(hwndList, 0, &column);

			column.cx = 150;
			column.pszText = "�l";
			ListView_InsertColumn(hwndList, 1, &column);

			LVITEM item;
			ZeroMemory(&item, sizeof(item));
			item.mask = LVIF_TEXT;
			item.iItem = 0;
			item.iSubItem = 0;
			item.pszText = "�T�C�Y(���~����)";
			ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d�~%d", s_bih.biWidth, s_bih.biHeight);
			ListView_SetItemText(hwndList, 0, 1, buf);

			item.iItem = 1;
			item.iSubItem = 0;
			item.pszText = "�F��";
			item.iItem = ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d�r�b�g �J���[", s_bih.biBitCount);
			ListView_SetItemText(hwndList, 1, 1, buf);

			item.iItem = 2;
			item.iSubItem = 0;
			item.pszText = "���k";
			item.iItem = ListView_InsertItem(hwndList, &item);

			switch (s_bih.biCompression) {
			case BI_RGB:	lstrcpy(buf, "�����k");	break;
			case BI_RLE8:	lstrcpy(buf, "8�r�b�g ���������O�X");	break;
			case BI_RLE4:	lstrcpy(buf, "4�r�b�g ���������O�X");	break;
			default:
				wsprintf(buf, "���m�̈��k (�l=%d)", s_bih.biCompression);
				break;
			}
			ListView_SetItemText(hwndList, 2, 1, buf);

			item.iItem = 3;
			item.iSubItem = 0;
			item.pszText = "�摜�f�[�^�T�C�Y";
			ListView_InsertItem(hwndList, &item);

			if (s_bih.biCompression == BI_RGB) {
				lstrcpy(buf, "*");
			} else {
				wsprintf(buf, "%d �o�C�g", s_bih.biSizeImage);
			}
			ListView_SetItemText(hwndList, 3, 1, buf);

			item.iItem = 4;
			item.iSubItem = 0;
			item.pszText = "���������𑜓x";
			ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d �s�N�Z��/���[�g��", s_bih.biXPelsPerMeter);
			ListView_SetItemText(hwndList, 4, 1, buf);

			item.iItem = 5;
			item.iSubItem = 0;
			item.pszText = "���������𑜓x";
			ListView_InsertItem(hwndList, &item);

			wsprintf(buf, "%d �s�N�Z��/���[�g��", s_bih.biYPelsPerMeter);
			ListView_SetItemText(hwndList, 5, 1, buf);

			item.iItem = 6;
			item.iSubItem = 0;
			item.pszText = "�p���b�g��";
			ListView_InsertItem(hwndList, &item);

			if (s_bih.biBitCount >= 16) {
				lstrcpy(buf, "*");
			} else {
				wsprintf(buf, "%d", s_bih.biClrUsed);
			}
			ListView_SetItemText(hwndList, 6, 1, buf);

			item.iItem = 7;
			item.iSubItem = 0;
			item.pszText = "�d�v������F";
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
	// �v���p�e�B�V�[�g���Z�b�g
	PROPSHEETPAGE psp;
	ZeroMemory(&psp, sizeof(psp));
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = g_hInst;
	psp.pszTitle = "BMP���";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_OLE_PROPPAGE_SMALL);
	psp.pfnDlgProc = (DLGPROC)pspProc;

	// �v���p�e�B�V�[�g���쐬���āA����������lpfnAddPage���Ă�
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
// �t�@�N�g���N���X
// IClassFactory, IUknown �̃R���|�[�l���g

class CFactory : public IClassFactory
{
private:
	long m_cRef;	// �Q�ƃJ�E���^

public:
	// �R���X�g���N�^/�f�X�g���N�^
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
// �N���X �t�@�N�g���̎���

/////////////////////////////////////////////////////////////////////////
// IUnknown �C���^�t�F�[�X

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
// IClassFactory �C���^�t�F�[�X

HRESULT __stdcall CFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
	// �W��ƕ�܂́A�T�|�[�g���Ă��Ȃ�
	if (pUnkOuter != NULL) {
		return CLASS_E_NOAGGREGATION;
	}

	// �R���|�[�l���g���쐬
	CPSBmpInfo* pPsbi = new CPSBmpInfo;
	if (pPsbi == NULL) {
		return E_OUTOFMEMORY;
	}

	// �R���|�[�l���g����v�����ꂽ�C���^�t�F�[�X���擾
	HRESULT hr = pPsbi->QueryInterface(riid, ppvObj);

	// �����AQueryInterface�Ɏ��s���Ă���Ȃ�A�R���|�[�l���g�́A�폜����܂�
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
// export���ꂽ�֐�

/////////////////////////////////////////////////////////////////////////
// DLL���A�����[�h�ł��邩�ǂ����m�F����

STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0)) {
		return S_OK;
	} else {
		return S_FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////
// �N���X�E�t�@�N�g���E�C���^�t�F�[�X�̎擾

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	// �������쐬�ł���R���|�[�l���g���H
	if (rclsid != CLSID_Component1) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	// �܂��A�N���X �t�@�N�g�����쐬����
	CFactory* pFactory = new CFactory;	// (�R���X�g���N�^��AddRef����Ă���)
	if (pFactory == NULL) {
		return E_OUTOFMEMORY ;
	}

	// �v�����ꂽ�C���^�t�F�[�X���擾����
	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release();

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// ���W�X�g���֓o�^

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
// ���W�X�g������o�^����

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
// DLL ���W���[�� �X�^�[�g

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		g_hInst = hInst;
	}

	return TRUE ;
}
