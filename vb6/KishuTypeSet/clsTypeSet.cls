VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTypeSet"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

'**********************************************************************
'<Type>タグデータを管理するクラス(ほぼ構造体)
'**********************************************************************
Option Explicit


Private Const MIN_TYPE_NO As Long = 1
Private Const MAX_TYPE_NO As Long = 16  '(システム標準の値を使う)

Private m_lngTypeNo As Long     'タイプ番号
Private m_strTypeName As String 'タイプ名


'**********************************************************************
'タイプ番号へのアクセサ(プロパティ)
'**********************************************************************
Public Property Get TypeNo() As Long
    TypeNo = m_lngTypeNo
End Property

Public Property Let TypeNo(ByVal newValue As Long)
    If MIN_TYPE_NO <= newValue And newValue <= MAX_TYPE_NO Then
        m_lngTypeNo = newValue
    Else
        Err.Raise 380   'error(プロパティの値が不正です)
    End If
End Property


'**********************************************************************
'タイプ名へのアクセサ(プロパティ)
'**********************************************************************
Public Property Get TypeName() As String
    TypeName = m_strTypeName
End Property

Public Property Let TypeName(newValue As String)
    m_strTypeName = newValue
End Property
