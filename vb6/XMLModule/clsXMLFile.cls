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

Private m_xmlSource As String   'XML�t�@�C���̖��O
Private m_useCache As Boolean   '�L���b�V���@�\���g�����ǂ���
Private m_cache As Dictionary   '�L���b�V��

'----------------------------------------------------------------------
'�L���b�V���@�\��ON/OFF
'
Public Property Get UseCache() As Boolean
    UseCache = m_useCache
End Property

Public Property Let UseCache(ByVal newValue As Boolean)
    m_useCache = newValue
End Property

'----------------------------------------------------------------------
'�L���b�V�����̃N���A
'
Public Sub ClearCache()
    m_cache.RemoveAll
End Sub

'----------------------------------------------------------------------
'��������
'
Public Function WriteText(ByRef tagName As String, ByRef newValue As String) As Boolean
    On Error GoTo ErrHandler
    
    'XML�t�@�C���ɒl����������
    WriteText = XMLWriteText(tagName, newValue, m_xmlSource)
    
    '�������݂ɐ����B���A�L���b�V���@�\ON�̎��́AtagName���L�[�ɂ��Ēl��ۑ�����
    If WriteText = True And m_useCache = True Then
        m_cache(tagName) = newValue
    End If
    
    Exit Function
    
ErrHandler:
    WriteText = False
End Function

'----------------------------------------------------------------------
'�ǂݍ���
'
Public Function ReadText(ByRef tagName As String, ByRef defaultValue As String) As String
    On Error GoTo ErrHandler
    
    '�L���b�V���@�\���L���̂Ƃ��́AtagName���L�[�ɂ��āA�l�����o���ďI��
    If m_useCache Then
        If m_cache.Exists(tagName) Then
            ReadText = m_cache(tagName)
            Exit Function
        End If
    End If
    
    'XML�t�@�C������l��ǂݍ���
    ReadText = XMLReadText(tagName, defaultValue, m_xmlSource)
    
    '�L���b�V���@�\���L���̂Ƃ��́AtagName���L�[�ɂ��āA�l��ۑ�����
    If m_useCache Then
        m_cache(tagName) = ReadText
    End If
    
    Exit Function

ErrHandler:
    '�Ƃ肠�����������܂���B
End Function

'----------------------------------------------------------------------
'�t�@�C�����J��(���ۂ́A�����̊֘A�t��)
'
Public Sub OpenFile(ByVal xmlSource As String)

    '�t�@�C������ۑ�����
    '�Ȍ�A���̃t�@�C���̃G���g���[��ǂݏ�������
    m_xmlSource = xmlSource
    
    '�t�@�C�������ύX���ꂽ�Ƃ��̂��߂ɁA�L���b�V���̓N���A����
    '(�ʃt�@�C���̓����G���g���[���L���b�V��������o���Ȃ��悤�ɂ���)
    ClearCache
    
End Sub

'----------------------------------------------------------------------
'�R���X�g���N�^
'
Private Sub Class_Initialize()
    Set m_cache = New Dictionary
End Sub

'----------------------------------------------------------------------
'�f�X�g���N�^
'
Private Sub Class_Terminate()
    Set m_cache = Nothing
End Sub