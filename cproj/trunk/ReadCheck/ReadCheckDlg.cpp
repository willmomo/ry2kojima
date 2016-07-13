// ReadCheckDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "ReadCheck.h"
#include "ReadCheckDlg.h"
#include "FileFindEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_THREAD_WATCHER	1	// �X���b�h�Ď��p�^�C�}�[ID

//---------------------------------------------------------------------------
// readcheck thread
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ���ȒP���O
//---------------------------------------------------------------------------
void loglog(LPCTSTR pszFormat, ...) {
	va_list marker;

	va_start(marker, pszFormat);
	FILE* fp = NULL;
	if (_tfopen_s(&fp, _T("ReadCheck.log"), _T("a+")) == 0) {
		SYSTEMTIME st;
		GetLocalTime(&st);

		_ftprintf(fp, _T("[%04d/%02d/%02d %02d:%02d:%02d:%03d] "),
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		_vftprintf(fp, pszFormat, marker);
		_ftprintf(fp, _T("\n"));

		fclose(fp);
	}
	va_end(marker);
}

//---------------------------------------------------------------------
// �w��t�@�C���̓ǂݍ��݃`�F�b�N���s��
//---------------------------------------------------------------------
char* error_string(DWORD dwError) {

	static char s_msg[8192];

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		0,
		s_msg,
		8192,
		NULL);

	return s_msg;
}

#define TS_STRMSG		0x01
#define TS_ONEPOS		0x02
#define TS_TOTALPOS		0x04
#define TS_RESULTLIST	0x08

bool read_check(CReadCheckDlg* pDlg, LPCTSTR fname, DWORD bytesPerSector, __int64& totalProg) {
	bool has_error = false;
	CString strMsg;
	strMsg.Format(_T("checking '%s'"), fname);

	pDlg->m_cs.Lock();
	///if (!pDlg->m_readCheckAbort) pDlg->m_statusLabel.SetWindowText(strMsg);
	///if (!pDlg->m_readCheckAbort) pDlg->m_progressOne.SetPos(0);
	pDlg->m_ts_strMsg = strMsg;
	pDlg->m_ts_onePos = 0;
	pDlg->m_cs.Unlock();
	pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_ONEPOS | TS_STRMSG);

	HANDLE hFile = CreateFile(fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {

		// �t�@�C���A�N�Z�X�̃o�C�g�����Z�N�^�T�C�Y�̐����{�ɍ��킹��
		// �o�b�t�@ �A�h���X����������ł̃f�B�X�N�̃Z�N�^���E�ɒ�������
		char* buffer = new char[bytesPerSector * 2];
		char* p;
		p = (char *) ((DWORD) (buffer + bytesPerSector - 1) & ~(bytesPerSector - 1));

		DWORD bytesRead;

		DWORD dwSizeHigh = 0;
		DWORD dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		LARGE_INTEGER li1 = {dwSizeLow, dwSizeHigh};
		LARGE_INTEGER li2 = {0, 0};

		while (!pDlg->m_readCheckAbort && li1.QuadPart > 0) {
			if (ReadFile(hFile, p, bytesPerSector, &bytesRead, NULL) == FALSE) {
				DWORD dwError = GetLastError();
				CString strMsg;
				strMsg.Format(_T("[ng] error %u: %s ... %s"), dwError, error_string(dwError), fname);
				kjm::GetLog().writeInfo(_T("[ng] error %u: %s ... %s"), dwError, CString(error_string(dwError)).Trim(), fname);
				
				pDlg->m_cs.Lock();
				///if (!pDlg->m_readCheckAbort) pDlg->m_statusLabel.SetWindowText(strMsg);
				///if (!pDlg->m_readCheckAbort) pDlg->m_resultList.AddString(strMsg);
				pDlg->m_ts_strMsg = strMsg;
				pDlg->m_ts_resultList.AddTail(strMsg);
				pDlg->m_cs.Unlock();
				pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_STRMSG | TS_RESULTLIST);

				has_error = true;
				break;
			}

			li2.QuadPart += bytesRead;
			totalProg += bytesRead;

			pDlg->m_cs.Lock();
			///if (!pDlg->m_readCheckAbort) pDlg->m_progressOne.SetPos(li2.QuadPart * 100 / li1.QuadPart);
			///if (!pDlg->m_readCheckAbort) pDlg->m_progressTotal.SetPos(totalProg * 100 / totalSize);
			pDlg->m_ts_onePos = li2.QuadPart * 100 / li1.QuadPart;
			pDlg->m_ts_totalPos = totalProg * 100 / pDlg->m_ts_totalSize;
			pDlg->m_cs.Unlock();
			pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_ONEPOS | TS_TOTALPOS);

			if (bytesRead != bytesPerSector) {
				strMsg.Format(_T("[ok] ... %s"), fname);
				///kjm::GetLog().writeInfo(strMsg);

				pDlg->m_cs.Lock();
				///if (!pDlg->m_readCheckAbort) pDlg->m_statusLabel.SetWindowText(strMsg);
				///if (!pDlg->m_readCheckAbort) pDlg->m_resultList.SetCurSel(pDlg->m_resultList.AddString(strMsg));
				///if (!pDlg->m_readCheckAbort) pDlg->m_progressTotal.SetPos(totalProg * 100 / totalSize);
				///pDlg->m_ts_resultList.AddTail(strMsg);
				pDlg->m_ts_totalPos = totalProg * 100 / pDlg->m_ts_totalSize;
				pDlg->m_cs.Unlock();
				pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_RESULTLIST | TS_TOTALPOS);

				break;
			}
		}

		delete[] buffer;

		CloseHandle(hFile);
	} else {
		DWORD dwError = GetLastError();
		CString strMsg, strMsg2;

		strMsg.Format(_T("�t�@�C�� '%s' ���A�I�[�v���ł��܂��� error: %u: %s"), fname, dwError, error_string(dwError));
		strMsg2.Format(_T("[NG] error %u : %s ... %s'"), dwError, error_string(dwError), fname);
		kjm::GetLog().writeInfo(_T("[NG] error %u : %s ... %s'"), dwError, CString(error_string(dwError)).Trim(), fname);

		pDlg->m_cs.Lock();
		///if (!pDlg->m_readCheckAbort) pDlg->m_statusLabel.SetWindowText(strMsg);
		pDlg->m_ts_resultList.AddTail(strMsg2);
		pDlg->m_ts_strMsg = strMsg;
		pDlg->m_cs.Unlock();
		pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_RESULTLIST | TS_STRMSG);

		if (dwError == ERROR_SHARING_VIOLATION || dwError == ERROR_ACCESS_DENIED) {
			// ���L�ᔽ�̂Ƃ��́A�������p�������܂��B
		} else {
			has_error = true;
		}
	}

	pDlg->m_cs.Lock();
	///if (!pDlg->m_readCheckAbort) pDlg->m_progressOne.SetPos(0);
	pDlg->m_ts_onePos = 0;
	pDlg->m_cs.Unlock();
	pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_ONEPOS);

	return has_error;
}


