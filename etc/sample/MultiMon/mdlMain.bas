Attribute VB_Name = "mdlMain"
Option Explicit

Public Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Public Declare Function EnumDisplayMonitors Lib "user32" ( _
    ByVal hdc As Long, _
    ByVal lprcClip As Long, _
    ByVal lpfnEnum As Long, _
    ByVal dwData As Long) As Long

Public Type MONITORINFO
    cbSize As Long
    rcMonitor As RECT
    rcWork As RECT
    dwFlags As Long
End Type

Public Declare Function GetMonitorInfo Lib "user32" Alias "GetMonitorInfoA" ( _
    ByVal hMonitor As Long, _
    ByRef lpmi As MONITORINFO) As Long
    
Public Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
    
Public Declare Function GetLastError Lib "kernel32" () As Long

Public g_mi() As MONITORINFO

Public Function GetWidth(rc As RECT) As Long
    GetWidth = rc.Right - rc.Left
End Function

Public Function GetHeight(rc As RECT) As Long
    GetHeight = rc.Bottom - rc.Top
End Function

Public Function EnumProc(ByVal hMonitor As Long, _
                         ByVal hdcMonitor As Long, _
                         ByRef lprcMonitor As RECT, _
                         ByVal dwData As Long) As Long
    
    Dim mi As MONITORINFO
    
    '
    ' 列挙されたモニタの詳細情報を取得
    '
    mi.cbSize = Len(mi)
    If GetMonitorInfo(hMonitor, mi) = 0 Then
        MsgBox "GetMonitorInfo error. " & Err.LastDllError
        EnumProc = 0
        Exit Function
    End If

    On Error GoTo ReDimHandler
    ReDim Preserve g_mi(UBound(g_mi) + 1)
    g_mi(UBound(g_mi)) = mi
    
    EnumProc = 1
    Exit Function
    
ReDimHandler:
    ReDim g_mi(0)
    Resume Next
    
End Function

Public Sub main()
    
    Dim i As Long
    Dim frm() As frmWindow

    '
    ' ディスプレイを列挙して、g_mi() に保存する
    '
    If EnumDisplayMonitors(0, 0, AddressOf EnumProc, 0) = 0 Then
        MsgBox "EnumDisplayMonitors error." & Err.LastDllError
        Exit Sub
    End If
    
    '
    ' ディスプレイの数だけフォームを作成して表示する
    '
    ReDim frm(UBound(g_mi))
    For i = 0 To UBound(g_mi)
        Set frm(i) = New frmWindow
        frm(i).MonitorNo = i
        frm(i).Show
    Next
    
    MsgBox "モニタ " & i & " 個分ウィンドウを作成しました。"
    
End Sub

