VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsWssLog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'=======================================================================
' WssLog.dllを扱うクラス
'
' 2009/04/17 小島 新規作成
'=======================================================================
Option Explicit

'///////////////////////////////////////////////////////////////////////
' 関数の定義
'
Private Declare Sub WinssaVB_Log Lib "WssLog.dll" (ByVal strSikibetu As String, ByVal nLoglevel As Long, ByVal strMsg As String)

'///////////////////////////////////////////////////////////////////////
' ログのレベル定義
'
Private Const LOGLV_DEBUG As Long = 0       'デバッグ用
Private Const LOGLV_TRACE As Long = 1       'トレース
Private Const LOGLV_WARNING As Long = 2     'ワーニング(通常はこのレベル以上を出力)
Private Const LOGLV_ERR As Long = 3         'エラー
Private Const LOGLV_FATALERR As Long = 4    '致命的エラー
Private Const LOGLV_INFO As Long = 5        '情報
Private Const LOGLV_ALWAYS As Long = 9      '必ずログを出す

'///////////////////////////////////////////////////////////////////////
' メンバー変数
'
Private m_strIdent As String    '識別子

'///////////////////////////////////////////////////////////////////////
' 識別子の取得/設定
'
Public Property Get Ident() As String
    Ident = m_strIdent
End Property

Public Property Let Ident(strNewIdent As String)
    m_strIdent = Left(strNewIdent, 6)
End Property

'///////////////////////////////////////////////////////////////////////
' 情報ログの出力
'
Public Function WriteInfo(strFuncName As String, strMsg As String) As String
    Dim strMsg2 As String
    
    If strFuncName = "" Then
        strMsg2 = strMsg
    Else
        strMsg2 = strFuncName & " : " & strMsg
    End If
    
    Call WinssaVB_Log(m_strIdent, LOGLV_INFO, strMsg2)
    Debug.Print Now, strMsg2
    
    WriteInfo = strMsg2
End Function

'///////////////////////////////////////////////////////////////////////
' エラーログの出力
'
Public Function WriteErr(strFuncName As String) As String
    Dim strMsg2 As String
    
    If strFuncName = "" Then
        strMsg2 = Err.Description & "(" & Err.Number & ")"
    Else
        strMsg2 = strFuncName & " : " & Err.Description & "(" & Err.Number & ")"
    End If
    
    Call WinssaVB_Log(m_strIdent, LOGLV_ERR, strMsg2)
    Debug.Print Now, strMsg2
    
    WriteErr = strMsg2
End Function

'///////////////////////////////////////////////////////////////////////
' コンストラクタ
'
Private Sub Class_Initialize()
    m_strIdent = Left(App.EXEName, 6)
End Sub
