VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "受信フォーム"
   ClientHeight    =   3150
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   2460
   LinkTopic       =   "Form1"
   ScaleHeight     =   3150
   ScaleWidth      =   2460
   StartUpPosition =   3  'Windows の既定値
   Begin VB.TextBox Text2 
      Height          =   375
      Left            =   1320
      TabIndex        =   11
      Top             =   240
      Width           =   975
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   0
      Left            =   960
      TabIndex        =   4
      Top             =   720
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   1
      Left            =   960
      TabIndex        =   3
      Top             =   1200
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   2
      Left            =   960
      TabIndex        =   2
      Top             =   1680
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   3
      Left            =   960
      TabIndex        =   1
      Top             =   2160
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Index           =   4
      Left            =   960
      TabIndex        =   0
      Top             =   2640
      Width           =   1335
   End
   Begin VB.Label Label1 
      Caption         =   "kno"
      Height          =   375
      Index           =   0
      Left            =   120
      TabIndex        =   10
      Top             =   840
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "ptup"
      DataField       =   "knokno"
      Height          =   375
      Index           =   1
      Left            =   120
      TabIndex        =   9
      Top             =   1320
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "mondai 0"
      DataField       =   "knokno"
      Height          =   375
      Index           =   2
      Left            =   120
      TabIndex        =   8
      Top             =   1800
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "mondai 1"
      DataField       =   "knokno"
      Height          =   375
      Index           =   3
      Left            =   120
      TabIndex        =   7
      Top             =   2280
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "mondai 2"
      DataField       =   "knokno"
      Height          =   375
      Index           =   4
      Left            =   120
      TabIndex        =   6
      Top             =   2760
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "dwData"
      Height          =   375
      Index           =   5
      Left            =   240
      TabIndex        =   5
      Top             =   240
      Width           =   3015
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Activate()
    Call Hook(Me.hwnd)
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    Call UnHook(Me.hwnd)
End Sub


