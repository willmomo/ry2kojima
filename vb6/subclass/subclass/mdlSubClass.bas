Attribute VB_Name = "mdlSubClass"
Option Explicit

'関数宣言
Public Declare Function SetWindowLong Lib "user32" _
    Alias "SetWindowLongA" (ByVal hwnd As Long, _
                            ByVal nIndex As Long, _
                            ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" _
    Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, _
                             ByVal hwnd As Long, _
                             ByVal Msg As Long, _
                             ByVal wParam As Long, _
                             ByVal lParam As Long) As Long

'定数宣言
Private Const GWL_WNDPROC = (-4)

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

'サブクラス化中のウィンドウプロシージャ
Public Function WndProc(ByVal hwnd As Long, _
                        ByVal uMsg As Long, _
                        ByVal wParam As Long, _
                        ByVal lParam As Long) As Long
    
    '元のウィンドウプロシージャを呼び出す
    WndProc = CallWindowProc(p_oldWndProc, hwnd, uMsg, wParam, lParam)
End Function

