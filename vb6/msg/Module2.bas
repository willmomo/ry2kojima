Attribute VB_Name = "Module1"
Option Explicit

'-------------------------------------------------------
'
'-------------------------------------------------------
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Const GWL_WNDPROC = (-4)

Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

Public Const WM_COPYDATA = &H4A

Public phWnd As Long

Public Declare Sub memcpy Lib "kernel32" Alias "RtlMoveMemory" (pDest As Any, pSrc As Any, ByVal iSize&)

Type KAIIN_DATA
        kno As Long
        ptup As Long
        mondai(2) As String * 100
End Type

Type COPYDATASTRUCT
        dwData As Long
        cbData As Long
        lpData As Long
End Type


'-------------------------------------------------------
'
'-------------------------------------------------------
Public Function WndProc(ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    Static bolWndProcCheck As Boolean
    
    Dim kdata   As KAIIN_DATA
    Dim str     As String
    
    Dim lngret As Long
    Dim sdtCOPYDATASTRUCT As COPYDATASTRUCT
    
    If Not bolWndProcCheck Then
        bolWndProcCheck = True
        Select Case uMsg
        Case WM_COPYDATA
            Call memcpy(ByVal VarPtr(sdtCOPYDATASTRUCT), ByVal lParam&, LenB(sdtCOPYDATASTRUCT))
            
            str = sdtCOPYDATASTRUCT.dwData
            
'            Call memcpy(ByVal VarPtr(kdata), ByVal sdtCOPYDATASTRUCT.lpData, sdtCOPYDATASTRUCT.cbData)
            Call memcpy(ByVal VarPtr(kdata), ByVal sdtCOPYDATASTRUCT.lpData, LenB(kdata))
            
            
            With Form1
                .Text2.Text = str
                .Text1(0).Text = kdata.kno
                .Text1(1).Text = kdata.ptup
                .Text1(2).Text = kdata.mondai(0)
                .Text1(3).Text = kdata.mondai(1)
                .Text1(4).Text = kdata.mondai(2)
                
            End With
            
        End Select
        bolWndProcCheck = False
    End If
    WndProc = CallWindowProc(phWnd, hwnd, uMsg, wParam, lParam)
End Function

Public Sub Hook(hwnd As Long)
    phWnd = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WndProc)
End Sub

Public Sub UnHook(hwnd As Long)
    Dim lngret As Long
    If phWnd <> 0 Then
        lngret = SetWindowLong(hwnd, GWL_WNDPROC, phWnd)
    End If
End Sub

