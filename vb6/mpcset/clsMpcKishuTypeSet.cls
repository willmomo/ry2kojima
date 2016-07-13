VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMpcKishuTypeSet"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'�@��ݒ�^�O
'<kishu>
'  <kishuno>�@��ԍ�(1�`MAX_KISHU)</kishuno>
'  <kishuname>�@�햼(�ő�20�o�C�g)</kishuname>
'  <typeno>�^�C�v�ԍ�(1�`MAX_TYPE)</typeno>
'</kishu>

Private Type KISHU_TAG
    m_kishuNo As String
    m_kishuName As String
    m_typeNo As String
End Type

'�^�C�v�ݒ�^�O
'<type>
'  <typeno>�^�C�v�ԍ�(1�`MAX_TYPE)</typeno>
'  <typename>�^�C�v��(�ő�20�o�C�g)</typename>
'</type>

Private Type TYPE_TAG
    m_typeNo As String
    m_typeName As String
End Type

Private m_kishu() As KISHU_TAG
Private m_type() As TYPE_TAG

'----------------------------------------------------------------------
'�z����g������
'
Private Sub GrowArray(ar, pres)
    Dim newUpper As Long
    
    On Error Resume Next
    newUpper = UBound(ar) + 1
    If Err.Number <> 0 Then
        newUpper = 0
    End If
    
    If pres = False Then
        ReDim ar(upper)
    Else
        ReDim Preserve ar(upper)
    End If
End Sub

'----------------------------------------------------------------------
'�w��@��̋@�햼���擾����
'
Public Property Get KishuName(ByVal kishuNo As Long) As String
    '�w��@��ւ̃C���f�b�N�X���擾
    '�@�킪������Ȃ��Ƃ��́A�u�s���ȃv���p�e�B�z��C���f�b�N�X�ł��B�v�G���[
    Dim Index As Long
    Index = IndexOfKishu(kishuNo)
    If Index = -1 Then
        Err.Raise 381
    End If
    
    KishuName = m_kishu(Index).m_kishuName
End Property

'----------------------------------------------------------------------
'�w��@��̋@�햼��ݒ肷��
'
Public Property Let KishuName(ByVal kishuNo As Long, ByRef newValue As String)
    '�w��@��ւ̃C���f�b�N�X���擾
    '�@�킪������Ȃ��Ƃ��́A�z����g�����Ēǉ�����B
    Dim Index As Long
    Index = IndexOfKishu(kishuNo)
    If Index = -1 Then
        GrowArray m_kishu, True
        Index = UBound(m_kishu)
    End If
    
    m_kishu(Index).KishuName = newValue
End Property

Public Property Get typeno(ByVal kishuNo As Long) As Long
    Dim Index As Long
    
    Index = IndexOfKishu(kishuNo)
    typeno = m_kishu(Index).typeno
End Property

Public Property Let typeno(ByVal kishuNo As Long, ByVal newValue As Long)
    Dim Index As Long
    
    Index = IndexOfKishu(kishuNo)
    m_kishu(Index).typeno = newValue
End Property

Public Property Get typename(ByVal typeno As Long) As String
    Dim Index As Long
    
    Index = IndexOfType(typeno)
    typename = m_type(Index).typename
End Property

Public Property Set typename(ByVal typeno As Long, ByRef newValue As String)
    Dim Index As Long
    
    Index = IndexOfType(typeno)
    m_type(Index).typename = newValue
End Property

'----------------------------------------------------------------------
'�w�肳�ꂽ�@��ԍ��̃f�[�^������m_kishu�ւ�Index��Ԃ�
'
Private Function IndexOfKishu(ByVal kishuNo As Long) As Long
    IndexOfKishu = -1
    
    Dim i As Long
    For i = 0 To UBound(m_kishu)
        If CLng(m_kishu(i).kishuNo) = kishuNo Then
            IndexOfKishu = i
            Exit For
        End If
    Next
End Function

'----------------------------------------------------------------------
'�w�肳�ꂽ�^�C�v�ԍ��̃f�[�^������m_type�ւ�Index��Ԃ�
'
Private Function IndexOfType(ByVal typeno As Long) As Long
    IndexOfType = -1
    
    Dim i As Long
    For i = 0 To UBound(m_type)
        If CLng(m_type(i).typeno) = typeno Then
            IndexOfType = i
            Exit For
        End If
    Next
End Function

'----------------------------------------------------------------------
'�q�m�[�h����
'
Private Function GetNamedNode(ByRef nodeList As IXMLDOMNodeList, ByRef nodeName As String) As IXMLDOMNode
    Dim node As IXMLDOMNode
    
    Set GetNamedNode = Nothing
    
    For Each node In nodeList
        If node.nodeName = nodeName Then
            Set GetNamedNode = node
            Exit For
        End If
    Next
End Function

'----------------------------------------------------------------------
'�t�@�C������MPC�@��E�^�C�v�ݒ��ǂݍ���
'
Public Function LoadFile(ByRef xmlFile As String) As Boolean

    'XML�t�@�C����ǂݍ���
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    If Not xmlDoc.Load(xmlFile) Then
        Set xmlDoc = Nothing
        LoadFile = False
        Exit Function
    End If
    
    '<kishu>�^�O����m_kishu������������
    Dim kishuNodeList As IXMLDOMNodeList
    Set kishuNodeList = xmlDoc.getElementsByTagName("kishu")
    
    ReDim m_kishu(kishuNodeList.length - 1)
    
    Dim i As Long
    For i = 0 To kishuNodeList.length - 1
        m_kishu(i).kishuNo = GetNamedNode(kishuNodeList.Item(i).childNodes, "kishuno").Text
        m_kishu(i).KishuName = GetNamedNode(kishuNodeList.Item(i).childNodes, "kishuname").Text
        m_kishu(i).typeno = GetNamedNode(kishuNodeList.Item(i).childNodes, "typeno").Text
    Next
    
    '<type>�^�O����m_type������������
    Dim typeNodeList As IXMLDOMNodeList
    Set typeNodeList = xmlDoc.getElementsByTagName("type")
    
    ReDim m_type(typeNodeList.length - 1)
    
    For i = 0 To typeNodeList.length - 1
        m_type(i).typeno = GetNamedNode(typeNodeList.Item(i).childNodes, "typeno").Text
        m_type(i).typename = GetNamedNode(typeNodeList.Item(i).childNodes, "typename").Text
    Next
    
    LoadFile = True
End Function