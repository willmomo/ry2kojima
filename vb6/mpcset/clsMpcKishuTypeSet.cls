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

'機種設定タグ
'<kishu>
'  <kishuno>機種番号(1～MAX_KISHU)</kishuno>
'  <kishuname>機種名(最大20バイト)</kishuname>
'  <typeno>タイプ番号(1～MAX_TYPE)</typeno>
'</kishu>

Private Type KISHU_TAG
    m_kishuNo As String
    m_kishuName As String
    m_typeNo As String
End Type

'タイプ設定タグ
'<type>
'  <typeno>タイプ番号(1～MAX_TYPE)</typeno>
'  <typename>タイプ名(最大20バイト)</typename>
'</type>

Private Type TYPE_TAG
    m_typeNo As String
    m_typeName As String
End Type

Private m_kishu() As KISHU_TAG
Private m_type() As TYPE_TAG

'----------------------------------------------------------------------
'配列を拡張する
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
'指定機種の機種名を取得する
'
Public Property Get KishuName(ByVal kishuNo As Long) As String
    '指定機種へのインデックスを取得
    '機種が見つからないときは、「不正なプロパティ配列インデックスです。」エラー
    Dim Index As Long
    Index = IndexOfKishu(kishuNo)
    If Index = -1 Then
        Err.Raise 381
    End If
    
    KishuName = m_kishu(Index).m_kishuName
End Property

'----------------------------------------------------------------------
'指定機種の機種名を設定する
'
Public Property Let KishuName(ByVal kishuNo As Long, ByRef newValue As String)
    '指定機種へのインデックスを取得
    '機種が見つからないときは、配列を拡張して追加する。
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
'指定された機種番号のデータを持つm_kishuへのIndexを返す
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
'指定されたタイプ番号のデータを持つm_typeへのIndexを返す
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
'子ノードから
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
'ファイルからMPC機種・タイプ設定を読み込む
'
Public Function LoadFile(ByRef xmlFile As String) As Boolean

    'XMLファイルを読み込む
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    If Not xmlDoc.Load(xmlFile) Then
        Set xmlDoc = Nothing
        LoadFile = False
        Exit Function
    End If
    
    '<kishu>タグからm_kishuを初期化する
    Dim kishuNodeList As IXMLDOMNodeList
    Set kishuNodeList = xmlDoc.getElementsByTagName("kishu")
    
    ReDim m_kishu(kishuNodeList.length - 1)
    
    Dim i As Long
    For i = 0 To kishuNodeList.length - 1
        m_kishu(i).kishuNo = GetNamedNode(kishuNodeList.Item(i).childNodes, "kishuno").Text
        m_kishu(i).KishuName = GetNamedNode(kishuNodeList.Item(i).childNodes, "kishuname").Text
        m_kishu(i).typeno = GetNamedNode(kishuNodeList.Item(i).childNodes, "typeno").Text
    Next
    
    '<type>タグからm_typeを初期化する
    Dim typeNodeList As IXMLDOMNodeList
    Set typeNodeList = xmlDoc.getElementsByTagName("type")
    
    ReDim m_type(typeNodeList.length - 1)
    
    For i = 0 To typeNodeList.length - 1
        m_type(i).typeno = GetNamedNode(typeNodeList.Item(i).childNodes, "typeno").Text
        m_type(i).typename = GetNamedNode(typeNodeList.Item(i).childNodes, "typename").Text
    Next
    
    LoadFile = True
End Function
