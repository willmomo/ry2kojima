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
   StartUpPosition =   2  '��ʂ̒���
   Begin VB.CommandButton cmdDeleteTokusyoRireki 
      Caption         =   "�폜�J�n(&D)"
      Enabled         =   0   'False
      Height          =   495
      Left            =   120
      TabIndex        =   5
      Top             =   2400
      Width           =   4455
   End
   Begin VB.CommandButton cmdReadTokusyoRireki 
      Caption         =   "���Ǎ�(&R)"
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
      Caption         =   "�ŏI�폜�̓��t�F"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   1200
      Width           =   1815
   End
   Begin VB.Label Label3 
      Caption         =   "�ł��V�������t�F"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   1815
   End
   Begin VB.Label Label2 
      Caption         =   "�ł��Â����t�F"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   1815
   End
   Begin VB.Label Label1 
      Caption         =   "���ܗ��𑍌����F"
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
' ���ܗ�����60�����c���āA�����I
'=======================================================================
Option Explicit

'///////////////////////////////////////////////////////////////////////
' �e�[�u��"TOKUSYO_RIREKI�̏����폜����
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
    
    '�ł��Â����t�̎��̌��̈����������폜���J�n����悤�ɂ���
    dt = DateAdd("m", 1, Format(Label6.Caption, "yyyy/mm/1"))
    
    '�폜����Ō�̓��t�́A�Z�o�ς݂Ȃ̂Ń��x������擾����
    dtEnd = Label8.Caption
    
    '�Ђƌ����Ƃɍ폜�A�R�~�b�g���J��Ԃ�SQL�t�@�C�����쐬
    fno = FreeFile
    Open "TokuReki60.sql" For Output As fno
    Print #fno, "spool TokuReki60.log"
    Print #fno, "set echo on"
    Print #fno, "set trimspool on"
    Print #fno, "set timing on"
    
    '�폜���t���Ђƌ����i�߂Ȃ���A�폜����Ō�̓��t���ȏ�ɂȂ����烋�[�v�𔲂���B
    Do While dt <= dtEnd
        strSql = "delete from TOKUSYO_RIREKI where TREKI_DATE < to_date(" & Format(dt, "yyyymmdd") & ",'yyyymmdd');"
        g_log.WriteInfo strFuncName, strSql
        Print #fno, strSql
        Print #fno, "commit;"
        
        dt = DateAdd("m", 1, dt)
    Loop
    
    '�Ō�ɍ폜����Ō�̓��t�ȉ����폜����SQL��ǉ�
    strSql = "delete from TOKUSYO_RIREKI where TREKI_DATE <= to_date(" & Format(dtEnd, "yyyymmdd") & ",'yyyymmdd');"
    g_log.WriteInfo strFuncName, strSql
    Print #fno, strSql
    Print #fno, "commit;"
    
    Print #fno, "spool off"
    Print #fno, "exit"
    Close fno
    
    'sqlplus�����s
    cmdLine = "sqlplus gmc/mpcadmin@mpc @TokuReki60.sql"
    g_log.WriteInfo strFuncName, cmdLine & " ���s�B"
    If objProc.Run(cmdLine) = True Then
        objProc.WaitIdle
        Do While objProc.IsStillActive = True
            DoEvents
        Loop
        
        g_log.WriteInfo strFuncName, "�߂�l = " & objProc.ExitCode
    Else
        MsgBox g_log.WriteInfo(strFuncName, "�R�}���h�̎��s�Ɏ��s���܂����B(" & Err.LastDllError & ")"), vbCritical
    End If
    
ExitHandler:
    g_log.WriteInfo strFuncName, "leave."
    Exit Sub
    
ErrHandler:
    MsgBox g_log.WriteErr(strFuncName), vbCritical
    GoTo ExitHandler
End Sub

'///////////////////////////////////////////////////////////////////////
' �e�[�u��"TOKUSYO_RIREKI�̏����擾����
'
Private Function ReadTokusyoRireki() As Boolean
    Dim strFuncName As String
    strFuncName = Me.Name & ".ReadTokusyoRireki()"

    On Error GoTo ErrHandler
    
    g_log.WriteInfo strFuncName, "enter."
    
    Dim strSql As String
    Dim cn As New ADODB.Connection
    Dim rs As New ADODB.Recordset
    
    '���s�ŏ�����
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
        
        '���R�[�h��񂪎擾�ł�����A�����ɂ���
        ReadTokusyoRireki = True
    Else
        MsgBox g_log.WriteInfo(strFuncName, "���ܗ����̌����� " & rs("CNT") & " �Ȃ̂ŁA���̃v���O���������s����K�v�͂���܂���B"), vbInformation
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
' �e�[�u��"TOKUSYO_RIREKI"����f�[�^���폜����
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
' �e�[�u��"TOKUSYO_RIREKI"�������ǂ�
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
' Initialize���b�Z�[�W����
'
Private Sub Form_Initialize()
    g_log.Ident = "TR60"
    g_log.WriteInfo "", "�������� " & App.EXEName & " �N�� ��������"
End Sub

'///////////////////////////////////////////////////////////////////////
' Terminate���b�Z�[�W����
'
Private Sub Form_Terminate()
    g_log.WriteInfo "", "�������� " & App.EXEName & " �I�� ��������"
End Sub
