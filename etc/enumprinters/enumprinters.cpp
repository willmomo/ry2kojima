//-----------------------------------------------------------------------
// �T���v���F�v�����^�̗񋓂ƃW���u�̍폜

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "resource.h"

/*
#include "../kjmlib/kjmlib.h"
#if defined(_DEBUG)
#pragma comment(lib, "../kjmlib/debug/kjmlib.lib")
#else
#pragma comment(lib, "../kjmlib/release/kjmlib.lib")
#endif
*/
#include "kjmlib.h"

#define WM_USER_UPDATE	(WM_USER + 1)

#define ID_TIMER	1
#define ELAPSE		1000	// 1sec

#define RECT_WIDTH(R)	((R).right - (R).left)
#define RECT_HEIGHT(R)	((R).bottom - (R).top)

HWND g_hwndPInfo;
std::vector<char> g_printerInfo2;

/**
 * �f�t�H���g�v�����^�̃v�����^DC���擾����
 */

HDC GetDefaultPrinterDC()
{
	static char szPrinter[64];
	char *szDevice, *szDriver, *szOutput;

	GetProfileString("windows",	"device", "", szPrinter, 64);

	if ((szDevice = strtok(szPrinter, ",")) &&
		(szDriver = strtok(NULL, ",")) &&
		(szOutput = strtok(NULL, ",")))
	{
		return CreateDC(szDriver, szDevice, szOutput, NULL);
	}

	return NULL;
}

/**
 * �R���{�{�b�N�X�őI�𒆂̃v�����^�̖��O���擾
 */

std::string GetSelectedPrinterName(HWND hwnd)
{
	HWND hwndCtrlPrinters = GetDlgItem(hwnd, IDC_PRINTERS);

	// �I�𒆂̃v�����^�̖��O���擾
	int nIndex = ComboBox_GetCurSel(hwndCtrlPrinters);
	std::vector<char> szPrinterName(ComboBox_GetLBTextLen(hwndCtrlPrinters, nIndex) + 1);
	ComboBox_GetLBText(hwndCtrlPrinters, nIndex, &szPrinterName[0]);

	return std::string(&szPrinterName[0]);
}

/**
 * �R���{�{�b�N�X�őI�𒆂̃v�����^��DC���擾
 */

HDC GetSelectedPrinterDC(HWND hwnd)
{
	HDC hdc = NULL;

	// �I�𒆂̃v�����^�̖��O���擾
	std::string strPrinterName = GetSelectedPrinterName(hwnd);

	HANDLE hPrinter;
	if (OpenPrinter((LPTSTR)strPrinterName.c_str(), &hPrinter, NULL)) {
		DWORD cbNeeded;
		PRINTER_INFO_2* ppi2;
		std::vector<char> pi2;

		// 1��ڂ͏����󂯎��̂ɕK�v�ȃo�b�t�@�T�C�Y�������炤
		GetPrinter(hPrinter, 2, NULL, 0, &cbNeeded);

		pi2.resize(cbNeeded);

		// 2��ڂ͎��ۂɏ����擾����
		GetPrinter(hPrinter, 2, (LPBYTE)&pi2[0], cbNeeded, &cbNeeded);

		ppi2 = (PRINTER_INFO_2*)&pi2[0];
		hdc = CreateDC(ppi2->pDriverName, ppi2->pPrinterName, NULL, NULL);

		// ��n��
		ClosePrinter(hPrinter);
	}

	return hdc;
}


/**
 * �v�����^�����R���g���[���ɔ��f������
 */

