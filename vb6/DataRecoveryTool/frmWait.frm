VERSION 5.00
Begin VB.Form frmWait 
   BackColor       =   &H80000003&
   BorderStyle     =   0  'なし
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '画面の中央
   Begin VB.Timer tim 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   60
      Top             =   60
   End
   Begin VB.Label lblMsg 
      Alignment       =   2  '中央揃え
      BackColor       =   &H80000003&
      Caption         =   "チェック中・・・"
      BeginProperty Font 
         Name            =   "ＭＳ Ｐゴシック"
         Size            =   27.75
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000013&
      Height          =   1215
      Left            =   120
      TabIndex        =   0
      Top             =   840
      Width           =   4395
   End
End
Attribute VB_Name = "frmWait"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_startTime As Date
Private m_hasError As Long      '0=正常終了 1=エラー 2=タイムアウト 3=未動作

Public Property Get Error() As Long
    Error = m_hasError
End Property

Private Sub Form_Load()

    '
    ' 初期状態は未動作
    '
    m_hasError = 3
    
    '
    ' 自身がLoadされたときをチェック開始時間とする
    '
    m_startTime = Now
    
    '
    ' チェックタイマー スタート
    '
    tim.Enabled = True
    
End Sub

Private Sub tim_Timer()
    Dim rc As Long
    Static m_lastCheck As Date
    
    '
    ' 2分経過してもファイルが削除されないときは、エラーとして終了する。
    '
    If DateDiff("s", m_startTime, Now) > 120 Then
        m_hasError = 2
        tim.Enabled = False
        Unload Me
    End If
    
    '
    ' 前回チェックから、10秒経過したらASの状態をチェック
    ' チェックが成功を戻したときは、エラーをOFFにして終了する。
    '
    If DateDiff("s", m_lastCheck, Now) > 10 Then
        rc = CheckRecovery
        
        If rc = 1 Then      ' 復旧正常
            m_hasError = 0
            tim.Enabled = False
            Unload Me
        ElseIf rc = 2 Then  '復旧異常
            m_hasError = 1
            tim.Enabled = False
            Unload Me
        End If
    End If
End Sub
