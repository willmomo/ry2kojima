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
' <Kishu>�^�O�f�[�^���Ǘ�����N���X(�قڍ\����)
'**********************************************************************
Option Explicit


Private Const MIN_KISHU_NO As Long = 1
Private Const MAX_KISHU_NO As Long = 96     '(�V�X�e���W���̒l���g��)

Private m_lngKishuNo As Long        '�@��ԍ�
Private m_strKishuName As String    '�@�햼
Private m_lngTypeNo As Long         '�^�C�v�ԍ�


'**********************************************************************
'�@��ԍ��ւ̃A�N�Z�T(�v���p�e�B)
'**********************************************************************
Public Property Get KishuNo() As Long
    KishuNo = m_lngKishuNo
End Property

Public Property Let KishuNo(ByVal newValue As Long)
    If MIN_KISHU_NO <= newValue And newValue <= MAX_KISHU_NO Then
        m_lngKishuNo = newValue
    Else
        Err.Raise 380   'error(�v���p�e�B�̒l���s���ł�)
    End If
End Property


'**********************************************************************
'�@�햼�ւ̃A�N�Z�T(�v���p�e�B)
'**********************************************************************
Public Property Get KishuName() As String
    KishuName = m_strKishuName
End Property

Public Property Let KishuName(newValue As String)
    m_strKishuName = newValue
End Property


'**********************************************************************
'�^�C�v�ԍ��ւ̃A�N�Z�T(�v���p�e�B)
'**********************************************************************
Public Property Get TypeNo() As Long
    TypeNo = m_lngTypeNo
End Property

Public Property Let TypeNo(ByVal newValue As Long)
    m_lngTypeNo = newValue
End Property