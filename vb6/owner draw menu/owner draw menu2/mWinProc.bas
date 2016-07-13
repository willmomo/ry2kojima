Attribute VB_Name = "mWinProc"
'
'*************************************************************************
'
'                        �T�u�N���X���ėp���W���[��
'
'*************************************************************************
'
Option Explicit

' ���� �ϐ��錾 ����

Private P_Orghwnd As Long   ' �T�u�N���X������Ă���E�C���h�E�̃n���h���ێ�
Private P_OrgProc As Long   ' �f�t�H���g�E�C���h�E�v���V�[�W���A�h���X�ێ�

' ���� �萔�錾 ����

Public Const GWL_WNDPROC = (-4)     ' �E�C���h�E�v���V�[�W���̕ύX��v������

' ���� API�֐��錾 ����

' �w��E�C���h�E�̑�����ύX����֐�
Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" ( _
                                                ByVal hwnd As Long, _
                                                ByVal nIndex As Long, _
                                                ByVal dwNewLong As Long) As Long

' �w��E�C���h�E�̑������擾����֐�
Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" ( _
                                                ByVal hwnd As Long, _
                                                ByVal nIndex As Long) As Long

' �w��E�C���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�֐�
Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" ( _
                                                ByVal lpPrevWndFunc As Long, _
                                                ByVal hwnd As Long, _
                                                ByVal msg As Long, _
                                                ByVal wParam As Long, _
                                                ByVal lParam As Long) As Long
                                                
'������ �萔�錾 ������

' �E�C���h�E���b�Z�[�W
Public Const WM_DRAWITEM = &H2B        ' �R���g���[���⃁�j���[�̊O�ς��ύX���ꂽ��
Public Const WM_MEASUREITEM = &H2C     ' Windows�����ڂ̑傫����v�����Ă�����
Public Const WM_MENUCHAR = &H120       ' ���j���[�A�N�e�B�u���ɃL�[�������ꂽ��
    Public Const MNC_IGNORE = 0
    Public Const MNC_CLOSE = 1
    Public Const MNC_EXECUTE = 2
    Public Const MNC_SELECT = 3

' �I�[�i�[�h���[�^�C�v
Public Const ODT_MENU = 1              ' ���j���[
Public Const ODT_LISTBOX = 2           ' ���X�g�{�b�N�X
Public Const ODT_COMBOBOX = 3          ' �R���{�{�b�N�X
Public Const ODT_BUTTON = 4            ' �{�^��
Public Const ODT_LISTVIEW = 102        ' ���X�g�r���[

' �I�[�i�[�h���[�A�N�V����
Public Const ODA_DRAWENTIRE = &H1      ' �S�̂�`��
Public Const ODA_SELECT = &H2          ' �t�H�[�J�X�ω�
Public Const ODA_FOCUS = &H4           ' �I����ԕω�

' �I�[�i�[�h���[�X�e�[�^�X
Public Const ODS_SELECTED = &H1        ' �I�����
Public Const ODS_GRAYED = &H2          ' �W�F�\��
Public Const ODS_DISABLED = &H4        ' �g�p�֎~
Public Const ODS_CHECKED = &H8         ' �`�F�b�N�i���j���[�̂ݗL���j
Public Const ODS_FOCUS = &H10          ' �t�H�[�J�X������
Public Const ODS_DEFAULT = &H20        ' �f�t�H���g
Public Const ODS_COMBOBOXEDIT = &H1000 ' �R���{�{�b�N�X�G�f�B�b�g

'������ �\���̐錾 ������
Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

'MEASUREITEMSTRUCT �\����
Type MEASUREITEMSTRUCT
    CtlType As Long     ' �R���g���[���^�C�v�i�萔�u�I�[�i�[�h���[�^�C�v�v�Q�Ɓj
    CtlID As Long       ' �R���g���[�� ID�B���j���[�̏ꍇ���g�p
    itemID As Long      ' ���j���[�̏ꍇ�A���j���[ID
    itemWidth As Long   ' ��
    itemHeight As Long  ' ����
    itemData As Long    ' �A�C�e���f�[�^�i�I�[�i�[�h���[��ݒ肵�����ɃZ�b�g�j
