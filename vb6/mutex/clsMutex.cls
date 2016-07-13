VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMutex"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------------------------------------------------------
' clsMutex: Mutex�Ǘ��N���X
'
' 2011/08/09 r.kojima �V�K�쐬
' 2011/08/12 r.kojima create���\�b�h���������߂�l��Ԃ��悤�ɏC���B
'------------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------------------------
' �^�̐錾

Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type

'------------------------------------------------------------------------------
' �֐��̐錾

Private Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" ( _
    lpMutexAttributes As SECURITY_ATTRIBUTES, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function ReleaseMutex Lib "kernel32" (ByVal hMutex As Long) As Long

'------------------------------------------------------------------------------
' �萔�̐錾

Private Const WAIT_OBJECT_0 As Long = 0     '�w�肵���I�u�W�F�N�g���V�O�i����ԂɂȂ���
Private Const WAIT_ABANDONED As Long = &H80 '�w�肳�ꂽ�I�u�W�F�N�g�́A�������ꂽ�~���[�e�b�N�X�I�u�W�F�N�g�ł���
                                            '�i����X���b�h�����L���������Ă��܂������A���̃X���b�h�͏��L����������Ȃ��ŏI�����܂����j�B
                                            '���̊֐����Ăяo�������ʁA���̃~���[�e�b�N�X�I�u�W�F�N�g�̏��L���͌Ăяo�����X���b�h�Ɉڂ�A
                                            '���̃~���[�e�b�N�X�͔�V�O�i����Ԃɐݒ肳��܂���
Private Const WAIT_TIMEOUT As Long = &H102  '�^�C���A�E�g���Ԃ��o�߂��A�w�肳�ꂽ�I�u�W�F�N�g����V�O�i����Ԃł��������Ƃ��Ӗ����܂�

Private Const ERROR_ALREADY_EXISTS = 183&

'------------------------------------------------------------------------------
' �����o�ϐ�

Private m_hMutex As Long            'Mutex�ւ̃n���h��
Private m_lockCnt As Long           'lock������
Private m_bAlreadyExists As Boolean 'ERROR_ALREADY_EXISTS ���ǂ�����ێ�����

'------------------------------------------------------------------------------
' ����Mutex���A���łɑ��݂������ǂ�����Ԃ�
'------------------------------------------------------------------------------
Public Property Get AlreadyExists() As Boolean
    AlreadyExists = m_bAlreadyExists
End Property

'------------------------------------------------------------------------------
' Mutex���쐬
'------------------------------------------------------------------------------
Public Function create(strName As String) As Boolean
    Dim sa As SECURITY_ATTRIBUTES
    
    ' ���łɃI�u�W�F�N�g�쐬�ς݂̉\�������邽�߁A
    ' closeMutex ���\�b�h���Ăяo���Ă���
    Me.closeMutex
    
    ' Mutex�̍쐬
    m_hMutex = CreateMutex(sa, 0, strName)
    m_bAlreadyExists = IIf(Err.LastDllError = ERROR_ALREADY_EXISTS, True, False)
    
    create = IIf(m_hMutex <> 0, True, False)
End Function


'------------------------------------------------------------------------------
' Mutex�����
'------------------------------------------------------------------------------
Public Function closeMutex() As Long
    closeMutex = 0
    
    If m_hMutex <> 0 Then
        
        Do While m_lockCnt > 0
            Me.unlockMutex
        Loop
        
        closeMutex = CloseHandle(m_hMutex)
    End If
    
    '�����o�ϐ����m���ɍď�����
    m_hMutex = 0
    m_lockCnt = 0
    m_bAlreadyExists = False
End Function


'------------------------------------------------------------------------------
' lock
'------------------------------------------------------------------------------
Public Function lockMutex(ByVal dwMilliseconds As Long) As Boolean
    Dim nRet As Long
    
    lockMutex = False
    
    nRet = WaitForSingleObject(m_hMutex, dwMilliseconds)
    If (nRet = WAIT_OBJECT_0) Or (nRet = WAIT_ABANDONED) Then
        '���b�N����
        m_lockCnt = m_lockCnt + 1
        lockMutex = True
    End If
End Function


'------------------------------------------------------------------------------
' unlock
'------------------------------------------------------------------------------
Public Function unlockMutex() As Boolean
    unlockMutex = False
    
    If m_lockCnt > 0 Then
        If ReleaseMutex(m_hMutex) <> 0 Then
            unlockMutex = True
        End If
        
        '�������[�v��}���邽�߁AReleaseMutex�̐��ۂ��킸�A
        '���b�N�J�E���g�����炵�Ă��܂��B
        
        m_lockCnt = m_lockCnt - 1
    End If
End Function


'------------------------------------------------------------------------------
' �R���X�g���N�^
'------------------------------------------------------------------------------
Private Sub Class_Initialize()
    m_hMutex = 0
    m_lockCnt = 0
    m_bAlreadyExists = False
End Sub


'------------------------------------------------------------------------------
' �f�X�g���N�^
'------------------------------------------------------------------------------
Private Sub Class_Terminate()
    '�n���h���̔j����ۏႷ��
    Me.closeMutex
End Sub