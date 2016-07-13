VERSION 5.00
Begin VB.UserControl ucPanel 
   ClientHeight    =   5010
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6585
   ScaleHeight     =   5010
   ScaleWidth      =   6585
   Begin VB.PictureBox picBack 
      Height          =   4155
      Left            =   120
      ScaleHeight     =   4095
      ScaleWidth      =   5415
      TabIndex        =   0
      Top             =   120
      Width           =   5475
      Begin VB.PictureBox picBase 
         Height          =   2835
         Left            =   300
         ScaleHeight     =   2775
         ScaleWidth      =   3735
         TabIndex        =   1
         Top             =   240
         Width           =   3795
      End
   End
End
Attribute VB_Name = "ucPanel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'----------------------------------------------------------------------
' 内部で使用するためのWin32API
'----------------------------------------------------------------------
Private Const LF_FACESIZE = 32

Private Type LOGFONT
    lfHeight As Long
    lfWidth As Long
    lfEscapement As Long
    lfOrientation As Long
    lfWeight As Long
    lfItalic As Byte
    lfUnderline As Byte
    lfStrikeOut As Byte
    lfCharSet As Byte
    lfOutPrecision As Byte
    lfClipPrecision As Byte
    lfQuality As Byte
    lfPitchAndFamily As Byte
    lfFaceName(LF_FACESIZE - 1) As Byte
