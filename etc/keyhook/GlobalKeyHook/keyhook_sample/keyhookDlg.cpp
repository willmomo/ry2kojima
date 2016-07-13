// keyhookDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "keyhook_sample.h"
#include "keyhookDlg.h"
#include "../keyhook/keyhook.h"

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
// CKeyhookDlg ダイアログ

CKeyhookDlg::CKeyhookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyhookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyhookDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyhookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyhookDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

static UINT NEAR WM_KEYHOOK = RegisterWindowMessage(KEYHOOK_MESSAGE);

BEGIN_MESSAGE_MAP(CKeyhookDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyhookDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(WM_KEYHOOK, OnKeyHook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyhookDlg メッセージ ハンドラ

BOOL CKeyhookDlg::OnInitDialog()
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
	SetHook(m_hWnd);

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CKeyhookDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKeyhookDlg::OnPaint() 
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
HCURSOR CKeyhookDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKeyhookDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	FreeHook();
}

LRESULT CKeyhookDlg::OnKeyHook(WPARAM wParam, LPARAM lParam)
{
	CString str;
	GetDlgItemText(IDC_LABEL9, str);
	SetDlgItemText(IDC_LABEL10, str);
	GetDlgItemText(IDC_LABEL8, str);
	SetDlgItemText(IDC_LABEL9, str);
	GetDlgItemText(IDC_LABEL7, str);
	SetDlgItemText(IDC_LABEL8, str);
	GetDlgItemText(IDC_LABEL6, str);
	SetDlgItemText(IDC_LABEL7, str);
	GetDlgItemText(IDC_LABEL5, str);
	SetDlgItemText(IDC_LABEL6, str);
	GetDlgItemText(IDC_LABEL4, str);
	SetDlgItemText(IDC_LABEL5, str);
	GetDlgItemText(IDC_LABEL3, str);
	SetDlgItemText(IDC_LABEL4, str);
	GetDlgItemText(IDC_LABEL2, str);
	SetDlgItemText(IDC_LABEL3, str);
	GetDlgItemText(IDC_LABEL, str);
	SetDlgItemText(IDC_LABEL2, str);


	char szBuff[256];
	wsprintf(szBuff, "wParam = 0x%02X, lParam = 0x%08X", wParam, lParam);
	SetDlgItemText(IDC_LABEL, szBuff);

	OutputDebugString(szBuff);
	OutputDebugString("\n");

	return 0;
}
