Attribute VB_Name = "mdlAPI"
Option Explicit

Public Const GWL_WNDPROC = (-4)

Public Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" ( _
    ByVal hwnd As Long, _
    ByVal nIndex As Long _
) As Long

Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" ( _
    ByVal hwnd As Long, _
    ByVal nIndex As Long, _
    ByVal dwNewLong As Long _
) As Long

Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" ( _
    ByVal lpPrevWndFunc As Long, _
    ByVal hwnd As Long, _
    ByVal Msg As Long, _
    ByVal wParam As Long, _
    ByVal lParam As Long _
) As Long

'Public Declare Function CreateDC Lib "gdi32" Alias "CreateDCA" ( _
    ByVal lpDriverName As String, _
    ByVal lpDeviceName As String, _
    ByVal lpOutput As String, _
    lpInitData As DEVMODE _
) As Long

Public Declare Function CreateDC Lib "gdi32" Alias "CreateDCA" ( _
    ByVal lpDriverName As String, _
    ByVal lpDeviceName As Long, _
    ByVal lpOutput As Long, _
    ByVal lpInitData As Long _
) As Long

Public Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long

Public Declare Function BitBlt Lib "gdi32" ( _
    ByVal hDestDC As Long, _
    ByVal X As Long, _
    ByVal Y As Long, _
    ByVal nWidth As Long, _
    ByVal nHeight As Long, _
    ByVal hSrcDC As Long, _
    ByVal xSrc As Long, _
    ByVal ySrc As Long, _
    ByVal dwRop As Long _
) As Long

Public Const SRCCOPY = &HCC0020 ' (DWORD) dest = source
Public Const CAPTUREBLT = &H40000000    '半透明ウィンドウもキャプチャ可能にする

Public Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long

Public Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hdc As Long) As Long

Public Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long

Public Const MOD_ALT = &H1
Public Const MOD_CONTROL = &H2
Public Const MOD_SHIFT = &H4

Public Const IDHOT_SNAPWINDOW = (-1)    '  SHIFT-PRINTSCRN
Public Const IDHOT_SNAPDESKTOP = (-2)    '  PRINTSCRN

Public Const VK_SNAPSHOT = &H2C

Public Declare Function RegisterHotKey Lib "user32" ( _
    ByVal hwnd As Long, _
    ByVal id As Long, _
    ByVal fsModifiers As Long, _
    ByVal vk As Long _
) As Long

Public Declare Function UnregisterHotKey Lib "user32" ( _
    ByVal hwnd As Long, _
    ByVal id As Long _
) As Long

Public Const WM_HOTKEY = &H312

' // Tray notification definitions

Public Type NOTIFYICONDATA
    cbSize As Long
    hwnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage As Long
    hIcon As Long
    szTip As String * 64
End Type

Public Const NIM_ADD = &H0
Public Const NIM_MODIFY = &H1
Public Const NIM_DELETE = &H2

Public Const NIF_MESSAGE = &H1
Public Const NIF_ICON = &H2
Public Const NIF_TIP = &H4

Public Declare Function Shell_NotifyIcon Lib "shell32.dll" Alias "Shell_NotifyIconA" ( _
    ByVal dwMessage As Long, _
    lpData As NOTIFYICONDATA _
) As Long

' // End Tray Notification Icons

Public Const IDI_APPLICATION = 32512&

'Public Declare Function LoadIcon Lib "user32" Alias "LoadIconA" (ByVal hInstance As Long, ByVal lpIconName As String) As Long

Public Declare Function LoadIcon Lib "user32" Alias "LoadIconA" (ByVal hInstance As Long, ByVal lpIconName As Long) As Long

Public Const WM_MOUSEMOVE = &H200
Public Const WM_LBUTTONDOWN = &H201
Public Const WM_RBUTTONDOWN = &H204

' Private Window Messages Start Here:
Public Const WM_USER = &H400

Public Declare Sub keybd_event Lib "user32.dll" ( _
    ByVal bVk As Byte, _
    ByVal bScan As Byte, _
    ByVal dwFlags As Long, _
    ByVal dwExtraInfo As Long _
)

Public Const VK_LMENU = &HA4               'Altキー
Public Const KEYEVENTF_KEYUP = &H2         'キーはアップ状態
Public Const KEYEVENTF_EXTENDEDKEY = &H1   'スキャンコードは拡張コード

Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Declare Function GetWindowDC Lib "user32" (ByVal hwnd As Long) As Long

