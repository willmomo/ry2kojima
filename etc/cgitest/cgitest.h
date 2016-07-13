#if !defined(AFX_CGITEST_H__D2D37831_04A8_46C7_8F39_9F3F95FB9BCA__INCLUDED_)
#define AFX_CGITEST_H__D2D37831_04A8_46C7_8F39_9F3F95FB9BCA__INCLUDED_

// CGITEST.H - インターネットサーバー用のヘッダー ファイル
//    cgitest Extension

#include "resource.h"

class CCgitestExtension : public CHttpServer
{
public:
	CCgitestExtension();
	~CCgitestExtension();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//    この位置に生成されるコードを編集しないでください !
	//{{AFX_VIRTUAL(CCgitestExtension)
	public:
	virtual BOOL GetExtensionVersion(HSE_VERSION_INFO* pVer);
	//}}AFX_VIRTUAL
	virtual BOOL TerminateExtension(DWORD dwFlags);

	// TODO: この位置にコマンド ハンドラを追加してください。
	// 例:

	void Default(CHttpServerContext* pCtxt);

	DECLARE_PARSE_MAP()

	//{{AFX_MSG(CCgitestExtension)
	//}}AFX_MSG
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CGITEST_H__D2D37831_04A8_46C7_8F39_9F3F95FB9BCA__INCLUDED)
