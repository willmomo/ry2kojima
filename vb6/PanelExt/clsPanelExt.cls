VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPanelExt"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' [概要]
' XMLに書かれた内容を指定されたDCに描画する。
'----------------------------------------------------------------------

Option Explicit

Private Declare Function SetRect Lib "user32" ( _
    lpRect As RECT, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long _
) As Long

Private Declare Function GetPrivateProfileString Lib "kernel32" _
    Alias "GetPrivateProfileStringA" ( _
        ByVal lpApplicationName As String, _
        ByVal lpKeyName As Any, _
        ByVal lpDefault As String, _
        ByVal lpReturnedString As String, _
        ByVal nSize As Long, _
        ByVal lpFileName As String _
) As Long

Private Declare Function GetPrivateProfileSection Lib "kernel32" _
    Alias "GetPrivateProfileSectionA" ( _
        ByVal lpAppName As String, _
        ByVal lpReturnedString As String, _
        ByVal nSize As Long, _
        ByVal lpFileName As String _
) As Long

Private Declare Function FrameRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long

Private Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long

Private Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long

Private Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

Private Declare Function MoveToEx Lib "gdi32" ( _
    ByVal hdc As Long, _
    ByVal x As Long, _
    ByVal y As Long, _
    lpPoint As POINTAPI _
) As Long

Private Declare Function LineTo Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long) As Long

Private Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long

Private Declare Function CreatePen Lib "gdi32" ( _
    ByVal nPenStyle As Long, _
    ByVal nWidth As Long, _
    ByVal crColor As Long _
) As Long

Private Type POINTAPI
    x As Long
    y As Long
End Type

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Const PS_SOLID = 0

'----------------------------------------------------------------------
' メンバー変数

Private m_xmlDoc As DOMDocument
Private m_cmdNodes As IXMLDOMNodeList
Private m_fname As String

'----------------------------------------------------------------------
' [概要]
' XMLファイルの読み込み
'
' [引数]
' fname     読み込むファイル名
' errMsg    エラー発生時のエラー内容
'
' [備考]
' 指定されたファイルを読み込み。ここで指定したファイルは、
' ReLoadFileメソッドで再読込み可能。
'----------------------------------------------------------------------

Public Function LoadFile(ByRef fname As String, errMsg As String) As Boolean
    On Error GoTo Err_LoadFile
    
    'ファイル名を保存
    m_fname = fname
    
    'XMLファイルとして読み込み
    LoadFile = m_xmlDoc.Load(fname)
    If LoadFile = False Then
        errMsg = m_xmlDoc.parseError.reason
        errMsg = errMsg & "リソース '" & m_xmlDoc.parseError.url & "' の実行エラーです。ライン " & m_xmlDoc.parseError.Line & "、位置 " & m_xmlDoc.parseError.linepos
        If m_xmlDoc.parseError.linepos > 0 Then
            errMsg = errMsg & vbCrLf & vbCrLf & Replace(m_xmlDoc.parseError.srcText, Chr(9), " ") & vbCrLf & String(m_xmlDoc.parseError.linepos - 1, "-") & "^"
        End If
    Else
        errMsg = ""
    End If
    
    'commandノードリストを取得しておく
    Set m_cmdNodes = m_xmlDoc.getElementsByTagName("panelex/command")
    
Exit_LoadFile:
    Exit Function
    
Err_LoadFile:
    LoadFile = False
    Resume Exit_LoadFile
    
End Function

'----------------------------------------------------------------------
' [概要]
' XMLファイルの再読み込み
'
' [引数]
' errMsg    エラー発生時のエラー内容
'
' [備考]
' LoadFileを実行した後に呼出可能。LoadFileで指定したファイルを
' 再読込みする。
'----------------------------------------------------------------------

