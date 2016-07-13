Attribute VB_Name = "mdlXML"
Option Explicit

'----------------------------------------------------------------------
'ひとつ上の階層のタグ名(XPath)を返す
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
'タグ名(XPath)の最後のタグを返す
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
'ノードを作成する
'
Private Function XMLCreateNode(ByRef tagName As String, ByRef xmlDoc As DOMDocument) As IXMLDOMNode
    Dim objNodeList As IXMLDOMNodeList
    
    '指定されたタグがあれば、作成することなくノードを返す
    Set objNodeList = xmlDoc.getElementsByTagName(tagName)
    If objNodeList.length > 0 Then
        Set XMLCreateNode = objNodeList.Item(0)
        Exit Function
    End If
    
    Dim parentTagName As String
    parentTagName = XMLGetParentTagName(tagName)
    
    '指定されたタグがルートタグの場合は、ドキュメントの直下に追加する。
    If parentTagName = "" Then
        Set XMLCreateNode = xmlDoc.appendChild(xmlDoc.createNode(NODE_ELEMENT, XMLGetTagName(tagName), ""))
    Else
        '親のタグを作成して、今回のノードを作成する
        Set XMLCreateNode = XMLCreateNode(parentTagName, xmlDoc).appendChild(xmlDoc.createNode(NODE_ELEMENT, XMLGetTagName(tagName), ""))
    End If
End Function

'----------------------------------------------------------------------
'XMLファイルに、文字列を書き込む
'
Public Function XMLWriteText(ByRef tagName As String, ByRef newValue As String, ByRef xmlSource As String) As Boolean
    
    '戻り値を[エラー]にしておく
    XMLWriteText = False
    
    Dim xmlDoc As DOMDocument
    
    Set xmlDoc = New DOMDocument
    
    '指定XMLファイルを読み込む。
    xmlDoc.Load xmlSource
    
    'ファイルが無いときは、XML宣言を追加して先に進める
    If xmlDoc.parseError = &H800C0005 Then
        xmlDoc.appendChild xmlDoc.createProcessingInstruction("xml", "version=""1.0"" encoding=""UTF-8""")
    
    'ファイルが無い以外のエラーのときは、エラー終了
    ElseIf xmlDoc.parseError.errorCode <> 0 Then
    
        '読み込みエラーのときは、詳細をエラーオブジェクトにセットして終了
        With xmlDoc.parseError
            Err.Number = .errorCode
            Err.Description = Replace("line " & .Line & ", pos " & .linepos & ", " & .reason, vbNewLine, "")
            Err.Source = .srcText
        End With
        
        Exit Function
    End If
    
    'ノードを作成して値を書き込む
    XMLCreateNode(tagName, xmlDoc).Text = newValue
    
    'ノードを設定する
    xmlDoc.save xmlSource
    
    '正常終了
    XMLWriteText = True
    
End Function

'----------------------------------------------------------------------
'XMLファイルから、文字列を読み込む
'
Public Function XMLReadText(ByRef tagName As String, ByRef defaultValue As String, ByRef xmlSource As String) As String
    
    Dim xmlDoc As DOMDocument
    Dim objNodeList As IXMLDOMNodeList
    
    Set xmlDoc = New DOMDocument
    
    '戻り値をデフォルト文字列にしておく
    XMLReadText = defaultValue
    
    'XMLファイルを読み込む
    If xmlDoc.Load(xmlSource) Then
    
        '指定タグのテキストを戻り値にする。ノードが複数有るときは、先頭のノードを使用。
        Set objNodeList = xmlDoc.getElementsByTagName(tagName)
        If objNodeList.length > 0 Then
            XMLReadText = objNodeList.Item(0).Text
        End If
    
        '念のためエラー状態はクリアしておく
        Err.Clear
    Else
    
        '読み込みエラーのときは、詳細をエラーオブジェクトにセット
        With xmlDoc.parseError
            Err.Number = .errorCode
            Err.Description = Replace("line " & .Line & ", pos " & .linepos & ", " & .reason, vbNewLine, "")
            Err.Source = .srcText
        End With
    End If

End Function

