VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsKishuTypeSet"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

'**********************************************************************
'���̃N���X���g�����߂ɂ́A�Q�Ɛݒ��
'  Microsoft XML, version 2.0 (msxml.dll)
'  Microsoft Scripting Runtime (scrrun.dll)
'��I������K�v������܂��
'**********************************************************************
Option Explicit


Private m_kishuSet As New Dictionary    '�@��ݒ�
Private m_typeSet As New Dictionary     '�^�C�v�ݒ�


'**********************************************************************
'�m�[�h���X�g����A�w��^�O�̃m�[�h�̃e�L�X�g��Ԃ�(���[�e�B���e�B�֐�)
'
'[���l]
'�m�[�h����������Ƃ��́A�ŏ��Ɍ������̃m�[�h�̃e�L�X�g
'������Ȃ��Ƃ��́A�󕶎����Ԃ�
'**********************************************************************
Private Function sub_GetNodeText(objNodeList As IXMLDOMNodeList, strNodeName As String) As String
    Dim i As Long
    
    sub_GetNodeText = ""
    
    For i = 0 To objNodeList.length - 1
        If objNodeList.Item(i).nodeName = strNodeName Then
            sub_GetNodeText = objNodeList.Item(i).Text
            Exit For
        End If
    Next
End Function


'**********************************************************************
'�m�[�h���X�g����A�w��^�O�̃m�[�h�̃e�L�X�g�𐔒l�ɂ��ĕԂ�(���[�e�B���e�B�֐�)
'
'[���l]
'�m�[�h����������Ƃ��́A�ŏ��Ɍ������̃m�[�h�̃e�L�X�g
'������Ȃ��Ƃ��́A0��Ԃ�
'**********************************************************************
Private Function sub_GetNodeTextLong(objNodeList As IXMLDOMNodeList, strNodeName As String) As Long
    Dim strText As String
    
    strText = sub_GetNodeText(objNodeList, strNodeName)
    If strText = "" Then
        strText = "0"
    End If

    sub_GetNodeTextLong = CLng(strText)
End Function


'**********************************************************************
'�w��@��̋@��ݒ�ɃA�N�Z�X����
'
'[���l]
'�w��@��̐ݒ肪�����Ƃ��́A��̐ݒ��ǉ����āA�Q�Ƃ�Ԃ�
'**********************************************************************
Public Property Get KishuSet(ByVal lngKishuNo As Long) As clsKishuSet
    If m_kishuSet.Exists(lngKishuNo) Then
        Set KishuSet = m_kishuSet.Item(lngKishuNo)
    Else
        Dim objKishuSet As clsKishuSet
        Set objKishuSet = New clsKishuSet
        objKishuSet.KishuNo = lngKishuNo
        m_kishuSet.Add lngKishuNo, objKishuSet
        Set KishuSet = objKishuSet
    End If
End Property


'**********************************************************************
'�w��^�C�v�̃^�C�v�ݒ�ɃA�N�Z�X����
'
'[���l]
'�w��^�C�v�̐ݒ肪�����Ƃ��́A��̐ݒ��ǉ����āA�Q�Ƃ�Ԃ�
'**********************************************************************
Public Property Get TypeSet(ByVal lngTypeNo As Long) As clsTypeSet
    If m_typeSet.Exists(lngTypeNo) Then
        Set TypeSet = m_typeSet.Item(lngTypeNo)
    Else
        Dim objTypeSet As clsTypeSet
        Set objTypeSet = New clsTypeSet
        objTypeSet.TypeNo = lngTypeNo
        m_typeSet.Add lngTypeNo, objTypeSet
        Set TypeSet = objTypeSet
    End If
End Property


'**********************************************************************
'�w��@��̐ݒ�L���𒲂ׂ�
'**********************************************************************
Public Property Get ExistsKishuSet(ByVal nKishuNo As Long) As Boolean
    ExistsKishuSet = m_kishuSet.Exists(nKishuNo)
End Property


