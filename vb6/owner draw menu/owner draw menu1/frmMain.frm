VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton cmdODMTest 
      Caption         =   "Owner Draw Test"
      Height          =   495
      Left            =   360
      TabIndex        =   0
      Top             =   480
      Width           =   1695
   End
   Begin VB.Menu mnuNo1 
      Caption         =   "���j���[1(&1)"
      Begin VB.Menu mnuMenu11 
         Caption         =   "���j���[1-1"
      End
      Begin VB.Menu mnuLine1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuMenu12 
         Caption         =   "���j���[1-2"
      End
   End
   Begin VB.Menu mnuMenu2 
      Caption         =   "���j���[2(&2)"
      Begin VB.Menu mnuMenu21 
         Caption         =   "���j���[2-1"
      End
      Begin VB.Menu mnuLine2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuMenu22 
         Caption         =   "���j���[2-2"
      End
   End
   Begin VB.Menu mnuMenu3 
      Caption         =   "���j���[3(&3)"
      Begin VB.Menu mnuMenu31 
         Caption         =   "���j���[3-1"
      End
      Begin VB.Menu mnuLine3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuMenu32 
         Caption         =   "���j���[3-2"
      End
      Begin VB.Menu mnuLine4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuMenu33 
         Caption         =   "���j���[3-3"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdODMTest_Click()
    SetOwnerDraw (GetMenu(Me.hwnd))
End Sub

Private Sub Form_Load()
    Call StartSubClass(Me.hwnd)
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Call EndSubClass(Me.hwnd)
End Sub
