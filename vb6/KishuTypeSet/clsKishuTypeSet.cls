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
'このクラスを使うためには、参照設定で
'  Microsoft XML, version 2.0 (msxml.dll)
'  Microsoft Scripting Runtime (scrrun.dll)
'を選択する必要があります｡
'**********************************************************************
Option Explicit


Private m_kishuSet As New Dictionary    '機種設定
Private m_typeSet As New Dictionary     'タイプ設定


'**********************************************************************
'ノードリストから、指定タグのノードのテキストを返す(ユーティリティ関数)
'
'[備考]
'ノードが複数あるときは、最初に見つけたのノードのテキスト
'見つからないときは、空文字列を返す
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
'ノードリストから、指定タグのノードのテキストを数値にして返す(ユーティリティ関数)
'
'[備考]
'ノードが複数あるときは、最初に見つけたのノードのテキスト
'見つからないときは、0を返す
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
'指定機種の機種設定にアクセスする
'
'[備考]
'指定機種の設定が無いときは、空の設定を追加して、参照を返す
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
'指定タイプのタイプ設定にアクセスする
'
'[備考]
'指定タイプの設定が無いときは、空の設定を追加して、参照を返す
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
'指定機種の設定有無を調べる
'**********************************************************************
Public Property Get ExistsKishuSet(ByVal nKishuNo As Long) As Boolean
    ExistsKishuSet = m_kishuSet.Exists(nKishuNo)
End Property


'**********************************************************************
'指定タイプの設定有無を調べる
'**********************************************************************
Public Property Get ExistsTypeSet(ByVal nTypeNo As Long) As Boolean
    ExistsTypeSet = m_typeSet.Exists(nTypeNo)
End Property


'**********************************************************************
'XML(DOMDocument)から機種設定を読み込む
'**********************************************************************
Private Sub LoadKishuSet(ByRef doc As DOMDocument)
    
    '現在の機種設定をクリアする
    m_kishuSet.RemoveAll
    
    '機種設定の読み込み
    Dim nodeList As IXMLDOMNodeList
    Set nodeList = doc.getElementsByTagName("Root/Kishu")
    
    Dim i As Long
    For i = 0 To nodeList.length - 1
    
        '機種設定オブジェクトを作成して機種設定を展開する
        Dim objKishuSet As clsKishuSet
        Set objKishuSet = New clsKishuSet
        
        objKishuSet.KishuNo = sub_GetNodeTextLong(nodeList.Item(i).childNodes, "KishuNo")
        objKishuSet.KishuName = sub_GetNodeText(nodeList.Item(i).childNodes, "KishuName")
        objKishuSet.TypeNo = sub_GetNodeTextLong(nodeList.Item(i).childNodes, "TypeNo")
        
        '機種設定辞書に機種番号と関連付けて登録する
        'すでに、関連付けられているときは、値を入れ替える
        If m_kishuSet.Exists(objKishuSet.KishuNo) Then
            Set m_kishuSet.Item(objKishuSet.KishuNo) = objKishuSet
        Else
            m_kishuSet.Add objKishuSet.KishuNo, objKishuSet
        End If
    Next
    
End Sub


'**********************************************************************
'XML(DOMDocument)からタイプ設定を読み込む
'**********************************************************************
Private Sub LoadTypeSet(ByRef doc As DOMDocument)

    '現在のタイプ設定をクリアする
    m_typeSet.RemoveAll
    
    'タイプ設定の読み込み
    Dim nodeList As IXMLDOMNodeList
    Set nodeList = doc.getElementsByTagName("Root/Type")
    
    Dim i As Long
    For i = 0 To nodeList.length - 1
    
        'タイプ設定オブジェクトを作成してタイプ設定を展開する
        Dim objTypeSet As clsTypeSet
        Set objTypeSet = New clsTypeSet
        
        objTypeSet.TypeNo = sub_GetNodeTextLong(nodeList.Item(i).childNodes, "TypeNo")
        objTypeSet.TypeName = sub_GetNodeText(nodeList.Item(i).childNodes, "TypeName")
        
        'タイプ設定辞書にタイプ番号と関連付けて登録する
        If m_typeSet.Exists(objTypeSet.TypeNo) Then
            Set m_typeSet.Item(objTypeSet.TypeNo) = objTypeSet
        Else
            m_typeSet.Add objTypeSet.TypeNo, objTypeSet
        End If
    Next
    
End Sub


