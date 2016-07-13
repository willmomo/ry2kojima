VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsSkin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_def_property As Dictionary
Private m_skin As DOMDocument

'----------------------------------------------------------------------
'   [�T�v]
'   �X�L������Form�̃v���p�e�B���擾
'
'   [���l]
'   �D�揇�ʂ͈ȉ��̒ʂ�:
'   1. �ŗL�t�H�[���̃v���p�e�B
'   3. �ėp�t�H�[���̃v���p�e�B

Public Function GetFormProperty(f As Form, strProp As String)
    If m_skin.documentElement Is Nothing Then GoTo Exit_GetFormProperty
    
    Dim node As IXMLDOMNode
    Dim attrNode As IXMLDOMNode
    
    '�ŗL�t�H�[����T��
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = TypeName(f) Then
            '�ŗL�t�H�[���Ƀv���p�e�B�����݂���?
            Set attrNode = node.Attributes.getNamedItem(strProp)
            If Not attrNode Is Nothing Then
                GetFormProperty = attrNode.Text
                Exit Function
            End If
        End If
    Next
    
    '�ėp�t�H�[����T��
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = "Form" Then
            '�ėp�t�H�[���Ƀv���p�e�B�����݂���?
            Set attrNode = node.Attributes.getNamedItem(strProp)
            If Not attrNode Is Nothing Then
                GetFormProperty = attrNode.Text
                Exit Function
            End If
        End If
    Next
    
Exit_GetFormProperty:
    GetFormProperty = m_def_property(strProp)
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   �X�L������Control�̃v���p�e�B���擾
'
'   [���l]
'   �D�揇�ʂ͈ȉ��̒ʂ�:
'   1. �ŗL�t�H�[���̌ŗL�R���g���[���̃v���p�e�B
'   2. �ŗL�t�H�[���̔ėp�R���g���[���̃v���p�e�B
'   3. �ėp�t�H�[���̌ŗL�R���g���[���̃v���p�e�B
'   4. �ėp�t�H�[���̔ėp�R���g���[���̃v���p�e�B
'   5. �R���g���[���̌��݂̃v���p�e�B

Public Function GetControlProperty(f As Form, c As Control, strProp As String)
    If m_skin.documentElement Is Nothing Then GoTo Exit_GetControlProperty
    
    Dim node As IXMLDOMNode
    Dim ctrlNode As IXMLDOMNode
    Dim attrNode As IXMLDOMNode
    
    '�ŗL�t�H�[����T��
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = TypeName(f) Then
            '�ŗL�t�H�[������ŗL�R���g���[����T��
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = c.Name Then
                    '�ŗL�t�H�[���̌ŗL�R���g���[���Ƀv���p�e�B�����݂���?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
            '�ŗL�t�H�[������ėp�R���g���[����T��
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = TypeName(c) Then
                    '�ŗL�t�H�[���̔ėp�R���g���[���Ƀv���p�e�B�����݂���?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
        End If
    Next
    
    '�ėp�t�H�[����T��
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = "Form" Then
            '�ėp�t�H�[������ŗL�R���g���[����T��
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = c.Name Then
                    '�ėp�t�H�[���̌ŗL�R���g���[���Ƀv���p�e�B�����݂���?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
            '�ėp�t�H�[������ėp�R���g���[����T��
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = TypeName(c) Then
                    '�ėp�t�H�[���̔ėp�R���g���[���Ƀv���p�e�B�����݂���?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
        End If
    Next
    
Exit_GetControlProperty:
    GetControlProperty = m_def_property(strProp)
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   �X�L����K�p����

Public Sub Apply(f As Form)
    If m_skin.documentElement Is Nothing Then GoTo Exit_Apply
    
    f.BackColor = GetFormProperty(f, "BackColor")
    
    Dim c As Control
    For Each c In f.Controls
        Select Case TypeName(c)
        Case "Label", "SSFrame", "SSActiveTabs", "SSRibbon", "SSCommand", "Frame"
            c.ForeColor = GetControlProperty(f, c, "ForeColor")
            c.BackColor = GetControlProperty(f, c, "BackColor")
        Case "xLed"
            c.CaptionColor = GetControlProperty(f, c, "CaptionColor")
            c.BackColor = GetControlProperty(f, c, "BackColor")
        Case "imNumLite"
            c.Caption.ForeColor = GetControlProperty(f, c, "CaptionForeColor")
            c.Caption.BackColor = GetControlProperty(f, c, "CaptionBackColor")
        Case Else
            'Debug.Print TypeName(c)
        End Select
    Next
    
Exit_Apply:

End Sub

'----------------------------------------------------------------------
'   [�T�v]
'   �X�L����ǂݍ���
'
'   [����]
'   xmlSource   Skin��`��XML�t�@�C��
'   errMsg      �G���[�������̕�����
'
'   [�߂�l]
'   True=����I�� False=�ُ�I��

Public Function LoadSkin(xmlSource As String, errmsg As String) As Boolean
    LoadSkin = m_skin.Load(xmlSource)
    
    If LoadSkin = False Then
        errmsg = m_skin.parseError.reason
        errmsg = errmsg & "���\�[�X '" & m_skin.parseError.URL & "' �̎��s�G���[�ł��B���C�� " & m_skin.parseError.Line & "�A�ʒu " & m_skin.parseError.linepos
        If m_skin.parseError.linepos > 0 Then
            errmsg = errmsg & vbCrLf & vbCrLf & Replace(m_skin.parseError.srcText, Chr(9), " ") & vbCrLf & String(m_skin.parseError.linepos - 1, "-") & "^"
        End If
    Else
        errmsg = ""
    End If
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   �N���X�̏�����

Private Sub Class_Initialize()
    Set m_skin = New DOMDocument
    Set m_def_property = New Dictionary
    
    '�f�t�H���g�v���p�e�B������
    m_def_property("ForeColor") = &H0&
    m_def_property("AccentForeColor") = &H0&
    m_def_property("BackColor") = &H8000000F
    m_def_property("AccentBackColor") = &H8000000F
    m_def_property("CaptionColor") = &H8000000F
End Sub

'----------------------------------------------------------------------
'   [�T�v]
'   �N���X�̔j������

Private Sub Class_Terminate()
    Set m_def_property = Nothing
    Set m_skin = Nothing
End Sub