//---------------------------------------------------------------------------
// �Z�N�^�����擾���āAreadcheck���͂��߂�
//---------------------------------------------------------------------------
UINT read_check(LPVOID pParam) {
	kjm::GetLog().writeInfo(_T("read_check enter."));

	// �X���b�h�N�����́A�v���Z�X���X�^���o�C������܂�
	EXECUTION_STATE pes;
	pes = SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
	kjm::GetLog().writeInfo(_T("SetThreadExecutionState call result: 0x%08X"), pes);

	CReadCheckDlg* pDlg = (CReadCheckDlg*)pParam;

	//char temp[MAX_PATH * 2];
	//memset(temp, 0, sizeof(temp));
	//strncpy(temp, pDlg->m_pathName, PathSkipRoot(pDlg->m_pathName) - pDlg->m_pathName);
	CString temp = _T("*:\\");
	CString txt;
	pDlg->m_drives.GetLBText(pDlg->m_drives.GetCurSel(), txt);
	temp.SetAt(0, txt.GetAt(0));

	DWORD bytesPerSector;
	if (!GetDiskFreeSpace(temp, NULL, &bytesPerSector, NULL, NULL)) {
		DWORD dwError = GetLastError();
		CString strMsg;
		strMsg.Format(_T("�Z�N�^�T�C�Y���擾�ł��܂���: %u: %s"), dwError, error_string(dwError));
		pDlg->m_statusLabel.SetWindowText(strMsg);
		return 1;
	} else {
		CString strMsg;
		strMsg.Format(_T("1�Z�N�^������̃o�C�g��: %u �o�C�g"), bytesPerSector);
		pDlg->m_statusLabel.SetWindowText(strMsg);
	}

	// �S�Ẵt�@�C�������X�g�A�b�v����
	pDlg->m_ts_totalSize = 0;
	CList<CString, CString&> fileList;		// �`�F�b�N����t�@�C���̐�
	CList<CString, CString&> folderList;	// ��������t�H���_�̐�

	folderList.AddTail(temp);
	while (!pDlg->m_readCheckAbort && !folderList.IsEmpty()) {
		CString strFolder = folderList.RemoveHead();
		CFileFindEx ff;
		TCHAR temp[MAX_PATH];
		PathAppend(lstrcpy(temp, strFolder), _T("*.*"));
		BOOL bWorking = ff.FindFile(temp);
		while (!pDlg->m_readCheckAbort && bWorking) {
			bWorking = ff.FindNextFile();

			if (ff.IsDots())
				continue;

			if (ff.IsDirectory()) {
				folderList.AddTail(ff.GetFilePath());
			} else {

				fileList.AddTail(ff.GetFilePath());
				pDlg->m_ts_totalSize += ff.GetLength();

				CString msg;
				msg.Format(_T("�t�@�C�����X�g�쐬�� ... %d"), fileList.GetCount());
				///if (!pDlg->m_readCheckAbort) pDlg->m_statusLabel.SetWindowText(msg);
				pDlg->m_cs.Lock();
				pDlg->m_ts_strMsg = msg;
				pDlg->m_cs.Unlock();
				pDlg->PostMessage(WM_USER_THREAD_NOTIFY, TS_STRMSG);
			}
		}
	}


	// �񋓂��ꂽ�t�@�C�������ׂēǂݍ��݃e�X�g����

	pDlg->m_ts_has_error = false;

	__int64 totalProg = 0;
	while (!pDlg->m_readCheckAbort && !fileList.IsEmpty()) {
		CString strFile = fileList.RemoveHead();

		// �G���[�������Ă������͌p������(�Ō�ɕ\��)
		bool result = read_check(pDlg, strFile, bytesPerSector, totalProg);
		if (result) {
			pDlg->m_ts_has_error = result;
		}
		//pDlg->m_ts_has_error = read_check(pDlg, strFile, bytesPerSector, totalProg);
		//if (pDlg->m_ts_has_error) {
		//	break;
		//}
	}

	// �X���b�h�I�������A�v���Z�X���X�^���o�C������Ԃɂ��܂��B
	pes = SetThreadExecutionState(ES_CONTINUOUS);
	kjm::GetLog().writeInfo(_T("SetThreadExecutionState call result: 0x%08X"), pes);

	kjm::GetLog().writeInfo(_T("read_check leave."));
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadCheckDlg �_�C�A���O

CReadCheckDlg::CReadCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadCheckDlg::IDD, pParent)
{
	m_readCheckThread = NULL;
	m_readCheckAbort = false;

	//{{AFX_DATA_INIT(CReadCheckDlg)
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReadCheckDlg)
	DDX_Control(pDX, IDC_DRIVES, m_drives);
	DDX_Control(pDX, IDC_PROGRESS_ONE, m_progressOne);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_progressTotal);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_RESULT_LIST, m_resultList);
	DDX_Control(pDX, IDC_STATUS_LABEL, m_statusLabel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, ID_SUSPEND_RESUME, m_btnSuspResu);
}

