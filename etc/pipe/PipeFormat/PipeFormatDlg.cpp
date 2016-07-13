// PipeFormatDlg.cpp : インプリメンテーション ファイル
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
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
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
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPipeFormatDlg ダイアログ

CPipeFormatDlg::CPipeFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPipeFormatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPipeFormatDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPipeFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPipeFormatDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
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
// CPipeFormatDlg メッセージ ハンドラ

BOOL CPipeFormatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
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

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CPipeFormatDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CPipeFormatDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPipeFormatDlg::OnStart() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	int hpipe_stdout[2];	// 子プロセスのstdoutに接続するパイプ
	int hpipe_stdin[2];		// 子プロセスのstdinに接続するパイプ

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

#define KEYWORD0	"準備ができたら Enter キーを押してください... "
#define KEYWORD1	"パーセント終了しました。"
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
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	int hpipe_stdout[2];	// 子プロセスのstdoutに接続するパイプ
	int hpipe_stdin[2];		// 子プロセスのstdinに接続するパイプ

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

#define KEYWORD0	"準備ができたら Enter キーを押してください... "
#define KEYWORD1	"パーセント終了しました。"
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
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HANDLE hprocess;
	enum PIPE { READ, WRITE };

	HANDLE hpipe_stdout[2];	// 子プロセスのstdoutに接続するパイプ
	HANDLE hpipe_stdin[2];		// 子プロセスのstdinに接続するパイプ

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
