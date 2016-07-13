Attribute VB_Name = "mdlWinAPI"
Option Explicit

'ç\ë¢ëÃêÈåæ
Public Type MENUITEMINFO
    cbSize As Long
    fMask As Long
    fType As Long
    fState As Long
    wID As Long
    hSubMenu As Long
    hbmpChecked As Long
    hbmpUnchecked As Long
    dwItemData As Long
    dwTypeData As String
    cch As Long
End Type

Public Type MEASUREITEMSTRUCT
        CtlType As Long
        CtlID As Long
        itemID As Long
        itemWidth As Long
        itemHeight As Long
        itemData As Long
End Type

Public Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type

Public Type DRAWITEMSTRUCT
        CtlType As Long
        CtlID As Long
        itemID As Long
        itemAction As Long
        itemState As Long
        hwndItem As Long
        hdc As Long
        rcItem As RECT
        itemData As Long
End Type

'ä÷êîêÈåæ
Public Declare Function GetMenu Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function GetMenuItemCount _
    Lib "user32" (ByVal hmenu As Long) As Long
Public Declare Function GetSubMenu _
    Lib "user32" (ByVal hmenu As Long, ByVal nPos As Long) As Long
Public Declare Function GetMenuItemID _
    Lib "user32" (ByVal hmenu As Long, ByVal nPos As Long) As Long
Public Declare Function GetMenuItemInfo Lib "user32" _
    Alias "GetMenuItemInfoA" (ByVal hmenu As Long, _
                              ByVal un As Long, _
                              ByVal b As Long, _
                              lpMenuItemInfo As MENUITEMINFO) As Long
Public Declare Function SetMenuItemInfo Lib "user32" _
    Alias "SetMenuItemInfoA" (ByVal hmenu As Long, _
                              ByVal un As Long, _
                              ByVal bool As Boolean, _
                              lpcMenuItemInfo As MENUITEMINFO) As Long
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
Public Declare Sub CopyMemory Lib "kernel32" _
    Alias "RtlMoveMemory" (Destination As Any, _
                           Source As Any, _
                           ByVal Length As Long)
Public Declare Function GetSysColor _
    Lib "user32" (ByVal nIndex As Long) As Long
Public Declare Function SetTextColor _
    Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
Public Declare Function CreateSolidBrush _
    Lib "gdi32" (ByVal crColor As Long) As Long
Public Declare Function FillRect _
    Lib "user32" (ByVal hdc As Long, _
                  lpRect As RECT, _
                  ByVal hBrush As Long) As Long
Public Declare Function DeleteObject _
    Lib "gdi32" (ByVal hObject As Long) As Long
Public Declare Function SetBkMode _
    Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long
Public Declare Function DrawText Lib "user32" _
    Alias "DrawTextA" (ByVal hdc As Long, _
                       ByVal lpStr As String, _
                       ByVal nCount As Long, _
                       lpRect As RECT, _
                       ByVal wFormat As Long) As Long

'íËêîêÈåæ
Public Const MIIM_SUBMENU = &H4
Public Const MIIM_TYPE = &H10
Public Const MIIM_DATA = &H20
Public Const MF_OWNERDRAW = &H100
Public Const MF_SEPARATOR = &H800

Public Const GWL_WNDPROC = (-4)
Public Const WM_MEASUREITEM = &H2C
Public Const WM_DRAWITEM = &H2B

Public Const COLOR_MENU = 4
Public Const COLOR_MENUTEXT = 7
Public Const COLOR_HIGHLIGHT = 13
Public Const COLOR_HIGHLIGHTTEXT = 14
Public Const COLOR_GRAYTEXT = 17

Public Const ODS_SELECTED = &H1
Public Const ODS_GRAYED = &H2
Public Const ODS_DISABLED = &H4

Public Const TRANSPARENT = 1

Public Const DT_EXPANDTABS = &H40
Public Const DT_LEFT = &H0
Public Const DT_VCENTER = &H4
Public Const DT_SINGLELINE = &H20


