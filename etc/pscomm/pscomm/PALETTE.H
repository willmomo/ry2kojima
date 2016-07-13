
#ifndef PALETTE_INCLUDED
#define PALETTE_INCLUDED

/* PALETTE.H */

// �E�B���h�E�g���o�C�g�̃T�C�Y(PALINFO�\���̂̃n���h�����i�[�����)
#define PAL_CBWNDEXTRA  (1 * sizeof (WORD))

typedef struct
   {
   HPALETTE hPal;                      // �\������Ă���p���b�g�����ʂ���
                                       // �n���h��
   WORD     wEntries;                  // �p���b�g�G���g���̌�
   int      nSquareSize;               // �p���b�g�Z���̃T�C�Y(PAL_SIZE���Q��)
   HWND     hInfoWnd;                  // ���o�[�E�B���h�E�̃n���h��
   int      nRows, nCols;              // �E�B���h�E�̃T�C�Y
   int      cxSquare, cySquare;        // �p���b�g�Z���̕��ƍ���(�s�N�Z���P��)
   WORD     wEntry;                    // ���ݑI������Ă���p���b�g�Z��
   } PALINFO, FAR *LPPALINFO;


// �E�B���h�E���[�h
#define WW_PAL_HPALINFO 0              // PALINFO�\���̂����ʂ���n���h��

// ���̒�`��CopyPaletteChangingFlags()�̂��߂̂���
#define DONT_CHANGE_FLAGS -1

// LOGPALETTE��palVersion�����o�ɐݒ肳���p���b�g�o�[�W����
#define PALVERSION   0x300

// PaletteWndProc()�Ńp���b�g��\��������@�������񋓌^���X�g
enum PAL_SIZE
   {
   PALSIZE_TINY = 0,
   PALSIZE_SMALL,
   PALSIZE_MEDIUM,
   PALSIZE_LARGE
   };


// Pallette Windows�̃��j���[��`
//  ��: TINY����FITWND�܂ŘA���������l�łȂ���΂Ȃ�Ȃ��B���l�̊֌W��ύX
//      �����ꍇ�́APALETTE.C��WM_INITMENUPOPUP���b�Z�[�W��WM_COMMAND���b
//      �Z�[�W�̏������ύX���Ȃ���΂Ȃ�Ȃ�

#define IDM_PAL_TINY        1000       // �ŏ��̃p���b�g�Z��
#define IDM_PAL_SMALL       1001       // �����ȃp���b�g�Z��
#define IDM_PAL_MEDIUM      1002       // ���ʂ̃p���b�g�Z��
#define IDM_PAL_LARGE       1003       // �傫�ȃp���b�g�Z��


// DIBVIEW.RC�̕�����e�[�u���̕�����̒�`
#define IDS_PAL_RGB         2048       // PALETTEENTRY.peFlags=0�̕�����
#define IDS_PAL_RESERVED    2049       // PALETTEENTRY.peFlags=1�̕�����
#define IDS_PAL_EXPLICIT    2050       // PALETTEENTRY.peFlags=2�̕�����
#define IDS_PAL_ERROR       2051       // PALETTEENTRY.peFlags=3�̕�����
#define IDS_PAL_NOCOLLAPSE  2052       // PALETTEENTRY.peFlags=4�̕�����

#define IDS_PAL_NOPAL       2053       // �p���b�g���n����Ă��Ȃ��Ƃ��̕�����
#define IDS_PAL_DISPRGB     2054       // �X�e�[�^�X�s�̏���������


// �֐��v���g�^�C�v
long FAR PASCAL __export PaletteWndProc (HWND hwnd,
				UINT message,
				WPARAM wParam,
				LPARAM lParam);

void SetPaletteWindowsPal (HWND hWnd, HPALETTE hPal);

HPALETTE GetSystemPalette (void);

HPALETTE CopyPaletteChangingFlags (HPALETTE hPal, BYTE bNewFlag);

void MyAnimatePalette (HWND hWnd, HPALETTE hPal);

int ColorsInPalette (HPALETTE hPal);

#define CopyPalette(hPal)  CopyPaletteChangingFlags (hPal,(BYTE) DONT_CHANGE_FLAGS)

#define CopyPalForAnimation(hPal) CopyPaletteChangingFlags (hPal, PC_RESERVED)

#endif // PALETTE_INCLUDED