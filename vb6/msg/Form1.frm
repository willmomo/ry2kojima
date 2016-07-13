VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "送信フォーム"
   ClientHeight    =   4290
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   2550
   LinkTopic       =   "Form1"
   ScaleHeight     =   4290
   ScaleWidth      =   2550
   StartUpPosition =   3  'Windows の既定値
   Begin VB.TextBox Text2 
      Height          =   375
      Left            =   1320
      TabIndex        =   12
      Text            =   "1"
      Top             =   480
      Width           =   975
   End
   Begin VB.CommandButton Command1 
      Caption         =   "sending!"
      Height          =   375
      Left            =   120
      TabIndex        =   10
      Top             =   3600
      Width           =   2295
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   4
      Left            =   960
      TabIndex        =   4
      Text            =   "1054"
      Top             =   2880
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   3
      Left            =   960
      TabIndex        =   3
      Text            =   "1023"
      Top             =   2400
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   2
      Left            =   960
      TabIndex        =   2
      Text            =   "1012"
      Top             =   1920
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   1
      Left            =   960
      TabIndex        =   1
      Text            =   "4"
      Top             =   1440
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   0
      Left            =   960
      TabIndex        =   0
      Text            =   "111"
      Top             =   960
      Width           =   1335
   End
   Begin VB.Label Label1 
      Caption         =   "数値でお願いします(ﾁｪｯｸﾅｼ）"
      Height          =   375
      Index           =   6
      Left            =   120
      TabIndex        =   13
      Top             =   120
      Width           =   3015
   End
   Begin VB.Label Label1 
      Caption         =   "dwData"
      Height          =   375
      Index           =   5
      Left            =   120
      TabIndex        =   11
      Top             =   480
      Width           =   3015
   End
   Begin VB.Label Label1 
      Caption         =   "mondai 2"
      DataField       =   "knokno"
      Height          =   375
      Index           =   4
      Left            =   120
      TabIndex        =   9
      Top             =   3000
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "mondai 1"
      DataField       =   "knokno"
      Height          =   375
      Index           =   3
      Left            =   120
      TabIndex        =   8
      Top             =   2520
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "mondai 0"
      DataField       =   "knokno"
      Height          =   375
      Index           =   2
      Left            =   120
      TabIndex        =   7
      Top             =   2040
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "ptup"
      DataField       =   "knokno"
      Height          =   375
      Index           =   1
      Left            =   120
      TabIndex        =   6
      Top             =   1560
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "kno"
      Height          =   375
      Index           =   0
      Left            =   120
      TabIndex        =   5
      Top             =   1080
      Width           =   735
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim hWndTo As Long
    Dim sdtCOPYDATASTRUCT As COPYDATASTRUCT
    
    Dim kdata As KAIIN_DATA
    
    
    sdtCOPYDATASTRUCT.dwData = nvl(Text2.Text)    '渡したいロング値
        
    kdata.kno = nvl(Text1(0).Text)
    kdata.ptup = nvl(Text1(1).Text)
    kdata.mondai(0) = nvl(Text1(2).Text)
    kdata.mondai(1) = nvl(Text1(3).Text)
    kdata.mondai(2) = nvl(Text1(4).Text)
    
    sdtCOPYDATASTRUCT.cbData = LenB(kdata) + 1
    
    sdtCOPYDATASTRUCT.lpData = VarPtr(kdata) 'kdata
    
    hWndTo = FindWindow(vbNullString, "受信フォーム")
    If hWndTo <> 0 Then
        Call SendMessage(hWndTo, WM_COPYDATA, Me.hwnd, sdtCOPYDATASTRUCT)
    End If


End Sub

Private Sub Label1_Click(Index As Integer)
nvl (Text1(0).Text)
End Sub
