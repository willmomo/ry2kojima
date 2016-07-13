VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsKishuSet"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

'**********************************************************************
' <Kishu>タグデータを管理するクラス(ほぼ構造体)
'**********************************************************************
Option Explicit


Private Const MIN_KISHU_NO As Long = 1
Private Const MAX_KISHU_NO As Long = 96     '(システム標準の値を使う)

Private m_lngKishuNo As Long        '機種番号
Private m_strKishuName As String    '機種名
Private m_lngTypeNo As Long         'タイプ番号


'**********************************************************************
'機種番号へのアクセサ(プロパティ)
'**********************************************************************
Public Property Get KishuNo() As Long
    KishuNo = m_lngKishuNo
End Property

Public Property Let KishuNo(ByVal newValue As Long)
    If MIN_KISHU_NO <= newValue And newValue <= MAX_KISHU_NO Then
        m_lngKishuNo = newValue
    Else
        Err.Raise 380   'error(プロパティの値が不正です)
    End If
End Property


'**********************************************************************
'機種名へのアクセサ(プロパティ)
'**********************************************************************
Public Property Get KishuName() As String
    KishuName = m_strKishuName
End Property

Public Property Let KishuName(newValue As String)
    m_strKishuName = newValue
End Property


'**********************************************************************
'タイプ番号へのアクセサ(プロパティ)
'**********************************************************************
Public Property Get TypeNo() As Long
    TypeNo = m_lngTypeNo
End Property

Public Property Let TypeNo(ByVal newValue As Long)
    m_lngTypeNo = newValue
End Property