Public Function ReLoadFile(errMsg As String) As Boolean
    On Error GoTo Err_ReLoadFile
    
    'XMLファイルの読み込み
    ReLoadFile = m_xmlDoc.Load(m_fname)
    If ReLoadFile = False Then
        errMsg = m_xmlDoc.parseError.reason
        errMsg = errMsg & "リソース '" & m_xmlDoc.parseError.url & "' の実行エラーです。ライン " & m_xmlDoc.parseError.Line & "、位置 " & m_xmlDoc.parseError.linepos
        If m_xmlDoc.parseError.linepos > 0 Then
            errMsg = errMsg & vbCrLf & vbCrLf & Replace(m_xmlDoc.parseError.srcText, Chr(9), " ") & vbCrLf & String(m_xmlDoc.parseError.linepos - 1, "-") & "^"
        End If
    Else
        errMsg = ""
    End If
    
    Set m_cmdNodes = m_xmlDoc.getElementsByTagName("panelex/command")
    
Exit_ReLoadFile:
    Exit Function
    
Err_ReLoadFile:
    ReLoadFile = False
    Resume Exit_ReLoadFile
    
End Function

'----------------------------------------------------------------------
' [概要]
' DCに描画
'
' [引数]
' hdc   描画されるDCへのハンドル
'
' [備考]
' commandノードリストに書かれた内容をDCに描画する
'----------------------------------------------------------------------

Public Function Draw(ByVal hdc As Long) As Boolean
    On Error GoTo Err_Draw
    
    Dim i As Long
    Dim e As IXMLDOMElement
    Dim rc As RECT
    Dim hbr As Long
    Dim pt As POINTAPI
    Dim hpen As Long
    Dim hold As Long
    
    For i = 0 To m_cmdNodes.length - 1
        Set e = m_cmdNodes.Item(i)
        
        Select Case UCase(e.getElementsByTagName("name").Item(0).Text)
        Case "LINE"
            'LINEコマンド
            SetRect rc, e.getElementsByTagName("rect/left").Item(0).Text, e.getElementsByTagName("rect/top").Item(0).Text, _
                e.getElementsByTagName("rect/right").Item(0).Text, e.getElementsByTagName("rect/bottom").Item(0).Text
            
            MoveToEx hdc, rc.Left, rc.Top, pt
            
            hpen = CreatePen(PS_SOLID, 0, RGB(e.getElementsByTagName("color/R").Item(0).Text, _
                e.getElementsByTagName("color/G").Item(0).Text, e.getElementsByTagName("color/B").Item(0).Text))
            
            hold = SelectObject(hdc, hpen)
            LineTo hdc, rc.Right, rc.Bottom
            SelectObject hdc, hold
            
            DeleteObject hbr
        
        Case "BOX"
            'BOXコマンド
            SetRect rc, e.getElementsByTagName("rect/left").Item(0).Text, e.getElementsByTagName("rect/top").Item(0).Text, _
                e.getElementsByTagName("rect/right").Item(0).Text, e.getElementsByTagName("rect/bottom").Item(0).Text
            
            hbr = CreateSolidBrush(RGB(e.getElementsByTagName("color/R").Item(0).Text, _
                e.getElementsByTagName("color/G").Item(0).Text, e.getElementsByTagName("color/B").Item(0).Text))
                
            FrameRect hdc, rc, hbr
            DeleteObject hbr
            
        Case "FILL"
            'FILLコマンド
            SetRect rc, e.getElementsByTagName("rect/left").Item(0).Text, e.getElementsByTagName("rect/top").Item(0).Text, _
                e.getElementsByTagName("rect/right").Item(0).Text, e.getElementsByTagName("rect/bottom").Item(0).Text
            
            hbr = CreateSolidBrush(RGB(e.getElementsByTagName("color/R").Item(0).Text, _
                e.getElementsByTagName("color/G").Item(0).Text, e.getElementsByTagName("color/B").Item(0).Text))
                
            FillRect hdc, rc, hbr
            DeleteObject hbr
        
        End Select
        
        Set e = Nothing
    Next
    
Exit_Draw:
    Exit Function
    
Err_Draw:
    Resume Exit_Draw
    
End Function

'----------------------------------------------------------------------
' [概要]
' コンストラクタ
'
' [備考]
' m_xmlDocのインスタンスを作成
'----------------------------------------------------------------------

Private Sub Class_Initialize()
    Set m_xmlDoc = New DOMDocument
End Sub

'----------------------------------------------------------------------
' [概要]
' デストラクタ
'
' [備考]
' m_xmlDocのインスタンスを破棄
'----------------------------------------------------------------------

Private Sub Class_Terminate()
    Set m_xmlDoc = Nothing
End Sub
