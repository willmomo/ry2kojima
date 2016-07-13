VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsProcess"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' プロセスの終了を待つために必要な処理を行うことのできるクラス
'
' 2009/01/24 小島  新規作成
'----------------------------------------------------------------------
Option Explicit

Private Const STATUS_PENDING As Long = &H103

Private Const STILL_ACTIVE = STATUS_PENDING

Private Type STARTUPINFO
    cb As Long
    lpReserved As String
    lpDesktop As String
    lpTitle As String
    dwX As Long
    dwY As Long
    dwXSize As Long
    dwYSize As Long
    dwXCountChars As Long
    dwYCountChars As Long
    dwFillAttribute As Long
    dwFlags As Long
    wShowWindow As Integer
    cbReserved2 As Integer
    lpReserved2 As Long
    hStdInput As Long
    hStdOutput As Long
    hStdError As Long
End Type

Private Type PROCESS_INFORMATION
    hProcess As Long
    hThread As Long
    dwProcessId As Long
    dwThreadId As Long
End Type

Private Declare Function CreateProcess Lib "kernel32" Alias "CreateProcessA" ( _
    ByVal lpApplicationName As String, _
    ByVal lpCommandLine As String, _
    lpProcessAttributes As Any, _
    lpThreadAttributes As Any, _
    ByVal bInheritHandles As Long, _
    ByVal dwCreationFlags As Long, _
    lpEnvironment As Any, _
    ByVal lpCurrentDriectory As String, _
    lpStartupInfo As STARTUPINFO, _
    lpProcessInformation As PROCESS_INFORMATION _
) As Long

Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private Declare Function GetExitCodeProcess Lib "kernel32" (ByVal hProcess As Long, lpExitCode As Long) As Long

Private Const NORMAL_PRIORITY_CLASS = &H20

'----------------------------------------------------------------------
' メンバー変数
'----------------------------------------------------------------------
Private m_pi As PROCESS_INFORMATION
Private m_si As STARTUPINFO

'----------------------------------------------------------------------
' プロセスが動作中かどうか調べる
'----------------------------------------------------------------------
Public Function IsStillActive() As Boolean
    Dim dwExitCode As Long
    
    Call GetExitCodeProcess(m_pi.hProcess, dwExitCode)
    
    IsStillActive = (dwExitCode = STILL_ACTIVE)
End Function

'----------------------------------------------------------------------
' プロセスを実行する
'
' [戻り値]
' True 正常終了。False エラー終了。
'----------------------------------------------------------------------
Public Function Run(cmdLine As String) As Boolean
    'すでに別のプロセス実行中の時は、エラーを返す。
    If m_pi.hProcess <> 0 Then
        Run = False
        Exit Function
    End If
    
    'プロセス実行
    Run = (CreateProcess(vbNullString, cmdLine, ByVal 0, ByVal 0, 1, NORMAL_PRIORITY_CLASS, ByVal 0, vbNullString, m_si, m_pi) <> 0)
End Function

'----------------------------------------------------------------------
' プロセスのハンドルを閉じる
'
' [備考]
' プロセスを実行していなくても、正常に処理できます。
' デストラクタで呼んでいるので、明記しなくても構いません。
'----------------------------------------------------------------------
Public Sub CloseProcessHandle()
    'スレッドのハンドルを閉じる
    If m_pi.hThread <> 0 Then
        Call CloseHandle(m_pi.hThread)
        m_pi.hThread = 0
    End If
    
    'プロセスのハンドルを閉じる
    If m_pi.hProcess <> 0 Then
        Call CloseHandle(m_pi.hProcess)
        m_pi.hProcess = 0
    End If
End Sub

'----------------------------------------------------------------------
' 初期化
'----------------------------------------------------------------------
Private Sub Class_Initialize()
    'スタートアップ情報を初期化
    m_si.cb = Len(m_si)
End Sub

'----------------------------------------------------------------------
' 後処理
'----------------------------------------------------------------------
Private Sub Class_Terminate()
    'プロセスハンドルの後処理
    Call CloseProcessHandle
End Sub
