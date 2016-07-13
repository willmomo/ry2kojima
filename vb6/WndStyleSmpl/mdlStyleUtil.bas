Attribute VB_Name = "mdlStyleUtil"
Option Explicit

Private Declare Function GetWindowLong Lib "user32" _
    Alias "GetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long) As Long
    
Private Declare Function SetWindowLong Lib "user32" _
    Alias "SetWindowLongA" ( _
        ByVal hwnd As Long, _
        ByVal nIndex As Long, _
        ByVal dwNewLong As Long) As Long

Public Const GWL_STYLE = (-16)
Public Const GWL_EXSTYLE = (-20)

Public Const WS_SYSMENU = &H80000
Public Const WS_CAPTION = &HC00000                  '  WS_BORDER Or WS_DLGFRAME

Public Const WS_EX_APPWINDOW = &H40000

''
' システムメニューを追加/削除する
'
Public Function hs_SetSysMenu(ByVal hwnd As Long, ByVal bFlag As Boolean) As Boolean
    Dim nStyle As Long
    
    nStyle = GetWindowLong(hwnd, GWL_STYLE)
    
    If bFlag Then
        nStyle = nStyle Or WS_SYSMENU
    Else
        nStyle = nStyle And (Not WS_SYSMENU)
    End If
    
    nStyle = SetWindowLong(hwnd, GWL_STYLE, nStyle)
    
    hs_SetSysMenu = IIf(nStyle And WS_SYSMENU, True, False)
End Function

''
' キャプションを追加/削除する
'
Public Function hs_SetCaption(ByVal hwnd As Long, ByVal bFlag As Boolean) As Boolean
    Dim nStyle As Long
    
    nStyle = GetWindowLong(hwnd, GWL_STYLE)
    
    If bFlag Then
        nStyle = nStyle Or WS_CAPTION
    Else
        nStyle = nStyle And (Not WS_CAPTION)
    End If
    
    nStyle = SetWindowLong(hwnd, GWL_STYLE, nStyle)
    
    hs_SetCaption = IIf(nStyle And WS_SYSMENU, True, False)
End Function

''
' アプリケーションウィンドウ属性を追加/削除する
'
Public Function hs_SetExAppWindow(ByVal hwnd As Long, ByVal bFlag As Boolean) As Boolean
    Dim nStyle As Long
    
    nStyle = GetWindowLong(hwnd, GWL_EXSTYLE)
    
    If bFlag Then
        nStyle = nStyle Or WS_EX_APPWINDOW
    Else
        nStyle = nStyle And (Not WS_EX_APPWINDOW)
    End If
    
    nStyle = SetWindowLong(hwnd, GWL_EXSTYLE, nStyle)
    
    hs_SetExAppWindow = IIf(nStyle And WS_SYSMENU, True, False)
End Function

