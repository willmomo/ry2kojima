VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsLockFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' VB6用ロックファイル処理(ver.0.2010.01.15)
'
' [使い方]
' Dim objLock As New clsLockFile
' If objLock.CreateLockFile(ロックファイルの名前) Then
'   ロック成功時の処理
'   objLock.DeleteLockFile
' End If
'----------------------------------------------------------------------
Option Explicit

Private Const INVALID_HANDLE_VALUE = -1

'dwDesiredAccessの定数
Private Const GENERIC_WRITE = &H40000000

'dwCreationDispositionの定数
Private Const CREATE_ALWAYS = 2

'dwFlagsAndAttributesの定数
Private Const FILE_ATTRIBUTE_NORMAL = &H80
Private Const FILE_ATTRIBUTE_TEMPORARY = &H100

Private Const FILE_FLAG_DELETE_ON_CLOSE = &H4000000

Private Declare Function CreateFile Lib "kernel32" Alias "CreateFileA" ( _
    ByVal lpFileName As String, _
    ByVal dwDesiredAccess As Long, _
    ByVal dwShareMode As Long, _
    ByVal lpSecurityAttributes As Long, _
    ByVal dwCreationDisposition As Long, _
    ByVal dwFlagsAndAttributes As Long, _
    ByVal hTemplateFile As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private m_hFile As Long

'----------------------------------------------------------------------
' ロックファイルを作成
'
' [戻り値]
' True   ロック成功。
' False  ロック失敗。
'
' このオブジェクトで、別のファイルをロックしているときは、ロックを解除
' してから、指定のファイルをロックします。
'----------------------------------------------------------------------
Public Function CreateLockFile(strLockFileName As String) As Boolean
    'ロック失敗で初期化。
    CreateLockFile = False

    '同じ、または、別のファイルをロックしいるかもしれないので、ロックファイルを閉じておく
    DeleteLockFile
    
    'ファイルを常に作成モード、全アクセス拒否で開く。
    'ファイルが開けたら、誰もロックしていないので、ロック成功を返す。
    m_hFile = CreateFile(strLockFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY Or FILE_FLAG_DELETE_ON_CLOSE, 0)
    If m_hFile <> INVALID_HANDLE_VALUE Then
        CreateLockFile = True
    End If
End Function

'----------------------------------------------------------------------
' ロックファイルを閉じる
'
' ロックしていなくても、正常に処理します。
'----------------------------------------------------------------------
Public Sub DeleteLockFile()
    If m_hFile <> INVALID_HANDLE_VALUE Then
        CloseHandle m_hFile
        m_hFile = INVALID_HANDLE_VALUE
    End If
End Sub

'----------------------------------------------------------------------
' クラスの初期化
'----------------------------------------------------------------------
Private Sub Class_Initialize()
    m_hFile = INVALID_HANDLE_VALUE
End Sub

'----------------------------------------------------------------------
' クラスの後始末
'----------------------------------------------------------------------
Private Sub Class_Terminate()
    DeleteLockFile
End Sub
