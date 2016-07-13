Attribute VB_Name = "Module1"
Option Explicit

Public Const GW_OWNER = 4

Public Declare Function EnumWindows Lib "user32" (ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Public Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hwnd As Long, lpdwProcessId As Long) As Long
Public Declare Function GetWindow Lib "user32" (ByVal hwnd As Long, ByVal wCmd As Long) As Long
Public Declare Function IsWindowVisible Lib "user32" (ByVal hwnd As Long) As Long

Public g_hwndFromPID As Long

Private Function EnumWindowsProc(ByVal hwnd As Long, ByVal lParam As Long) As Long
    Dim pid As Long
    
    GetWindowThreadProcessId hwnd, pid
    
    If pid = lParam Then
        Debug.Print Now, hwnd, pid, GetWindow(hwnd, GW_OWNER), IsWindowVisible(hwnd)
        
        g_hwndFromPID = hwnd
        EnumWindowsProc = 0
    Else
        EnumWindowsProc = 1
    End If
    
End Function

Public Function GetWindowHandleFromPID(ByVal pid As Long) As Long

    g_hwndFromPID = 0
    
    EnumWindows AddressOf EnumWindowsProc, pid
    
    GetWindowHandleFromPID = g_hwndFromPID
    
End Function
