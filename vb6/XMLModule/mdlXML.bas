Attribute VB_Name = "mdlXML"
Option Explicit

'----------------------------------------------------------------------
'�ЂƂ�̊K�w�̃^�O��(XPath)��Ԃ�
'
Private Function XMLGetParentTagName(ByRef tagName As String) As String
    Dim slashPos As Long
    
    slashPos = InStrRev(tagName, "/")
    If slashPos > 0 Then
        XMLGetParentTagName = Left(tagName, slashPos - 1)
    Else
        XMLGetParentTagName = ""
    End If
End Function

'----------------------------------------------------------------------
'�^�O��(XPath)�̍Ō�̃^�O��Ԃ�
'
Private Function XMLGetTagName(ByRef tagName As String) As String
    Dim slashPos As Long
    
    slashPos = InStrRev(tagName, "/")
    If slashPos > 0 Then
        XMLGetTagName = Mid(tagName, slashPos + 1)
    Else
        XMLGetTagName = tagName
    End If
End Function

'----------------------------------------------------------------------
'�m�[�h���쐬����
'
Private Function XMLCreateNode(ByRef tagName As String, ByRef xmlDoc As DOMDocument) As IXMLDOMNode
    Dim objNodeList As IXMLDOMNodeList
    
    '�w�肳�ꂽ�^�O������΁A�쐬���邱�ƂȂ��m�[�h��Ԃ�
    Set objNodeList = xmlDoc.getElementsByTagName(tagName)
    If objNodeList.length > 0 Then
        Set XMLCreateNode = objNodeList.Item(0)
        Exit Function
    End If
    
    Dim parentTagName As String
    parentTagName = XMLGetParentTagName(tagName)
    
    '�w�肳�ꂽ�^�O�����[�g�^�O�̏ꍇ�́A�h�L�������g�̒����ɒǉ�����B
    If parentTagName = "" Then
        Set XMLCreateNode = xmlDoc.appendChild(xmlDoc.createNode(NODE_ELEMENT, XMLGetTagName(tagName), ""))
    Else
        '�e�̃^�O���쐬���āA����̃m�[�h���쐬����
        Set XMLCreateNode = XMLCreateNode(parentTagName, xmlDoc).appendChild(xmlDoc.createNode(NODE_ELEMENT, XMLGetTagName(tagName), ""))
    End If
End Function

'----------------------------------------------------------------------
'XML�t�@�C���ɁA���������������
'
Public Function XMLWriteText(ByRef tagName As String, ByRef newValue As String, ByRef xmlSource As String) As Boolean
    
    '�߂�l��[�G���[]�ɂ��Ă���
    XMLWriteText = False
    
    Dim xmlDoc As DOMDocument
    
    Set xmlDoc = New DOMDocument
    
    '�w��XML�t�@�C����ǂݍ��ށB
    xmlDoc.Load xmlSource
    
    '�t�@�C���������Ƃ��́AXML�錾��ǉ����Đ�ɐi�߂�
    If xmlDoc.parseError = &H800C0005 Then
        xmlDoc.appendChild xmlDoc.createProcessingInstruction("xml", "version=""1.0"" encoding=""UTF-8""")
    
    '�t�@�C���������ȊO�̃G���[�̂Ƃ��́A�G���[�I��
    ElseIf xmlDoc.parseError.errorCode <> 0 Then
    
        '�ǂݍ��݃G���[�̂Ƃ��́A�ڍׂ��G���[�I�u�W�F�N�g�ɃZ�b�g���ďI��
        With xmlDoc.parseError
            Err.Number = .errorCode
            Err.Description = Replace("line " & .Line & ", pos " & .linepos & ", " & .reason, vbNewLine, "")
            Err.Source = .srcText
        End With
        
        Exit Function
    End If
    
    '�m�[�h���쐬���Ēl����������
    XMLCreateNode(tagName, xmlDoc).Text = newValue
    
    '�m�[�h��ݒ肷��
    xmlDoc.save xmlSource
    
    '����I��
    XMLWriteText = True
    
End Function

'----------------------------------------------------------------------
'XML�t�@�C������A�������ǂݍ���
'
Public Function XMLReadText(ByRef tagName As String, ByRef defaultValue As String, ByRef xmlSource As String) As String
    
    Dim xmlDoc As DOMDocument
    Dim objNodeList As IXMLDOMNodeList
    
    Set xmlDoc = New DOMDocument
    
    '�߂�l���f�t�H���g������ɂ��Ă���
    XMLReadText = defaultValue
    
    'XML�t�@�C����ǂݍ���
    If xmlDoc.Load(xmlSource) Then
    
        '�w��^�O�̃e�L�X�g��߂�l�ɂ���B�m�[�h�������L��Ƃ��́A�擪�̃m�[�h���g�p�B
        Set objNodeList = xmlDoc.getElementsByTagName(tagName)
        If objNodeList.length > 0 Then
            XMLReadText = objNodeList.Item(0).Text
        End If
    
        '�O�̂��߃G���[��Ԃ̓N���A���Ă���
        Err.Clear
    Else
    
        '�ǂݍ��݃G���[�̂Ƃ��́A�ڍׂ��G���[�I�u�W�F�N�g�ɃZ�b�g
        With xmlDoc.parseError
            Err.Number = .errorCode
            Err.Description = Replace("line " & .Line & ", pos " & .linepos & ", " & .reason, vbNewLine, "")
            Err.Source = .srcText
        End With
    End If

End Function

