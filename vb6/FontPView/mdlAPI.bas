Attribute VB_Name = "mdlAPI"
Option Explicit

Public Const LF_FACESIZE = 32

Public Type LOGFONT
    lfHeight As Long
    lfWidth As Long
    lfEscapement As Long
    lfOrientation As Long
    lfWeight As Long
    lfItalic As Byte
    lfUnderline As Byte
    lfStrikeOut As Byte
    lfCharSet As Byte
    lfOutPrecision As Byte
    lfClipPrecision As Byte
    lfQuality As Byte
    lfPitchAndFamily As Byte
    'lfFaceName(1 To LF_FACESIZE) As Byte
    lfFaceName As String * LF_FACESIZE
End Type

Public Const WHITE_BRUSH = 0
Public Const DEFAULT_GUI_FONT = 17

Public Declare Function GetStockObject Lib "gdi32" (ByVal nIndex As Long) As Long

Public Declare Function GetObject Lib "gdi32" _
    Alias "GetObjectA" (ByVal hObject As Long, ByVal nCount As Long, lpObject As Any) As Long

Public Declare Function CreateCompatibleBitmap Lib "gdi32" ( _
    ByVal hdc As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long

Public Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long

Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long

Public Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Public Declare Function SetRect Lib "user32" ( _
    lpRect As RECT, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long

Public Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long

Public Declare Function SetBkMode Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long

Public Const BDR_RAISED = &H5
Public Const BF_LEFT = &H1
Public Const BF_TOP = &H2
Public Const BF_RIGHT = &H4
Public Const BF_BOTTOM = &H8
Public Const BF_RECT = (BF_LEFT Or BF_TOP Or BF_RIGHT Or BF_BOTTOM)
Public Const BF_ADJUST = &H2000   ' Calculate the space left over.

Public Declare Function DrawEdge Lib "user32" ( _
    ByVal hdc As Long, qrc As RECT, ByVal edge As Long, ByVal grfFlags As Long) As Long
    
Public Declare Function InflateRect Lib "user32" (lpRect As RECT, ByVal x As Long, ByVal y As Long) As Long

Public Declare Function TextOut Lib "gdi32" Alias "TextOutA" ( _
    ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal lpString As String, ByVal nCount As Long) As Long

Public Declare Function FrameRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long

Public Const SRCCOPY = &HCC0020 ' (DWORD) dest = source

Public Declare Function BitBlt Lib "gdi32" ( _
    ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, _
    ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long

Public Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long

Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

Public Declare Function CreateFontIndirect Lib "gdi32" Alias "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long

Public Const DT_CENTER = &H1
Public Const DT_VCENTER = &H4
Public Const DT_SINGLELINE = &H20

Public Declare Function DrawText Lib "user32" Alias "DrawTextA" ( _
    ByVal hdc As Long, ByVal lpStr As String, ByVal nCount As Long, lpRect As RECT, ByVal wFormat As Long) As Long

Public Declare Function OffsetRect Lib "user32" (lpRect As RECT, ByVal x As Long, ByVal y As Long) As Long

Public Declare Function MulDiv Lib "kernel32" ( _
    ByVal nNumber As Long, ByVal nNumerator As Long, ByVal nDenominator As Long) As Long

Public Const LOGPIXELSY = 90        '  Logical pixels/inch in Y

Public Declare Function GetDeviceCaps Lib "gdi32" (ByVal hdc As Long, ByVal nIndex As Long) As Long

Public Declare Function SetTextCharacterExtra Lib "gdi32" (ByVal hdc As Long, ByVal nCharExtra As Long) As Long

Public Declare Function GetTextCharacterExtra Lib "gdi32" (ByVal hdc As Long) As Long