BEGIN_MESSAGE_MAP(CReadCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CReadCheckDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_THREAD_NOTIFY, CReadCheckDlg::OnUserThreadNotify)	// (MFC WM_USER) ���b�Z�[�W�}�b�v��ǉ�
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CReadCheckDlg::OnBnClickedButtonSave)
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	ON_BN_CLICKED(IDOK, &CReadCheckDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_SUSPEND_RESUME, &CReadCheckDlg::OnBnClickedSuspendResume)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadCheckDlg ���b�Z�[�W �n���h��

BOOL CReadCheckDlg::OnInitDialog()
{
	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnInitDialog enter."));

	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	if (SetProcessShutdownParameters(0x04FF, 0) == FALSE) {
		DWORD dwErr = GetLastError();
		kjm::GetLog().writeError(_T("SetProcessShutdownParameters error %d: %s"), dwErr, kjm::util::formatMessageBySystem(dwErr).c_str());
	}

	// �_���h���C�u�ŃR���{�{�b�N�X������������
	CDriveInfoArray drives = CDriveInfo::GetDrives();
	for (int i = 0; i < drives.GetSize(); i++) {
		CString msg;

		switch (drives[i].DriveType()) {
		case Ram:
			msg.Format(_T("%c �h���C�u (RAM�f�B�X�N)"), drives[i].Name()[0]);
			break;
		case CDRom:
			msg.Format(_T("%c �h���C�u (CD-ROM�h���C�u)"), drives[i].Name()[0]);
			break;
		case Network:
			msg.Format(_T("%c �h���C�u (�l�b�g���[�N�h���C�u)"), drives[i].Name()[0]);
			break;
		case Fixed:
			msg.Format(_T("%c �h���C�u (�Œ�f�B�X�N)"), drives[i].Name()[0]);
			break;
		case Removable:
			msg.Format(_T("%c �h���C�u (�����[�o�u���f�B�X�N)"), drives[i].Name()[0]);
			break;
		case NoRootDirectory:
			msg.Format(_T("%c �h���C�u (���[�g�f�B���N�g�������݂��Ȃ�)"), drives[i].Name()[0]);
			break;
		default:
			msg.Format(_T("%c �h���C�u (�s���ȃf�B�X�N)"), drives[i].Name()[0]);
			break;
		}

		m_drives.AddString(msg);
	}
	m_drives.SetCurSel(0);

	// total �v���O���X�o�[�̐F���I�����W���ۂ�����
	m_progressTotal.SendMessage(PBM_SETBARCOLOR, 0, RGB(255, 160, 80));

	// �� �v���O���X�o�[�������Ɛɂ���
	m_progressOne.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 0, 255));

	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnInitDialog leave."));
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CReadCheckDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CReadCheckDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CReadCheckDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CReadCheckDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	//m_btnOK.EnableWindow(FALSE);
	if (m_readCheckThread == NULL) {
		m_btnOK.SetWindowText(_T("���~"));
		m_resultList.ResetContent();
		m_resultList.SetHorizontalExtent(0);

		UpdateData(TRUE);
		m_readCheckThread = AfxBeginThread(read_check, this);
		m_readCheckThread->m_bAutoDelete = FALSE;

		m_drives.EnableWindow(FALSE);

		m_btnSuspResu.SetWindowText(_T("���f"));
		m_btnSuspResu.EnableWindow();

		SetTimer(ID_THREAD_WATCHER, 500, NULL);
	} else {
		KillTimer(ID_THREAD_WATCHER);

		kjm::GetLog().writeInfo(_T("�I���҂� �J�n"));

		m_cs.Lock();
		m_readCheckAbort = true;
		m_cs.Unlock();

		DWORD dwRet = WaitForSingleObject(m_readCheckThread->m_hThread, INFINITE);

		kjm::GetLog().writeInfo(_T("�I���҂� ����"));

		delete m_readCheckThread;
		m_readCheckThread = NULL;
		m_readCheckAbort = false;

		m_btnSuspResu.SetWindowText(_T("���f/�ĊJ"));
		m_btnSuspResu.EnableWindow(FALSE);

		m_btnOK.SetWindowText(_T("�J�n"));
		m_statusLabel.SetWindowText(_T(""));
		m_drives.EnableWindow(TRUE);
		m_progressTotal.SetPos(0);
		m_progressOne.SetPos(0);
	}
	// �v���Z�X�͏I�������Ȃ�
