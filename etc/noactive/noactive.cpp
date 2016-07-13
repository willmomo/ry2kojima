#include <windows.h>
#include <tchar.h>

#define WS_EX_NOACTIVATE	0x8000000

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
//	case WM_MOUSEACTIVATE:
//		return MA_NOACTIVATE;
//		return MA_NOACTIVATEANDEAT;
	case WM_LBUTTONDOWN:
		{
			static unsigned char r = 0;
			static unsigned char g = 0;
			static unsigned char b = 0;
			RECT rc;
			HDC hdc = GetDC(hWnd);
			GetClientRect(hWnd, &rc);
			HBRUSH br = CreateSolidBrush(RGB(r, g, b));
			FillRect(hdc, &rc, br);
			DeleteObject(br);
			ReleaseDC(hWnd, hdc);

			r += 20;
			g += 20;
			b += 20;
		}
		return 0;

	case WM_DESTROY:  // �E�B���h�E��j������Ƃ�
		PostQuitMessage( 0 );
		return 0;
	}

	// ���̃��b�Z�[�W�́A�f�t�H���g�̏������s��
	return DefWindowProc( hWnd, msg, wp, lp );
}


// �J�n�ʒu
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd)
{
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

	// �E�B���h�E�N���X�̏���ݒ�
	wc.cbSize = sizeof(wc);               // �\���̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW;   // �X�^�C��
	wc.lpfnWndProc = WndProc;             // �E�B���h�E�v���V�[�W��
	wc.cbClsExtra = 0;                    // �g�����P
	wc.cbWndExtra = 0;                    // �g�����Q
	wc.hInstance = hInst;                 // �C���X�^���X�n���h��
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc.hIcon = (HICON)LoadImage(          // �A�C�R��
//		NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON,
//		0, 0, LR_DEFAULTSIZE | LR_SHARED
//	);
	wc.hIconSm = wc.hIcon;                // �q�A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hCursor = (HCURSOR)LoadImage(      // �}�E�X�J�[�\��
//		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
//		0, 0, LR_DEFAULTSIZE | LR_SHARED
//	);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // �E�B���h�E�w�i
	wc.lpszMenuName = NULL;                     // ���j���[��
	wc.lpszClassName = _T("NOACTIVE");// �E�B���h�E�N���X��
	
	// �E�B���h�E�N���X��o�^����
	if( RegisterClassEx( &wc ) == 0 ){ return 1; }

	// �E�B���h�E���쐬����
	hWnd = CreateWindowEx(
		WS_EX_NOACTIVATE,
		wc.lpszClassName,      // �E�B���h�E�N���X��
		_T("Sample Program"),  // �^�C�g���o�[�ɕ\�����镶����
		WS_OVERLAPPEDWINDOW,   // �E�B���h�E�̎��
		CW_USEDEFAULT,         // �E�B���h�E��\������ʒu�iX���W�j
		CW_USEDEFAULT,         // �E�B���h�E��\������ʒu�iY���W�j
		CW_USEDEFAULT,         // �E�B���h�E�̕�
		CW_USEDEFAULT,         // �E�B���h�E�̍���
		NULL,                  // �e�E�B���h�E�̃E�B���h�E�n���h��
		NULL,                  // ���j���[�n���h��
		hInst,                 // �C���X�^���X�n���h��
		NULL                   // ���̑��̍쐬�f�[�^
	);
	if( hWnd == NULL ){ return 1; }

	// �E�B���h�E��\������
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	// ���b�Z�[�W���[�v
	while( 1 )
	{
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // ���b�Z�[�W���擾����
		if( ret == 0 || ret == -1 )
		{
			// �A�v���P�[�V�������I�������郁�b�Z�[�W�����Ă�����A
			// ���邢�� GetMessage() �����s������( -1 ���Ԃ��ꂽ�� �j�A���[�v�𔲂���
			break;
		}
		else
		{
			// ���b�Z�[�W����������
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return 0;
}
