VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "ショートカットの作成"
   ClientHeight    =   3975
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11730
   LinkTopic       =   "Form1"
   ScaleHeight     =   3975
   ScaleWidth      =   11730
   StartUpPosition =   3  'Windows の既定値
   Begin VB.ComboBox Combo1 
      Height          =   300
      ItemData        =   "frmMain.frx":0000
      Left            =   1080
      List            =   "frmMain.frx":0034
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   5
      Top             =   360
      Width           =   2055
   End
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   3360
      Locked          =   -1  'True
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   240
      Width           =   7935
   End
   Begin VB.TextBox txtWorkingDirectory 
      Height          =   495
      Left            =   1560
      TabIndex        =   3
      Text            =   "Text1"
      Top             =   2280
      Width           =   4815
   End
   Begin VB.TextBox txtTargetPath 
      Height          =   495
      Left            =   1560
      TabIndex        =   2
      Text            =   "Text1"
      Top             =   1680
      Width           =   4815
   End
   Begin VB.TextBox txtShortcutName 
      Height          =   495
      Left            =   1560
      TabIndex        =   1
      Top             =   1080
      Width           =   4815
   End
   Begin VB.CommandButton cmdCreate 
      Caption         =   "作成"
      Height          =   495
      Left            =   2760
      TabIndex        =   0
      Top             =   3120
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'
' Windows Script Host Object Model
'

Option Explicit

Private Sub cmdCreate_Click()
    Dim objShell As New WshShell
    Dim objCut As WshShortcut
    
    objCut = objShell.CreateShortcut("ほげ.lnk")
    objCut.Save
    
    Set objCut = Nothing
End Sub

Private Sub Combo1_Click()
    Dim objShell As New WshShell
    
    Text1.Text = objShell.SpecialFolders(Combo1.Text)
End Sub
