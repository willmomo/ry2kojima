VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsXMLFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_xmlSource As String   'XMLファイルの名前
Private m_useCache As Boolean   'キャッシュ機能を使うかどうか
Private m_cache As Dictionary   'キャッシュ

'----------------------------------------------------------------------
'キャッシュ機能のON/OFF
'
Public Property Get UseCache() As Boolean
    UseCache = m_useCache
End Property

Public Property Let UseCache(ByVal newValue As Boolean)
    m_useCache = newValue
End Property

'----------------------------------------------------------------------
'キャッシュ情報のクリア
'
Public Sub ClearCache()
    m_cache.RemoveAll
End Sub

'----------------------------------------------------------------------
'書き込み
'
Public Function WriteText(ByRef tagName As String, ByRef newValue As String) As Boolean
    On Error GoTo ErrHandler
    
    'XMLファイルに値を書き込む
    WriteText = XMLWriteText(tagName, newValue, m_xmlSource)
    
    '書き込みに成功。かつ、キャッシュ機能ONの時は、tagNameをキーにして値を保存する
    If WriteText = True And m_useCache = True Then
        m_cache(tagName) = newValue
    End If
    
    Exit Function
    
ErrHandler:
    WriteText = False
End Function

'----------------------------------------------------------------------
'読み込み
'
Public Function ReadText(ByRef tagName As String, ByRef defaultValue As String) As String
    On Error GoTo ErrHandler
    
    'キャッシュ機能が有効のときは、tagNameをキーにして、値を取り出して終了
    If m_useCache Then
        If m_cache.Exists(tagName) Then
            ReadText = m_cache(tagName)
            Exit Function
        End If
    End If
    
    'XMLファイルから値を読み込む
    ReadText = XMLReadText(tagName, defaultValue, m_xmlSource)
    
    'キャッシュ機能が有効のときは、tagNameをキーにして、値を保存する
    If m_useCache Then
        m_cache(tagName) = ReadText
    End If
    
    Exit Function

ErrHandler:
    'とりあえず何もしません。
End Function

'----------------------------------------------------------------------
'ファイルを開く(実際は、ただの関連付け)
'
Public Sub OpenFile(ByVal xmlSource As String)

    'ファイル名を保存する
    '以後、このファイルのエントリーを読み書きする
    m_xmlSource = xmlSource
    
    'ファイル名が変更されたときのために、キャッシュはクリアする
    '(別ファイルの同じエントリーをキャッシュから取り出さないようにする)
    ClearCache
    
End Sub

'----------------------------------------------------------------------
'コンストラクタ
'
Private Sub Class_Initialize()
    Set m_cache = New Dictionary
End Sub

'----------------------------------------------------------------------
'デストラクタ
'
Private Sub Class_Terminate()
    Set m_cache = Nothing
End Sub