///	CDialog::OnOK();
}

void CReadCheckDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	DWORD dwExitCode;
	BOOL bRet = GetExitCodeThread(m_readCheckThread->m_hThread, &dwExitCode);
	if (bRet == 0) {
		DWORD dwExitCode = 0;
	}

	if (dwExitCode != STILL_ACTIVE) {
		KillTimer(ID_THREAD_WATCHER);

		delete m_readCheckThread;
		m_readCheckThread = NULL;

		if (!m_ts_has_error) {
			m_resultList.SetCurSel(m_resultList.AddString(_T("�G���[�́A����܂���ł����B")));
			MessageBox(_T("�G���[�́A����܂���ł����B"), NULL, MB_ICONINFORMATION);
		} else {
			m_resultList.SetCurSel(m_resultList.AddString(_T("�G���[�I�����܂����B")));
			MessageBox(_T("�G���[�I�����܂����B"), NULL, MB_ICONERROR);
		}

		m_btnOK.SetWindowText(_T("�J�n"));
		m_statusLabel.SetWindowText(_T(""));
		m_drives.EnableWindow(TRUE);
		m_progressTotal.SetPos(0);
		m_progressOne.SetPos(0);
	} else {
	}

	CDialog::OnTimer(nIDEvent);
}

void CReadCheckDlg::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	// �X���b�h���쒆�́A�X���b�h�̏I����҂�
	if (m_readCheckThread != NULL) {
		KillTimer(ID_THREAD_WATCHER);

		m_cs.Lock();
		m_readCheckAbort = true;
		m_cs.Unlock();

		WaitForSingleObject(m_readCheckThread->m_hThread, INFINITE);
		delete m_readCheckThread;
		m_readCheckThread = NULL;
		m_readCheckAbort = false;

		m_btnOK.SetWindowText(_T("�J�n"));
		m_statusLabel.SetWindowText(_T(""));
		m_drives.EnableWindow(TRUE);
		m_progressTotal.SetPos(0);
		m_progressOne.SetPos(0);
	}

	CDialog::OnCancel();
}

