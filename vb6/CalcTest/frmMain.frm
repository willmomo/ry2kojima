VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{F13C99C0-4D73-11D2-B8B2-0000C00A958C}#7.0#0"; "fpSpr70.ocx"
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   7800
   ClientLeft      =   60
   ClientTop       =   750
   ClientWidth     =   11760
   LinkTopic       =   "Form1"
   ScaleHeight     =   7800
   ScaleWidth      =   11760
   StartUpPosition =   3  'Windows の既定値
   Begin FPSpreadADO.fpSpread fp 
      Height          =   5115
      Left            =   60
      TabIndex        =   1
      Top             =   1260
      Width           =   9855
      _Version        =   458752
      _ExtentX        =   17383
      _ExtentY        =   9022
      _StockProps     =   64
      SpreadDesigner  =   "frmMain.frx":0000
   End
   Begin VB.Frame fra 
      Caption         =   "条件設定"
      Height          =   855
      Left            =   60
      TabIndex        =   0
      Top             =   120
      Width           =   9915
      Begin VB.CommandButton cmdUpdate 
         Caption         =   "更新(&U)"
         Height          =   435
         Left            =   4380
         TabIndex        =   5
         Top             =   300
         Width           =   1155
      End
      Begin VB.TextBox txtNum 
         Height          =   315
         Left            =   3240
         TabIndex        =   4
         Top             =   360
         Width           =   915
      End
      Begin MSComCtl2.DTPicker dtpBegin 
         Height          =   315
         Left            =   180
         TabIndex        =   2
         Top             =   360
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   556
         _Version        =   393216
         Format          =   25231361
         CurrentDate     =   38795
      End
      Begin MSComCtl2.DTPicker dtpEnd 
         Height          =   315
         Left            =   1740
         TabIndex        =   3
         Top             =   360
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   556
         _Version        =   393216
         Format          =   25231361
         CurrentDate     =   38795
      End
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
      Begin VB.Menu mnuFileExit 
         Caption         =   "終了(&X)"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Resize()

    If Me.WindowState = vbNormal Then
        With fra
            .Left = 0
            .Top = 0
            .Width = Me.ScaleWidth
        End With
        
        With fp
            .Left = 0
            .Top = fra.Top + fra.Height
            .Width = Me.ScaleWidth
            .Height = Me.ScaleHeight - .Top
        End With
    End If
    
End Sub

Private Sub mnuFileExit_Click()

    Unload Me
    
End Sub
