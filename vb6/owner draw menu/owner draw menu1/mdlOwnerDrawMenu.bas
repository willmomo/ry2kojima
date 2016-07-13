Attribute VB_Name = "mdlOwnerDrawMenu"
Option Explicit


'�ϐ��錾
Public g_menuString() As String * 128

'�^����ꂽ���j���[�ȉ����ׂẴ��j���[�A�C�e����
'�I�[�i�[�E�h���[������
Public Sub SetOwnerDraw(hmenu As Long)
    Dim menuCnt As Long
    Dim mif As MENUITEMINFO
    Dim i As Long
    Dim idx As Long
    
    '���j���[�̒��ɂ���A�C�e���̐��������[�v
    For i = 0 To GetMenuItemCount(hmenu) - 1
        
        mif.cbSize = Len(mif)
        mif.fMask = MIIM_TYPE Or MIIM_DATA Or MIIM_SUBMENU
        mif.dwTypeData = Space(255)
        mif.cch = 255
        
        GetMenuItemInfo hmenu, i, True, mif
        
        '�T�u���j���[�̂Ƃ���...
        '�Z�p���[�^�̂Ƃ���...
        If mif.hSubMenu <> 0 Then
            SetOwnerDraw (mif.hSubMenu)
        ElseIf mif.fType And MF_SEPARATOR Then
            '�Z�p���[�^�Ȃ牽�����Ȃ�
        Else
            On Error Resume Next
            idx = UBound(g_menuString)
            If Err.Number = 9 Then
                idx = 0
                ReDim g_menuString(idx)
            End If
            On Error GoTo 0
            g_menuString(idx) = mif.dwTypeData
            ReDim Preserve g_menuString(idx + 1)
            
            mif.dwItemData = idx
            mif.fType = mif.fType Or MF_OWNERDRAW
            SetMenuItemInfo hmenu, i, True, mif
        End If
    Next i
End Sub
