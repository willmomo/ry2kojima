VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   6000
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   8940
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   6000
   ScaleWidth      =   8940
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin PanelCtrl.ucPanel ucPanel1 
      Height          =   4095
      Left            =   360
      TabIndex        =   0
      Top             =   360
      Width           =   6795
      _ExtentX        =   11986
      _ExtentY        =   7223
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "‚l‚r ‚oƒSƒVƒbƒN"
      FontSize        =   9
      FontStrikethrough=   0   'False
      NormalDaiColor  =   14737632
      NormalDaiForeColor=   0
      SelectedDaiColor=   12648384
      SelectedDaiForeColor=   255
      DisabledDaiColor=   4210752
      DisabledDaiForeColor=   12632256
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ƒtƒ@ƒCƒ‹(&F)"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "ŠJ‚­(&O)"
         Shortcut        =   ^O
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "•ÒW(&E)"
      Begin VB.Menu mnuEditAll 
         Caption         =   "‚·‚×‚Ä‘I‘ð(&A)"
      End
      Begin VB.Menu mnuEditClear 
         Caption         =   "‚·‚×‚Ä‘I‘ð‰ðœ(&C)"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "•\Ž¦(&V)"
      Begin VB.Menu mnuViewUp 
         Caption         =   "Šg‘å(&U)"
      End
      Begin VB.Menu mnuViewDown 
         Caption         =   "k¬(&D)"
      End
      Begin VB.Menu mnuViewRedraw 
         Caption         =   "Ä•`‰æ(&R)"
         Shortcut        =   {F5}
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Click()
    Debug.Print Now, "Form_Click"
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)

    Debug.Print Now, "Form_KeyDown"
    
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)

    Debug.Print Now, KeyAscii
    
End Sub

Private Sub Form_Resize()

    ucPanel1.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
    
    If ucPanel1.PanelWidth > 0 Then
        ucPanel1.PanelScale = ucPanel1.ViewWidth * 100 / ucPanel1.PanelWidth
        ucPanel1.Update
        'ucpanel1.Refresh
    End If
End Sub

Private Sub mnuEditAll_Click()

    Call ucPanel1.SelectAll(True)
    ucPanel1.Update
    ucPanel1.Refresh

End Sub

Private Sub mnuEditClear_Click()


    Call ucPanel1.SelectAll(False)
    ucPanel1.Update
    ucPanel1.Refresh
    
End Sub

Private Sub mnuFileOpen_Click()

    ucPanel1.LoadXML "panellayout.xml", ""
    ucPanel1.DaiEnabled(15) = False
    ucPanel1.DaiSelected(16) = True
    
    ucPanel1.EnableAll False
    
    Dim i As Long
    For i = 200 To 210
        ucPanel1.DaiCusromNormalColor(i) = RGB(0, 255, 255)
        ucPanel1.DaiEnabled(i) = True
    Next
    For i = 211 To 220
        ucPanel1.DaiCusromNormalColor(i) = RGB(0, 255, 0)
        ucPanel1.DaiCustomSelectedColor(i) = RGB(255, 0, 0)
        ucPanel1.DaiEnabled(i) = True
    Next
    
    ucPanel1.Update
    ucPanel1.Refresh
        
End Sub

Private Sub mnuViewDown_Click()

    ucPanel1.PanelScale = ucPanel1.PanelScale - 10
    ucPanel1.Update
    ucPanel1.Refresh

End Sub

Private Sub mnuViewRedraw_Click()

    ucPanel1.Update
    ucPanel1.Refresh
    
End Sub

Private Sub mnuViewUp_Click()

    ucPanel1.PanelScale = ucPanel1.PanelScale + 10
    ucPanel1.Update
    ucPanel1.Refresh
    
End Sub

Private Sub ucPanel1_BeforeSelect(hdb As Long, Enabled As Boolean, Selected As Boolean, Cancel As Boolean)

    If hdb = 5 Then
        Cancel = True
    End If
    
End Sub

Private Sub ucPanel1_Click()
    
    Dim buf() As Long
    Dim cnt As Long
    Dim i As Long
    
    cnt = ucPanel1.GetSelected(buf, True)
    For i = 0 To cnt - 1
        Debug.Print i, buf(i)
    Next
    
End Sub

Private Sub ucPanel1_KeyDown(KeyCode As Integer, Shift As Integer)

    Select Case KeyCode
    Case vbKeyLeft
        ucPanel1.OffsetX = ucPanel1.OffsetX + 10
    Case vbKeyRight
        ucPanel1.OffsetX = ucPanel1.OffsetX - 10
    Case vbKeyUp
        ucPanel1.OffsetY = ucPanel1.OffsetY + 10
    Case vbKeyDown
        ucPanel1.OffsetY = ucPanel1.OffsetY - 10
    End Select
    
End Sub
