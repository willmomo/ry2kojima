VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton Command2 
      Caption         =   "ロックファイルを閉じる"
      Enabled         =   0   'False
      Height          =   495
      Left            =   840
      TabIndex        =   1
      Top             =   1680
      Width           =   3135
   End
   Begin VB.CommandButton Command1 
      Caption         =   "ロックファイルを作成"
      Height          =   495
      Left            =   840
      TabIndex        =   0
      Top             =   720
      Width           =   3135
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_handle As Long

Private Sub Command1_Click()
    Command1.Enabled = False
    
    Dim ret As Long
    
    ret = CreateShareLockfile()
    
    If ret > 0 Then
        MsgBox "lockfile created."
        Command2.Enabled = True
    ElseIf ret = 0 Then
        MsgBox "lockfile already exist."
        Command1.Enabled = True
        Command2.Enabled = False
    Else
        MsgBox "err err err. " & Err.LastDllError
        Command1.Enabled = True
        Command2.Enabled = False
    End If
End Sub

Private Sub Command2_Click()
    CloseShareLockfile
    
    Command2.Enabled = False
    Command1.Enabled = True
End Sub

