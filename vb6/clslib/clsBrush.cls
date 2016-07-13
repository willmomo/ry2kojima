VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsBrush"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'----------------------------------------------------------------------
'   [概要]
'   ブラシ管理クラス
'
'   [情報]
'   作成者: Ryota Kojima
'   作成日: 2003/10/09
'   リファレンス: clsBrush.html

Option Explicit

'----------------------------------------------------------------------
' 関数宣言

Private Declare Function apiCreateSolidBrush Lib "gdi32" Alias "CreateSolidBrush" (ByVal crColor As Long) As Long
Private Declare Function apiDeleteObject Lib "gdi32" Alias "DeleteObject" (ByVal hObject As Long) As Long

'----------------------------------------------------------------------
' メンバー変数

Private m_hbrush As Long            '管理するブラシへのハンドル
Private m_mustDelete As Boolean     'Createしたブラシを保持するときはTrue
                                    'DeleteObjectでFalse
Private m_attached As Boolean       'AttachされたときにTrue
                                    'DetachされたときにFalse

'----------------------------------------------------------------------
' プロパティ

'----------------------------------------------------------------------
'   [概要]
'   ブラシへのハンドルを取得
'
'   [引数]
'   なし
'
'   [戻り値]
'   保持するブラシへのハンドル
'
'   [備考]
'   なし

Public Property Get Brush() As Long
    Brush = m_hbrush
End Property

'----------------------------------------------------------------------
' メソッド

'----------------------------------------------------------------------
'   [概要]
'   ソリッドカラーのブラシを作成
'
'   [引数]
'   crColor ブラシの色
'
'   [戻り値]
'   True=正常終了。False=エラー
'
'   [備考]
'   二重作成のときは、Assertします。

Public Function CreateSolidBrush(ByVal crColor As Long) As Boolean
    Debug.Assert m_hbrush = 0
    
    'インスタンスがすでにブラシを持っているときは、エラー終了
    If m_hbrush <> 0 Then
        CreateSolidBrush = False
        Exit Function
    End If
    
    'ソリッドカラーのブラシを作成
    m_hbrush = apiCreateSolidBrush(crColor)
    If m_hbrush <> 0 Then
        m_mustDelete = True         '削除の必要があるブラシ
        m_attached = False
        CreateSolidBrush = True
    Else
        m_mustDelete = False
        m_attached = False
        CreateSolidBrush = False
    End If
End Function

'----------------------------------------------------------------------
'   [概要]
'   ブラシの削除
'
'   [引数]
'   なし
'
'   [戻り値]
'   True=正常終了。False=エラー
'
'   [備考]
'   削除の必要が無いブラシを保持しているときはAssertします。

Public Function DeleteObject() As Boolean
    'インスタンスがブラシを保持していないときは、
    '何もせずに終了。
    If m_hbrush = 0 Then
        DeleteObject = True
        Exit Function
    End If
    
    Debug.Assert m_mustDelete = True
    
    '削除の必要が無い
    If m_mustDelete = False Then
        DeleteObject = True
        Exit Function
    End If
    
    'ブラシを削除
    If apiDeleteObject(m_hbrush) <> 0 Then
        m_hbrush = 0
        m_mustDelete = False
        m_attached = False
        DeleteObject = True
    Else
        '削除失敗のときは、ハンドルやフラグなど状態を保持
        DeleteObject = False
    End If
End Function

'----------------------------------------------------------------------
'   [概要]
'   ブラシを割り当てる
'
'   [引数]
'   hbrush  GDIブラシ
'
'   [戻り値]
'   True=正常終了。False=エラー
'
'   [備考]
'   すでにブラシを保持しているときは、Assertします。

Public Function Attach(ByVal hbrush As Long) As Boolean
    Debug.Assert m_hbrush = 0
    
    'すでにオブジェクトを保有している?
    If m_hbrush <> 0 Then
        Attach = False
        Exit Function
    End If
    
    m_hbrush = hbrush
    m_mustDelete = False    '削除の必要なし
    m_attached = True
    
    Attach = True
End Function

'----------------------------------------------------------------------
'   [概要]
'   割り当てたブラシを開放する
'
'   [引数]
'   なし
'
'   [戻り値]
'   割り当ててあったブラシ
'
'   [備考]
'   Attachされたブラシを管理していないときは、Assertします。

Public Function Detach() As Long
    'インスタンスがブラシを保持していないときは、
    '何もせずに終了。
    If m_hbrush = 0 Then
        Detach = 0
        Exit Function
    End If
    
    Debug.Assert m_attached = True
    
    '所有するオブジェクトは、Attachされたものではない。
    If m_attached = False Then
        Detach = 0
        Exit Function
    End If
    
    Detach = m_hbrush
    
    m_hbrush = 0
    m_mustDelete = False
    m_attached = False
End Function

'----------------------------------------------------------------------
'   [概要]
'   コンストラクタ
'
'   [引数]
'   なし
'
'   [戻り値]
'   なし
'
'   [備考]
'   なし

Private Sub Class_Initialize()
    m_hbrush = 0
    m_mustDelete = False
    m_attached = False
End Sub

'----------------------------------------------------------------------
'   [概要]
'   デストラクタ
'
'   [引数]
'   なし
'
'   [戻り値]
'   なし
'
'   [備考]
'   AttachしたブラシをDetachせずに終了した場合、Assertします。
'   DeleteObjectに失敗した場合、Assertします。

Private Sub Class_Terminate()
    Debug.Assert m_attached = False
    
    Dim ret As Boolean
    
    '削除の必要があるブラシが確保されたまま?
    If m_hbrush <> 0 And m_mustDelete = True Then
        ret = DeleteObject
        Debug.Assert ret = True
    End If
End Sub
