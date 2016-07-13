//-----------------------------------------------------------------------------
// �V���[�g�J�b�g���쐬����v���O����
//
// �g�p�@�Fmkshortcut <�V���[�g�J�b�g�̃t���p�X> <���s�t�@�C���̃t���p�X>
//-----------------------------------------------------------------------------
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <intshcut.h>
#include <shlobj.h>

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "resource.h"

//-----------------------------------------------------------------------------
// �V���[�g�J�b�g�̍쐬
//
// CoInitialize(NULL)/CoUninitialize()�́A�A�v���P�[�V�������s���Ă��������B
//-----------------------------------------------------------------------------
bool createShortcut(LPCTSTR  strFileName, LPCTSTR strFile, LPCTSTR strArgs = NULL, LPCTSTR strDir = NULL) {
	HRESULT hRes;
	IShellLink *isLink = NULL; 
	IPersistFile *ipFile = NULL;

	//IShellLink���擾
	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&isLink);
	if (SUCCEEDED(hRes)) {
		//�p�X�̐ݒ�
		isLink->SetPath(strFile);

		//���s�������i�X�C�b�`�j�̐ݒ�
		if (strArgs)	isLink->SetArguments(strArgs);

		//��ƃt�H���_�̐ݒ�
		if (strDir)		isLink->SetWorkingDirectory(strDir);

		//IPersistFile���擾
		hRes = isLink->QueryInterface(IID_IPersistFile, (void**)&ipFile);
		if (SUCCEEDED(hRes)) {
			//�f�B�X�N�ɕۑ�����
			_bstr_t path = strFileName;
			hRes = ipFile->Save((wchar_t*)path, TRUE);

			//IPersistFile�̔j��
			ipFile->Release();
		}

		//IShellLink��j��
		isLink->Release();
	}else{
		//���s
	}

	return (SUCCEEDED(hRes) ? true : false);
}


//-----------------------------------------------------------------------------
// �v���O���� �X�^�[�g
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (__argc != 3) {
		MessageBox(NULL, _T("�g�p�@�Fmkshortcut <�V���[�g�J�b�g�t�@�C����> <�����N��t�@�C����>"), _T("mkshortcut"), MB_OK);
		return 0;
	}

	CoInitialize(NULL);
	
	TCHAR workDir[MAX_PATH];
	PathRemoveFileSpec(lstrcpy(workDir, __argv[2]));
	int ret = (createShortcut(__argv[1], __argv[2], NULL, workDir)) ? 0 : 1;

	CoUninitialize();
	return 0;
}
