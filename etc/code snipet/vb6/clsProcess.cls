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
'------------------------------------------------------------------------------
' プロセスの終了を待つために必要な処理を行うことのできるクラス
'
' 2009/01/24 小島 新規作成
'------------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------------------------
' API関数の定義
'------------------------------------------------------------------------------
Private Const STATUS_PENDING As Long = &H103

Private Const STILL_ACTIVE = STATUS_PENDING

Private Const STARTF_USESHOWWINDOW As Long = &H1

Private Const SW_HIDE As Integer = 0
Private Const SW_SHOWNORMAL As Integer = 1
Private Const SW_NORMAL As Integer = 1
Private Const SW_SHOWMINIMIZED As Integer = 2
Private Const SW_SHOWMAXIMIZED As Integer = 3
Private Const SW_MAXIMIZE As Integer = 3
Private Const SW_SHOWNOACTIVATE As Integer = 4
Private Const SW_SHOW As Integer = 5
Private Const SW_MINIMIZE As Integer = 6
Private Const SW_SHOWMINNOACTIVE As Integer = 7
Private Const SW_SHOWNA As Integer = 8
Private Const SW_RESTORE As Integer = 9
Private Const SW_SHOWDEFAULT As Integer = 10
Private Const SW_FORCEMINIMIZE As Integer = 11

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

Private Declare Function WaitForInputIdle Lib "user32" (ByVal hProcess As Long, ByVal dwMilliseconds As Long) As Long

Private Const INFINITE = &HFFFF      '  Infinite timeout

'------------------------------------------------------------------------------
' メンバー変数
'------------------------------------------------------------------------------
Private m_pi As PROCESS_INFORMATION
Private m_si As STARTUPINFO

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値をリセットする(使わない状態にする)
'------------------------------------------------------------------------------
Public Sub Reset_ShowWindow()
    m_si.dwFlags = m_si.dwFlags And (Not STARTF_USESHOWWINDOW)
    m_si.wShowWindow = 0
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値をSW_HIDEにする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_Hide()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_HIDE
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWNORMAL(SW_NORMAL と同じ)にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowNormal()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWNORMAL
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWMINIMIZED にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowMinimized()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWMINIMIZED
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWMAXIMIZED(SW_MAXIMIZE と同じ) にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowMaximized()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWMAXIMIZED
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWNOACTIVATE にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowNoActivate()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWNOACTIVATE
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOW にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_Show()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOW
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_MINIMIZE にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_Minimize()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_MINIMIZE
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWMINNOACTIVE にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowMinNoActivate()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWMINNOACTIVE
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWNA にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowNA()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWNA
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_RESTORE にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_Restore()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_RESTORE
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_SHOWDEFAULT にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ShowDefault()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_SHOWDEFAULT
End Sub

'------------------------------------------------------------------------------
' STARTUPINFO.wShowWindow の値を SW_FORCEMINIMIZE にする
'------------------------------------------------------------------------------
Public Sub Set_ShowWindow_ForceMinimize()
    m_si.dwFlags = m_si.dwFlags Or STARTF_USESHOWWINDOW
    m_si.wShowWindow = SW_FORCEMINIMIZE
End Sub

'------------------------------------------------------------------------------
' プロセスの終了コードを取得する
'------------------------------------------------------------------------------
Public Property Get ExitCode() As Long
    Dim dwExitCode As Long
    
    Call GetExitCodeProcess(m_pi.hProcess, dwExitCode)
    
    ExitCode = dwExitCode
End Property

'------------------------------------------------------------------------------
' プロセスが動作中かどうか調べる
'------------------------------------------------------------------------------
Public Property Get IsStillActive() As Boolean
    IsStillActive = IIf(ExitCode = STILL_ACTIVE, True, False)
End Property

'------------------------------------------------------------------------------
' プロセスを実行する
'
' [戻り値]
' True 正常終了。False エラー終了。
'------------------------------------------------------------------------------
Public Function Run(cmdLine As String) As Boolean
    'すでに別のプロセス実行中の時は、エラーを返す。
    If m_pi.hProcess <> 0 Then
        Run = False
        Exit Function
    End If
    
    'スタートアップ情報を初期化
    m_si.cb = Len(m_si)
    
    'プロセス実行
    Run = (CreateProcess(vbNullString, cmdLine, ByVal 0, ByVal 0, 1, NORMAL_PRIORITY_CLASS, ByVal 0, vbNullString, m_si, m_pi) <> 0)
End Function

'------------------------------------------------------------------------------
' プロセスのハンドルを閉じる
'
' [備考]
' プロセスを実行していなくても、正常に処理できます。
' デストラクタで呼んでいるので、明記しなくても構いません。
'------------------------------------------------------------------------------
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

'------------------------------------------------------------------------------
' プロセスがアイドル状態になるまで待機
'------------------------------------------------------------------------------
Public Function WaitIdle() As Long
    WaitIdle = WaitForInputIdle(m_pi.hProcess, INFINITE)
End Function

'------------------------------------------------------------------------------
' デストラクタ
'------------------------------------------------------------------------------
Private Sub Class_Terminate()
    'プロセスハンドルの後処理
    Call CloseProcessHandle
End Sub

