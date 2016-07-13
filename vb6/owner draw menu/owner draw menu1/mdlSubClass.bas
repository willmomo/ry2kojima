Attribute VB_Name = "mdlSubClass"
Option Explicit


'変数宣言
Private p_oldWndProc As Long

'サブクラス化の開始
Public Sub StartSubClass(ByVal hwnd As Long)
    '既にサブクラス化済みの場合は、サブクラス化終了
    If p_oldWndProc <> 0 Then Call EndSubClass(hwnd)
    
    'サブクラス化スタート
    p_oldWndProc = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WndProc)
End Sub

'サブクラス化の終了
Public Sub EndSubClass(ByVal hwnd As Long)
    If p_oldWndProc <> 0 Then _
        Call SetWindowLong(hwnd, GWL_WNDPROC, p_oldWndProc)
    p_oldWndProc = 0
End Sub

'WM_DRAWITEMメッセージ処理
Private Sub OnDrawItem(dis As DRAWITEMSTRUCT)
    Dim oldTextColor As Long
    Dim oldBkMode As Long
    Dim backColor As Long
    Dim hBrush As Long
    Dim rcTemp As RECT
    Dim idx(1) As Long
    
    With dis
        oldTextColor = SetTextColor(.hdc, GetSysColor(COLOR_MENUTEXT))
        backColor = GetSysColor(COLOR_MENU)
        If .itemState And (ODS_GRAYED Or ODS_DISABLED) Then
            SetTextColor .hdc, GetSysColor(COLOR_GRAYTEXT)
        ElseIf .itemState And ODS_SELECTED Then
            SetTextColor .hdc, GetSysColor(COLOR_HIGHLIGHTTEXT)
            backColor = GetSysColor(COLOR_HIGHLIGHT)
        End If
        
        ' 背景の塗りつぶしブラシ作成
        hBrush = CreateSolidBrush(backColor)
        ' 背景の塗りつぶし
        FillRect .hdc, .rcItem, hBrush
        ' 背景の塗りつぶしブラシ破棄
        DeleteObject hBrush
        
        rcTemp = .rcItem
        rcTemp.Left = rcTemp.Left + 32
        oldBkMode = SetBkMode(.hdc, TRANSPARENT)
        
        DrawText .hdc, g_menuString(dis.itemData), -1, rcTemp, _
            DT_EXPANDTABS Or DT_LEFT Or DT_VCENTER Or DT_SINGLELINE
        
        '色などを元に戻す
        SetTextColor .hdc, oldTextColor
    End With
End Sub

'サブクラス化中のウィンドウプロシージャ
Public Function WndProc(ByVal hwnd As Long, _
                        ByVal uMsg As Long, _
                        ByVal wParam As Long, _
                        ByVal lParam As Long) As Long
    Dim mis As MEASUREITEMSTRUCT
    Dim dis As DRAWITEMSTRUCT
    
    Select Case uMsg
    Case WM_MEASUREITEM
        CopyMemory mis, ByVal lParam, Len(mis)
        mis.itemWidth = 128
        mis.itemHeight = 64
        CopyMemory ByVal lParam, mis, Len(mis)
        WndProc = 0
        Exit Function
    Case WM_DRAWITEM
        CopyMemory dis, ByVal lParam, Len(dis)
        Call OnDrawItem(dis)
        CopyMemory ByVal lParam, dis, Len(dis)
        WndProc = 0
        Exit Function
    End Select

    '元のウィンドウプロシージャを呼び出す
    WndProc = CallWindowProc(p_oldWndProc, hwnd, uMsg, wParam, lParam)
End Function

