VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Mutex"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Private Type SECURITY_ATTRIBUTES
        nLength As Long
        lpSecurityDescriptor As Long
        bInheritHandle As Long
End Type

Private Declare Function apiGetLastError Lib "kernel32" Alias "GetLastError" () As Long
Private Declare Function apiCreateMutex Lib "kernel32" Alias "CreateMutexA" (lpMutexAttributes As SECURITY_ATTRIBUTES, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Private Declare Function apiCloseHandle Lib "kernel32" Alias "CloseHandle" (ByVal hObject As Long) As Long
Private Declare Function apiWaitForSingleObject Lib "kernel32" Alias "WaitForSingleObject" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function apiReleaseMutex Lib "kernel32" Alias "ReleaseMutex" (ByVal hMutex As Long) As Long

Private m_hMutex As Long
Private m_lastError As Long

'----------------------------------------------------------------------
'[概要]
'   Create時のエラー詳細を扱うプロパティ

Public Property Get LastError() As Long
    LastError = m_lastError
End Property

'----------------------------------------------------------------------
'[概要]
'   指定された名前のMutexを作成する

Public Function CreateMutex(lpName As String) As Boolean
    Dim mutexAttributes As SECURITY_ATTRIBUTES
    
    m_hMutex = apiCreateMutex(mutexAttributes, 0, lpName)
    m_lastError = apiGetLastError
    CreateMutex = IIf(m_hMutex = 0, False, True)
End Function

'----------------------------------------------------------------------
'[概要]
'   Mutexの破棄(作ってあれば)

Public Sub CloseMutex()
    If m_hMutex <> 0 Then
        apiCloseHandle m_hMutex
        m_hMutex = 0
    End If
End Sub

Public Function Wait(ByVal dwMilliseconds As Long) As Long
    If m_hMutex <> 0 Then
        Wait = apiWaitForSingleObject(m_hMutex, dwMilliseconds)
    End If
End Function

Public Function Release() As Long
    If m_hMutex <> 0 Then
        Release = apiReleaseMutex(m_hMutex)
    End If
End Function
