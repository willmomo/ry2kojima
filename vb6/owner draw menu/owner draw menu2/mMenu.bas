Attribute VB_Name = "mMenu"
'
'*************************************************************************
'
'                        ���j���[�֘A���W���[��
'
'*************************************************************************
'
Option Explicit

'���� �萔�錾 ����
'---------------------------------------------------------
'     MENUITEMINFO �\���̂� fType �����o�Ŏg�p����
'---------------------------------------------------------
Public Const MF_STRING = &H0&
Public Const MF_BITMAP = &H4&
Public Const MF_MENUBARBREAK = &H20&
Public Const MF_MENUBREAK = &H40&
Public Const MF_OWNERDRAW = &H100&
Public Const MF_SEPARATOR = &H800&
Public Const MF_RIGHTJUSTIFY = &H4000&

Public Const MFT_STRING = MF_STRING                 ' �e�L�X�g������A�C�e��   : dwTypeData = ������̃|�C���^
                                                    '                          : cch        = ������T�C�Y
Public Const MFT_BITMAP = MF_BITMAP                 ' �r�b�g�}�b�v�A�C�e��     : dwTypeData = �r�b�g�}�b�v�n���h��
Public Const MFT_MENUBARBREAK = MF_MENUBARBREAK     ' �o�[�u���[�N�A�C�e��
Public Const MFT_MENUBREAK = MF_MENUBREAK           ' �u���[�N�A�C�e��
Public Const MFT_OWNERDRAW = MF_OWNERDRAW           ' �I�[�i�[�h���[�A�C�e��   : dwTypeData = ���[�U�[32�r�b�g�l
Public Const MFT_RADIOCHECK = &H200&                ' hbmpChecked �� NULL�̏ꍇ ���W�I�{�^���}�[�N
Public Const MFT_SEPARATOR = MF_SEPARATOR           ' �Z�p���[�^�A�C�e��
Public Const MFT_RIGHTORDER = &H2000&               ' �E���獶�ւ̕����i�A���r�A��jWindows95/NT 5.0 �ȍ~
Public Const MFT_RIGHTJUSTIFY = MF_RIGHTJUSTIFY     '

' MFT_BITMAP, MFT_SEPARATOR, MFT_STRING �����̃t���O�͓����Ɏg�p�ł��Ȃ�

'---------------------------------------------------------
'    MENUITEMINFO �\���̂� fState �����o�Ŏg�p����
'---------------------------------------------------------
Public Const MF_UNCHECKED = &H0&
Public Const MF_ENABLED = &H0&
Public Const MF_GRAYED = &H1&
Public Const MF_DISABLED = &H2&
Public Const MF_CHECKED = &H8&
Public Const MF_POPUP = &H10&
Public Const MF_UNHILITE = &H0&
Public Const MF_HILITE = &H80&
Public Const MF_DEFAULT = &H1000&

Public Const MFS_GRAYED = &H3&                      ' �I��s�i�D�F�j�A�C�e��
Public Const MFS_DISABLED = MFS_GRAYED              ' �I��s�i�D�F�j�A�C�e��
Public Const MFS_CHECKED = MF_CHECKED               ' �`�F�b�N�A�C�e��
Public Const MFS_HILITE = MF_HILITE                 ' �����A�C�e��
Public Const MFS_ENABLED = MF_ENABLED               ' �I���A�C�e��
Public Const MFS_UNCHECKED = MF_UNCHECKED           ' ��`�F�b�N�A�C�e��
Public Const MFS_UNHILITE = MF_UNHILITE             ' �񋭒��A�C�e��
Public Const MFS_DEFAULT = MF_DEFAULT               ' �f�t�H���g�A�C�e���i����?�j

'---------------------------------------------------------
'     MENUITEMINFO �\���̂� fMask �����o�Ŏg�p����
'---------------------------------------------------------
Public Const MIIM_STATE = &H1       ' fState �����o�̎擾�܂��͐ݒ�
Public Const MIIM_ID = &H2          ' wID �����o�̎擾�܂��͐ݒ�
Public Const MIIM_SUBMENU = &H4     ' hSubMenu �����o�̎擾�܂��͐ݒ�
Public Const MIIM_CHECKMARKS = &H8  ' hbmpChecked,hbmpUnchecked �����o�̎擾�܂��͐ݒ�
Public Const MIIM_TYPE = &H10       ' fType,dwTypeData �����o�̎擾�܂��͐ݒ�
Public Const MIIM_DATA = &H20       ' dwItemData �����o�̎擾�܂��͐ݒ�

'���� �\���̐錾 ����
Type MENUITEMINFO
    cbSize As Long          ' ���̍\���̂̃T�C�Y
    fMask As Long           ' �擾�܂��͐ݒ肷�郁���o�̎w��
    fType As Long           ' ���j���[�A�C�e���̃^�C�v
    fState As Long          ' ���j���[�A�C�e���̏��
    wID As Long             ' ���j���[�A�C�e������ID
    hSubMenu As Long        ' �T�u���j���[�n���h���i�T�u���j���[�������Ȃ��ꍇNULL�j
    hbmpChecked As Long     ' �`�F�b�N���r�b�g�}�b�v�n���h��
    hbmpUnchecked As Long   ' ��`�F�b�N���r�b�g�}�b�v�n���h��
    dwItemData As Long      ' ���j���[�A�C�e���Ɋ֘A�����A�v���P�[�V������`���
    dwTypeData As String    ' ���j���[�A�C�e���̓��e
    cch As Long             ' �e�L�X�g�̒���
