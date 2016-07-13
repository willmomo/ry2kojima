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
' WssLog.dll�������N���X
'
' 2009/04/17 ���� �V�K�쐬
'=======================================================================
Option Explicit

'///////////////////////////////////////////////////////////////////////
' �֐��̒�`
'
Private Declare Sub WinssaVB_Log Lib "WssLog.dll" (ByVal strSikibetu As String, ByVal nLoglevel As Long, ByVal strMsg As String)

'///////////////////////////////////////////////////////////////////////
' ���O�̃��x����`
'
Private Const LOGLV_DEBUG As Long = 0       '�f�o�b�O�p
Private Const LOGLV_TRACE As Long = 1       '�g���[�X
Private Const LOGLV_WARNING As Long = 2     '���[�j���O(�ʏ�͂��̃��x���ȏ���o��)
Private Const LOGLV_ERR As Long = 3         '�G���[
Private Const LOGLV_FATALERR As Long = 4    '�v���I�G���[
Private Const LOGLV_INFO As Long = 5        '���
Private Const LOGLV_ALWAYS As Long = 9      '�K�����O���o��

'///////////////////////////////////////////////////////////////////////
' �����o�[�ϐ�
'
Private m_strIdent As String    '���ʎq

'///////////////////////////////////////////////////////////////////////
' ���ʎq�̎擾/�ݒ�
'
Public Property Get Ident() As String
    Ident = m_strIdent
End Property

Public Property Let Ident(strNewIdent As String)
    m_strIdent = Left(strNewIdent, 6)
End Property

'///////////////////////////////////////////////////////////////////////
' ��񃍃O�̏o��
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
' �G���[���O�̏o��
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
' �R���X�g���N�^
'
Private Sub Class_Initialize()
    m_strIdent = Left(App.EXEName, 6)
End Sub