Attribute VB_Name = "mdlOwnerDrawMenu"
Option Explicit


'変数宣言
Public g_menuString() As String * 128

'与えられたメニュー以下すべてのメニューアイテムを
'オーナー・ドロー化する
Public Sub SetOwnerDraw(hmenu As Long)
    Dim menuCnt As Long
    Dim mif As MENUITEMINFO
    Dim i As Long
    Dim idx As Long
    
    'メニューの中にあるアイテムの数だけループ
    For i = 0 To GetMenuItemCount(hmenu) - 1
        
        mif.cbSize = Len(mif)
        mif.fMask = MIIM_TYPE Or MIIM_DATA Or MIIM_SUBMENU
        mif.dwTypeData = Space(255)
        mif.cch = 255
        
        GetMenuItemInfo hmenu, i, True, mif
        
        'サブメニューのときは...
        'セパレータのときは...
        If mif.hSubMenu <> 0 Then
            SetOwnerDraw (mif.hSubMenu)
        ElseIf mif.fType And MF_SEPARATOR Then
            'セパレータなら何もしない
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