End Type

'���� API�֐��錾 ����
' �E�B���h�E�Ɋ��蓖�Ă�ꂽ���j���[�̃n���h�����擾
Declare Function GetMenu Lib "user32" (ByVal hwnd As Long) As Long
' �T�u���j���[�̃n���h�����擾
Declare Function GetSubMenu Lib "user32" (ByVal hMenu As Long, ByVal nPos As Long) As Long
' ���j���[�̍��ڐ��擾
Declare Function GetMenuItemCount Lib "user32" (ByVal hMenu As Long) As Long
' ���j���[���ڂ�ID���擾
Declare Function GetMenuItemID Lib "user32" (ByVal hMenu As Long, ByVal nPos As Long) As Long
' ���j���[���ڂɊւ�������擾
Declare Function GetMenuItemInfo Lib "user32" Alias "GetMenuItemInfoA" (ByVal hMenu As Long, ByVal un As Long, ByVal b As Boolean, lpMenuItemInfo As MENUITEMINFO) As Long
' ���j���[���ڂɊւ������ݒ�
Declare Function SetMenuItemInfo Lib "user32" Alias "SetMenuItemInfoA" (ByVal hMenu As Long, ByVal un As Long, ByVal bool As Boolean, lpcMenuItemInfo As MENUITEMINFO) As Long
' ���j���[�o�[���ĕ`��
Declare Function DrawMenuBar Lib "user32" (ByVal hwnd As Long) As Long

'���� ���j���[����ۑ�����ׂ̐錾 ����
Type tMenuStruct
    hMenu As Long               ' ���j���[�n���h��
    lngPosition As Long         ' ���j���[�|�W�V����(��ԏ�̍��ڂ�0�Ƃ����ꍇ�̈ʒu)
    strMenu As String           ' ���j���[������
End Type

Public tMenu() As tMenuStruct   ' ���j���[����ۑ�����z��
Public lngtMenuCount As Long    ' tMenu �z��̐�

'---------------------------------------
' ���j���[���ۑ� & �I�[�i�[�h���[�ݒ�
'---------------------------------------
Private Sub psubGetMenuItemInfo(ByVal hMenu As Long, ByVal lngID As Long, ByVal booKey As Boolean, ByVal lngPosition As Long)
    Dim tMENUINFO As MENUITEMINFO   ' MENUITEMINFO�\����
    Dim strMenu As String           ' ���j���[������
    
    With tMENUINFO
        .cbSize = Len(tMENUINFO)
        .fMask = MIIM_TYPE Or MIIM_DATA
        .dwTypeData = Space(255)
        .cch = 255
        ' ���j���[���ڏ��擾
        If GetMenuItemInfo(hMenu, lngID, booKey, tMENUINFO) <> 0 Then
            ' �Z�p���[�^�̏ꍇ�̓I�[�i�[�h���[�ɂ��Ȃ�
            If .fType And MF_SEPARATOR Then Exit Sub
            ' ���j���[�e�L�X�g���擾
            strMenu = Left(.dwTypeData, InStr(.dwTypeData, Chr(0)) - 1)
            ' ���j���[����ۑ�����z��̊m��
            ReDim Preserve tMenu(lngtMenuCount)
            ' ���j���[����ۑ�
            tMenu(lngtMenuCount).hMenu = hMenu
            tMenu(lngtMenuCount).lngPosition = lngPosition
            tMenu(lngtMenuCount).strMenu = strMenu
            ' �I�[�i�[�h���[�t���O & �A�C�e���f�[�^ �Z�b�g
            .fType = .fType Or MF_OWNERDRAW
            .dwItemData = lngtMenuCount
            Call SetMenuItemInfo(hMenu, lngID, booKey, tMENUINFO)
            lngtMenuCount = lngtMenuCount + 1
        End If
    End With
End Sub

'------------------------------------------------------
' �w�肳�ꂽ�E�C���h�E�̃��j���[���I�[�i�[�h���[�ɐݒ�
'------------------------------------------------------
Public Sub SetMenuOwnerDraw(ByVal hMenu As Long)
    Dim lngCount As Long    ' ���j���[�̍��ڐ�
    Dim i As Long           ' ���[�v�J�E���^
    Dim lngID As Long       ' ���j���[���ڎ���ID
    
    ' ���j���[�̍��ڐ��擾
    lngCount = GetMenuItemCount(hMenu)
    ' ���j���[���ڃ��[�v
    For i = 0 To lngCount - 1
        ' ���j���[���ڂ�ID�擾
        lngID = GetMenuItemID(hMenu, i)
        If lngID = -1 Then
            ' �T�u���j���[���ڂ̏ꍇ
            ' ���j���[���ۑ� & �I�[�i�[�h���[�ݒ�
            Call psubGetMenuItemInfo(hMenu, i, True, i)
            ' �ēx���j���[���ڗ�
            Call SetMenuOwnerDraw(GetSubMenu(hMenu, i))
        Else
            ' ���j���[���ڂ̏ꍇ
            ' ���j���[���ۑ� & �I�[�i�[�h���[�ݒ�
            Call psubGetMenuItemInfo(hMenu, lngID, False, i)
        End If
    Next i
End Sub