End Type

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Declare Function SetTextColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
Private Declare Function CreateCompatibleBitmap Lib "gdi32" (ByVal hdc As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Private Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
Private Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Private Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long
Private Declare Function GetStockObject Lib "gdi32" (ByVal nIndex As Long) As Long
Private Declare Function SetBkMode Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long
Private Declare Function DrawEdge Lib "user32" (ByVal hdc As Long, qrc As RECT, ByVal edge As Long, ByVal grfFlags As Long) As Long
Private Declare Function InflateRect Lib "user32" (lpRect As RECT, ByVal X As Long, ByVal Y As Long) As Long
Private Declare Function DrawText Lib "user32" Alias "DrawTextA" (ByVal hdc As Long, ByVal lpStr As String, ByVal nCount As Long, lpRect As RECT, ByVal wFormat As Long) As Long
Private Declare Function FrameRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long
Private Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long
Private Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Declare Function GetObject Lib "gdi32" Alias "GetObjectA" (ByVal hObject As Long, ByVal nCount As Long, lpObject As Any) As Long
Private Declare Function GetDeviceCaps Lib "gdi32" (ByVal hdc As Long, ByVal nIndex As Long) As Long
Private Declare Function CreateFontIndirect Lib "gdi32" Alias "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long
Private Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long
Private Declare Function PtInRect Lib "user32" (lpRect As RECT, ByVal X As Long, ByVal Y As Long) As Long

Private Const BF_LEFT = &H1
Private Const BF_TOP = &H2
Private Const BF_RIGHT = &H4
Private Const BF_BOTTOM = &H8

Private Const WHITE_BRUSH = 0
Private Const TRANSPARENT = 1
Private Const GRAY_BRUSH = 2
Private Const BDR_RAISED = &H5
Private Const BF_RECT = (BF_LEFT Or BF_TOP Or BF_RIGHT Or BF_BOTTOM)
Private Const BF_ADJUST = &H2000   ' Calculate the space left over.
Private Const DT_NOCLIP = &H100
Private Const DT_NOPREFIX = &H800
Private Const DT_CENTER = &H1
Private Const DT_VCENTER = &H4
Private Const DT_SINGLELINE = &H20
Private Const SRCCOPY = &HCC0020 ' (DWORD) dest = source
Private Const DEFAULT_GUI_FONT = 17
Private Const LOGPIXELSY = 90        '  Logical pixels/inch in Y
Private Const FW_BOLD = 700
Private Const FW_NORMAL = 400

'----------------------------------------------------------------------
' 内部変数
'----------------------------------------------------------------------
Private Type DAI_OBJECT
    hdb As Long '表示台番号
    rc As RECT  '表示する矩形
    
    m_select As Boolean '選択状態
    m_enable As Boolean '有効/無効状態
    
    m_CustomNormalColor As Long
    m_CustomNormalForeColor As Long
    m_CustomSelectedColor As Long
    m_CustomSelectedForeColor As Long
    m_CustomDisabledColor As Long
    m_CustomDisabledForeColor As Long
End Type

Private m_hdb2idx As New Dictionary
Private m_dais() As DAI_OBJECT  '台情報の可変長配列
Private m_rcBound As RECT       'パネル全体を表示するための矩形(picBaseと同じサイズになる)
Private m_scale As Long         '現在の拡大縮小率(default:100)
Private m_offsetX As Long       'パネルの表示位置オフセット
Private m_offsetY As Long       'パネルの表示位置オフセット
Private m_NormalDaiColor As OLE_COLOR      '通常状態のパネルの色
Private m_NormalDaiForeColor As OLE_COLOR  '通常状態の文字色
Private m_SelectedDaiColor As OLE_COLOR      '選択状態のパネルの色
Private m_SelectedDaiForeColor As OLE_COLOR  '選択状態の文字色
Private m_DisabledDaiColor As OLE_COLOR     '無効状態のパネルの色
Private m_DisabledDaiForeColor As OLE_COLOR '無効状態の文字色

'----------------------------------------------------------------------
' イベント宣言
'----------------------------------------------------------------------
Public Event Click()
Public Event DblClick()
Public Event KeyDown(KeyCode As Integer, Shift As Integer)
Public Event KeyPress(KeyAscii As Integer)
Public Event KeyUp(KeyCode As Integer, Shift As Integer)
Public Event MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Public Event MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Public Event MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
Public Event BeforeSelect(hdb As Long, Enabled As Boolean, Selected As Boolean, Cancel As Boolean)

'----------------------------------------------------------------------
' 台情報にアクセスするプロパティ
'----------------------------------------------------------------------
Public Property Get DaiSelected(ByVal hdb As Long) As Boolean
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiSelected = m_dais(m_hdb2idx(hdb)).m_select
End Property

Public Property Let DaiSelected(ByVal hdb As Long, ByVal newValue As Boolean)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_select = newValue
End Property

Public Property Get DaiEnabled(ByVal hdb As Long) As Boolean
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiEnabled = m_dais(m_hdb2idx(hdb)).m_enable
End Property

Public Property Let DaiEnabled(ByVal hdb As Long, ByVal newValue As Boolean)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_enable = newValue
End Property

Public Property Get DaiCusromNormalColor(ByVal hdb As Long) As Long
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiCusromNormalColor = m_dais(m_hdb2idx(hdb)).m_CustomNormalColor
End Property

Public Property Let DaiCusromNormalColor(ByVal hdb As Long, ByVal newValue As Long)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_CustomNormalColor = newValue
End Property

Public Property Get DaiCusromNormalForeColor(ByVal hdb As Long) As Long
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiCusromNormalForeColor = m_dais(m_hdb2idx(hdb)).m_CustomNormalForeColor
End Property

Public Property Let DaiCusromNormalForeColor(ByVal hdb As Long, ByVal newValue As Long)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_CustomNormalForeColor = newValue
End Property

Public Property Get DaiCustomSelectedColor(ByVal hdb As Long) As Long
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiCustomSelectedColor = m_dais(m_hdb2idx(hdb)).m_CustomSelectedColor
End Property

Public Property Let DaiCustomSelectedColor(ByVal hdb As Long, ByVal newValue As Long)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_CustomSelectedColor = newValue
End Property

Public Property Get DaiCustomSelectedForeColor(ByVal hdb As Long) As Long
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiCustomSelectedForeColor = m_dais(m_hdb2idx(hdb)).m_CustomSelectedForeColor
End Property

Public Property Let DaiCustomSelectedForeColor(ByVal hdb As Long, ByVal newValue As Long)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_CustomSelectedForeColor = newValue
End Property

Public Property Get DaiCustomDisabledColor(ByVal hdb As Long) As Long
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiCustomDisabledColor = m_dais(m_hdb2idx(hdb)).m_CustomDisabledColor
End Property

Public Property Let DaiCustomDisabledColor(ByVal hdb As Long, ByVal newValue As Long)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_CustomDisabledColor = newValue
End Property

Public Property Get DaiCustomDisabledForeColor(ByVal hdb As Long) As Long
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    DaiCustomDisabledForeColor = m_dais(m_hdb2idx(hdb)).m_CustomDisabledForeColor
End Property

Public Property Let DaiCustomDisabledForeColor(ByVal hdb As Long, ByVal newValue As Long)
    If Not m_hdb2idx.Exists(hdb) Then Err.Raise 9   'Indexエラー
    m_dais(m_hdb2idx(hdb)).m_CustomDisabledForeColor = newValue
End Property

'----------------------------------------------------------------------
' パネルに使う色情報
'----------------------------------------------------------------------
Public Property Get NormalDaiColor() As OLE_COLOR
    NormalDaiColor = m_NormalDaiColor
End Property

Public Property Let NormalDaiColor(ByVal newValue As OLE_COLOR)
    m_NormalDaiColor = newValue
End Property

Public Property Get NormalDaiForeColor() As OLE_COLOR
    NormalDaiForeColor = m_NormalDaiForeColor
End Property

Public Property Let NormalDaiForeColor(ByVal newValue As OLE_COLOR)
    m_NormalDaiForeColor = newValue
End Property

Public Property Get SelectedDaiColor() As OLE_COLOR
    SelectedDaiColor = m_SelectedDaiColor
End Property

Public Property Let SelectedDaiColor(ByVal newValue As OLE_COLOR)
    m_SelectedDaiColor = newValue
End Property

Public Property Get SelectedDaiForeColor() As OLE_COLOR
    SelectedDaiForeColor = m_SelectedDaiForeColor
End Property

Public Property Let SelectedDaiForeColor(ByVal newValue As OLE_COLOR)
    m_SelectedDaiForeColor = newValue
End Property

Public Property Get DisabledDaiColor() As OLE_COLOR
    DisabledDaiColor = m_DisabledDaiColor
End Property

Public Property Let DisabledDaiColor(ByVal newValue As OLE_COLOR)
    m_DisabledDaiColor = newValue
End Property

Public Property Get DisabledDaiForeColor() As OLE_COLOR
    DisabledDaiForeColor = m_DisabledDaiForeColor
End Property

Public Property Let DisabledDaiForeColor(ByVal newValue As OLE_COLOR)
    m_DisabledDaiForeColor = newValue
End Property

'----------------------------------------------------------------------
' フォント情報は、picBaseの物を使用、ばらして使用する。
'----------------------------------------------------------------------
Public Property Get FontName() As String
    FontName = picBase.Font.Name
End Property

Public Property Let FontName(ByVal newValue As String)
    picBase.Font.Name = newValue
End Property

Public Property Get FontBold() As Boolean
    FontBold = picBase.Font.Bold
End Property

Public Property Let FontBold(ByVal newValue As Boolean)
    picBase.Font.Bold = newValue
End Property

Public Property Get FontItalic() As Boolean
    FontItalic = picBase.Font.Italic
End Property

Public Property Let FontItalic(ByVal newValue As Boolean)
    picBase.Font.Italic = newValue
End Property

Public Property Get FontSize() As Long
    FontSize = picBase.Font.Size
End Property

Public Property Let FontSize(ByVal newValue As Long)
    picBase.Font.Size = newValue
End Property

Public Property Get FontStrikethrough() As Boolean
    FontStrikethrough = picBase.Font.Strikethrough
End Property

Public Property Let FontFontStrikethrough(ByVal newValue As Boolean)
    picBase.Font.Strikethrough = newValue
End Property

'----------------------------------------------------------------------
' オフセットX,YのGET/SET
'----------------------------------------------------------------------
Public Property Get OffsetX() As Long
    OffsetX = m_offsetX
End Property

Public Property Let OffsetX(ByVal newValue As Long)
    m_offsetX = newValue
    picBase.Left = m_offsetX * Screen.TwipsPerPixelX
End Property

Public Property Get OffsetY() As Long
    OffsetY = m_offsetY
End Property

Public Property Let OffsetY(ByVal newValue As Long)
    m_offsetY = newValue
    picBase.Top = m_offsetY * Screen.TwipsPerPixelY
End Property

'----------------------------------------------------------------------
' 拡大縮小率のGET/SET
'----------------------------------------------------------------------
Public Property Get PanelScale() As Long
    PanelScale = m_scale
End Property

Public Property Let PanelScale(ByVal newValue As Long)
    m_scale = newValue

    If GetWidth(m_rcBound) = 0 And GetHeight(m_rcBound) = 0 Then
        picBase.Width = picBack.ScaleWidth
        picBase.Height = picBack.ScaleHeight
    Else
        picBase.Width = GetWidth(m_rcBound) * Screen.TwipsPerPixelX * m_scale / 100
        picBase.Height = GetHeight(m_rcBound) * Screen.TwipsPerPixelY * m_scale / 100
    End If
End Property

'----------------------------------------------------------------------
' パネルのサイズGET
'----------------------------------------------------------------------
Public Property Get PanelWidth() As Long
    PanelWidth = GetWidth(m_rcBound) * Screen.TwipsPerPixelX
End Property

Public Property Get PanelHeight() As Long
    PanelHeight = GetHeight(m_rcBound) * Screen.TwipsPerPixelY
End Property

Public Property Get ViewWidth() As Long
    ViewWidth = picBack.ScaleWidth
End Property

Public Property Get ViewHeight() As Long
    ViewHeight = picBack.ScaleHeight
End Property

'----------------------------------------------------------------------
' 全ての台を選択状態にする
'----------------------------------------------------------------------
Public Sub SelectAll(ByVal flag As Boolean)
    Dim i As Long
    
    '台情報分ループして、選択しているものをリストに追加していく
    On Error Resume Next
    For i = 0 To UBound(m_dais)
        If Err.Number <> 0 Then Exit For
        
        If m_dais(i).m_enable = True Then
            m_dais(i).m_select = flag
        End If
    Next
End Sub

'----------------------------------------------------------------------
' 指定した台を選択状態にする
'----------------------------------------------------------------------
Public Sub SelecteDaiList(daiList() As Long, ByVal flag As Boolean)
    Dim i As Long
    Dim idx As Long
    
    For i = 0 To UBound(daiList)
        If m_hdb2idx.Exists(daiList(i)) Then
            idx = m_hdb2idx(daiList(i))
            
            If m_dais(idx).m_enable = True Then
                m_dais(idx).m_select = flag
            End If
        End If
    Next
End Sub

'----------------------------------------------------------------------
' 選択されている台を配列で返す
'
' @param daiList    可変長配列
'----------------------------------------------------------------------
Public Function GetSelected(daiList() As Long, ByVal flag As Boolean) As Long
    Dim i As Long
    Dim cnt As Long
    Dim loopMax As Long
    
    'panellayout.xml読込み前は、エラーになるので0を返す
    On Error Resume Next
    loopMax = UBound(m_dais)
    If Err.Number <> 0 Then
        GetSelected = 0
        Exit Function
    End If
    
    '台情報分ループして、選択しているものをリストに追加していく
    For i = 0 To UBound(m_dais)
        If m_dais(i).m_enable = True And m_dais(i).m_select = flag Then
            daiList(cnt) = m_dais(i).hdb
            
            'エラーが出たときは、配列を拡張して格納
            If Err.Number <> 0 Then
                ReDim Preserve daiList(cnt)
                daiList(cnt) = m_dais(i).hdb
            End If
            cnt = cnt + 1
        End If
    Next
    
    GetSelected = cnt
End Function


'----------------------------------------------------------------------
' 全ての台の有効状態をフラグで指定した状態にする
'----------------------------------------------------------------------
Public Sub EnableAll(ByVal flag As Boolean)
    Dim i As Long
    
    '台情報分ループして、選択しているものをリストに追加していく
    On Error Resume Next
    For i = 0 To UBound(m_dais)
        If Err.Number <> 0 Then Exit For
    
        m_dais(i).m_enable = flag
    Next
End Sub

'----------------------------------------------------------------------
' 指定した台の有効状態をフラグで指定した状態にする
'----------------------------------------------------------------------
Public Sub EnableDaiList(daiList() As Long, ByVal flag As Boolean)
    Dim i As Long
    Dim idx As Long
    
    For i = 0 To UBound(daiList)
        If m_hdb2idx.Exists(daiList(i)) Then
            idx = m_hdb2idx(daiList(i))
            
            m_dais(idx).m_enable = flag
        End If
    Next
End Sub

'----------------------------------------------------------------------
' 有効状態が指定された状態の台を配列で返す
'
' @param daiList    可変長配列
'----------------------------------------------------------------------
Public Function GetEnabled(daiList() As Long, ByVal flag As Boolean) As Long
    Dim i As Long
    Dim cnt As Long
    Dim loopMax As Long
    
    'panellayout.xml読込み前は、エラーになるので0を返す
    On Error Resume Next
    loopMax = UBound(m_dais)
    If Err.Number <> 0 Then
        GetEnabled = 0
        Exit Function
    End If
    
    '台情報分ループして、選択しているものをリストに追加していく
    For i = 0 To UBound(m_dais)
        If m_dais(i).m_enable = flag Then
            daiList(cnt) = m_dais(i).hdb
            
            'エラーが出たときは、配列を拡張して格納
            If Err.Number <> 0 Then
                ReDim Preserve daiList(cnt)
                daiList(cnt) = m_dais(i).hdb
            End If
            cnt = cnt + 1
        End If
    Next
    
    GetEnabled = cnt
End Function

'----------------------------------------------------------------------
' 再描画を促す
'----------------------------------------------------------------------
Public Sub Refresh()
    
    picBase.Refresh
    
End Sub

'----------------------------------------------------------------------
' 描画
'----------------------------------------------------------------------
Public Sub Update()
    
    DrawPanel
    
End Sub

'----------------------------------------------------------------------
' レイアウト用XMLファイルを読み込む
'----------------------------------------------------------------------
Public Function LoadXML(ByVal layoutXML As String, ByVal extLayoutXML As String) As Boolean

    Call LoadLayoutXML(layoutXML)
    
    ArrangeComponent
    
End Function

'----------------------------------------------------------------------
' RECT型を拡大縮小する
'----------------------------------------------------------------------
Private Sub ScaleRect(ByRef rc As RECT, ByVal nScale As Long)
    With rc
        .Left = .Left * nScale / 100
        .Top = .Top * nScale / 100
        .Right = .Right * nScale / 100
        .Bottom = .Bottom * nScale / 100
    End With
End Sub

'----------------------------------------------------------------------
' RECT型の幅を返す
'----------------------------------------------------------------------
Private Function GetWidth(ByRef rc As RECT) As Long
    GetWidth = rc.Right - rc.Left
End Function

'----------------------------------------------------------------------
' RECT型の高さを返す
'----------------------------------------------------------------------
Private Function GetHeight(ByRef rc As RECT) As Long
    GetHeight = rc.Bottom - rc.Top
End Function

'----------------------------------------------------------------------
' 表示台番号から、m_daisのインデックスを取得する
'----------------------------------------------------------------------
Private Function AddDaiData(ByVal hdb As Long) As Long
    
    AddDaiData = -1
    
    If m_hdb2idx.Exists(hdb) Then
        AddDaiData = m_hdb2idx(hdb)
        Exit Function
    End If
    
    On Error Resume Next
    
    Dim ub As Long
    ub = UBound(m_dais)
    If Err.Number = 0 Then
        ub = ub + 1
    Else
        ub = 0
    End If
    
    ReDim Preserve m_dais(ub)
    m_dais(ub).hdb = hdb
    m_hdb2idx(hdb) = ub
    
    AddDaiData = ub
    
End Function

'----------------------------------------------------------------------
' レイアウト用のXMLファイルからオブジェクト情報を作成する
'----------------------------------------------------------------------
Private Sub LoadLayoutXML(ByVal layoutXML As String)

    Dim i As Long
    Dim xmlInput As DOMDocument
    Dim xmlBoundRectNodes As IXMLDOMNodeList
    Dim xmlDaiNodes As IXMLDOMNodeList
    Dim arTemp As Variant
    Dim errMsg As String
    Dim hdb As Long
    Dim rdb As Long

    'bound rectを空にする
    With m_rcBound
        .Left = 0
        .Top = 0
        .Right = 0
        .Bottom = 0
    End With

    'XMLファイルの読み込み
    Set xmlInput = New DOMDocument

    If xmlInput.Load(layoutXML) = False Then
        Exit Sub
    End If

    'bound rectを読込む
    Set xmlBoundRectNodes = xmlInput.getElementsByTagName("panellayout/boundrect")
    If xmlBoundRectNodes.length = 1 Then
        arTemp = Split(xmlBoundRectNodes.Item(0).Text, ",")
        
        With m_rcBound
            .Left = arTemp(0)
            .Top = arTemp(1)
            .Right = arTemp(2)
            .Bottom = arTemp(3)
        End With
    End If

    'dai を読込む
    Set xmlDaiNodes = xmlInput.getElementsByTagName("panellayout/dai")

    For i = 0 To xmlDaiNodes.length - 1
        hdb = xmlDaiNodes(i).selectNodes("hdb")(0).Text

        Dim idx As Long
        idx = AddDaiData(hdb)
        If idx >= 0 Then
            arTemp = Split(xmlDaiNodes(i).selectNodes("rect")(0).Text, ",")
            
            m_dais(idx).m_enable = True
            m_dais(idx).m_select = False
            m_dais(idx).m_CustomNormalColor = -1
            m_dais(idx).m_CustomSelectedColor = -1
            m_dais(idx).m_CustomDisabledColor = -1
            
            With m_dais(idx).rc
                .Left = arTemp(0)
                .Top = arTemp(1)
                .Right = arTemp(2)
                .Bottom = arTemp(3)
            End With
        End If
    Next

    '後始末
    Set xmlBoundRectNodes = Nothing
    Set xmlDaiNodes = Nothing
    Set xmlInput = Nothing

End Sub

'----------------------------------------------------------------------
' 台番に使用するフォントを作成する
'----------------------------------------------------------------------
Private Function CreatePanelFont(ByVal hdc As Long) As Long
    
    Dim i As Long
    Dim lf As LOGFONT
    Dim faceName() As Byte
    
    Call GetObject(GetStockObject(DEFAULT_GUI_FONT), LenB(lf), lf)
    
    lf.lfHeight = -(picBase.Font.Size * GetDeviceCaps(hdc, LOGPIXELSY) / 72) * m_scale / 100
    lf.lfWeight = IIf(picBase.Font.Bold, FW_BOLD, FW_NORMAL)
    lf.lfItalic = IIf(picBase.Font.Italic, 1, 0)
    lf.lfStrikeOut = IIf(picBase.Font.Strikethrough, 1, 0)
    faceName = StrConv(picBase.Font.Name, vbFromUnicode)
    
    For i = 0 To IIf(UBound(lf.lfFaceName) < UBound(faceName), UBound(lf.lfFaceName), UBound(faceName))
        lf.lfFaceName(i) = faceName(i)
    Next
    
    CreatePanelFont = CreateFontIndirect(lf)
    
End Function

'----------------------------------------------------------------------
' パネル描画
'----------------------------------------------------------------------
Private Sub DrawPanel()

    picBase.Cls

    Dim hBitmap As Long
    Dim hMemDC As Long
    Dim rcTemp As RECT
    Dim hBrH As Long

    Dim rowHeight, colWidth

    colWidth = m_rcBound.Right * m_scale / 100
    rowHeight = m_rcBound.Bottom * m_scale / 100

    hBitmap = CreateCompatibleBitmap(picBase.hdc, colWidth, rowHeight)
    If hBitmap <> 0 Then
        hMemDC = CreateCompatibleDC(picBase.hdc)
        If hMemDC <> 0 Then
            SelectObject hMemDC, hBitmap

            With rcTemp
                .Left = 0
                .Top = 0
                .Right = colWidth
                .Bottom = rowHeight
            End With
            
            FillRect hMemDC, rcTemp, GetStockObject(WHITE_BRUSH)
            'SelectObject hMemDC, s_hfont

            '拡張パネル情報を使って描画
            ''m_pnlex.Draw hMemDC, m_scale

            '三種類のブラシを作成
            Dim br_normal As Long, br_selected As Long, br_disabled As Long
            br_normal = CreateSolidBrush(m_NormalDaiColor)
            br_selected = CreateSolidBrush(m_SelectedDaiColor)
            br_disabled = CreateSolidBrush(m_DisabledDaiColor)
            
            Dim hFont As Long, hFontOld As Long
            hFont = CreatePanelFont(hMemDC)
            hFontOld = SelectObject(hMemDC, hFont)
            
            Dim hbrCustom As Long
            Dim i As Long
            Dim rcTemp2 As RECT
            Dim s As String
            SetBkMode hMemDC, TRANSPARENT
            
            On Error GoTo NoDai
            
            For i = 0 To UBound(m_dais)
                rcTemp2 = m_dais(i).rc
                ScaleRect rcTemp2, m_scale

                If m_dais(i).m_enable = False Then
                    If m_dais(i).m_CustomDisabledColor <> -1 Then
                        hbrCustom = CreateSolidBrush(m_dais(i).m_CustomDisabledColor)
                        FillRect hMemDC, rcTemp2, hbrCustom
                        Call DeleteObject(hbrCustom)
                    Else
                        FillRect hMemDC, rcTemp2, br_disabled
                    End If
                    
                    If m_dais(i).m_CustomNormalForeColor <> -1 Then
                        Call SetTextColor(hMemDC, m_dais(i).m_CustomNormalForeColor)
                    Else
                        Call SetTextColor(hMemDC, m_DisabledDaiForeColor)
                    End If
                ElseIf m_dais(i).m_select = True Then
                    If m_dais(i).m_CustomSelectedColor <> -1 Then
                        hbrCustom = CreateSolidBrush(m_dais(i).m_CustomSelectedColor)
                        FillRect hMemDC, rcTemp2, hbrCustom
                        Call DeleteObject(hbrCustom)
                    Else
                        FillRect hMemDC, rcTemp2, br_selected
                    End If
                    
                    If m_dais(i).m_CustomSelectedForeColor <> -1 Then
                        Call SetTextColor(hMemDC, m_dais(i).m_CustomSelectedForeColor)
                    Else
                        Call SetTextColor(hMemDC, m_SelectedDaiForeColor)
                    End If
                Else
                    If m_dais(i).m_CustomNormalColor <> -1 Then
                        hbrCustom = CreateSolidBrush(m_dais(i).m_CustomNormalColor)
                        FillRect hMemDC, rcTemp2, hbrCustom
                        Call DeleteObject(hbrCustom)
                    Else
                        FillRect hMemDC, rcTemp2, br_normal
                    End If
                    
                    If m_dais(i).m_CustomNormalForeColor <> -1 Then
                        Call SetTextColor(hMemDC, m_dais(i).m_CustomNormalForeColor)
                    Else
                        Call SetTextColor(hMemDC, m_NormalDaiForeColor)
                    End If
                End If
                
                DrawEdge hMemDC, rcTemp2, BDR_RAISED, BF_RECT Or BF_ADJUST

                rcTemp2 = m_dais(i).rc
                InflateRect rcTemp2, -1, -1
                ScaleRect rcTemp2, m_scale
                
                DrawText hMemDC, m_dais(i).hdb, -1, rcTemp2, _
                    DT_NOCLIP Or DT_NOPREFIX Or DT_CENTER Or DT_VCENTER Or DT_SINGLELINE
            Next i

NoDai:
            On Error GoTo 0
            
            Call FrameRect(hMemDC, rcTemp, GetStockObject(GRAY_BRUSH))

            Call BitBlt(picBase.hdc, 0, 0, colWidth, rowHeight, hMemDC, 0, 0, SRCCOPY)

            Call SelectObject(hMemDC, hFontOld)
            Call DeleteObject(hFont)
            Call DeleteObject(br_normal)
            Call DeleteObject(br_selected)
            Call DeleteObject(br_disabled)
            
            DeleteDC hMemDC
        End If
        DeleteObject hBitmap
    End If

End Sub

'----------------------------------------------------------------------
' 各コントロールの初期属性を設定する
'----------------------------------------------------------------------
Private Sub InitializeComponent()

    With picBase
        .Appearance = 0         'フラット
        .BorderStyle = vbBSNone '境界線なし
        .BackColor = RGB(192, 192, 192)
        .AutoRedraw = True
    End With
    
End Sub

'----------------------------------------------------------------------
' 各コントロールをフォーム内に再配置する
'----------------------------------------------------------------------
Private Sub ArrangeComponent()

    picBack.Move 0, 0, UserControl.ScaleWidth, UserControl.ScaleHeight
    
    If m_rcBound.Left = 0 And m_rcBound.Top = 0 And m_rcBound.Right = 0 And m_rcBound.Bottom = 0 Then
        picBase.Move 0, 0, picBack.ScaleWidth, picBack.ScaleHeight
    Else
        picBase.Move 0, 0, _
            GetWidth(m_rcBound) * Screen.TwipsPerPixelX * m_scale / 100, _
            GetHeight(m_rcBound) * Screen.TwipsPerPixelY * m_scale / 100
    End If
    
End Sub

Private Sub picBack_Click()
    RaiseEvent Click
End Sub

Private Sub picBack_DblClick()
    RaiseEvent DblClick
End Sub

Private Sub picBack_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub picBack_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub picBack_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

Private Sub picBack_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub picBack_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub picBack_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

Private Sub picBase_Click()
    RaiseEvent Click
End Sub

Private Sub picBase_DblClick()
    RaiseEvent DblClick
End Sub

Private Sub picBase_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub picBase_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub picBase_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

Private Sub picBase_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)

    Dim Cancel As Boolean
    Dim i As Long
    Dim pt As POINTAPI
    pt.X = X / Screen.TwipsPerPixelX
    pt.Y = Y / Screen.TwipsPerPixelY
    
    Dim rc As RECT
    Dim hit As Long
    hit = -1
    
    If Button = vbLeftButton And Shift = 0 Then
    
        On Error Resume Next
        For i = 0 To UBound(m_dais)
            rc = m_dais(i).rc
            Call ScaleRect(rc, m_scale)
            If PtInRect(rc, pt.X, pt.Y) <> 0 Then
                hit = i
                Exit For
            End If
        Next
        
        If hit >= 0 Then
            
            RaiseEvent BeforeSelect(m_dais(hit).hdb, m_dais(hit).m_enable, m_dais(hit).m_select, Cancel)
            If Cancel = False Then
            
                If m_dais(hit).m_enable Then
                    If m_dais(hit).m_select = True Then
                        m_dais(hit).m_select = False
                    Else
                        m_dais(hit).m_select = True
                    End If
                End If
            End If
        End If
    End If
    
    RaiseEvent MouseDown(Button, Shift, X, Y)
    
    Update
    Refresh