End Type

'DRAWITEMSTRUCT �\����
Type DRAWITEMSTRUCT
    CtlType As Long     ' �R���g���[���^�C�v�i�萔�u�I�[�i�[�h���[�^�C�v�v�Q�Ɓj
    CtlID As Long       ' �R���g���[�� ID
    itemID As Long      ' MEASUREITEMSTRUCT �\���̎Q��
    itemAction As Long  ' �A�N�V�����i�萔�u�I�[�i�[�h���[�A�N�V�����v�Q�Ɓj
    itemState As Long   ' �A�C�e����ԁi�萔�u�I�[�i�[�h���[�X�e�[�^�X�v�Q�Ɓj
    hwndItem As Long    ' �E�C���h�E�n���h��
    hdc As Long         ' �f�o�C�X�R���e�L�X�g
    rcItem As RECT      ' �A�C�e���̕`��̈�
    itemData As Long    ' MEASUREITEMSTRUCT �\���̎Q��
End Type

'----------------------------
' �I�[�i�[�h���[���j���[�\��
'----------------------------
'tDRAWITEMSTRUCT    :DRAWITEMSTRUCT �\����
Private Sub psubDRAWITEM(tDRAWITEMSTRUCT As DRAWITEMSTRUCT)
    Dim lngPrevTextColor As Long ' �ݒ�O�̃e�L�X�g�F
    Dim lngPrevBackMode As Long  ' �ݒ�O�̔w�i���[�h
    Dim lngBackColor As Long     ' �w�i�F
    Dim hBrush As Long           ' �u���V�n���h��
    Dim tRCText As RECT          ' RECT�\����-����
    Dim strImageKey As String    ' �C���[�W���X�g�L�[
    Dim lngImageFlag As Long     ' �C���[�W�`��t���O
    
    With tDRAWITEMSTRUCT
        ' �X�e�[�^�X�ʂ̐ݒ�
        ' �f�t�H���g�i�ʏ�̃��j���[���ڂ̏ꍇ�j
        lngPrevTextColor = SetTextColor(.hdc, GetSysColor(COLOR_MENUTEXT))
        lngBackColor = GetSysColor(COLOR_MENU)
        If .itemState And (ODS_GRAYED Or ODS_DISABLED) Then
            ' �I��s����
            lngPrevTextColor = SetTextColor(.hdc, GetSysColor(COLOR_GRAYTEXT))
        ElseIf .itemState And ODS_SELECTED Then
            ' �I������Ă��郁�j���[���ڂ̏ꍇ
            lngPrevTextColor = SetTextColor(.hdc, GetSysColor(COLOR_HIGHLIGHTTEXT))
            lngBackColor = GetSysColor(COLOR_HIGHLIGHT)
        End If
        
        ' �w�i�̓h��Ԃ��u���V�쐬
        hBrush = CreateSolidBrush(lngBackColor)
        ' �w�i�̓h��Ԃ�
        Call FillRect(.hdc, .rcItem, hBrush)
        ' �w�i�̓h��Ԃ��u���V�j��
        Call DeleteObject(hBrush)
        
        ' �eRECT�\���̏�����
        tRCText = .rcItem
        tRCText.Left = tRCText.Left + 22
        ' �w�i���[�h�̐ݒ�
        lngPrevBackMode = SetBkMode(.hdc, TRANSPARENT)
        ' ���j���[������`��
        DrawText .hdc, tMenu(.itemData).strMenu, -1, tRCText, DT_EXPANDTABS Or DT_LEFT Or DT_VCENTER Or DT_SINGLELINE
        ' �A�C�R���̕`��
        strImageKey = tMenu(.itemData).strMenu
        If InStr(strImageKey, Chr(9)) Then strImageKey = Left(strImageKey, InStr(strImageKey, Chr(9)) - 1)
        lngImageFlag = imlTransparent
        If .itemState And ODS_CHECKED Then lngImageFlag = imlSelected
        On Error Resume Next
        Form1.ImageList1.ListImages(strImageKey).Draw .hdc, .rcItem.Left + 2, .rcItem.Top, lngImageFlag
        On Error GoTo 0
        ' �e�L�X�g�F�A�w�i�F�A�w�i���[�h�����Ƃɖ߂�
        SetTextColor .hdc, lngPrevTextColor
        SetBkMode .hdc, lngPrevBackMode
    End With
