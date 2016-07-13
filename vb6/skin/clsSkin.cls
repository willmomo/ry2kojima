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
'   [概要]
'   スキンからFormのプロパティを取得
'
'   [備考]
'   優先順位は以下の通り:
'   1. 固有フォームのプロパティ
'   3. 汎用フォームのプロパティ

Public Function GetFormProperty(f As Form, strProp As String)
    If m_skin.documentElement Is Nothing Then GoTo Exit_GetFormProperty
    
    Dim node As IXMLDOMNode
    Dim attrNode As IXMLDOMNode
    
    '固有フォームを探す
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = TypeName(f) Then
            '固有フォームにプロパティが存在する?
            Set attrNode = node.Attributes.getNamedItem(strProp)
            If Not attrNode Is Nothing Then
                GetFormProperty = attrNode.Text
                Exit Function
            End If
        End If
    Next
    
    '汎用フォームを探す
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = "Form" Then
            '汎用フォームにプロパティが存在する?
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
'   [概要]
'   スキンからControlのプロパティを取得
'
'   [備考]
'   優先順位は以下の通り:
'   1. 固有フォームの固有コントロールのプロパティ
'   2. 固有フォームの汎用コントロールのプロパティ
'   3. 汎用フォームの固有コントロールのプロパティ
'   4. 汎用フォームの汎用コントロールのプロパティ
'   5. コントロールの現在のプロパティ

Public Function GetControlProperty(f As Form, c As Control, strProp As String)
    If m_skin.documentElement Is Nothing Then GoTo Exit_GetControlProperty
    
    Dim node As IXMLDOMNode
    Dim ctrlNode As IXMLDOMNode
    Dim attrNode As IXMLDOMNode
    
    '固有フォームを探す
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = TypeName(f) Then
            '固有フォームから固有コントロールを探す
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = c.Name Then
                    '固有フォームの固有コントロールにプロパティが存在する?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
            '固有フォームから汎用コントロールを探す
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = TypeName(c) Then
                    '固有フォームの汎用コントロールにプロパティが存在する?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
        End If
    Next
    
    '汎用フォームを探す
    For Each node In m_skin.documentElement.childNodes
        If node.nodeName = "Form" And node.Attributes.getNamedItem("Name").Text = "Form" Then
            '汎用フォームから固有コントロールを探す
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = c.Name Then
                    '汎用フォームの固有コントロールにプロパティが存在する?
                    Set attrNode = ctrlNode.Attributes.getNamedItem(strProp)
                    If Not attrNode Is Nothing Then
                        GetControlProperty = attrNode.Text
                        Exit Function
                    End If
                End If
            Next
            '汎用フォームから汎用コントロールを探す
            For Each ctrlNode In node.childNodes
                If ctrlNode.nodeName = TypeName(c) And ctrlNode.Attributes.getNamedItem("Name").Text = TypeName(c) Then
                    '汎用フォームの汎用コントロールにプロパティが存在する?
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
'   [概要]
'   スキンを適用する

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
'   [概要]
'   スキンを読み込む
'
'   [引数]
'   xmlSource   Skin定義のXMLファイル
'   errMsg      エラー発生時の文字列
'
'   [戻り値]
'   True=正常終了 False=異常終了

Public Function LoadSkin(xmlSource As String, errmsg As String) As Boolean
    LoadSkin = m_skin.Load(xmlSource)
    
    If LoadSkin = False Then
        errmsg = m_skin.parseError.reason
        errmsg = errmsg & "リソース '" & m_skin.parseError.URL & "' の実行エラーです。ライン " & m_skin.parseError.Line & "、位置 " & m_skin.parseError.linepos
        If m_skin.parseError.linepos > 0 Then
            errmsg = errmsg & vbCrLf & vbCrLf & Replace(m_skin.parseError.srcText, Chr(9), " ") & vbCrLf & String(m_skin.parseError.linepos - 1, "-") & "^"
        End If
    Else
        errmsg = ""
    End If
End Function

'----------------------------------------------------------------------
'   [概要]
'   クラスの初期化

Private Sub Class_Initialize()
    Set m_skin = New DOMDocument
    Set m_def_property = New Dictionary
    
    'デフォルトプロパティを準備
    m_def_property("ForeColor") = &H0&
    m_def_property("AccentForeColor") = &H0&
    m_def_property("BackColor") = &H8000000F
    m_def_property("AccentBackColor") = &H8000000F
    m_def_property("CaptionColor") = &H8000000F
End Sub

'----------------------------------------------------------------------
'   [概要]
'   クラスの破棄処理

Private Sub Class_Terminate()
    Set m_def_property = Nothing
    Set m_skin = Nothing
End Sub