'**********************************************************************
'�w��^�C�v�̐ݒ�L���𒲂ׂ�
'**********************************************************************
Public Property Get ExistsTypeSet(ByVal nTypeNo As Long) As Boolean
    ExistsTypeSet = m_typeSet.Exists(nTypeNo)
End Property


'**********************************************************************
'XML(DOMDocument)����@��ݒ��ǂݍ���
'**********************************************************************
Private Sub LoadKishuSet(ByRef doc As DOMDocument)
    
    '���݂̋@��ݒ���N���A����
    m_kishuSet.RemoveAll
    
    '�@��ݒ�̓ǂݍ���
    Dim nodeList As IXMLDOMNodeList
    Set nodeList = doc.getElementsByTagName("Root/Kishu")
    
    Dim i As Long
    For i = 0 To nodeList.length - 1
    
        '�@��ݒ�I�u�W�F�N�g���쐬���ċ@��ݒ��W�J����
        Dim objKishuSet As clsKishuSet
        Set objKishuSet = New clsKishuSet
        
        objKishuSet.KishuNo = sub_GetNodeTextLong(nodeList.Item(i).childNodes, "KishuNo")
        objKishuSet.KishuName = sub_GetNodeText(nodeList.Item(i).childNodes, "KishuName")
        objKishuSet.TypeNo = sub_GetNodeTextLong(nodeList.Item(i).childNodes, "TypeNo")
        
        '�@��ݒ莫���ɋ@��ԍ��Ɗ֘A�t���ēo�^����
        '���łɁA�֘A�t�����Ă���Ƃ��́A�l�����ւ���
        If m_kishuSet.Exists(objKishuSet.KishuNo) Then
            Set m_kishuSet.Item(objKishuSet.KishuNo) = objKishuSet
        Else
            m_kishuSet.Add objKishuSet.KishuNo, objKishuSet
        End If
    Next
    
End Sub


'**********************************************************************
'XML(DOMDocument)����^�C�v�ݒ��ǂݍ���
'**********************************************************************
Private Sub LoadTypeSet(ByRef doc As DOMDocument)

    '���݂̃^�C�v�ݒ���N���A����
    m_typeSet.RemoveAll
    
    '�^�C�v�ݒ�̓ǂݍ���
    Dim nodeList As IXMLDOMNodeList
    Set nodeList = doc.getElementsByTagName("Root/Type")
    
    Dim i As Long
    For i = 0 To nodeList.length - 1
    
        '�^�C�v�ݒ�I�u�W�F�N�g���쐬���ă^�C�v�ݒ��W�J����
        Dim objTypeSet As clsTypeSet
        Set objTypeSet = New clsTypeSet
        
        objTypeSet.TypeNo = sub_GetNodeTextLong(nodeList.Item(i).childNodes, "TypeNo")
        objTypeSet.TypeName = sub_GetNodeText(nodeList.Item(i).childNodes, "TypeName")
        
        '�^�C�v�ݒ莫���Ƀ^�C�v�ԍ��Ɗ֘A�t���ēo�^����
        If m_typeSet.Exists(objTypeSet.TypeNo) Then
            Set m_typeSet.Item(objTypeSet.TypeNo) = objTypeSet
        Else
            m_typeSet.Add objTypeSet.TypeNo, objTypeSet
        End If
    Next
    
End Sub


'**********************************************************************
'�t�@�C������ݒ��ǂݍ���
'
'[���l]
'XML�I�ɓǂݍ��߂Ȃ��Ƃ��́A�G���[�ɂȂ�B�_���I�ȃG���[�̓G���[�ɂȂ�Ȃ��B
'**********************************************************************
Public Function LoadFile(xmlFile As String, errMsg As String) As Boolean
    
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    Do
        If Not xmlDoc.Load(xmlFile) Then
        
            '�G���[���b�Z�[�W���쐬���ăG���[�I��
            errMsg = _
                xmlDoc.parseError.reason & _
                "���\�[�X '" & xmlDoc.parseError.url & "' �̎��s�G���[�ł��B" & _
                "���C�� " & xmlDoc.parseError.Line & "�A�ʒu " & xmlDoc.parseError.linepos & vbNewLine & _
                vbNewLine & _
                xmlDoc.parseError.srcText & vbNewLine & _
                String(xmlDoc.parseError.linepos - 1, " ") & "^"
                
            LoadFile = False
            Exit Do
        End If
            
        '�@��ݒ�̓ǂݍ���
        LoadKishuSet xmlDoc
        
        '�^�C�v�ݒ�̓ǂݍ���
        LoadTypeSet xmlDoc
    
        LoadFile = True
    Loop While False
    
    'Finally��������
    Set xmlDoc = Nothing
