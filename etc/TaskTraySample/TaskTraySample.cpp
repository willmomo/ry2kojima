//-----------------------------------------------------------------------------
// �^�X�N�g���C�ɃA�C�R����\������E�B���h�E��\�����Ȃ��A�v���P�[�V�����̃T���v��
//
// !! �d�v !!
// ��{�I�ȍ�Ƃ́A���[�J�[�X���b�h�ōs���B
// ���C���X���b�h���������ƁA�G�N�X�v���[�����ł߂邽�߁B
// 
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#include "mainFrm.h"

#include "resource.h"

//-----------------------------------------------------------------------------
// �v���O�����X�^�[�g
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// �E�B���h�E�N���X�̓o�^
	if (MainFrm_RegisterClass(hInstance) == FALSE) {
		return -1;
	}

	// �E�B���h�E�̍쐬
	if (MainFrm_CreateWindow(hInstance, nCmdShow) == FALSE) {
		return -1;
	}

	// ���b�Z�[�W���[�v
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			msg.wParam = -1;
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
