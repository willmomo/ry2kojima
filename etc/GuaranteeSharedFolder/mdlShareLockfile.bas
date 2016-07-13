Attribute VB_Name = "mdlShareLockfile"
Option Explicit

Private Declare Function CreateFile Lib "kernel32" Alias "CreateFileA" ( _
    ByVal lpFileName As String, _
    ByVal dwDesiredAccess As Long, _
    ByVal dwShareMode As Long, _
    lpSecurityAttributes As Any, _
    ByVal dwCreationDisposition As Long, _
    ByVal dwFlagsAndAttributes As Long, _
    ByVal hTemplateFile As Long) As Long

Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private Const GENERIC_WRITE = &H40000000

Private Const CREATE_NEW = 1

Private Const FILE_ATTRIBUTE_NORMAL = &H80
Private Const FILE_FLAG_DELETE_ON_CLOSE = &H4000000

Private Const INVALID_HANDLE_VALUE = -1

Private Const ERROR_FILE_EXISTS = 80&

'���b�N�t�@�C���ւ̃n���h��
Private m_handle As Long

'----------------------------------------------------------------------
' [�T�v]
' ���b�N�t�@�C�����쐬����
'
' [�߂�l]
' =1 �쐬�ł���
' =0 �쐬�ł��Ȃ�����
' =-1 �G���[
'----------------------------------------------------------------------

Public Function CreateShareLockfile() As Long
    
    '���b�N�t�@�C���́A�����ō�����܂�
    If m_handle > 0 Then
        CreateShareLockfile = -1
        Exit Function
    End If
    
    Dim strLockfile As String
    
    'TODO: �o�͐��ݒ肷��
    strLockfile = "\\wss\share\lockfile"
    
    '���b�N�t�@�C�����쐬
    m_handle = CreateFile( _
        strLockfile, _
        GENERIC_WRITE, _
        0, _
        ByVal 0, _
        CREATE_NEW, _
        FILE_ATTRIBUTE_NORMAL Or FILE_FLAG_DELETE_ON_CLOSE, _
        0)
        
    If m_handle <> INVALID_HANDLE_VALUE Then
        CreateShareLockfile = 1
    ElseIf Err.LastDllError = ERROR_FILE_EXISTS Then
        CreateShareLockfile = 0
    Else
        CreateShareLockfile = -1
    End If
End Function

'----------------------------------------------------------------------
' [�T�v]
' ���b�N�t�@�C�������(�폜����)
'----------------------------------------------------------------------

Public Sub CloseShareLockfile()
    If m_handle > 0 Then
        CloseHandle m_handle
        m_handle = INVALID_HANDLE_VALUE
    End If
End Sub
