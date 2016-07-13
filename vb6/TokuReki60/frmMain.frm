VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   2  '画面の中央
   Begin VB.CommandButton cmdDeleteTokusyoRireki 
      Caption         =   "削除開始(&D)"
      Enabled         =   0   'False
      Height          =   495
      Left            =   120
      TabIndex        =   5
      Top             =   2400
      Width           =   4455
   End
   Begin VB.CommandButton cmdReadTokusyoRireki 
      Caption         =   "情報読込(&R)"
      Height          =   495
      Left            =   120
      TabIndex        =   4
      Top             =   1800
      Width           =   4455
   End
   Begin VB.Label Label8 
      Caption         =   "Label8"
      Height          =   255
      Left            =   2280
      TabIndex        =   9
      Top             =   1200
      Width           =   1575
   End
   Begin VB.Label Label7 
      Caption         =   "Label7"
      Height          =   255
      Left            =   2280
      TabIndex        =   8
      Top             =   840
      Width           =   1575
   End
   Begin VB.Label Label6 
      Caption         =   "Label6"
      Height          =   255
      Left            =   2280
      TabIndex        =   7
      Top             =   480
      Width           =   1575
   End
   Begin VB.Label Label5 
      Caption         =   "Label5"
      Height          =   255
      Left            =   2280
      TabIndex        =   6
      Top             =   120
      Width           =   1575
   End
   Begin VB.Label Label4 
      Caption         =   "最終削除の日付："
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   1200
      Width           =   1815
   End
   Begin VB.Label Label3 
      Caption         =   "最も新しい日付："
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   1815
   End
   Begin VB.Label Label2 
      Caption         =   "最も古い日付："
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   1815
   End
   Begin VB.Label Label1 
      Caption         =   "特賞履歴総件数："
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1815
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'=======================================================================
' 特賞履歴を60日分残して、消す！
'=======================================================================
Option Explicit

'///////////////////////////////////////////////////////////////////////
' テーブル"TOKUSYO_RIREKIの情報を削除する
'
Private Sub DeleteTokusyoRireki()
    Dim strFuncName As String
    strFuncName = Me.Name & ".DeleteTokusyoRireki()"
    
    On Error GoTo ErrHandler
    
    g_log.WriteInfo strFuncName, "enter."
    
    Dim fno As Integer
    Dim dt As Date
    Dim dtEnd As Date
    Dim cmdLine As String
    Dim strSql As String
    Dim objProc As New clsProcess
    
    '最も古い日付の次の月の一日未満から削除を開始するようにする
    dt = DateAdd("m", 1, Format(Label6.Caption, "yyyy/mm/1"))
    
    '削除する最後の日付は、算出済みなのでラベルから取得する
    dtEnd = Label8.Caption
    
    'ひと月ごとに削除、コミットを繰り返すSQLファイルを作成
    fno = FreeFile
    Open "TokuReki60.sql" For Output As fno
    Print #fno, "spool TokuReki60.log"
    Print #fno, "set echo on"
    Print #fno, "set trimspool on"
    Print #fno, "set timing on"
    
    '削除日付をひと月ずつ進めながら、削除する最後の日付を以上になったらループを抜ける。
    Do While dt <= dtEnd
        strSql = "delete from TOKUSYO_RIREKI where TREKI_DATE < to_date(" & Format(dt, "yyyymmdd") & ",'yyyymmdd');"
        g_log.WriteInfo strFuncName, strSql
        Print #fno, strSql
        Print #fno, "commit;"
        
        dt = DateAdd("m", 1, dt)
    Loop
    
    '最後に削除する最後の日付以下を削除するSQLを追加
    strSql = "delete from TOKUSYO_RIREKI where TREKI_DATE <= to_date(" & Format(dtEnd, "yyyymmdd") & ",'yyyymmdd');"
    g_log.WriteInfo strFuncName, strSql
    Print #fno, strSql
    Print #fno, "commit;"
    
    Print #fno, "spool off"
    Print #fno, "exit"
    Close fno
    
    'sqlplusを実行
    cmdLine = "sqlplus gmc/mpcadmin@mpc @TokuReki60.sql"
    g_log.WriteInfo strFuncName, cmdLine & " 実行。"
    If objProc.Run(cmdLine) = True Then
        objProc.WaitIdle
        Do While objProc.IsStillActive = True
            DoEvents
        Loop
        
        g_log.WriteInfo strFuncName, "戻り値 = " & objProc.ExitCode
    Else
        MsgBox g_log.WriteInfo(strFuncName, "コマンドの実行に失敗しました。(" & Err.LastDllError & ")"), vbCritical
    End If
    
