#include <windows.h>
#include <tchar.h>
#include <vector>

#define countof(A)	(sizeof((A)) / sizeof((A)[0]))

using namespace std;

int HsGetDefaultPrinter(LPTSTR pszPrinterName, int cbPrinterName,
						  LPTSTR pszDriverName, int cbDriverName,
						  LPTSTR pszPort, int cbPort)
{
	TCHAR work[256], *p;

	GetProfileString(_T("windows"), _T("device"), _T(""), work, countof(work));
	if (lstrcmp(work, _T("")) == 0)
		return -1;

	// <printer name>�؂�o��
	p = _tcstok(work, _T(","));
	if (p == NULL) {
		return -2;	// ERR: <printer name>���L��܂���
	}
	else {
		if (pszPrinterName != NULL && cbPrinterName > 0) {
			if (cbPrinterName < lstrlen(p) + 1) {
				return -21;	// ERR: �o�b�t�@�̃T�C�Y������������
			}
			else {
				lstrcpyn(pszPrinterName, p, cbPrinterName);
			}
		}
	}

	// <driver name>�؂�o��
	p = _tcstok(NULL, _T(","));
	if (p == NULL) {
		return -3;	// ERR: <driver name>���L��܂���
	}
	else {
		if (pszDriverName != NULL && cbDriverName > 0) {
			if (cbDriverName < lstrlen(p) + 1) {
				return -31;	// ERR: �o�b�t�@�̃T�C�Y������������
			}
			else {
				lstrcpyn(pszDriverName, p, cbDriverName);
			}
		}
	}

	// <port name>�؂�o��
	p = _tcstok(NULL, _T(","));
	if (p == NULL) {
		return -4;	// ERR: <port name>���L��܂���
	}
	else {
		if (pszPort != NULL && cbPort > 0) {
			if (cbPort < lstrlen(p) + 1) {
				return -41;	// ERR: �o�b�t�@�̃T�C�Y������������
			}
			else {
				lstrcpyn(pszPort, p, cbPort);
			}
		}
	}

	return 0;
}

BOOL GetLogFont(HDC hdc, LOGFONT *logFont)
{
	BOOL bRet = FALSE;
	int iRet = 0;
	HFONT hCurFont;

	// ���ݑI������Ă���Font�̃n���h�����擾���܂�
	hCurFont = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
	if(hCurFont != NULL) {
		// Font�̘_���t�H���g����LOGFONT�Ɏ擾���܂��B
		iRet = GetObject(hCurFont, sizeof(LOGFONT), logFont);
		if(iRet != 0) {
			bRet = TRUE;
		}
	}
	return(bRet);
}

BOOL SelectPrintFont(HDC hdc, HFONT *hBaseFont, HFONT *hPrtFont)
{
	BOOL bRet = FALSE;

	if( *hPrtFont != NULL ) {
		// ����pFont�I�u�W�F�N�g�ɍ����ւ���
		*hBaseFont = (HFONT)SelectObject(hdc, *hPrtFont);
		if( *hBaseFont == NULL ) {
			bRet = FALSE;
		} else {
			bRet = TRUE;
		}
	}
	return(bRet);
}

BOOL SelectBaseFont(HDC hdc, HFONT *hBaseFont, HFONT *hPrtFont)
{
	BOOL bRet = TRUE;

	if( *hBaseFont != NULL ) {
		*hPrtFont = (HFONT)SelectObject(hdc, *hBaseFont);
		if( *hPrtFont == NULL ) {
			bRet = FALSE;
		} else {
			bRet = TRUE;
		}
	}
	return(bRet);
}

void PrintString( const char* psz ) {
	char szDevice[256], szDriver[256];
	int ret = HsGetDefaultPrinter(szDevice, countof(szDevice), szDriver, countof(szDriver), NULL, 0);
	if (ret != 0) {
		return;
	}

	vector<char> lines( &psz[0], &psz[ lstrlen( psz ) ] + 1 );

	DOCINFO docInfo;					//	DOCINFO�\����
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	docInfo.lpszDocName = "������";

	HDC hdc;							//	������DC
	hdc = CreateDC(szDriver, szDevice, NULL, NULL);
	int iJobId;
	iJobId = StartDoc(hdc, &docInfo); // ����W���u�J�n
	if( iJobId != SP_ERROR ) {

		int vres = ::GetDeviceCaps( hdc, VERTRES );

		int CurrentX = 0, CurrentY = 0;
		if( StartPage(hdc) >= 0 ) { // �v�����^�h���C�o�̏���

			TEXTMETRIC textmetric;				// ���ݑI�𒆃t�H���g�̏��
			if( GetTextMetrics(hdc, &textmetric) != FALSE ) { // ���ݑI�𒆃t�H���g�̏����擾

				char* p = strtok( lines.begin(), "\n" );
				while ( p ) {
					char tmp[ 512 ];

					if ( CurrentY + textmetric.tmHeight > vres ) {
						EndPage( hdc );
						CurrentY = 0;
					}

					wsprintf( tmp, "Y = %d / %d : %s", CurrentY, vres, p );
					TextOut( hdc, CurrentX, CurrentY, tmp, lstrlen( tmp ));
					CurrentY += textmetric.tmHeight;

					p = strtok( NULL, "\n" );
				}
				
			}
			if( EndPage(hdc) < 0 ) { // �P�ł̏I����ʒm
			}
		}

		if( EndDoc(hdc) == SP_ERROR ) { // ����W���u�I��
		}
	}
	else {
	}
}

int main( int argc, char** argv ) {

	string str;

	for ( int j = 0; j < 5; j++ ) {
		str = "";

		for ( int i = 0; i < 4; i++ ) {
			char temp[1024];
			wsprintf( temp, "line: %d\n", i );
			str += temp;
		}

		PrintString( str.c_str() );
	}

	return 0;
}

