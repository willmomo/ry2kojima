// PipeFormatDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "PipeFormat.h"
#include "PipeFormatDlg.h"
#include <stdio.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CPipeFormatDlg �_�C�A���O

CPipeFormatDlg::CPipeFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPipeFormatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPipeFormatDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPipeFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPipeFormatDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPipeFormatDlg, CDialog)
	//{{AFX_MSG_MAP(CPipeFormatDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_START2, OnStart2)
	ON_BN_CLICKED(IDC_START3, OnStart3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPipeFormatDlg ���b�Z�[�W �n���h��

BOOL CPipeFormatDlg::OnInitDialog()
{
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
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CPipeFormatDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPipeFormatDlg::OnPaint() 
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
HCURSOR CPipeFormatDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPipeFormatDlg::OnStart() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	int hpipe_stdout[2];	// �q�v���Z�X��stdout�ɐڑ�����p�C�v
	int hpipe_stdin[2];		// �q�v���Z�X��stdin�ɐڑ�����p�C�v

	_pipe(hpipe_stdout, 128, O_TEXT|O_NOINHERIT);
	_pipe(hpipe_stdin, 128, O_TEXT|O_NOINHERIT);

	int save_stdout = _dup(_fileno(stdout));
	_dup2(hpipe_stdout[WRITE], _fileno(stdout));
	_close(hpipe_stdout[WRITE]);
	int save_stdin = _dup(_fileno(stdin));
	_dup2(hpipe_stdin[READ], _fileno(stdin));
	_close(hpipe_stdin[READ]);
	
	char* args[] = {
		"format", "a:", "/v:", NULL
	};
	hprocess = (HANDLE)spawnvp(P_NOWAIT, args[0], (const char* const*)&args[0]);

	_dup2(save_stdout, _fileno(stdout));
	_close(save_stdout);
	_dup2(save_stdin, _fileno(stdin));
	_close(save_stdin);

#define KEYWORD0	"�������ł����� Enter �L�[�������Ă�������... "
#define KEYWORD1	"�p�[�Z���g�I�����܂����B"
#define KEYWORD2	"(Y/N)? "

	int stat = 0;
	int n = 0;
	char buffer[1024];
	FILE* fp = _fdopen(hpipe_stdout[READ], "r");
	while (!feof(fp)) {
		buffer[n++] = fgetc(fp);
		buffer[n] = '\0';
		
		if (stat == 0) {
			char* p = strstr(buffer, KEYWORD0);
			if (p) {
				strcpy(buffer, p + strlen(KEYWORD0));
				n = strlen(buffer);
				_write(hpipe_stdin[WRITE], "\n", 1);
				stat = 1;
			}
		} else if (stat == 1) {
			char* p = strstr(buffer, KEYWORD1);
			if (p) {
				int i;
				printf("%d %%\n", i = atoi(buffer));
				strcpy(buffer, p + strlen(KEYWORD1));
				n = strlen(buffer);
				if (i == 100)
					stat = 2;
			} else if (buffer[n-1] == '\n') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			} else if (buffer[n-1] == '\x0d') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			}
		} else if (stat == 2) {
			char* p = strstr(buffer, KEYWORD2);
			if (p) {
				_write(hpipe_stdin[WRITE], "N", 1);
				_write(hpipe_stdin[WRITE], "\n", 1);
			} else if (buffer[n-1] == '\n') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			}
		} else {
			printf(buffer);
		}
	}

	fclose(fp);
	_close(hpipe_stdin[WRITE]);
}
  