End Sub

Private Sub picBase_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub picBase_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

Private Sub UserControl_Click()
    RaiseEvent Click
End Sub

Private Sub UserControl_DblClick()
    RaiseEvent DblClick
End Sub

Private Sub UserControl_Initialize()

    m_scale = 100
    
    InitializeComponent
    
End Sub

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub UserControl_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub UserControl_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)

    picBase.Font.Bold = PropBag.ReadProperty("FontBold", picBase.Font.Bold)
    picBase.Font.Italic = PropBag.ReadProperty("FontItalic", picBase.Font.Italic)
    picBase.Font.Name = PropBag.ReadProperty("FontName", picBase.Font.Name)
    picBase.Font.Size = PropBag.ReadProperty("FontSize", picBase.Font.Size)
    picBase.Font.Strikethrough = PropBag.ReadProperty("FontStrikethrough", picBase.Font.Strikethrough)
    
    m_offsetX = PropBag.ReadProperty("OffsetX", 0)
    m_offsetY = PropBag.ReadProperty("OffsetY", 0)
    m_scale = PropBag.ReadProperty("PanelScale", 100)
    
    m_NormalDaiColor = PropBag.ReadProperty("NormalDaiColor", RGB(192, 192, 192))
    m_NormalDaiForeColor = PropBag.ReadProperty("NormalDaiForeColor", RGB(0, 0, 0))
    m_SelectedDaiColor = PropBag.ReadProperty("SelectedDaiColor", RGB(192, 255, 192))
    m_SelectedDaiForeColor = PropBag.ReadProperty("SelectedDaiForeColor", RGB(0, 0, 0))
    m_DisabledDaiColor = PropBag.ReadProperty("DisabledDaiColor", RGB(64, 64, 192))
    m_DisabledDaiForeColor = PropBag.ReadProperty("DisabledDaiForeColor", RGB(0, 0, 0))
    
