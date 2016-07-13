//---------------------------------------------------------------------
// smartDlg: S.M.A.R.T����\�����郂�[�h���X�_�C�A���O
//

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include "kjm.h"
#include "resource.h"


//---------------------------------------------------------------------
#define ID_TIMER	1


static HWND s_hwndDlg = NULL;


//---------------------------------------------------------------------
// �f�[�^���X�V����
//
static void UpdateData( HWND hwnd ) {


	kjm::physical_drive drive;
	if ( drive.OpenDrive( "\\\\.\\PhysicalDrive0" ) ) {

		if ( !drive.isSupportedSMART() ) {

			SetDlgItemText( hwnd, IDC_SMARTSTAT, _T( "S.M.A.R.T���Ή�" ) );

		} else {
			TCHAR buf[32];
			wsprintf( buf, _T( "%d ��" ), drive.getTemperature() );
			SetDlgItemText( hwnd, IDC_TEMPERATURE, buf );

			switch ( drive.smartReturnStatus() ) {
			case 0:
				wsprintf( buf, _T( "���Ȃ�" ) );
				break;
			case 1:
				wsprintf( buf, _T( "��肠��" ) );
				break;
			default:
				wsprintf( buf, _T( "SMART�ŃG���[" ) );
				break;
			}

			SetDlgItemText( hwnd, IDC_SMARTSTAT, buf );
		}
	}

	SMARTATTRIBUTESDATA attrData;
	SMARTHRESHOLDSDATA thData;

	drive.smartReadAttributeValues( &attrData );
	drive.smartReadAttributeThresholds( &thData );

	LVITEM lvi;
	memset( &lvi, 0, sizeof( lvi ) );
	lvi.mask = LVIF_TEXT;

	ListView_DeleteAllItems( GetDlgItem( hwnd, IDC_SMARTLIST ) );

	for ( int index = 0; index < NUM_ATTRIBUTE_STRUCTS; ++index ) {
		int attrID = attrData.AttributeData[ index ].attributeID;
		if ( 0 != attrID ) {
			char*	attrName;
			int		arrayIndex = 0;
			while (0 != kjm::attrValueAndNames[arrayIndex].value) {
				if (attrID == kjm::attrValueAndNames[arrayIndex].value)
					break;
				++arrayIndex;
			}
			
			if (0 != kjm::attrValueAndNames[arrayIndex].value) {
				attrName = kjm::attrValueAndNames[arrayIndex].name;
			} else {
				attrName = "Unknown Attribute";
			}

			std::stringstream s;
			s << attrID;
			std::string a = s.str();

			lvi.pszText = (char*)a.c_str();
			int itemIdx = ListView_InsertItem( GetDlgItem( hwnd, IDC_SMARTLIST ), &lvi );

			ListView_SetItemText( GetDlgItem( hwnd, IDC_SMARTLIST ), itemIdx, 1, attrName );

			s.str( "" );
			s << (int)attrData.AttributeData[index].attributeValue;
			a = s.str();
			ListView_SetItemText( GetDlgItem( hwnd, IDC_SMARTLIST ), itemIdx, 2, (char*)a.c_str() );

			s.str( "" );
			s << (int)attrData.AttributeData[index].worstValue;
			a = s.str();
			ListView_SetItemText( GetDlgItem( hwnd, IDC_SMARTLIST ), itemIdx, 3, (char*)a.c_str() );

			s.str( "" );
			s << (int)thData.ThresholdData[index].attributeThreshold;
			a = s.str();
			ListView_SetItemText( GetDlgItem( hwnd, IDC_SMARTLIST ), itemIdx, 4, (char*)a.c_str() );

			s.str( "" );
			s << *( WORD * )attrData.AttributeData[index].vendorSpecific;
			a = s.str();
			ListView_SetItemText( GetDlgItem( hwnd, IDC_SMARTLIST ), itemIdx, 5, (char*)a.c_str() );

			lvi.iItem = itemIdx + 1;
		}
	}
}


