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
   StartUpPosition =   3  'Windows の既定値
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
' 整形されたXMLを作成する例(ノードを孫から構築していく)
'----------------------------------------------------------------------
Private Sub Command1_Click()

    Dim xmlOut As New DOMDocument
    Dim xmlRoot As IXMLDOMNode
    Dim xmlChild As IXMLDOMNode
    Dim xmlGrandchild As IXMLDOMNode
    
    'XML宣言
    xmlOut.appendChild xmlOut.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'")
    
    '孫ノードを作成
    Set xmlGrandchild = xmlOut.createNode(NODE_ELEMENT, "grandchild", "")
    xmlGrandchild.Text = "孫"
    
    '子ノードを作成。前後に改行とタブを加えつつ孫を結合
    Set xmlChild = xmlOut.createNode(NODE_ELEMENT, "child", "")
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(String(2, vbTab))
    xmlChild.appendChild xmlGrandchild
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(vbTab)
    
    'ルートノードを作成。前後に改行とタブを加えつつ子を結合
    Set xmlRoot = xmlOut.createNode(NODE_ELEMENT, "root", "")
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    xmlRoot.appendChild xmlOut.createTextNode(vbTab)
    xmlRoot.appendChild xmlChild
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    
    'ドキュメントにルートノードを結合
    xmlOut.appendChild xmlRoot
    
    '保存
    xmlOut.save "hoge.xml"
    
End Sub


'----------------------------------------------------------------------
' 整形されたXMLを作成する例(ノードをルートから構築していく)
'----------------------------------------------------------------------
Private Sub Command2_Click()

    Dim xmlOut As New DOMDocument
    Dim xmlRoot As IXMLDOMNode
    Dim xmlChild As IXMLDOMNode
    Dim xmlGrandchild As IXMLDOMNode
    
    'XML宣言
    xmlOut.appendChild xmlOut.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'")
    
    'ルートノードを作成
    Set xmlRoot = xmlOut.appendChild(xmlOut.createNode(NODE_ELEMENT, "root", ""))
    
    '前後に改行とタブを加えつつ子ノードを作成
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    xmlRoot.appendChild xmlOut.createTextNode(vbTab)
    Set xmlChild = xmlRoot.appendChild(xmlOut.createNode(NODE_ELEMENT, "child", ""))
    xmlRoot.appendChild xmlOut.createTextNode(vbNewLine)
    
    '前後に改行とタブを加えつつ孫ノードを作成
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(String(2, vbTab))
    Set xmlGrandchild = xmlChild.appendChild(xmlOut.createNode(NODE_ELEMENT, "grandchild", ""))
    xmlChild.appendChild xmlOut.createTextNode(vbNewLine)
    xmlChild.appendChild xmlOut.createTextNode(vbTab)
    
    '孫ノードの値を設定
    xmlGrandchild.Text = "孫"
    
    '保存
    xmlOut.save "hoge2.xml"

End Sub