void CPipeFormatDlg::OnStart2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	int hpipe_stdout[2];	// �q�v���Z�X��stdout�ɐڑ�����p�C�v
	int hpipe_stdin[2];		// �q�v���Z�X��stdin�ɐڑ�����p�C�v

	_pipe(hpipe_stdout, 128, O_TEXT|O_NOINHERIT);
	_pipe(hpipe_stdin, 128, O_TEXT|O_NOINHERIT);

	int save_stdout = _dup(_fileno(stdout));
	_dup2(hpipe_stdout[WRITE], _fileno(stdout));
	_close(hpipe_stdout[WRITE]);
	int save_stdin = _dup(_fileno(stdin));
	_dup2(hpipe_stdin[READ], _fileno(stdin));
	_close(hpipe_stdin[READ]);
	
	char* argv[] = {"", "help", "help", NULL};
	hprocess = (HANDLE)spawnvp(P_NOWAIT, argv[1], (const char* const*)&argv[1]);

	_dup2(save_stdout, _fileno(stdout));
	_close(save_stdout);
	_dup2(save_stdin, _fileno(stdin));
	_close(save_stdin);

	FILE* fp = _fdopen(hpipe_stdout[READ], "r");
#if 0

#define KEYWORD0	"�������ł����� Enter �L�[�������Ă�������... "
#define KEYWORD1	"�p�[�Z���g�I�����܂����B"
#define KEYWORD2	"(Y/N)? "

	int stat = 0;
	int n = 0;
	char buffer[1024];
	while (!feof(fp)) {
		buffer[n++] = fgetc(fp);
		buffer[n] = '\0';
		
		if (stat == 0) {
			char* p = strstr(buffer, KEYWORD0);
			if (p) {
				strcpy(buffer, p + strlen(KEYWORD0));
				n = strlen(buffer);
				_write(hpipe_stdin[WRITE], "\n", 1);
				stat = 1;
			}
		} else if (stat == 1) {
			char* p = strstr(buffer, KEYWORD1);
			if (p) {
				int i;
				printf("%d %%\n", i = atoi(buffer));
				strcpy(buffer, p + strlen(KEYWORD1));
				n = strlen(buffer);
				if (i == 100)
					stat = 2;
			} else if (buffer[n-1] == '\n') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			} else if (buffer[n-1] == '\x0d') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			}
		} else if (stat == 2) {
			char* p = strstr(buffer, KEYWORD2);
			if (p) {
				_write(hpipe_stdin[WRITE], "N", 1);
				_write(hpipe_stdin[WRITE], "\n", 1);
			} else if (buffer[n-1] == '\n') {
				strcpy(buffer, &buffer[n]);
				n = strlen(buffer);
			}
		} else {
			printf(buffer);
		}
	}
#endif

	fclose(fp);
	_close(hpipe_stdin[WRITE]);
}

void CPipeFormatDlg::OnStart3() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	HANDLE hpipe_stdout[2];	// �q�v���Z�X��stdout�ɐڑ�����p�C�v
	HANDLE hpipe_stdin[2];		// �q�v���Z�X��stdin�ɐڑ�����p�C�v

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CreatePipe(&hpipe_stdout[0], &hpipe_stdout[1], &sa, 128);
	CreatePipe(&hpipe_stdin[0], &hpipe_stdin[1], &sa, 128);

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE save_stdout;
	if (DuplicateHandle(GetCurrentProcess(), GetStdHandle(STD_OUTPUT_HANDLE), GetCurrentProcess(), 
		&save_stdout, 0, FALSE, DUPLICATE_SAME_ACCESS) == 0) {
		return;
	}
	
	DuplicateHandle(GetCurrentProcess(), hpipe_stdout[WRITE], GetCurrentProcess(), 
		GetStdHandle(STD_OUTPUT_HANDLE));
//	_close(hpipe_stdout[WRITE]);
//	int save_stdin = _dup(_fileno(stdin));
//	_dup2(hpipe_stdin[READ], _fileno(stdin));
//	_close(hpipe_stdin[READ]);
	
//	char* argv[] = {"", "help", "help", NULL};
//	hprocess = (HANDLE)spawnvp(P_NOWAIT, argv[1], (const char* const*)&argv[1]);

//	_dup2(save_stdout, _fileno(stdout));
//	_close(save_stdout);
//	_dup2(save_stdin, _fileno(stdin));
//	_close(save_stdin);

//	FILE* fp = _fdopen(hpipe_stdout[READ], "r");

//	fclose(fp);
//	_close(hpipe_stdin[WRITE]);
}
