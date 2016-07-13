// CGITEST.CPP - �C���^�[�l�b�g �T�[�o�[�p�̃C���v�������e�[�V���� �t�@�C��
//    cgitest Extension

#include "stdafx.h"
#include "cgitest.h"

///////////////////////////////////////////////////////////////////////
// �B��� CWinApp �I�u�W�F�N�g
// ����: MFC �� DLL ���Ŏg�p���Ȃ��悤�ɕύX����ꍇ�́A���̃I�u�W�F�N�g
// ���폜���Ă����܂��܂���B

CWinApp theApp;

///////////////////////////////////////////////////////////////////////
// �R�}���h��̓}�b�v

BEGIN_PARSE_MAP(CCgitestExtension, CHttpServer)
	// TODO: ���̈ʒu�ɃR�}���h���t�b�N���邽�߂� ON_PARSE_COMMAND() 
	// ����� ON_PARSE_COMMAND_PARAMS() ��}�����Ă��������B
	// ��:

	ON_PARSE_COMMAND(Default, CCgitestExtension, ITS_EMPTY)
	DEFAULT_PARSE_COMMAND(Default, CCgitestExtension)
END_PARSE_MAP(CCgitestExtension)


///////////////////////////////////////////////////////////////////////
// �B��� CCgitestExtension �I�u�W�F�N�g

CCgitestExtension theExtension;


///////////////////////////////////////////////////////////////////////
// CCgitestExtension �N���X�̃C���v�������e�[�V����

CCgitestExtension::CCgitestExtension()
{
}

CCgitestExtension::~CCgitestExtension()
{
}

BOOL CCgitestExtension::GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	// �����������p�Ƀf�t�H���g�̃C���v�������e�[�V�������Ăяo���܂�
	CHttpServer::GetExtensionVersion(pVer);

	// �����̕������ǂݍ��݂܂�
	TCHAR sz[HSE_MAX_EXT_DLL_NAME_LEN+1];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
			IDS_SERVER, sz, HSE_MAX_EXT_DLL_NAME_LEN));
	_tcscpy(pVer->lpszExtensionDesc, sz);
	return TRUE;
}

BOOL CCgitestExtension::TerminateExtension(DWORD dwFlags)
{
	// �g�����I�����܂�
	//TODO: �C���X�^���X���̃��\�[�X���������Ă�������
	return TRUE;
}

///////////////////////////////////////////////////////////////////////
// CCgitestExtension �N���X�̃R�}���h �w�b�_�[

void CCgitestExtension::Default(CHttpServerContext* pCtxt)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << _T("This default message was produced by the Internet");
	*pCtxt << _T(" Server DLL Wizard. Edit your CCgitestExtension::Default()");
	*pCtxt << _T(" implementation to change it.\r\n");

	EndContent(pCtxt);
}

// ClassWizard ���K�v�Ƃ���ȉ��̍s��ҏW���Ȃ��ł��������B
#if 0
BEGIN_MESSAGE_MAP(CCgitestExtension, CHttpServer)
	//{{AFX_MSG_MAP(CCgitestExtension)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0



///////////////////////////////////////////////////////////////////////
// �g���I�u�W�F�N�g�� MFC ���g�p���Ȃ��ꍇ�A�g���I�u�W�F�N�g�����W���[����
// �΂��郊�\�[�X �n���h�����m���Ɍ����邽�߂ɁA���̃R�[�h���K�v�ƂȂ�܂��B
// �����g���I�u�W�F�N�g�� MFC ���g�p����悤�ɕύX����ꍇ�́A�ȉ���
// AfxGetResourceHandle() ����� DllMain() �֐��� g_hInstance global  
// �Ɠ��l�ɍ폜���Ă��������B

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
