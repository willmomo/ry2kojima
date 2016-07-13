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
' �v���Z�X�̏I����҂��߂ɕK�v�ȏ������s�����Ƃ̂ł���N���X
'
' 2009/01/24 ����  �V�K�쐬
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
' �����o�[�ϐ�
'----------------------------------------------------------------------
Private m_pi As PROCESS_INFORMATION
Private m_si As STARTUPINFO

'----------------------------------------------------------------------
' �v���Z�X�����쒆���ǂ������ׂ�
'----------------------------------------------------------------------
Public Function IsStillActive() As Boolean
    Dim dwExitCode As Long
    
    Call GetExitCodeProcess(m_pi.hProcess, dwExitCode)
    
    IsStillActive = (dwExitCode = STILL_ACTIVE)
End Function

'----------------------------------------------------------------------
' �v���Z�X�����s����
'
' [�߂�l]
' True ����I���BFalse �G���[�I���B
'----------------------------------------------------------------------
Public Function Run(cmdLine As String) As Boolean
    '���łɕʂ̃v���Z�X���s���̎��́A�G���[��Ԃ��B
    If m_pi.hProcess <> 0 Then
        Run = False
        Exit Function
    End If
    
    '�v���Z�X���s
    Run = (CreateProcess(vbNullString, cmdLine, ByVal 0, ByVal 0, 1, NORMAL_PRIORITY_CLASS, ByVal 0, vbNullString, m_si, m_pi) <> 0)
End Function

'----------------------------------------------------------------------
' �v���Z�X�̃n���h�������
'
' [���l]
' �v���Z�X�����s���Ă��Ȃ��Ă��A����ɏ����ł��܂��B
' �f�X�g���N�^�ŌĂ�ł���̂ŁA���L���Ȃ��Ă��\���܂���B
'----------------------------------------------------------------------
Public Sub CloseProcessHandle()
    '�X���b�h�̃n���h�������
    If m_pi.hThread <> 0 Then
        Call CloseHandle(m_pi.hThread)
        m_pi.hThread = 0
    End If
    
    '�v���Z�X�̃n���h�������
    If m_pi.hProcess <> 0 Then
        Call CloseHandle(m_pi.hProcess)
        m_pi.hProcess = 0
    End If
End Sub

'----------------------------------------------------------------------
' ������
'----------------------------------------------------------------------
Private Sub Class_Initialize()
    '�X�^�[�g�A�b�v����������
    m_si.cb = Len(m_si)
End Sub

'----------------------------------------------------------------------
' �㏈��
'----------------------------------------------------------------------
Private Sub Class_Terminate()
    '�v���Z�X�n���h���̌㏈��
    Call CloseProcessHandle
End Sub