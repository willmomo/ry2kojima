VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  '固定(実線)
   Caption         =   "use_date.dllのテスト"
   ClientHeight    =   2385
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2385
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton cmdClose 
      Caption         =   "閉じる(&C)"
      Height          =   375
      Left            =   3360
      TabIndex        =   11
      Top             =   1920
      Width           =   1215
   End
   Begin VB.Timer Timer1 
      Interval        =   1000
      Left            =   120
      Top             =   1920
   End
   Begin VB.Label lblTime 
      BorderStyle     =   1  '実線
      Height          =   255
      Left            =   3240
      TabIndex        =   10
      Top             =   1560
      Width           =   1335
   End
   Begin VB.Label lblDate 
      BorderStyle     =   1  '実線
      Height          =   255
      Left            =   1800
      TabIndex        =   9
      Top             =   1560
      Width           =   1335
   End
   Begin VB.Label Label5 
      Caption         =   "Dateを日/時に分割："
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   1560
      Width           =   1575
   End
   Begin VB.Label lbl2String 
      BorderStyle     =   1  '実線
      Height          =   255
      Left            =   1800
      TabIndex        =   7
      Top             =   1200
      Width           =   2775
   End
   Begin VB.Label Label4 
      Caption         =   "Date型を文字列化："
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1200
      Width           =   1575
   End
   Begin VB.Label lblDos2Vb 
      BorderStyle     =   1  '実線
      Height          =   255
      Left            =   1800
      TabIndex        =   5
      Top             =   840
      Width           =   2775
   End
   Begin VB.Label Label3 
      Caption         =   "DOSからDate変換："
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   840
      Width           =   1575
   End
   Begin VB.Label lblNowDos 
      BorderStyle     =   1  '実線
      Height          =   255
      Left            =   1800
      TabIndex        =   3
      Top             =   480
      Width           =   2775
   End
   Begin VB.Label Label2 
      Caption         =   "DOS形式時刻："
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   480
      Width           =   1215
   End
   Begin VB.Label lblNow 
      BorderStyle     =   1  '実線
      Height          =   255
      Left            =   1800
      TabIndex        =   1
      Top             =   120
      Width           =   2775
   End
   Begin VB.Label Label1 
      Caption         =   "ただいまの時刻："
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1335
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdClose_Click()
    Unload Me
End Sub

Private Sub Form_Load()
    Timer1_Timer
End Sub

Private Sub Timer1_Timer()
    Dim now_time As Date
    
    now_time = Now
    
    lblNow.Caption = now_time
    lblNowDos.Caption = Hex(VbDate2DosDate(now_time))
    lblDos2Vb.Caption = DosDate2VbDate(VbDate2DosDate(now_time))
    lbl2String.Caption = VbDate2String(now_time)
    
    Dim ymd As String
    Dim hms As String
    VbDate2String2 now_time, ymd, hms
    lblDate.Caption = ymd
    lblTime.Caption = Time
End Sub
