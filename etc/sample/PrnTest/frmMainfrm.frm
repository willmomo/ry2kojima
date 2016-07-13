VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin MSComDlg.CommonDialog openDlg 
      Left            =   4080
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Command3"
      Height          =   495
      Left            =   540
      TabIndex        =   2
      Top             =   1980
      Width           =   1215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   480
      TabIndex        =   1
      Top             =   1140
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   420
      TabIndex        =   0
      Top             =   360
      Width           =   1215
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
      Begin VB.Menu mnuFilePrint 
         Caption         =   "印刷(&P)..."
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_objFS As New FileSystemObject

Private Sub Command1_Click()

    DumPrint1
    
End Sub

Private Sub Command2_Click()

    DumPrint2
    
End Sub

Private Sub Command3_Click()

    Call dlgPrinting.PrintString(DumPrint3, Me)

End Sub

'----------------------------------------------------------------------
' ファイルを選択して印刷
'----------------------------------------------------------------------
Private Sub mnuFilePrint_Click()
    
    openDlg.CancelError = True
    
    On Error Resume Next
    openDlg.ShowOpen
    If Err.Number = 0 Then
    
        Dim txtStream As TextStream
        Set txtStream = m_objFS.OpenTextFile(openDlg.FileName)
        
        dlgPrinting.PrintString txtStream.ReadAll, Me
        
        txtStream.Close
    End If
    
End Sub

