VERSION 5.00
Object = "{065E6FD1-1BF9-11D2-BAE8-00104B9E0792}#3.0#0"; "ssa3d30.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3585
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6330
   LinkTopic       =   "Form1"
   ScaleHeight     =   3585
   ScaleWidth      =   6330
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin Threed.SSCommand SSCommand2 
      Height          =   735
      Left            =   480
      TabIndex        =   2
      Top             =   2400
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   1296
      _Version        =   196610
      Caption         =   "SSCommand2"
   End
   Begin Threed.SSCommand SSCommand1 
      Height          =   735
      Left            =   480
      TabIndex        =   1
      Top             =   360
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   1296
      _Version        =   196610
      Caption         =   "SSCommand1"
   End
   Begin Threed.SSFrame SSFrame1 
      Height          =   3135
      Left            =   3600
      TabIndex        =   0
      Top             =   240
      Width           =   2535
      _ExtentX        =   4471
      _ExtentY        =   5530
      _Version        =   196610
      Caption         =   "SSFrame1"
      Begin Threed.SSRibbon SSRibbon1 
         Height          =   735
         Index           =   0
         Left            =   480
         TabIndex        =   3
         Top             =   480
         Width           =   1575
         _ExtentX        =   2778
         _ExtentY        =   1296
         _Version        =   196610
         Caption         =   "SSRibbon1"
      End
      Begin Threed.SSRibbon SSRibbon1 
         Height          =   735
         Index           =   1
         Left            =   480
         TabIndex        =   4
         Top             =   1920
         Width           =   1575
         _ExtentX        =   2778
         _ExtentY        =   1296
         _Version        =   196610
         Caption         =   "SSRibbon1"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim m_skin As New clsSkin

Private Sub Form_Initialize()
    Dim errMsg As String
    If Not m_skin.LoadSkin(App.Path & "\sample skin.xml", errMsg) Then
        MsgBox errMsg, vbOKOnly, "Skil Load error"
    End If
End Sub

Private Sub Form_Load()
    m_skin.Apply Me
    
    SSRibbon1(0).Value = True
End Sub

Private Sub SSRibbon1_Click(Index As Integer, Value As Integer)
    SSRibbon1(Index).BackColor = IIf(Value, _
        m_skin.GetControlProperty(Me, SSRibbon1(Index), "AccentBackColor"), _
        m_skin.GetControlProperty(Me, SSRibbon1(Index), "BackColor"))
End Sub
