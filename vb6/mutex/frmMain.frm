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
   Begin VB.CommandButton cmdClose 
      Caption         =   "閉じる"
      Height          =   495
      Left            =   120
      TabIndex        =   4
      Top             =   1320
      Width           =   1215
   End
   Begin VB.CommandButton cmdCreate 
      Caption         =   "作成"
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   720
      Width           =   1215
   End
   Begin VB.CommandButton cmdUnlock 
      Caption         =   "アンロック"
      Height          =   495
      Left            =   1440
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   120
      Width           =   4455
   End
   Begin VB.CommandButton cmdLock 
      Caption         =   "ロック"
      Height          =   495
      Left            =   1440
      TabIndex        =   0
      Top             =   720
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_mutex As clsMutex

Private Sub cmdClose_Click()
    m_mutex.closeMutex
End Sub

Private Sub cmdCreate_Click()
    Dim bRet As Boolean
    Dim bExists As Boolean
    
    bRet = m_mutex.create(Text1.Text)
    bExists = m_mutex.AlreadyExists
End Sub

Private Sub cmdLock_Click()
    m_mutex.lockMutex 200
End Sub

Private Sub cmdUnlock_Click()
    m_mutex.unlockMutex
End Sub

Private Sub Form_Load()
    Set m_mutex = New clsMutex
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_mutex = Nothing
End Sub