//---------------------------------------------------------------------
// �_�C�A���O�Ǝq�R���g���[���̏�����
//
static BOOL OnInitDialog( HWND hwnd ) {

	// �q�R���g���[��������

	// ���X�g�r���[����s�I���A�O���b�h�\���ɂ���
	DWORD ex_style = ListView_GetExtendedListViewStyle( GetDlgItem( hwnd, IDC_SMARTLIST ) );
	ex_style |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle( GetDlgItem( hwnd, IDC_SMARTLIST ), ex_style );

	LVCOLUMN col;

	memset( &col, 0, sizeof( col ) );
	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 26;
	col.iSubItem = 0;
	col.pszText = _T( "ID" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_SMARTLIST), 0, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 227;
	col.iSubItem = 0;
	col.pszText = _T( "Name" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_SMARTLIST), 1, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_RIGHT ;
	col.cx = 30;
	col.iSubItem = 0;
	col.pszText = _T( "����" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_SMARTLIST), 2, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_RIGHT ;
	col.cx = 30;
	col.iSubItem = 0;
	col.pszText = _T( "�Œ�" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_SMARTLIST), 3, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_RIGHT ;
	col.cx = 30;
	col.iSubItem = 0;
	col.pszText = _T( "臒l" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_SMARTLIST), 4, &col );

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_RIGHT ;
	col.cx = 36;
	col.iSubItem = 0;
	col.pszText = _T( "��data" );
	ListView_InsertColumn( GetDlgItem( hwnd, IDC_SMARTLIST ), 5, &col );

	// �����ʍX�V
	UpdateData( hwnd );

	// �^�C�}�[�N��
	SetTimer( hwnd, ID_TIMER, 30 * 1000, NULL );

	return TRUE;
}


//---------------------------------------------------------------------
// CSV�t�@�C���ɏo�͂���
//
static void OutputCSVFile() {

	std::ofstream ofs;

	ofs.open( _T( "C:\\smart.csv" ), std::ios::out | std::ios::app );
	if ( ofs.is_open() ) {

		kjm::physical_drive drive;
		if ( drive.OpenDrive( "\\\\.\\PhysicalDrive0" ) ) {

			ofs << kjm::time::GetCurrentTime().ctime().c_str() << std::endl;

			if ( !drive.isSupportedSMART() ) {
				ofs << "S.M.A.R.T���Ή�" << std::endl;
			} else {
				ofs << "��ԁF" << drive.smartReturnStatus() << std::endl;
				ofs << drive.toCSVString() << std::endl;
			}
		}
		ofs.close();
	}
}


//---------------------------------------------------------------------
// S.M.A.R.T�Ď��_�C�A���O�̃_�C�A���O�v���V�[�W��
//
static BOOL CALLBACK DlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	switch ( msg ) {
	case WM_INITDIALOG:
		return OnInitDialog( hwnd );

	case WM_COMMAND:
		{
			WORD wID = LOWORD( wParam );
			if ( wParam == IDOK || wParam == IDCANCEL ) {
				ShowWindow( hwnd, SW_HIDE );
				return TRUE;
			} else if ( wParam == IDC_DEBUG ) {

				for ( int i = 0; i < 6; i++ ) {
					int col_width = ListView_GetColumnWidth( GetDlgItem( hwnd, IDC_SMARTLIST ), i );
					char buf[96];
					wsprintf( buf, _T( "%d: %d\n" ), i, col_width );
					OutputDebugString( buf );
				}

				OutputCSVFile();
			}
		}
		break;

	case WM_TIMER:
		if ( IsWindowVisible( hwnd ) )	{
			UpdateData( hwnd );
		}
		return TRUE;

	case WM_DESTROY:
		KillTimer( hwnd, ID_TIMER );
		return FALSE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
// S.M.A.R.T���Ď��_�C�A���O�̍쐬
//
BOOL CreateSmartWatchDialog( HINSTANCE hInstance, HWND hWndParent ) {

	s_hwndDlg = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_SMART ), hWndParent, DlgProc );

	if ( s_hwndDlg == NULL ) {
		return FALSE;
	}

	return TRUE;
}


//---------------------------------------------------------------------
// S.M.A.R.T���Ď��_�C�A���O�̔j��
//
BOOL DestroySmartWatchDialog() {
	
	if ( s_hwndDlg ) {
		DestroyWindow( s_hwndDlg );
		s_hwndDlg = NULL;
	}

	return TRUE;
}


//---------------------------------------------------------------------
// S.M.A.R.T���Ď��_�C�A���O�̕\��
//
void ShowSmartWatchDialog( int nCmdShow ) {
	
	if ( s_hwndDlg ) {
		ShowWindow( s_hwndDlg, nCmdShow );
	}
}