'**********************************************************************
'ファイルから設定を読み込む
'
'[備考]
'XML的に読み込めないときは、エラーになる。論理的なエラーはエラーにならない。
'**********************************************************************
Public Function LoadFile(xmlFile As String, errMsg As String) As Boolean
    
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    Do
        If Not xmlDoc.Load(xmlFile) Then
        
            'エラーメッセージを作成してエラー終了
            errMsg = _
                xmlDoc.parseError.reason & _
                "リソース '" & xmlDoc.parseError.url & "' の実行エラーです。" & _
                "ライン " & xmlDoc.parseError.Line & "、位置 " & xmlDoc.parseError.linepos & vbNewLine & _
                vbNewLine & _
                xmlDoc.parseError.srcText & vbNewLine & _
                String(xmlDoc.parseError.linepos - 1, " ") & "^"
                
            LoadFile = False
            Exit Do
        End If
            
        '機種設定の読み込み
        LoadKishuSet xmlDoc
        
        'タイプ設定の読み込み
        LoadTypeSet xmlDoc
    
        LoadFile = True
    Loop While False
    
    'Finally相当処理
    Set xmlDoc = Nothing
End Function


'**********************************************************************
'ファイルに設定を書き込む
'
'[備考]
'createTextNodeは、エディタで見やすいように整形するためで、本来は不要
'**********************************************************************
Public Function SaveFile(xmlFile As String, errMsg As String) As Boolean
    Dim i As Long
    Dim objNode As IXMLDOMNode
    
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    Do
        'ルートノードを作成
        Dim objRootNode As IXMLDOMNode
        Set objRootNode = xmlDoc.createNode(NODE_ELEMENT, "Root", "")
        objRootNode.appendChild xmlDoc.createTextNode(vbNewLine)
        
        '機種設定の数だけ、Kishuノードを作成しながらRootノードに追加する
        For i = 0 To m_kishuSet.Count - 1
            Dim objKishuNode As IXMLDOMNode
            Set objKishuNode = xmlDoc.createNode(NODE_ELEMENT, "Kishu", "")
            
            'Root/Kishu/KishuNoを作成
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "KishuNo", "")
            objNode.Text = CStr(m_kishuSet.Items(i).KishuNo)
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objKishuNode.appendChild(objNode)
            
            'Root/Kishu/KishuNameを作成
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "KishuName", "")
            objNode.Text = m_kishuSet.Items(i).KishuName
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objKishuNode.appendChild(objNode)
            
            'Root/Kishu/TypeNoを作成
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "TypeNo", "")
            objNode.Text = CStr(m_kishuSet.Items(i).TypeNo)
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objKishuNode.appendChild(objNode)
            Call objKishuNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab))
            
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbTab))
            Call objRootNode.appendChild(objKishuNode)
            
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbNewLine))
        Next
        
        'タイプ設定の数だけ、Typeノードを作成しながらRootノードに追加する
        For i = 0 To m_typeSet.Count - 1
            Dim objTypeNode As IXMLDOMNode
            Set objTypeNode = xmlDoc.createNode(NODE_ELEMENT, "Type", "")
            
            'Root/Type/TypeNoを作成
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "TypeNo", "")
            objNode.Text = CStr(m_typeSet.Items(i).TypeNo)
            Call objTypeNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objTypeNode.appendChild(objNode)
            
            'Root/Type/TypeNameを作成
            Set objNode = xmlDoc.createNode(NODE_ELEMENT, "TypeName", "")
            objNode.Text = m_typeSet.Items(i).TypeName
            Call objTypeNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab & vbTab))
            Call objTypeNode.appendChild(objNode)
            Call objTypeNode.appendChild(xmlDoc.createTextNode(vbNewLine & vbTab))
            
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbTab))
            Call objRootNode.appendChild(objTypeNode)
        
            Call objRootNode.appendChild(xmlDoc.createTextNode(vbNewLine))
        Next
        
        'ルートノードをドキュメントに追加
        xmlDoc.appendChild objRootNode
        
        '先頭にXML宣言を追加する
        xmlDoc.insertBefore xmlDoc.createProcessingInstruction("xml", "version=""1.0"" encoding=""UTF-8"""), xmlDoc.childNodes.Item(0)
        
        '保存する
        On Error Resume Next
        xmlDoc.save xmlFile
        If Err.Number = 0 Then
            SaveFile = True
        Else
            SaveFile = False
            errMsg = "実行時エラー '" & Err.Number & " (" & Right("00000000" & Hex(Err.Number), 8) & ")':" & vbNewLine
            errMsg = errMsg & vbNewLine
            errMsg = errMsg & Err.Description
        End If
        On Error GoTo 0
        
    Loop While False
    
    'Finally相当処理
    Set xmlDoc = Nothing
End Function
