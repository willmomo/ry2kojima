VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   660
      TabIndex        =   1
      Top             =   1620
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   660
      TabIndex        =   0
      Top             =   420
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


'----------------------------------------------------------------------
' ���`���ꂽXML���쐬�����(�m�[�h�𑷂���\�z���Ă���)
'----------------------------------------------------------------------
Private Sub Command1_Click()

    Dim xmlOut As New DOMDocument
    Dim xmlRoot As IXMLDOMNode
    Dim xmlChild As IXMLDOMNode
    Dim xmlGrandchild As IXMLDOMNode
    
    'XML�錾
    xmlOut.appendChild xmlOut.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'")
    
    '���m�[�h���쐬
    Set xmlGrandchild = xmlOut.createNode(NODE_ELEMENT, "grandchild", "")
    xmlGrandchild.Text = "��"
    
    '�q�m�[�h���쐬�B�O��ɉ��s�ƃ^�u��������������
    Set xmlChild = xmlOut.createNode(NODE_ELEMENT, "child", "")
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(String(2, vbTab))
    xmlChild.appendChild xmlGrandchild
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(vbTab)
    
    '���[�g�m�[�h���쐬�B�O��ɉ��s�ƃ^�u�������q������
    Set xmlRoot = xmlOut.createNode(NODE_ELEMENT, "root", "")
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    xmlRoot.appendChild xmlOut.createTextNode(vbTab)
    xmlRoot.appendChild xmlChild
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    
    '�h�L�������g�Ƀ��[�g�m�[�h������
    xmlOut.appendChild xmlRoot
    
    '�ۑ�
    xmlOut.save "hoge.xml"
    
End Sub


'----------------------------------------------------------------------
' ���`���ꂽXML���쐬�����(�m�[�h�����[�g����\�z���Ă���)
'----------------------------------------------------------------------
Private Sub Command2_Click()

    Dim xmlOut As New DOMDocument
    Dim xmlRoot As IXMLDOMNode
    Dim xmlChild As IXMLDOMNode
    Dim xmlGrandchild As IXMLDOMNode
    
    'XML�錾
    xmlOut.appendChild xmlOut.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'")
    
    '���[�g�m�[�h���쐬
    Set xmlRoot = xmlOut.appendChild(xmlOut.createNode(NODE_ELEMENT, "root", ""))
    
    '�O��ɉ��s�ƃ^�u�������q�m�[�h���쐬
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    xmlRoot.appendChild xmlOut.createTextNode(vbTab)
    Set xmlChild = xmlRoot.appendChild(xmlOut.createNode(NODE_ELEMENT, "child", ""))
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    
    '�O��ɉ��s�ƃ^�u���������m�[�h���쐬
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(String(2, vbTab))
    Set xmlGrandchild = xmlChild.appendChild(xmlOut.createNode(NODE_ELEMENT, "grandchild", ""))
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(vbTab)
    
    '���m�[�h�̒l��ݒ�
    xmlGrandchild.Text = "��"
    
    '�ۑ�
    xmlOut.save "hoge2.xml"

End Sub