End Sub

Private Sub UserControl_Resize()

    ArrangeComponent
    
End Sub

Private Sub UserControl_WriteProperties(PropBag As PropertyBag)

    Call PropBag.WriteProperty("FontBold", picBase.Font.Bold)
    Call PropBag.WriteProperty("FontItalic", picBase.Font.Italic)
    Call PropBag.WriteProperty("FontName", picBase.Font.Name)
    Call PropBag.WriteProperty("FontSize", picBase.Font.Size)
    Call PropBag.WriteProperty("FontStrikethrough", picBase.Font.Strikethrough)
    
    Call PropBag.WriteProperty("OffsetX", m_offsetX, 0)
    Call PropBag.WriteProperty("OffsetY", m_offsetY, 0)
    Call PropBag.WriteProperty("PanelScale", m_scale, 100)
    
    Call PropBag.WriteProperty("NormalDaiColor", m_NormalDaiColor)
    Call PropBag.WriteProperty("NormalDaiForeColor", m_NormalDaiForeColor)
    Call PropBag.WriteProperty("SelectedDaiColor", m_SelectedDaiColor)
    Call PropBag.WriteProperty("SelectedDaiForeColor", m_SelectedDaiForeColor)
    Call PropBag.WriteProperty("DisabledDaiColor", m_DisabledDaiColor)
    Call PropBag.WriteProperty("DisabledDaiForeColor", m_DisabledDaiForeColor)
    
End Sub
