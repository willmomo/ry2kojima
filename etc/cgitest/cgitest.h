#if !defined(AFX_CGITEST_H__D2D37831_04A8_46C7_8F39_9F3F95FB9BCA__INCLUDED_)
#define AFX_CGITEST_H__D2D37831_04A8_46C7_8F39_9F3F95FB9BCA__INCLUDED_

// CGITEST.H - �C���^�[�l�b�g�T�[�o�[�p�̃w�b�_�[ �t�@�C��
//    cgitest Extension

#include "resource.h"

class CCgitestExtension : public CHttpServer
{
public:
	CCgitestExtension();
	~CCgitestExtension();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł������� !
	//{{AFX_VIRTUAL(CCgitestExtension)
	public:
	virtual BOOL GetExtensionVersion(HSE_VERSION_INFO* pVer);
	//}}AFX_VIRTUAL
	virtual BOOL TerminateExtension(DWORD dwFlags);

	// TODO: ���̈ʒu�ɃR�}���h �n���h����ǉ����Ă��������B
	// ��:

	void Default(CHttpServerContext* pCtxt);

	DECLARE_PARSE_MAP()

	//{{AFX_MSG(CCgitestExtension)
	//}}AFX_MSG
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CGITEST_H__D2D37831_04A8_46C7_8F39_9F3F95FB9BCA__INCLUDED)
