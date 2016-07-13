Attribute VB_Name = "mdlAPI"
Option Explicit

Public Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Public Const BDR_RAISEDOUTER = &H1
Public Const BDR_RAISEDINNER = &H4
Public Const EDGE_RAISED = (BDR_RAISEDOUTER Or BDR_RAISEDINNER)

Public Const BF_LEFT = &H1
Public Const BF_TOP = &H2
Public Const BF_RIGHT = &H4
Public Const BF_BOTTOM = &H8
Public Const BF_RECT = (BF_LEFT Or BF_TOP Or BF_RIGHT Or BF_BOTTOM)

Public Const BF_MIDDLE = &H800    ' Fill in the middle.
Public Const BF_ADJUST = &H2000   ' Calculate the space left over.

Public Declare Function DrawEdge Lib "user32" ( _
    ByVal hdc As Long, _
    qrc As RECT, _
    ByVal edge As Long, _
    ByVal grfFlags As Long _
) As Long

Public Declare Function SetRect Lib "user32" ( _
    lpRect As RECT, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long _
) As Long


Public Const DEFAULT_GUI_FONT = 17

Public Declare Function GetStockObject Lib "gdi32" (ByVal nIndex As Long) As Long

Public Declare Function GetObject Lib "gdi32" Alias "GetObjectA" ( _
    ByVal hObject As Long, _
    ByVal nCount As Long, _
    lpObject As Any _
) As Long

' Logical Font
Public Const LF_FACESIZE = 32
Public Const LF_FULLFACESIZE = 64

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
    lfFaceName(1 To LF_FACESIZE) As Byte
End Type

Public Declare Function CreateFontIndirect Lib "gdi32" Alias "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long

Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long

Public Const DT_CENTER = &H1
Public Const DT_VCENTER = &H4
Public Const DT_SINGLELINE = &H20

Public Declare Function DrawText Lib "user32" Alias "DrawTextA" ( _
    ByVal hdc As Long, _
    ByVal lpStr As String, _
    ByVal nCount As Long, _
    lpRect As RECT, _
    ByVal wFormat As Long _
) As Long

Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

Public Declare Function OffsetRect Lib "user32" (lpRect As RECT, ByVal x As Long, ByVal y As Long) As Long

Public Const TA_LEFT = 0
Public Const TA_RIGHT = 2
Public Const TA_CENTER = 6
Public Const TA_BASELINE = 24

Public Declare Function SetTextAlign Lib "gdi32" (ByVal hdc As Long, ByVal wFlags As Long) As Long

Declare Function TextOut Lib "gdi32" Alias "TextOutA" ( _
    ByVal hdc As Long, _
    ByVal x As Long, _
    ByVal y As Long, _
    ByVal lpString As String, _
    ByVal nCount As Long _
) As Long

Public Const LOGPIXELSX = 88        '  Logical pixels/inch in X
Public Const LOGPIXELSY = 90        '  Logical pixels/inch in Y

Public Declare Function GetDeviceCaps Lib "gdi32" (ByVal hdc As Long, ByVal nIndex As Long) As Long

Public Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long

Public Declare Function UnionRect Lib "user32" (lpDestRect As RECT, lpSrc1Rect As RECT, lpSrc2Rect As RECT) As Long

'----------------------------------------------------------------------
'point size ‚ð pixel size ‚É•ÏŠ·‚·‚é
'
Public Function PixelToPoint(ByVal pixelValue As Long) As Long
    PixelToPoint = (-pixelValue * 72) / GetDeviceCaps(GetDC(0), LOGPIXELSY)
End Function

'----------------------------------------------------------------------
'pixel size ‚ð point size ‚É•ÏŠ·‚·‚é
'
Public Function PointToPixel(ByVal pointValue As Long) As Long
    PointToPixel = -((pointValue * GetDeviceCaps(GetDC(0), LOGPIXELSY)) / 72)
End Function

