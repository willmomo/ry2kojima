VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   Caption         =   "HaraiDasiEmu"
   ClientHeight    =   6900
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10560
   LinkTopic       =   "Form1"
   ScaleHeight     =   6900
   ScaleWidth      =   10560
   StartUpPosition =   3  'Windows の既定値
   Begin VB.ComboBox Combo5 
      Height          =   300
      ItemData        =   "frmMain.frx":0000
      Left            =   4080
      List            =   "frmMain.frx":000A
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   11
      Top             =   1560
      Width           =   1215
   End
   Begin VB.ComboBox Combo4 
      Height          =   300
      ItemData        =   "frmMain.frx":001C
      Left            =   2760
      List            =   "frmMain.frx":0026
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   10
      Top             =   1560
      Width           =   1215
   End
   Begin VB.ComboBox Combo3 
      Height          =   300
      ItemData        =   "frmMain.frx":0036
      Left            =   1440
      List            =   "frmMain.frx":0043
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   9
      Top             =   1560
      Width           =   1215
   End
   Begin VB.ComboBox Combo2 
      Height          =   300
      ItemData        =   "frmMain.frx":0057
      Left            =   120
      List            =   "frmMain.frx":0064
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   8
      Top             =   1560
      Width           =   1215
   End
   Begin VB.ComboBox Combo1 
      Height          =   300
      ItemData        =   "frmMain.frx":007A
      Left            =   120
      List            =   "frmMain.frx":0099
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   7
      Top             =   1080
      Width           =   1215
   End
   Begin VB.CommandButton Command4 
      Caption         =   "テスト中断"
      Height          =   495
      Left            =   6240
      TabIndex        =   6
      Top             =   960
      Width           =   1215
   End
   Begin VB.CommandButton Command3 
      Caption         =   "参照(&B)..."
      Height          =   495
      Left            =   9120
      TabIndex        =   5
      Top             =   360
      Width           =   1215
   End
   Begin MSComDlg.CommonDialog openDlg 
      Left            =   9720
      Top             =   960
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   120
      TabIndex        =   4
      Text            =   "test.txt"
      Top             =   360
      Width           =   8775
   End
   Begin VB.ListBox List1 
      BeginProperty Font 
         Name            =   "ＭＳ ゴシック"
         Size            =   12
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   4710
      IntegralHeight  =   0   'False
      Left            =   120
      TabIndex        =   2
      Top             =   1920
      Width           =   10215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "テスト開始"
      Height          =   495
      Left            =   4800
      TabIndex        =   1
      Top             =   960
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   3000
      TabIndex        =   0
      Top             =   960
      Width           =   1215
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   9000
      Top             =   960
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.Label Label1 
      Caption         =   "パターンファイル"
      Height          =   255
      Left            =   120
      TabIndex        =   3
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

Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Private m_stop As Boolean

'----------------------------------------------------------------------
' 通信ポートの初期化
'----------------------------------------------------------------------
Private Sub InitMSComm()
    Dim strSet As String
    
    Select Case Combo2.ListIndex
    Case 0  '9600
        strSet = "9600,"
    Case 1  '4800
        strSet = "4800,"
    Case 2  '2400
        strSet = "2400,"
    End Select
    
    Select Case Combo3.ListIndex
    Case 0
        strSet = strSet & "N,"
    Case 1
        strSet = strSet & "E,"
    Case 2
        strSet = strSet & "O,"
    End Select
    
    Select Case Combo4.ListIndex
    Case 0
        strSet = strSet & "8,"
    Case 1
        strSet = strSet & "7,"
    End Select
    
    Select Case Combo5.ListIndex
    Case 0
        strSet = strSet & "1"
    Case 1
        strSet = strSet & "2"
    End Select
    
    With MSComm1
        'ボーレートなど
        MsgBox strSet
        .Settings = strSet
        
        'バイナリモードに設定
        .InputMode = comInputModeBinary
    End With
End Sub

'----------------------------------------------------------------------
' UI状態を変化させる
'----------------------------------------------------------------------
Private Sub UpdateUI()
    Command1.Caption = IIf(MSComm1.PortOpen, "閉じる", "開く")
    Command2.Enabled = MSComm1.PortOpen
    Command4.Enabled = False
End Sub

'----------------------------------------------------------------------
' 通信ポートOpen/Close
'----------------------------------------------------------------------
Private Sub Command1_Click()
    On Error GoTo ErrHandler
    
    Dim ret As Long
    
    If Not MSComm1.PortOpen Then

        '通信デバイスの初期化
        Call InitMSComm
        
        MSComm1.CommPort = Combo1.ListIndex + 1
    End If
    
    MSComm1.PortOpen = Not MSComm1.PortOpen
    
    'UI状態を変化させる
    Call UpdateUI
    
    Exit Sub
