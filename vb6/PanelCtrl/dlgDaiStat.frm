VERSION 5.00
Begin VB.Form dlgDaiStat 
   BorderStyle     =   3  'å≈íË¿ﬁ≤±€∏ﬁ
   Caption         =   "ë‰èÛë‘"
   ClientHeight    =   1155
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1155
   ScaleWidth      =   6030
   ShowInTaskbar   =   0   'False
   Begin VB.CheckBox chkSelected 
      Caption         =   "ëIë"
      Height          =   315
      Left            =   1740
      TabIndex        =   3
      Top             =   600
      Width           =   1215
   End
   Begin VB.CheckBox chkEnable 
      Caption         =   "óLå¯"
      Height          =   315
      Left            =   300
      TabIndex        =   2
      Top             =   600
      Width           =   1215
   End
   Begin VB.TextBox txtDai 
      Height          =   315
      Left            =   840
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
   Begin VB.CommandButton CancelButton 
      Cancel          =   -1  'True
      Caption         =   "∑¨›æŸ"
      Height          =   375
      Left            =   4680
      TabIndex        =   5
      Top             =   600
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   4680
      TabIndex        =   4
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label lblDai 
      Caption         =   "ë‰î‘çÜ"
      Height          =   195
      Left            =   180
      TabIndex        =   0
      Top             =   180
      Width           =   615
   End
End
Attribute VB_Name = "dlgDaiStat"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit
