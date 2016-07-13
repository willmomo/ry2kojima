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
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   1800
      TabIndex        =   0
      Top             =   1320
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const xml_source = "D:\home\ebichu\Projects\xml_test\datareq.xml"

Private Sub Command1_Click()
    Dim xdoc As DOMDocument
    Set xdoc = New DOMDocument
    
    xdoc.appendChild xdoc.createNode(NODE_ELEMENT, "応答", "")
    xdoc.getElementsByTagName("応答").Item(0).appendChild _
        xdoc.createNode(NODE_ELEMENT, "データリスト", "")
    xdoc.getElementsByTagName("データリスト").Item(0).appendChild _
        xdoc.createNode(NODE_ELEMENT, "データ", "")
    
    Debug.Print xdoc.xml
End Sub

Private Sub Form_Load()
    Dim xdoc As DOMDocument
    Set xdoc = New DOMDocument
    
    Dim ret As Boolean
    ret = xdoc.Load(xml_source)
    Debug.Print xdoc.xml
    
    If ret = True Then
        Dim xnode As IXMLDOMNodeList
        
'        Debug.Print xdoc.getElementsByTagName("開始番号").Item(0).Text
'        Debug.Print xdoc.getElementsByTagName("終了番号").Item(0).Text
'        Debug.Print xdoc.getElementsByTagName("番号").Item(0).Attributes.getNamedItem("type").Text
    Else
        MsgBox xml_source & "のロードに失敗しました。"
    End If
End Sub