ExitHandler:
    g_log.WriteInfo strFuncName, "leave."
    Exit Sub
    
ErrHandler:
    MsgBox g_log.WriteErr(strFuncName), vbCritical
    GoTo ExitHandler
End Sub

'///////////////////////////////////////////////////////////////////////
' テーブル"TOKUSYO_RIREKIの情報を取得する
'
Private Function ReadTokusyoRireki() As Boolean
    Dim strFuncName As String
    strFuncName = Me.Name & ".ReadTokusyoRireki()"

    On Error GoTo ErrHandler
    
    g_log.WriteInfo strFuncName, "enter."
    
    Dim strSql As String
    Dim cn As New ADODB.Connection
    Dim rs As New ADODB.Recordset
    
    '失敗で初期化
    ReadTokusyoRireki = False
    
    cn.Open "DSN=MPC", "gmc", "mpcadmin"
    strSql = "select count(*) CNT,min(TREKI_DATE) FDATE,max(TREKI_DATE) LDATE from TOKUSYO_RIREKI;"
    g_log.WriteInfo strFuncName, strSql
    rs.Open strSql, cn
    
    If rs("CNT") > 0 Then
        Label5.Caption = Format(rs("CNT"), "#,##0")
        g_log.WriteInfo strFuncName, Label1.Caption & " " & Label5.Caption
        
        Label6.Caption = rs("FDATE")
        g_log.WriteInfo strFuncName, Label2.Caption & " " & Label6.Caption
        
        Label7.Caption = rs("LDATE")
        g_log.WriteInfo strFuncName, Label3.Caption & " " & Label7.Caption
        
        Label8.Caption = DateAdd("d", -60, Date)
        g_log.WriteInfo strFuncName, Label4.Caption & " " & Label8.Caption
        
        'レコード情報が取得できたら、成功にする
        ReadTokusyoRireki = True
    Else
        MsgBox g_log.WriteInfo(strFuncName, "特賞履歴の件数が " & rs("CNT") & " なので、このプログラムを実行する必要はありません。"), vbInformation
    End If
    
    rs.Close
    cn.Close
    
ExitHandler:
    g_log.WriteInfo strFuncName, "leave."
    Exit Function

ErrHandler:
    MsgBox g_log.WriteErr(strFuncName), vbCritical
    GoTo ExitHandler
End Function

'///////////////////////////////////////////////////////////////////////
' テーブル"TOKUSYO_RIREKI"からデータを削除する
'
Private Sub cmdDeleteTokusyoRireki_Click()
    Dim strFuncName As String
    strFuncName = Me.Name & ".cmdDeleteTokusyoRireki_Click()"
    
    On Error GoTo ErrHandler
    
    g_log.WriteInfo strFuncName, "enter."
    cmdDeleteTokusyoRireki.Enabled = False
    
    DeleteTokusyoRireki
    
ExitHandler:
    cmdDeleteTokusyoRireki.Enabled = True
    g_log.WriteInfo strFuncName, "leave."
    Exit Sub
    
ErrHandler:
    MsgBox g_log.WriteErr(strFuncName), vbCritical
    GoTo ExitHandler
End Sub

'///////////////////////////////////////////////////////////////////////
' テーブル"TOKUSYO_RIREKI"から情報を読む
'
Private Sub cmdReadTokusyoRireki_Click()
    Dim strFuncName As String
    strFuncName = Me.Name & ".cmdReadTokusyoRireki_Click()"
    
    On Error GoTo ErrHandler
    
    g_log.WriteInfo strFuncName, "enter."
    cmdReadTokusyoRireki.Enabled = False
    
    If ReadTokusyoRireki = True Then
        cmdDeleteTokusyoRireki.Enabled = True
    End If
    
ExitHandler:
    cmdReadTokusyoRireki.Enabled = True
    g_log.WriteInfo strFuncName, "leave."
    Exit Sub
    
ErrHandler:
    MsgBox g_log.WriteErr(strFuncName), vbExclamation
    GoTo ExitHandler
End Sub

'///////////////////////////////////////////////////////////////////////
' Initializeメッセージ処理
'
Private Sub Form_Initialize()
    g_log.Ident = "TR60"
    g_log.WriteInfo "", "▼▼▼▼ " & App.EXEName & " 起動 ▼▼▼▼"
End Sub

'///////////////////////////////////////////////////////////////////////
' Terminateメッセージ処理
'
Private Sub Form_Terminate()
    g_log.WriteInfo "", "▲▲▲▲ " & App.EXEName & " 終了 ▲▲▲▲"
End Sub
