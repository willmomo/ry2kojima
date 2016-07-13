VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6045
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8745
   LinkTopic       =   "Form1"
   ScaleHeight     =   6045
   ScaleWidth      =   8745
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.Timer Timer1 
      Interval        =   500
      Left            =   7560
      Top             =   3720
   End
   Begin VB.PictureBox pic 
      Height          =   4815
      Left            =   240
      ScaleHeight     =   4755
      ScaleWidth      =   6795
      TabIndex        =   0
      Top             =   240
      Width           =   6855
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function GetTickCount Lib "kernel32" () As Long

Private m_pnlex As clsPanelExt

Private Sub Form_Load()
    Dim strErr As String
    
    Set m_pnlex = New clsPanelExt
    m_pnlex.LoadFile ".\panelext.xml", strErr
    
    Debug.Print Now, strErr
End Sub

Private Sub Form_Resize()
    pic.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_pnlex = Nothing
End Sub

Private Sub pic_Click()
    Dim strErr As String
    
    m_pnlex.ReLoadFile strErr
    
    Debug.Print Now, strErr
End Sub

Private Sub pic_Paint()
    On Error GoTo Err_pic_Paint
    
    Dim n As Long
    n = GetTickCount
    m_pnlex.Draw pic.hdc
    
Exit_pic_Paint:
    Exit Sub
    
Err_pic_Paint:
    Debug.Print Err.Number & ":" & Err.Description
    Resume Exit_pic_Paint
    
End Sub

Private Sub Timer1_Timer()
    pic.Refresh
End Sub
