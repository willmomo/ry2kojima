VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "clsBrush Sample"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.PictureBox pic 
      Height          =   1575
      Left            =   240
      ScaleHeight     =   1515
      ScaleWidth      =   2715
      TabIndex        =   0
      Top             =   240
      Width           =   2775
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Private Declare Function Rectangle Lib "gdi32" (ByVal hdc As Long, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long

Private Sub Form_Load()
    pic.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
End Sub

Private Sub Form_Resize()
    pic.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
    pic.Refresh
End Sub

Private Sub pic_Paint()
    Dim ret As Long
    Dim br(1) As New clsBrush
    Dim brOld As New clsBrush
    
    If br(0).CreateSolidBrush(RGB(255, 0, 0)) = False Then GoTo Exit_pic_Paint
    If br(1).CreateSolidBrush(RGB(0, 0, 255)) = False Then GoTo Exit_pic_Paint
    
    brOld.Attach SelectObject(pic.hdc, br(0).Brush)

    Rectangle pic.hdc, 10, 10, 100, 100
    
    SelectObject pic.hdc, br(1).Brush
    Rectangle pic.hdc, (pic.ScaleWidth / Screen.TwipsPerPixelX) - 100, _
        (pic.ScaleHeight / Screen.TwipsPerPixelY) - 100, _
        (pic.ScaleWidth / Screen.TwipsPerPixelX), _
        (pic.ScaleHeight / Screen.TwipsPerPixelY)
    
    
Exit_pic_Paint:
    If brOld.Brush <> 0 Then SelectObject pic.hdc, brOld.Detach
    br(0).DeleteObject
    br(1).DeleteObject
End Sub