End Sub

'--------------------------
' �Ǝ��E�C���h�E�v���V�[�W��
'--------------------------
Public Function WindowProc(ByVal hwnd As Long, _
                           ByVal uMsg As Long, _
                           ByVal wParam As Long, _
                           ByVal lParam As Long) As Long

    Dim hdc As Long     ' �f�o�C�X�R���e�L�X�g�n���h��
    Dim tRC As RECT     ' RECT�\����
    Dim tMEASUREITEMSTRUCT As MEASUREITEMSTRUCT
    Dim tDRAWITEMSTRUCT As DRAWITEMSTRUCT
    
    Select Case uMsg
    ' ���j���[�T�C�Y�̃Z�b�g
    Case WM_MEASUREITEM
        ' lParam�̎w��MEASUREITEMSTRUCT�\���̂̓��e���R�s�[
        CopyMemory tMEASUREITEMSTRUCT, ByVal lParam, ByVal Len(tMEASUREITEMSTRUCT)
        ' �E�C���h�E�n���h�����f�o�C�X�R���e�L�X�g�n���h���擾
        hdc = GetDC(hwnd)
        ' ������T�C�Y�擾
        With tMenu(tMEASUREITEMSTRUCT.itemData)
            Call DrawText(hdc, .strMenu, -1, tRC, DT_CALCRECT Or DT_EXPANDTABS)
        End With
        ' �T�C�Y�̐ݒ�
        tMEASUREITEMSTRUCT.itemWidth = tRC.Right + 22
        tMEASUREITEMSTRUCT.itemHeight = tRC.Bottom + 8
        ' �f�o�C�X�R���e�L�X�g�n���h���̊J��
        ReleaseDC hwnd, hdc
        ' �ݒ肵��MEASUREITEMSTRUCT�\���̂������߂�
        CopyMemory ByVal lParam, tMEASUREITEMSTRUCT, ByVal Len(tMEASUREITEMSTRUCT)
        Exit Function
    ' �I�[�i�[�h���[�`��
    Case WM_DRAWITEM
        CopyMemory tDRAWITEMSTRUCT, ByVal lParam, ByVal Len(tDRAWITEMSTRUCT)
        Call psubDRAWITEM(tDRAWITEMSTRUCT)
        Exit Function
    End Select
    
    ' ���̃E�C���h�E�v���V�[�W���̌Ăяo��
    WindowProc = CallWindowProc(P_OrgProc, hwnd, uMsg, wParam, lParam)
End Function

'------------------
' �T�u�N���X���J�n
'------------------
Public Sub SubClass(ByVal hwnd As Long)
    Dim lngRet As Long
    ' ���ɃT�u�N���X������Ă��邩�`�F�b�N
    If P_OrgProc <> 0 Then
        ' ���ɂɃT�u�N���X������Ă���ꍇ�A�T�u�N���X���I��
        Call UnSubClass
    End If
    ' �T�u�N���X���J�n
    P_OrgProc = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WindowProc)
    P_Orghwnd = hwnd
End Sub

'------------------
' �T�u�N���X���I��
'------------------
Public Sub UnSubClass()
    Dim lngRet As Long
    ' �T�u�N���X������Ă���ꍇ
    If P_OrgProc <> 0 Then
        ' �E�C���h�E�v���V�[�W�������̃f�t�H���g�E�C���h�E�v���V�[�W���ɐݒ�
        lngRet = SetWindowLong(P_Orghwnd, GWL_WNDPROC, P_OrgProc)
        P_Orghwnd = 0
        P_OrgProc = 0
    End If
End Sub
