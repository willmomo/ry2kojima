Attribute VB_Name = "mldSubClass"
Option Explicit

Private m_oldWndProc As Long

Public Sub StartSubClass(ByVal hwnd As Long)
    m_oldWndProc = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WndProc)
    
    If RegisterHotKey(hwnd, 0, MOD_CONTROL, VK_SNAPSHOT) = 0 Then
        MsgBox "Ctrl + PrintScreen を使用するアプリケーションがすでに動いています。", vbCritical Or vbOKOnly
    End If
    
    If RegisterHotKey(hwnd, 1, MOD_SHIFT, VK_SNAPSHOT) = 0 Then
        MsgBox "Shift + PrintScreen を使用するアプリケーションがすでに動いています。", vbCritical Or vbOKOnly
    End If
End Sub

Public Sub EndSubClass(ByVal hwnd As Long)
    UnregisterHotKey hwnd, 0
    UnregisterHotKey hwnd, 1
    
    SetWindowLong hwnd, GWL_WNDPROC, m_oldWndProc
End Sub

Public Function WndProc(ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

    Select Case Msg
        Case WM_HOTKEY
            If wParam = IDHOT_SNAPDESKTOP Then
                frmMain.SnapShot
            ElseIf wParam = 0 Then
                frmMain.SnapShot
            ElseIf wParam = 1 Then
                Shell "explorer " & App.Path & "\data", vbNormalFocus
            End If
    End Select
    
    WndProc = CallWindowProc(m_oldWndProc, hwnd, Msg, wParam, lParam)
End Function