void OnUserUpdate(HWND hwnd)
{
	PRINTER_INFO_2* ppi2 = (PRINTER_INFO_2*)&g_printerInfo2[0];

	SetDlgItemText(hwnd, IDC_SERVER_NAME, (ppi2->pServerName) ? ppi2->pServerName : "(local printer)");
	SetDlgItemText(hwnd, IDC_PRINTER_NAME, ppi2->pPrinterName);
	SetDlgItemText(hwnd, IDC_SHARE_NAME, ppi2->pShareName);
	SetDlgItemText(hwnd, IDC_PORT_NAME, ppi2->pPortName);
	SetDlgItemText(hwnd, IDC_DRIVER_NAME, ppi2->pDriverName);
	SetDlgItemText(hwnd, IDC_COMMENT, ppi2->pComment);
	SetDlgItemText(hwnd, IDC_LOCATION, ppi2->pLocation);
	SetDlgItemText(hwnd, IDC_SEP_FILE, ppi2->pSepFile);
	SetDlgItemText(hwnd, IDC_PRINT_PROCESSOR, ppi2->pPrintProcessor);
	SetDlgItemText(hwnd, IDC_DATATYPE, ppi2->pDatatype);
	SetDlgItemText(hwnd, IDC_PARAMETERS, ppi2->pParameters);

	SetDlgItemText(hwnd, IDC_ATTRIBUTE_DEFAULT, (ppi2->Attributes & PRINTER_ATTRIBUTE_DEFAULT) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_DIRECT, (ppi2->Attributes & PRINTER_ATTRIBUTE_DIRECT) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_DO_COMPLETE_FIRST, (ppi2->Attributes & PRINTER_ATTRIBUTE_DO_COMPLETE_FIRST) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_ENABLE_BIDI, (ppi2->Attributes & PRINTER_ATTRIBUTE_ENABLE_BIDI) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_ENABLE_DEVQ, (ppi2->Attributes & PRINTER_ATTRIBUTE_ENABLE_DEVQ) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_KEEPPRINTEDJOBS, (ppi2->Attributes & PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_QUEUED, (ppi2->Attributes & PRINTER_ATTRIBUTE_QUEUED) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_SHARED, (ppi2->Attributes & PRINTER_ATTRIBUTE_SHARED) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_WORK_OFFLINE, (ppi2->Attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE) ? "Yes" : "No");
	SetDlgItemText(hwnd, IDC_ATTRIBUTE_PUBLISHED, (ppi2->Attributes & PRINTER_ATTRIBUTE_PUBLISHED) ? "Yes" : "No");

	SetDlgItemInt(hwnd, IDC_PRIORITY, ppi2->Priority, FALSE);
	SetDlgItemInt(hwnd, IDC_DEFAULT_PRIORITY, ppi2->DefaultPriority, FALSE);
	SetDlgItemInt(hwnd, IDC_START_TIME, ppi2->StartTime, FALSE);
	SetDlgItemInt(hwnd, IDC_UNTIL_TIME, ppi2->UntilTime, FALSE);
	SetDlgItemInt(hwnd, IDC_JOBS, ppi2->cJobs, FALSE);
	SetDlgItemInt(hwnd, IDC_AVERAGE_PPM, ppi2->AveragePPM, FALSE);

	SetDlgItemText(hwnd, IDC_STATUS_PAUSED, (ppi2->Status & PRINTER_STATUS_PAUSED) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PENDING_DELETION, (ppi2->Status & PRINTER_STATUS_PENDING_DELETION) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_BUSY, (ppi2->Status & PRINTER_STATUS_BUSY) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_DOOR_OPEN, (ppi2->Status & PRINTER_STATUS_DOOR_OPEN) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_ERROR, (ppi2->Status & PRINTER_STATUS_ERROR) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_INITIALIZING, (ppi2->Status & PRINTER_STATUS_INITIALIZING) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_IO_ACTIVE, (ppi2->Status & PRINTER_STATUS_IO_ACTIVE) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_MANUAL_FEED, (ppi2->Status & PRINTER_STATUS_MANUAL_FEED) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_NO_TONER, (ppi2->Status & PRINTER_STATUS_NO_TONER) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_NOT_AVAILABLE, (ppi2->Status & PRINTER_STATUS_NOT_AVAILABLE) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_OFFLINE, (ppi2->Status & PRINTER_STATUS_OFFLINE) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_OUT_OF_MEMORY, (ppi2->Status & PRINTER_STATUS_OUT_OF_MEMORY) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_OUTPUT_BIN_FULL, (ppi2->Status & PRINTER_STATUS_OUTPUT_BIN_FULL) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PAGE_PUNT, (ppi2->Status & PRINTER_STATUS_PAGE_PUNT) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PAPER_JAM, (ppi2->Status & PRINTER_STATUS_PAPER_JAM) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PAPER_OUT, (ppi2->Status & PRINTER_STATUS_PAPER_OUT) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PAPER_PROBLEM, (ppi2->Status & PRINTER_STATUS_PAPER_PROBLEM) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PRINTING, (ppi2->Status & PRINTER_STATUS_PRINTING) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_PROCESSING, (ppi2->Status & PRINTER_STATUS_PROCESSING) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_TONER_LOW, (ppi2->Status & PRINTER_STATUS_TONER_LOW) ? "ON" : "off");
///	SetDlgItemText(hwnd, IDC_STATUS_UNAVAILABLE, (ppi2->Status & PRINTER_STATUS_UNAVAILABLE) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_USER_INTERVENTION, (ppi2->Status & PRINTER_STATUS_USER_INTERVENTION) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_WAITING, (ppi2->Status & PRINTER_STATUS_WAITING) ? "ON" : "off");
	SetDlgItemText(hwnd, IDC_STATUS_WARMING_UP, (ppi2->Status & PRINTER_STATUS_WARMING_UP) ? "ON" : "off");
}

/**
 * �v�����^���_�C�A���O(���[�h���X)�̃E�B���h�E�v���V�[�W��
 */

BOOL CALLBACK PInfo_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_USER_UPDATE:
		OnUserUpdate(hwnd);
		return TRUE;

	case WM_INITDIALOG:
		return FALSE;

	case WM_DESTROY:
		g_hwndPInfo = NULL;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			DestroyWindow(hwnd);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

//-----------------------------------------------------------------------
// WM_COMMAND: IDC_PURGE�{�^���̏���
// �I������Ă���v�����^��Job��S�č폜����

static BOOL OnPurge(HWND hwnd)
{
	HWND hwndCtrlJobs = GetDlgItem(hwnd, IDC_JOBS);

	// �I�𒆂̃v�����^�̖��O���擾
	std::string strPrinterName = GetSelectedPrinterName(hwnd);

	// JOB���X�g�{�b�N�X�ɓo�^����Ă���JOB ID�Ńh�L�������g�̍폜�����݂�
	HANDLE hPrinter;
	if (OpenPrinter((LPTSTR)strPrinterName.c_str(), &hPrinter, NULL)) {
		for (int i = 0; i < ListBox_GetCount(hwndCtrlJobs); i++) {
			SetJob(hPrinter, ListBox_GetItemData(hwndCtrlJobs, i), 0, NULL, JOB_CONTROL_CANCEL);
		}
		ClosePrinter(hPrinter);
	}

	return TRUE;
}

//-----------------------------------------------------------------------
// WM_INITDIALOG���b�Z�[�W����

static BOOL OnInitDialog(HWND hwnd)
{
	// �E�B���h�E��screen�̐^���Ɉړ�
	kjm::win32::MoveCenterWorkarea(hwnd);
	
	// 1��ڂ͏����󂯎��̂ɕK�v�ȃo�b�t�@�T�C�Y�����Ⴄ
	// cbNeeded�ɕK�v�ȃo�b�t�@�T�C�Y������
	DWORD dwLevel = 2;
	PRINTER_INFO_2* pInfo;
	DWORD cbNeeded;
	DWORD cbReturned;
	BOOL ret;

	ret = EnumPrinters(
		PRINTER_ENUM_CONNECTIONS|PRINTER_ENUM_LOCAL,
		NULL,
		dwLevel,
		NULL,
		0,
		&cbNeeded,
		&cbReturned);

	// �����󂯎��̂ɕK�v�ȃo�b�t�@���m��
	pInfo = (PRINTER_INFO_2*)malloc(cbNeeded);

	// 2��ڂ͏����󂯎��
	ret = EnumPrinters(
		PRINTER_ENUM_CONNECTIONS|PRINTER_ENUM_LOCAL,
		NULL,
		dwLevel,
		(LPBYTE)pInfo,
		cbNeeded,
		&cbNeeded,
		&cbReturned);

	// �v�����^�ꗗ���X�g�{�b�N�X�ɒǉ�
	HWND hwndCtrl = GetDlgItem(hwnd, IDC_PRINTERS);
	for (DWORD i = 0; i < cbReturned; i++) {
		ComboBox_AddString(hwndCtrl, pInfo[i].pPrinterName);
	}

	// �f�t�H���g�v�����^��I����Ԃɂ���
	char szDefaultPrinter[256];
	char *szDevice, *szDriver, *szOutput;
	GetProfileString("windows", "device", "", szDefaultPrinter, sizeof(szDefaultPrinter));
	szDevice = strtok(szDefaultPrinter, "," );
	szDriver = strtok(NULL, ", ");
	szOutput = strtok(NULL, ", ");
	ComboBox_SelectString(hwndCtrl, 0, szDevice);

	free(pInfo);

	// 1�b����Job�̍X�V������^�C�}�[���N��
	SetTimer(hwnd, ID_TIMER, ELAPSE, NULL);

	return FALSE;	// focus�̓V�X�e���ɔC����
}

//-----------------------------------------------------------------------
// WM_TIMER���b�Z�[�W����

static BOOL OnTimer(HWND hwnd)
{
	HWND hwndCtrlJobs = GetDlgItem(hwnd, IDC_JOBS);

	// �Ƃ肠�����AJob�ꗗ����ɂ���
	ListBox_ResetContent(hwndCtrlJobs);

	// �I�𒆂̃v�����^�̖��O���擾
	std::string strPrinterName = GetSelectedPrinterName(hwnd);

	HANDLE hPrinter;
	if (OpenPrinter((LPTSTR)strPrinterName.c_str(), &hPrinter, NULL)) {
		DWORD cbNeeded;
		DWORD cbReturned;
		JOB_INFO_1* pJobInfo;

		// 1��ڂ͏����󂯎��̂ɕK�v�ȃo�b�t�@�T�C�Y�����Ⴄ
		// cbNeeded�ɕK�v�ȃo�b�t�@�T�C�Y������
		EnumJobs(hPrinter, 0, 10, 1, NULL, 0, &cbNeeded, &cbReturned);

		// �����󂯎��̂ɕK�v�ȃo�b�t�@���m��
		pJobInfo = (JOB_INFO_1*)malloc(cbNeeded);

		// 2��ڂ͏����󂯎��
		EnumJobs(hPrinter, 0, 10, 1, (LPBYTE)pJobInfo, cbNeeded, &cbNeeded, &cbReturned);

		// Job�ꗗ�ɓo�^�B
		// ���̂Ƃ��A�֘A�f�[�^��JobId��ݒ肷��
		for (DWORD i = 0; i < cbReturned; i++) {
			ListBox_SetItemData(hwndCtrlJobs, ListBox_AddString(hwndCtrlJobs, pJobInfo[i].pDocument), pJobInfo[i].JobId);
		}

		free(pJobInfo);

		// �v�����^���_�C�A���O���o�Ă���Ƃ��́A��������X�V����
		if (g_hwndPInfo) {
			DWORD cbNeeded;
			//PRINTER_INFO_2* ppi2;

			// 1��ڂ͏����󂯎��̂ɕK�v�ȃo�b�t�@�T�C�Y�������炤
			GetPrinter(hPrinter, 2, NULL, 0, &cbNeeded);

			g_printerInfo2.resize(cbNeeded);

			// 2��ڂ͎��ۂɏ����擾����
			GetPrinter(hPrinter, 2, (LPBYTE)&g_printerInfo2[0], cbNeeded, &cbNeeded);

			// �X�V�w��
			SendMessage(g_hwndPInfo, WM_USER_UPDATE, 0, 0);
		}

		ClosePrinter(hPrinter);
	}

	return TRUE;
}

/**
 * DeviceCaps��񕶎�����쐬
 */

void CreateDevCapsText(HDC hdc, std::stringstream& ss)
{
	int n;

	ss << "DRIVERVERSION: " << GetDeviceCaps(hdc, DRIVERVERSION) << "\n";

	switch (GetDeviceCaps(hdc, TECHNOLOGY)) {
	case DT_PLOTTER:	ss << "TECHNOLOGY: �x�N�^�v���b�^\n";		break;
	case DT_RASDISPLAY:	ss << "TECHNOLOGY: ���X�^�f�B�X�v���C\n";	break;
	case DT_RASPRINTER:	ss << "TECHNOLOGY: ���X�^�v�����^\n";		break;
	case DT_RASCAMERA:	ss << "TECHNOLOGY: ���X�^�J����\n";			break;
	case DT_CHARSTREAM:	ss << "TECHNOLOGY: �����X�g���[��\n";		break;
	case DT_METAFILE:	ss << "TECHNOLOGY: ���^�t�@�C��\n";			break;
	case DT_DISPFILE:	ss << "TECHNOLOGY: �f�B�X�v���C�t�@�C���f�B�X�v���C�t�@�C��\n";	break;
	default:	ss << "TECHNOLOGY: unknown\n";	break;
	}

	ss << "SIZE: (" << GetDeviceCaps(hdc, HORZSIZE) << "," << GetDeviceCaps(hdc, VERTSIZE) << ") mm\n";
	ss << "RES: (" << GetDeviceCaps(hdc, HORZRES) << "," << GetDeviceCaps(hdc, VERTRES) << ") �s�N�Z��\n";
	ss << "LOGPIXELS: (" << GetDeviceCaps(hdc, LOGPIXELSX) << "," << GetDeviceCaps(hdc, LOGPIXELSY) << ") �s�N�Z��(�_���C���`������)\n";
	ss << "BITSPIXEL: " << GetDeviceCaps(hdc, BITSPIXEL) << "�r�b�g(�v���[������)\n";
	ss << "PLANES: " << GetDeviceCaps(hdc, PLANES) << "\n";
	ss << "NUMBRUSHES: " << GetDeviceCaps(hdc, NUMBRUSHES) << "\n";
	ss << "NUMPENS: " << GetDeviceCaps(hdc, NUMPENS) << "\n";
	ss << "NUMFONTS: " << GetDeviceCaps(hdc, NUMFONTS) << "\n";
	ss << "NUMCOLORS: " << GetDeviceCaps(hdc, NUMCOLORS) << "\n";
	ss << "ASPECT: (" << GetDeviceCaps(hdc, ASPECTX) << "," << GetDeviceCaps(hdc, ASPECTY) << ")\n";
	ss << "ASPECTXY: " << GetDeviceCaps(hdc, ASPECTXY) << "\n";
	ss << "CLIPCAPS: " << GetDeviceCaps(hdc, CLIPCAPS) << "\n";
	ss << "SIZEPALETTE: " << GetDeviceCaps(hdc, SIZEPALETTE) << "\n";
	ss << "NUMRESERVED: " << GetDeviceCaps(hdc, NUMRESERVED) << "\n";
	ss << "COLORRES: " << GetDeviceCaps(hdc, COLORRES) << "\n";
	ss << "PHYSICAL: (" << GetDeviceCaps(hdc, PHYSICALWIDTH) << "," << GetDeviceCaps(hdc, PHYSICALHEIGHT) << ")\n";
	ss << "PHYSICALOFFSET: (" << GetDeviceCaps(hdc, PHYSICALOFFSETX) << "," << GetDeviceCaps(hdc, PHYSICALOFFSETY) << ")\n";
	ss << "VREFRESH: " << GetDeviceCaps(hdc, VREFRESH) << "\n";
	ss << "DESKTOPHORZRES: " << GetDeviceCaps(hdc, DESKTOPHORZRES) << "�s�N�Z��\n";
	ss << "DESKTOPVERTRES: " << GetDeviceCaps(hdc, DESKTOPVERTRES) << "�s�N�Z��\n";
	ss << "BLTALIGNMENT: " << GetDeviceCaps(hdc, BLTALIGNMENT) << "\n";

	n = GetDeviceCaps(hdc, RASTERCAPS);
	
	ss << "RASTERCAPS:\n";
	ss << "  RC_BANDING: " << ((n & RC_BANDING) ? "on" : "off") << "\n";
	ss << "  RC_BITBLT: " << ((n & RC_BITBLT) ? "on" : "off") << "\n";
	ss << "  RC_BITMAP64: " << ((n & RC_BITMAP64) ? "on" : "off") << "\n";
	ss << "  RC_DI_BITMAP: " << ((n & RC_DI_BITMAP) ? "on" : "off") << "\n";
	ss << "  RC_DIBTODEV: " << ((n & RC_DIBTODEV) ? "on" : "off") << "\n";
	ss << "  RC_FLOODFILL: " << ((n & RC_FLOODFILL) ? "on" : "off") << "\n";
	ss << "  RC_GDI20_OUTPUT: " << ((n & RC_GDI20_OUTPUT) ? "on" : "off") << "\n";
	ss << "  RC_PALETTE: " << ((n & RC_PALETTE) ? "on" : "off") << "\n";
	ss << "  RC_SCALING: " << ((n & RC_SCALING) ? "on" : "off") << "\n";
	ss << "  RC_STRETCHBLT: " << ((n & RC_STRETCHBLT) ? "on" : "off") << "\n";
	ss << "  RC_STRETCHDIB: " << ((n & RC_STRETCHDIB) ? "on" : "off") << "\n";

	n = GetDeviceCaps(hdc, CURVECAPS);

	ss << "CURVECAPS:\n";
	ss << "  CC_NONE: " << ((n & CC_NONE) ? "on" : "off") << "\n";
	ss << "  CC_CIRCLES: " << ((n & CC_CIRCLES) ? "on" : "off") << "\n";
	ss << "  CC_PIE: " << ((n & CC_PIE) ? "on" : "off") << "\n";
	ss << "  CC_CHORD: " << ((n & CC_CHORD) ? "on" : "off") << "\n";
	ss << "  CC_ELLIPSES: " << ((n & CC_ELLIPSES) ? "on" : "off") << "\n";
	ss << "  CC_WIDE: " << ((n & CC_WIDE) ? "on" : "off") << "\n";
	ss << "  CC_STYLED: " << ((n & CC_STYLED) ? "on" : "off") << "\n";
	ss << "  CC_WIDESTYLED: " << ((n & CC_WIDESTYLED) ? "on" : "off") << "\n";
	ss << "  CC_INTERIORS: " << ((n & CC_INTERIORS) ? "on" : "off") << "\n";
	ss << "  CC_ROUNDRECT: " << ((n & CC_ROUNDRECT) ? "on" : "off") << "\n";

	n = GetDeviceCaps(hdc, LINECAPS);

	ss << "LINECAPS:\n";
	ss << "  LC_NONE: " << ((n & LC_NONE) ? "on" : "off") << "\n";
	ss << "  LC_POLYLINE: " << ((n & LC_POLYLINE) ? "on" : "off") << "\n";
	ss << "  LC_MARKER: " << ((n & LC_MARKER) ? "on" : "off") << "\n";
	ss << "  LC_POLYMARKER: " << ((n & LC_POLYMARKER) ? "on" : "off") << "\n";
	ss << "  LC_WIDE: " << ((n & LC_WIDE) ? "on" : "off") << "\n";
	ss << "  LC_STYLED: " << ((n & LC_STYLED) ? "on" : "off") << "\n";
	ss << "  LC_WIDESTYLED: " << ((n & LC_WIDESTYLED) ? "on" : "off") << "\n";
	ss << "  LC_INTERIORS: " << ((n & LC_INTERIORS) ? "on" : "off") << "\n";

	n = GetDeviceCaps(hdc, POLYGONALCAPS);

	ss << "POLYGONALCAPS:\n";
	ss << "  PC_NONE: " << ((n & PC_NONE) ? "on" : "off") << "\n";
	ss << "  PC_POLYGON: " << ((n & PC_POLYGON) ? "on" : "off") << "\n";
	ss << "  PC_RECTANGLE: " << ((n & PC_RECTANGLE) ? "on" : "off") << "\n";
	ss << "  PC_WINDPOLYGON: " << ((n & PC_WINDPOLYGON) ? "on" : "off") << "\n";
	ss << "  PC_SCANLINE: " << ((n & PC_SCANLINE) ? "on" : "off") << "\n";
	ss << "  PC_WIDE: " << ((n & PC_WIDE) ? "on" : "off") << "\n";
	ss << "  PC_STYLED: " << ((n & PC_STYLED) ? "on" : "off") << "\n";
	ss << "  PC_WIDESTYLED: " << ((n & PC_WIDESTYLED) ? "on" : "off") << "\n";
	ss << "  PC_INTERIORS: " << ((n & PC_INTERIORS) ? "on" : "off") << "\n";

	n = GetDeviceCaps(hdc, TEXTCAPS);

	ss << "TEXTCAPS:\n";
	ss << "  TC_OP_CHARACTER: " << ((n & TC_OP_CHARACTER) ? "on" : "off") << "\n";
	ss << "  TC_OP_STROKE: " << ((n & TC_OP_STROKE) ? "on" : "off") << "\n";
	ss << "  TC_CP_STROKE: " << ((n & TC_CP_STROKE) ? "on" : "off") << "\n";
	ss << "  TC_CR_90: " << ((n & TC_CR_90) ? "on" : "off") << "\n";
	ss << "  TC_CR_ANY: " << ((n & TC_CR_ANY) ? "on" : "off") << "\n";
	ss << "  TC_SF_X_YINDEP: " << ((n & TC_SF_X_YINDEP) ? "on" : "off") << "\n";
	ss << "  TC_SA_DOUBLE: " << ((n & TC_SA_DOUBLE) ? "on" : "off") << "\n";
	ss << "  TC_SA_INTEGER: " << ((n & TC_SA_INTEGER) ? "on" : "off") << "\n";
	ss << "  TC_SA_CONTIN: " << ((n & TC_SA_CONTIN) ? "on" : "off") << "\n";
	ss << "  TC_EA_DOUBLE: " << ((n & TC_EA_DOUBLE) ? "on" : "off") << "\n";
	ss << "  TC_IA_ABLE: " << ((n & TC_IA_ABLE) ? "on" : "off") << "\n";
	ss << "  TC_UA_ABLE: " << ((n & TC_UA_ABLE) ? "on" : "off") << "\n";
	ss << "  TC_SO_ABLE: " << ((n & TC_SO_ABLE) ? "on" : "off") << "\n";
	ss << "  TC_RA_ABLE: " << ((n & TC_RA_ABLE) ? "on" : "off") << "\n";
	ss << "  TC_VA_ABLE: " << ((n & TC_VA_ABLE) ? "on" : "off") << "\n";
	ss << "  TC_SCROLLBLT: " << ((n & TC_SCROLLBLT) ? "on" : "off") << "\n";

	ss << std::ends;
}

/**
 * IDM_PINFO_VIEW on WM_COMMAND ���b�Z�[�W����
 */

void OnPInfoPrint(HWND hwnd)
{
	std::string::size_type spos, epos;
	TEXTMETRIC tm;
	std::stringstream buf;
	std::string strBuf;

	HDC hdcPrint = GetSelectedPrinterDC(hwnd);
	if (hdcPrint) {

		// DeviceCaps��񕶎�����쐬
		CreateDevCapsText(hdcPrint, buf);
		strBuf = buf.str();

		// �f�t�H���g�󎚃t�H���g�̃e�L�X�g�����擾
		GetTextMetrics(hdcPrint, &tm);

		// ����h�L�������g����ݒ�
		DOCINFO di = {sizeof(DOCINFO), "DevCaps���", NULL, NULL, 0};

		spos = epos = 0;

		// ����J�n
		if (StartDoc(hdcPrint, &di) > 0) {
			int x, y;

			do {
				x = y = 0;

				if (StartPage(hdcPrint) > 0) {

					int horz = GetDeviceCaps(hdcPrint, HORZRES);
					int vert = GetDeviceCaps(hdcPrint, VERTRES);

					RECT rc = {0, 0, horz, vert};
					FrameRect(hdcPrint, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

					HBRUSH hbr;
					
					RECT rc2 = {horz - 102, 2, horz - 2, 102};
					FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(255, 0, 0)));
					DeleteObject(hbr);
					
					OffsetRect(&rc2, 0, 102);
					FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(0, 255, 0)));
					DeleteObject(hbr);
					
					OffsetRect(&rc2, 0, 102);
					FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(0, 0, 255)));
					DeleteObject(hbr);
					
					OffsetRect(&rc2, 0, 102);
					FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(255, 255, 0)));
					DeleteObject(hbr);
					
					OffsetRect(&rc2, 0, 102);
					FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(255, 0, 255)));
					DeleteObject(hbr);
					
					OffsetRect(&rc2, 0, 102);
					FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(0, 255, 255)));
					DeleteObject(hbr);

					for (int nGray = 0; nGray <= 256; nGray += 32) {
						OffsetRect(&rc2, 0, 102);
						FillRect(hdcPrint, &rc2, hbr = CreateSolidBrush(RGB(min(nGray, 255), min(nGray, 255), min(nGray, 255))));
						DeleteObject(hbr);
					}

					do {
						epos = strBuf.find('\n', spos);
						if (epos == std::string::npos) {
							break;
						}

						std::string strTemp = strBuf.substr(spos, epos - spos);
						TextOut(hdcPrint, x + 2, y + 2, strTemp.c_str(), strTemp.length());
						spos = epos + 1;
						y += tm.tmHeight + tm.tmExternalLeading;
					} while (y + tm.tmHeight + tm.tmExternalLeading + 2 < vert);

					EndPage(hdcPrint);
				}
			} while (epos != std::string::npos);

			EndDoc(hdcPrint);
		}

		DeleteDC(hdcPrint);
	}
}

//-----------------------------------------------------------------------
// Main Dialog�v���V�[�W��

static BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return OnInitDialog(hwnd);
	case WM_TIMER:
		return OnTimer(hwnd);
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			KillTimer(hwnd, ID_TIMER);
			EndDialog(hwnd, 0);
			return TRUE;

		case IDC_PURGE:
			return OnPurge(hwnd);

		case IDC_PRINTERS:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				// �v�����^�̑I�����ς������
				// ������Job�ꗗ���X�V������
				return OnTimer(hwnd);
			}
			break;

		case IDM_PINFO_VIEW:
			g_hwndPInfo = CreateDialog(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_PINFO), hwnd, PInfo_DlgProc);
			return TRUE;

		case IDM_PINFO_PRINT:
			OnPInfoPrint(hwnd);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------
// �v���O������X�^�[�g

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)Main_DlgProc);
}