End Function


'**********************************************************************
'�t�@�C���ɐݒ����������
'
'[���l]
'createTextNode�́A�G�f�B�^�Ō��₷���悤�ɐ��`���邽�߂ŁA�{���͕s�v
'**********************************************************************
Public Function SaveFile(xmlFile As String, errMsg As String) As Boolean
    Dim i As Long
    Dim objNode As IXMLDOMNode
    
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    Do
        '���[�g�m�[�h���쐬
        Dim objRootNode As IXMLDOMNode
        Set objRootNode = xmlDoc.createNode(NODE_ELEMENT, "Root", "")
        objRootNode.appendChild xmlDoc.createTextNode(vbNewLine)
        
        '�@��ݒ�̐������AKishu�m�[�h���쐬���Ȃ���Root�m�[�h�ɒǉ�����
        For i = 0 To m_kishuSet.Count - 1
            Dim objKishuNode As IXMLDOMNode
            Set objKishuNode = xmlDoc.createNode(NODE_ELEMENT, "Kishu", "")
            
            'Root/Kishu/KishuNo���쐬
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "KishuNo", "")
            objNode.Text = CStr(m_kishuSet.Items(i).KishuNo)
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objKishuNode.appendChild(objNode)
            
            'Root/Kishu/KishuName���쐬
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "KishuName", "")
            objNode.Text = m_kishuSet.Items(i).KishuName
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objKishuNode.appendChild(objNode)
            
            'Root/Kishu/TypeNo���쐬
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "TypeNo", "")
            objNode.Text = CStr(m_kishuSet.Items(i).TypeNo)
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objKishuNode.appendChild(objNode)
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab))
            
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbTab))
            Call objRootNode.appendChild(objKishuNode)
            
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbNewLine))
        Next
        
        '�^�C�v�ݒ�̐������AType�m�[�h���쐬���Ȃ���Root�m�[�h�ɒǉ�����
        For i = 0 To m_typeSet.Count - 1
            Dim objTypeNode As IXMLDOMNode
            Set objTypeNode = xmlDoc.createNode(NODE_ELEMENT, "Type", "")
            
            'Root/Type/TypeNo���쐬
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "TypeNo", "")
            objNode.Text = CStr(m_typeSet.Items(i).TypeNo)
            Call objTypeNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objTypeNode.appendChild(objNode)
            
            'Root/Type/TypeName���쐬
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "TypeName", "")
            objNode.Text = m_typeSet.Items(i).TypeName
            Call objTypeNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objTypeNode.appendChild(objNode)
            Call objTypeNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab))
            
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbTab))
            Call objRootNode.appendChild(objTypeNode)
        
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbNewLine))
        Next
        
        '���[�g�m�[�h���h�L�������g�ɒǉ�
        xmlDoc.appendChild objRootNode
        
        '�擪��XML�錾��ǉ�����
        xmlDoc.insertBefore xmlDoc.createProcessingInstruction("xml", "version=""1.0"" encoding=""UTF-8"""), xmlDoc.childNodes.Item(0)
        
        '�ۑ�����
        On Error Resume Next
        xmlDoc.save xmlFile
        If Err.Number = 0 Then
            SaveFile = True
        Else
            SaveFile = False
            errMsg = "���s���G���[ '" & Err.Number & " (" & Right("00000000" & Hex(Err.Number), 8) & ")':" & vbNewLine
            errMsg = errMsg & vbNewLine
            errMsg = errMsg & Err.Description
        End If
        On Error GoTo 0
        
    Loop While False
    
    'Finally��������
    Set xmlDoc = Nothing
End Function