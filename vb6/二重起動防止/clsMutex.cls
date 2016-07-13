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
' clsMutex: Mutex管理クラス
'
' 2011/08/09 r.kojima 新規作成
' 2011/08/12 r.kojima createメソッドが正しく戻り値を返すように修正。
'------------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------------------------
' 型の宣言

Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type

'------------------------------------------------------------------------------
' 関数の宣言

Private Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" ( _
    lpMutexAttributes As SECURITY_ATTRIBUTES, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function ReleaseMutex Lib "kernel32" (ByVal hMutex As Long) As Long

'------------------------------------------------------------------------------
' 定数の宣言

Private Const WAIT_OBJECT_0 As Long = 0     '指定したオブジェクトがシグナル状態になった
Private Const WAIT_ABANDONED As Long = &H80 '指定されたオブジェクトは、放棄されたミューテックスオブジェクトでした
                                            '（あるスレッドが所有権を持っていましたが、そのスレッドは所有権を解放しないで終了しました）。
                                            'この関数を呼び出した結果、そのミューテックスオブジェクトの所有権は呼び出し側スレッドに移り、
                                            'そのミューテックスは非シグナル状態に設定されました
Private Const WAIT_TIMEOUT As Long = &H102  'タイムアウト時間が経過し、指定されたオブジェクトが非シグナル状態であったことを意味します

Private Const ERROR_ALREADY_EXISTS = 183&

'------------------------------------------------------------------------------
' メンバ変数

Private m_hMutex As Long            'Mutexへのハンドル
Private m_lockCnt As Long           'lockした回数
Private m_bAlreadyExists As Boolean 'ERROR_ALREADY_EXISTS かどうかを保持する

'------------------------------------------------------------------------------
' 同名Mutexが、すでに存在したかどうかを返す
'------------------------------------------------------------------------------
Public Property Get AlreadyExists() As Boolean
    AlreadyExists = m_bAlreadyExists
End Property

'------------------------------------------------------------------------------
' Mutexを作成
'------------------------------------------------------------------------------
Public Function create(strName As String) As Boolean
    Dim sa As SECURITY_ATTRIBUTES
    
    ' すでにオブジェクト作成済みの可能性があるため、
    ' closeMutex メソッドを呼び出しておく
    Me.closeMutex
    
    ' Mutexの作成
    m_hMutex = CreateMutex(sa, 0, strName)
    m_bAlreadyExists = IIf(Err.LastDllError = ERROR_ALREADY_EXISTS, True, False)
    
    create = IIf(m_hMutex <> 0, True, False)
End Function


'------------------------------------------------------------------------------
' Mutexを閉じる
'------------------------------------------------------------------------------
Public Function closeMutex() As Long
    closeMutex = 0
    
    If m_hMutex <> 0 Then
        
        Do While m_lockCnt > 0
            Me.unlockMutex
        Loop
        
        closeMutex = CloseHandle(m_hMutex)
    End If
    
    'メンバ変数を確実に再初期化
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
        'ロック成功
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
        
        '無限ループを抑えるため、ReleaseMutexの成否を問わず、
        'ロックカウントを減らしています。
        
        m_lockCnt = m_lockCnt - 1
    End If
End Function


'------------------------------------------------------------------------------
' コンストラクタ
'------------------------------------------------------------------------------
Private Sub Class_Initialize()
    m_hMutex = 0
    m_lockCnt = 0
    m_bAlreadyExists = False
End Sub


'------------------------------------------------------------------------------
' デストラクタ
'------------------------------------------------------------------------------
Private Sub Class_Terminate()
    'ハンドルの破棄を保障する
    Me.closeMutex
End Sub
