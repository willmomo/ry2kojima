VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "SysUtils"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   1800
      TabIndex        =   1
      Top             =   1320
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   3120
      TabIndex        =   0
      Top             =   2400
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim ret As Long
    
    ret = SetOpacity(Me.hWnd, 0.8)
    If ret <> 0 Then
        MsgBox ret
    End If
End Sub

Private Sub Command2_Click()
    Dim ret As Long
    
    ret = SetLayered(Me.hWnd, False)
    If ret <> 0 Then
        MsgBox ret
    End If
End Sub