// (MFC WM_USER) ���\�b�h�̎���
LRESULT CReadCheckDlg::OnUserThreadNotify(WPARAM wParam, LPARAM lParam) {
	if (m_readCheckThread == NULL) return 0;

	m_cs.Lock();
	if (wParam & TS_STRMSG)		m_statusLabel.SetWindowText(m_ts_strMsg);
	if (wParam & TS_ONEPOS)		m_progressOne.SetPos(m_ts_onePos);
	if (wParam & TS_TOTALPOS)	m_progressTotal.SetPos(m_ts_totalPos);
	
	while (!m_ts_resultList.IsEmpty()) {
		CString strMsg = m_ts_resultList.RemoveHead();

		CDC* pDC = m_resultList.GetDC();

		// �f�o�C�X�R���e�L�X�g�Ƀ��X�g�{�b�N�X�̃t�H���g��ݒ�
		CFont* pOldFont = pDC->SelectObject(m_resultList.GetFont());

		int newSize = pDC->GetTextExtent(strMsg).cx;
		newSize = newSize * 105 / 100;

		pDC->SelectObject(pOldFont);
		m_resultList.ReleaseDC(pDC);

		int nowSize = m_resultList.GetHorizontalExtent();
		if (nowSize < newSize) {
			m_resultList.SetHorizontalExtent(newSize);
		}

		m_resultList.SetCurSel(m_resultList.AddString(strMsg));
	}
	m_cs.Unlock();
	
	return 0;
}


void CReadCheckDlg::OnBnClickedButtonSave()
{
	// C:\ReadCheck.txt���J���ă��X�g�̓��e��ۑ�����B
	CFile file(_T("c:\\ReadCheck.txt"), CFile::modeWrite | CFile::modeCreate);

	for (int i = 0; i < m_resultList.GetCount(); i++) {
		CString text;
		m_resultList.GetText(i, text);
		file.Write((LPCTSTR)text, lstrlen((LPCTSTR)text));
		file.Write(_T("\x0d\x0a"), 2);
	}

	file.Close();

	MessageBox(_T("C:\\ReadCheck.txt �ɕۑ����܂����B"), _T("ReadCheck"), MB_OK);
}

BOOL CReadCheckDlg::OnQueryEndSession()
{
	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnQueryEndSession enter?."));
	BOOL ret = FALSE;

	if (!CDialog::OnQueryEndSession())
		return ret;

	// TODO:  �����ɓ���ȃN�G���̏I���Z�b�V���� �R�[�h��ǉ����Ă��������B
	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnQueryEndSession enter."));

	// �X���b�h���쐬����Ă���Ƃ��́A���쒆���ǂ������m�F�B
	// ���쒆�ł���΁A�I�������ۂ���B
	// �ꗥ�A�I�������ۂ���̂ł�����Ɨ��\����
	if (m_readCheckThread) {
		DWORD dwExitCode;
		BOOL bRet = GetExitCodeThread(m_readCheckThread->m_hThread, &dwExitCode);
		if (bRet == 0) {
			DWORD dwExitCode = 0;
		}

		ret = (dwExitCode != STILL_ACTIVE) ? TRUE : FALSE;
	}

	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnQueryEndSession leave with %s."), (ret ? _T("TRUE") : _T("FALSE")));
	return ret;
}

void CReadCheckDlg::OnEndSession(BOOL bEnding)
{
	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnEndSession enter."));

	CDialog::OnEndSession(bEnding);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	kjm::GetLog().writeInfo(_T("CReadCheckDlg::OnEndSession leave."));
}

void CReadCheckDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	OnOK();
}

void CReadCheckDlg::OnBnClickedSuspendResume()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}