ErrHandler:
    MsgBox Err.Description
End Sub

'----------------------------------------------------------------------
' パターン処理スタート
'----------------------------------------------------------------------
Private Sub Command2_Click()
    Dim msg As String
    Dim i As Long
    Dim ret As Long
    Dim cmd As Byte
    Dim param As Byte
    Dim comData(512) As Byte
    Dim rcvData As Variant
    Dim sndData() As Byte
    Dim cmpData() As Byte
    
    Command1.Enabled = False
    Command2.Enabled = False
    Command4.Enabled = True
    
    ret = scriptOpen(Text1.Text)
    If ret <> 0 Then
        MsgBox Text1.Text & " が、オープンできません。"
        GoTo ExitHandler
    End If
    
    List1.AddItem Now & vbTab & "----[ テスト開始 ]----"
    List1.ListIndex = List1.ListCount - 1
    
    Do While True
        ret = scriptGetLine(cmd, param, comData(0))
        If ret <> 0 Then
            Exit Do
        End If
        
        Select Case cmd
        Case Asc("r")
            msg = Now & vbTab & "受信待:"
            For i = 0 To param - 1
                msg = msg & "[" & Right("0" & Hex(comData(i)), 2) & "]"
            Next
            List1.AddItem msg
            List1.ListIndex = List1.ListCount - 1
            
            Do
                If m_stop Then
                    m_stop = False
                    GoTo ExitHandler
                End If
                
                DoEvents
            Loop Until MSComm1.InBufferCount >= param
            rcvData = MSComm1.Input
        
            msg = Now & vbTab & "受信　:"
            ReDim cmpData(param - 1)
            For i = 0 To param - 1
                cmpData(i) = rcvData(i)
                msg = msg & "[" & Right("0" & Hex(cmpData(i)), 2) & "]"
            Next
            List1.AddItem msg
            List1.ListIndex = List1.ListCount - 1
            
            ret = scriptCompare(cmd, param, cmpData(0))
            If ret <> 0 Then
                List1.AddItem Now & vbTab & "----[ テストエラー ]----"
                List1.ListIndex = List1.ListCount - 1
                MsgBox "エラー"
                GoTo ExitHandler
            End If
        
        Case Asc("s")
            msg = Now & vbTab & "送信　:"
            For i = 0 To param - 1
                msg = msg & "[" & Right("0" & Hex(comData(i)), 2) & "]"
            Next
            List1.AddItem msg
            List1.ListIndex = List1.ListCount - 1
            
            ReDim sndData(param - 1)
            For i = 0 To param - 1
                sndData(i) = comData(i)
            Next
            
            MSComm1.Output = sndData
            Do
                If m_stop Then
                    m_stop = False
                    Exit Do
                End If
                
                DoEvents
            Loop Until MSComm1.OutBufferCount = 0
            
        Case Asc("w")
            List1.AddItem Now & vbTab & "待ち　:" & (param * 100) & " ms"
            List1.ListIndex = List1.ListCount - 1
            Call Sleep(param * 100)
            
        Case Else
        End Select
    Loop
    ret = scriptClose
    
    List1.AddItem Now & vbTab & "----[ テスト終了 ]----"
    List1.ListIndex = List1.ListCount - 1

ExitHandler:
    Command1.Enabled = True
    Command4.Enabled = False
    Command2.Enabled = True
End Sub

'----------------------------------------------------------------------
' 参照ダイアログの表示
'----------------------------------------------------------------------
Private Sub Command3_Click()
    On Error GoTo ErrHandler
    
    openDlg.CancelError = True
    openDlg.ShowOpen
    Text1.Text = openDlg.FileName
    
    Exit Sub
ErrHandler:

End Sub

Private Sub Command4_Click()
    m_stop = True
End Sub

'----------------------------------------------------------------------
' Loadイベント
'----------------------------------------------------------------------
Private Sub Form_Load()
    Combo1.ListIndex = 0
    
    Combo2.ListIndex = 0
    Combo3.ListIndex = 1
    Combo4.ListIndex = 0
    Combo5.ListIndex = 1
    
    'UI状態の初期化
    Call UpdateUI
End Sub

'----------------------------------------------------------------------
' 通信イベント
'----------------------------------------------------------------------
Private Sub MSComm1_OnComm()
    
    Select Case MSComm1.CommEvent
    Case comEvCTS
        List1.AddItem Now & vbTab & "CTSラインの状態が変化しました。"
        List1.ListIndex = List1.ListCount - 1
    Case comEvCD
        List1.AddItem Now & vbTab & "CDラインの状態が変化しました。"
        List1.ListIndex = List1.ListCount - 1
    Case Else
        List1.AddItem Now & vbTab & MSComm1.CommEvent
        List1.ListIndex = List1.ListCount - 1
    End Select
End Sub
