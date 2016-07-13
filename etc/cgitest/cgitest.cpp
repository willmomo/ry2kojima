// CGITEST.CPP - インターネット サーバー用のインプリメンテーション ファイル
//    cgitest Extension

#include "stdafx.h"
#include "cgitest.h"

///////////////////////////////////////////////////////////////////////
// 唯一の CWinApp オブジェクト
// メモ: MFC を DLL 内で使用しないように変更する場合は、このオブジェクト
// を削除してもかまいません。

CWinApp theApp;

///////////////////////////////////////////////////////////////////////
// コマンド解析マップ

BEGIN_PARSE_MAP(CCgitestExtension, CHttpServer)
	// TODO: この位置にコマンドをフックするために ON_PARSE_COMMAND() 
	// および ON_PARSE_COMMAND_PARAMS() を挿入してください。
	// 例:

	ON_PARSE_COMMAND(Default, CCgitestExtension, ITS_EMPTY)
	DEFAULT_PARSE_COMMAND(Default, CCgitestExtension)
END_PARSE_MAP(CCgitestExtension)


///////////////////////////////////////////////////////////////////////
// 唯一の CCgitestExtension オブジェクト

CCgitestExtension theExtension;


///////////////////////////////////////////////////////////////////////
// CCgitestExtension クラスのインプリメンテーション

CCgitestExtension::CCgitestExtension()
{
}

CCgitestExtension::~CCgitestExtension()
{
}

BOOL CCgitestExtension::GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	// 初期化処理用にデフォルトのインプリメンテーションを呼び出します
	CHttpServer::GetExtensionVersion(pVer);

	// 説明の文字列を読み込みます
	TCHAR sz[HSE_MAX_EXT_DLL_NAME_LEN+1];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
			IDS_SERVER, sz, HSE_MAX_EXT_DLL_NAME_LEN));
	_tcscpy(pVer->lpszExtensionDesc, sz);
	return TRUE;
}

BOOL CCgitestExtension::TerminateExtension(DWORD dwFlags)
{
	// 拡張を終了します
	//TODO: インスタンス毎のリソースを消去してください
	return TRUE;
}

///////////////////////////////////////////////////////////////////////
// CCgitestExtension クラスのコマンド ヘッダー

void CCgitestExtension::Default(CHttpServerContext* pCtxt)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << _T("This default message was produced by the Internet");
	*pCtxt << _T(" Server DLL Wizard. Edit your CCgitestExtension::Default()");
	*pCtxt << _T(" implementation to change it.\r\n");

	EndContent(pCtxt);
}

// ClassWizard が必要とする以下の行を編集しないでください。
#if 0
BEGIN_MESSAGE_MAP(CCgitestExtension, CHttpServer)
	//{{AFX_MSG_MAP(CCgitestExtension)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0



///////////////////////////////////////////////////////////////////////
// 拡張オブジェクトが MFC を使用しない場合、拡張オブジェクトがモジュールに
// 対するリソース ハンドルを確実に見つけるために、このコードが必要となります。
// もし拡張オブジェクトが MFC を使用するように変更する場合は、以下の
// AfxGetResourceHandle() および DllMain() 関数を g_hInstance global  
// と同様に削除してください。

/****

static HINSTANCE g_hInstance;

HINSTANCE AFXISAPI AfxGetResourceHandle()
{
	return g_hInstance;
}

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ulReason,
					LPVOID lpReserved)
{
	if (ulReason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInst;
	}

	return TRUE;
}

****/
