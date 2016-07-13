VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "CreateProcessサンプル"
   ClientHeight    =   5400
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7920
   LinkTopic       =   "Form1"
   ScaleHeight     =   5400
   ScaleWidth      =   7920
   StartUpPosition =   3  'Windows の既定値
   Begin VB.Frame fraStartupInfo 
      Caption         =   "lpStartupInfo"
      Height          =   3375
      Left            =   120
      TabIndex        =   3
      Top             =   840
      Width           =   7455
      Begin VB.CheckBox chkShowWindow 
         Caption         =   "wShowWindow"
         Height          =   375
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   1575
      End
      Begin VB.ComboBox cmbShowWindow 
         Height          =   300
         ItemData        =   "frmMain.frx":0000
         Left            =   1800
         List            =   "frmMain.frx":0002
         Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
         TabIndex        =   4
         Top             =   360
         Width           =   3735
      End
   End
   Begin VB.CommandButton cmdCreateProcess 
      Caption         =   "CreateProcess"
      Height          =   495
      Left            =   3840
      TabIndex        =   1
      Top             =   4440
      Width           =   3735
   End
   Begin VB.TextBox txtCommandLine 
      Height          =   270
      Left            =   1440
      TabIndex        =   0
      Top             =   360
      Width           =   5895
   End
   Begin VB.Label lblCommandLine 
      Caption         =   "lpCommandLine"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   360
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' CreateProcess APIのサンプルプログラム
'----------------------------------------------------------------------

Option Explicit

Private Sub InitControls()

    'cmbShowWindowの初期化
    With cmbShowWindow
        .AddItem "SW_HIDE"
        .ItemData(.NewIndex) = SW_HIDE
        .AddItem "SW_MAXIMIZE"
        .ItemData(.NewIndex) = SW_MAXIMIZE
        .AddItem "SW_MINIMIZE"
        .ItemData(.NewIndex) = SW_MINIMIZE
        .AddItem "SW_RESTORE"
        .ItemData(.NewIndex) = SW_RESTORE
        .AddItem "SW_SHOW"
        .ItemData(.NewIndex) = SW_SHOW
        .AddItem "SW_SHOWDEFAULT"
        .ItemData(.NewIndex) = SW_SHOWDEFAULT
        .AddItem "SW_SHOWMAXIMIZED"
        .ItemData(.NewIndex) = SW_SHOWMAXIMIZED
        .AddItem "SW_SHOWMINIMIZED"
        .ItemData(.NewIndex) = SW_SHOWMINIMIZED
        .AddItem "SW_SHOWMINNOACTIVE"
        .ItemData(.NewIndex) = SW_SHOWMINNOACTIVE
        .AddItem "SW_SHOWNA"
        .ItemData(.NewIndex) = SW_SHOWNA
        .AddItem "SW_SHOWNOACTIVATE"
        .ItemData(.NewIndex) = SW_SHOWNOACTIVATE
        .AddItem "SW_SHOWNORMAL"
        .ItemData(.NewIndex) = SW_SHOWNORMAL
        
        .ListIndex = 0
        .Enabled = False
    End With

End Sub


Private Sub chkShowWindow_Click()
    cmbShowWindow.Enabled = chkShowWindow.Value
End Sub

Private Sub cmdCreateProcess_Click()

    Dim ret As Long
    Dim si As STARTUPINFO
    Dim pi As PROCESS_INFORMATION
    
    si.cb = LenB(si)
    
    If chkShowWindow.Value = True Then
        si.dwFlags = si.dwFlags Or STARTF_USESHOWWINDOW
        si.wShowWindow = cmbShowWindow.ItemData(cmbShowWindow.ListIndex)
    End If
    
    ret = CreateProcess( _
        vbNullString, _
        txtCommandLine.Text, _
        0, _
        0, _
        0, _
        0, _
        ByVal 0, _
        vbNullString, _
        si, _
        pi)

    MsgBox "CreateProcessの戻り値 = " & ret
    
    CloseHandle pi.hThread
    CloseHandle pi.hProcess
    
End Sub

Private Sub Form_Load()
    InitControls
End Sub
