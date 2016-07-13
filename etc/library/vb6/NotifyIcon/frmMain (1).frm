VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "NotifyIcon Sample"
   ClientHeight    =   6915
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7725
   LinkTopic       =   "Form1"
   ScaleHeight     =   6915
   ScaleWidth      =   7725
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton cmdDelete 
      Caption         =   "タスクトレイから削除(&A)"
      Height          =   495
      Left            =   2520
      TabIndex        =   5
      Top             =   1560
      Width           =   2175
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "タスクトレイに追加(&A)"
      Height          =   495
      Left            =   120
      TabIndex        =   4
      Top             =   1560
      Width           =   2175
   End
   Begin VB.TextBox Text2 
      Height          =   270
      Left            =   120
      TabIndex        =   3
      Top             =   1080
      Width           =   7455
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   120
      TabIndex        =   1
      Top             =   360
      Width           =   7455
   End
   Begin VB.Label Label2 
      Caption         =   "テキスト(&T)"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   7335
   End
   Begin VB.Label Label1 
      Caption         =   "アイコン(&I)"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7335
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private m_ni As New clsNotifyIcon


Private Sub Command1_Click()

    Dim buf(64 - 1) As Byte
    Dim work() As Byte
    
    m_ni.Add Me, 1, Me.icon, "小島"

End Sub

Private Sub Form_Load()
    
End Sub

Private Sub Form_Unload(Cancel As Integer)

    m_ni.Delete
    
End Sub
