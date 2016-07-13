VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAnyShape"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_type As Long  '0:四角 1:円 2:台
Private m_left As Long
Private m_top As Long
Private m_right As Long
Private m_bottom As Long
Private m_color As Long
Private m_selected As Boolean

'タイプ四角、円用プロパティ
Private m_shapeText As String
Private m_fontBold As Boolean
Private m_fontItalic As Boolean
Private m_fontStrikeThru As Boolean
Private m_fontUnderLine As Boolean
Private m_fontSize As Long
Private m_fontName As String
Private m_rotate As Long

'タイプ台用プロパティ
Private m_numDai As Long    '台数
Private m_daiNo() As Long   '台数分の台番号配列
Private m_daiDir As Long    '台の並ぶ方向: 0 ↓ 1 ↑ 2 → 3 ←

Private Function CalcSnapX(x)
    CalcSnapX = Int((x) / (10 * Screen.TwipsPerPixelX)) * (10 * Screen.TwipsPerPixelX)
End Function

Private Function CalcSnapY(y)
    CalcSnapY = Int((y) / (10 * Screen.TwipsPerPixelY)) * (10 * Screen.TwipsPerPixelY)
End Function

Public Property Get Rotate() As Long
    Rotate = m_rotate
End Property

Public Property Let Rotate(ByVal newValue As Long)
    m_rotate = newValue
End Property

Public Property Get ShapeText() As String
    ShapeText = m_shapeText
End Property

Public Property Let ShapeText(newValue As String)
    m_shapeText = newValue
End Property

Public Property Get DaiCount()
    DaiCount = m_numDai
End Property

Public Property Let DaiCount(newValue)
    m_numDai = newValue
    ReDim Preserve m_daiNo(m_numDai - 1)
    
    Select Case m_daiDir
    Case 0
        m_bottom = m_top + ((10 * 2) * Screen.TwipsPerPixelY) * m_numDai
        m_right = m_left + (10 * 4 * Screen.TwipsPerPixelX)
    Case 1
        m_top = m_bottom - ((10 * 2) * Screen.TwipsPerPixelY) * m_numDai
        m_right = m_left + (10 * 4 * Screen.TwipsPerPixelX)
    Case 2
        m_right = m_left + ((10 * 4) * Screen.TwipsPerPixelX) * m_numDai
        m_bottom = m_top + (10 * 2 * Screen.TwipsPerPixelY)
    Case 3
        m_left = m_right - ((10 * 4) * Screen.TwipsPerPixelX) * m_numDai
        m_bottom = m_top + (10 * 2 * Screen.TwipsPerPixelY)
    End Select
    
End Property

Public Property Get DaiNumber(Index)
    DaiNumber = m_daiNo(Index)
End Property

Public Property Let DaiNumber(Index, newValue)
    m_daiNo(Index) = newValue
End Property

Public Property Get DaiDirection()
    DaiDirection = m_daiDir
End Property

Public Property Let DaiDirection(newValue)
    m_daiDir = newValue

    Select Case m_daiDir
    Case 0
        m_bottom = m_top + ((10 * 2) * Screen.TwipsPerPixelY) * m_numDai
        m_right = m_left + (10 * 4 * Screen.TwipsPerPixelX)
    Case 1
        m_top = m_bottom - ((10 * 2) * Screen.TwipsPerPixelY) * m_numDai
        m_right = m_left + (10 * 4 * Screen.TwipsPerPixelX)
    Case 2
        m_right = m_left + ((10 * 4) * Screen.TwipsPerPixelX) * m_numDai
        m_bottom = m_top + (10 * 2 * Screen.TwipsPerPixelY)
    Case 3
        m_left = m_right - ((10 * 4) * Screen.TwipsPerPixelX) * m_numDai
        m_bottom = m_top + (10 * 2 * Screen.TwipsPerPixelY)
    End Select

End Property

Public Property Get ShapeType() As Long
    ShapeType = m_type
End Property

Public Property Let ShapeType(ByVal Value As Long)
    m_type = Value
    
    If m_type = 2 Then
        m_right = m_left + ((10 * 4) * Screen.TwipsPerPixelX)
        m_bottom = m_top + ((10 * 2) * Screen.TwipsPerPixelY)
        
        If m_numDai = 0 Then
            m_numDai = 1
            ReDim m_daiNo(m_numDai - 1)
            m_daiNo(0) = 0
        End If
    End If
End Property

Public Property Get Selected() As Boolean
    Selected = m_selected
End Property

Public Property Let Selected(ByVal Value As Boolean)
    m_selected = Value
End Property

Public Property Get Left() As Long
    Left = m_left
End Property

Public Property Let Left(ByVal Value As Long)
    m_left = Value
End Property

Public Property Get Top() As Long
    Top = m_top
End Property

Public Property Let Top(ByVal Value As Long)
    m_top = Value
End Property

Public Property Get Right() As Long
    Right = m_right
End Property

Public Property Let Right(ByVal Value As Long)
    m_right = Value
End Property

Public Property Get Bottom() As Long
    Bottom = m_bottom
End Property

Public Property Let Bottom(ByVal Value As Long)
    m_bottom = Value
End Property

Public Property Get Color() As Long
    Color = m_color
End Property

Public Property Let Color(ByVal Value As Long)
    m_color = Value
End Property

Public Function PtInShape(x As Single, y As Single) As Boolean

    PtInShape = IIf((m_left < x And x < m_right) And (m_top < y And y < m_bottom), True, False)
        
End Function

Public Function PtInHandle(x As Single, y As Single) As Long

    If (m_left < x And x < m_left + 150) And (m_top < y And y < m_top + 150) Then
        PtInHandle = 1
    ElseIf (m_right - 150 < x And x < m_right) And (m_top < y And y < m_top + 150) Then
        PtInHandle = 2
    ElseIf (m_right - 150 < x And x < m_right) And (m_bottom - 150 < y And y < m_bottom) Then
        PtInHandle = 3
    ElseIf (m_left < x And x < m_left + 150) And (m_bottom - 150 < y And y < m_bottom) Then
        PtInHandle = 4
    Else
        PtInHandle = 0
    End If
    
End Function

Public Sub DrawShape(ByRef pic As PictureBox)
    Dim radius
    Dim rc As RECT
    Dim lf As LOGFONT
    Dim hfont As Long
    Dim hfontOld As Long
    Dim oldAlign As Long
    
    Select Case m_type
    Case 0
        pic.DrawMode = vbCopyPen
        pic.FillColor = m_color
        pic.FillStyle = vbFSSolid
        pic.Line (m_left, m_top)-(m_right, m_bottom), RGB(0, 0, 0), B
        
        SetRect rc, _
            m_left / Screen.TwipsPerPixelX, _
            m_top / Screen.TwipsPerPixelY, _
            m_right / Screen.TwipsPerPixelX, _
            m_bottom / Screen.TwipsPerPixelY
            
        Call GetObject(GetStockObject(DEFAULT_GUI_FONT), LenB(lf), lf)
        lf.lfEscapement = IIf(m_rotate >= 0, m_rotate * 10, (360 + m_rotate) * 10)
        lf.lfOrientation = lf.lfEscapement
        hfont = CreateFontIndirect(lf)
        
        hfontOld = SelectObject(pic.hdc, hfont)
        oldAlign = SetTextAlign(pic.hdc, TA_CENTER Or TA_BASELINE)
        ''DrawText pic.hdc, m_shapeText, -1, rc, DT_CENTER Or DT_VCENTER Or DT_SINGLELINE
        TextOut pic.hdc, rc.Left + (rc.Right - rc.Left) / 2, rc.Top + (rc.Bottom - rc.Top) / 2, m_shapeText, LenB(StrConv(m_shapeText, vbFromUnicode))
        SetTextAlign pic.hdc, oldAlign
        Call DeleteObject(SelectObject(pic.hdc, hfontOld))
        
    Case 1
        pic.DrawMode = vbCopyPen
        pic.FillColor = m_color
        pic.FillStyle = vbFSSolid
        
        radius = IIf(m_right - m_left > m_bottom - m_top, m_right - m_left, m_bottom - m_top) / 2
        pic.Circle ((m_right - m_left) / 2 + m_left, (m_bottom - m_top) / 2 + m_top), radius, RGB(0, 0, 0), , , _
            (m_bottom - m_top) / (m_right - m_left)
            
        SetRect rc, _
            m_left / Screen.TwipsPerPixelX, _
            m_top / Screen.TwipsPerPixelY, _
            m_right / Screen.TwipsPerPixelX, _
            m_bottom / Screen.TwipsPerPixelY
            
        Call GetObject(GetStockObject(DEFAULT_GUI_FONT), LenB(lf), lf)
        lf.lfEscapement = IIf(m_rotate >= 0, m_rotate * 10, (360 + m_rotate) * 10)
        lf.lfOrientation = lf.lfEscapement
        hfont = CreateFontIndirect(lf)
        
        hfontOld = SelectObject(pic.hdc, hfont)
        DrawText pic.hdc, m_shapeText, -1, rc, DT_CENTER Or DT_VCENTER Or DT_SINGLELINE
        Call DeleteObject(SelectObject(pic.hdc, hfontOld))
    Case 2
        pic.DrawMode = vbCopyPen
        pic.FillColor = m_color
        pic.FillStyle = vbFSSolid
'''        pic.Line (m_left, m_top)-(m_right, m_bottom), RGB(0, 0, 0), B
        
        Select Case m_daiDir
        Case 0, 2
            Call SetRect(rc, _
                m_left / Screen.TwipsPerPixelX, _
                m_top / Screen.TwipsPerPixelY, _
                (m_left + (10 * 4 * Screen.TwipsPerPixelX)) / Screen.TwipsPerPixelX, _
                (m_top + (10 * 2 * Screen.TwipsPerPixelY)) / Screen.TwipsPerPixelY)
        Case 1, 3
            Call SetRect(rc, _
                (m_right - 10 * 4 * Screen.TwipsPerPixelX) / Screen.TwipsPerPixelX, _
                (m_bottom - 10 * 2 * Screen.TwipsPerPixelY) / Screen.TwipsPerPixelY, _
                ((m_right - 10 * 4 * Screen.TwipsPerPixelX) + (10 * 4 * Screen.TwipsPerPixelX)) / Screen.TwipsPerPixelX, _
                ((m_bottom - 10 * 2 * Screen.TwipsPerPixelY) + (10 * 2 * Screen.TwipsPerPixelY)) / Screen.TwipsPerPixelY)
        End Select
            
        Call GetObject(GetStockObject(DEFAULT_GUI_FONT), LenB(lf), lf)
        lf.lfHeight = 11
        
        hfont = CreateFontIndirect(lf)
        
        Dim i As Long
        For i = 0 To m_numDai - 1
            Call DrawEdge(pic.hdc, rc, EDGE_RAISED, BF_RECT Or BF_MIDDLE)
            
            hfontOld = SelectObject(pic.hdc, hfont)
            Call DrawText(pic.hdc, CStr(m_daiNo(i)), -1, rc, DT_CENTER Or DT_VCENTER Or DT_SINGLELINE)
            
            Select Case m_daiDir
            Case 0
                OffsetRect rc, 0, (10 * 2)
            Case 1
                OffsetRect rc, 0, -(10 * 2)
            Case 2
                OffsetRect rc, (10 * 4), 0
            Case 3
                OffsetRect rc, -(10 * 4), 0
            End Select
        Next
        Call DeleteObject(SelectObject(pic.hdc, hfontOld))
        
    End Select
    
    If m_selected Then
    
        pic.FillColor = RGB(255, 255, 255)
        pic.FillStyle = vbFSSolid
        
        If m_type <> 2 Then
            pic.Line (m_left, m_top)-(m_left + 150, m_top + 150), RGB(0, 0, 0), B
            pic.Line (m_right - 150, m_top)-(m_right, m_top + 150), RGB(0, 0, 0), B
            pic.Line (m_right - 150, m_bottom - 150)-(m_right, m_bottom), RGB(0, 0, 0), B
            pic.Line (m_left, m_bottom - 150)-(m_left + 150, m_bottom), RGB(0, 0, 0), B
        Else
            pic.Line (m_left, m_top)-(m_left + (5 * Screen.TwipsPerPixelX), m_top + (5 * Screen.TwipsPerPixelY)), RGB(0, 0, 0), B
            pic.Line (m_right - (5 * Screen.TwipsPerPixelX), m_top)-(m_right, m_top + (5 * Screen.TwipsPerPixelY)), RGB(0, 0, 0), B
            pic.Line (m_right - (5 * Screen.TwipsPerPixelX), m_bottom - (5 * Screen.TwipsPerPixelY))-(m_right, m_bottom), RGB(0, 0, 0), B
            pic.Line (m_left, m_bottom - (5 * Screen.TwipsPerPixelX))-(m_left + (5 * Screen.TwipsPerPixelY), m_bottom), RGB(0, 0, 0), B
        End If
    End If
    
End Sub

Public Sub DrawMovingShape(ByRef pic As PictureBox, ofsX, ofsY, snap)

    Dim snpLeft, snpTop, snpRight, snpBottom
    
    pic.DrawMode = vbXorPen
    pic.FillColor = m_color
    pic.FillStyle = vbFSSolid
    
    If snap Then
        snpLeft = Int((m_left + ofsX) / (10 * Screen.TwipsPerPixelX)) * (10 * Screen.TwipsPerPixelX)
        snpTop = Int((m_top + ofsY) / (10 * Screen.TwipsPerPixelY)) * (10 * Screen.TwipsPerPixelY)
        snpRight = Int((m_right + ofsX) / (10 * Screen.TwipsPerPixelX)) * (10 * Screen.TwipsPerPixelX)
        snpBottom = Int((m_bottom + ofsY) / (10 * Screen.TwipsPerPixelY)) * (10 * Screen.TwipsPerPixelY)
    Else
        snpLeft = m_left + ofsX
        snpTop = m_top + ofsY
        snpRight = m_right + ofsX
        snpBottom = m_top + ofsY
    End If
    
    pic.Line (snpLeft, snpTop)-(snpRight, snpBottom), RGB(0, 0, 0), B
    
End Sub

Public Sub ResizeShape(ofsX, ofsY, handlePos, snap)

    Select Case handlePos
        Case 1  '左上ハンドル
            If snap Then
                m_left = CalcSnapX(m_left + ofsX)
                m_top = CalcSnapY(m_top + ofsY)
            Else
                m_left = m_left + ofsX
                m_top = m_top + ofsY
            End If
        Case 2  '右上ハンドル
            If snap Then
                m_top = CalcSnapY(m_top + ofsY)
                m_right = CalcSnapX(m_right + ofsX)
            Else
                m_top = m_top + ofsY
                m_right = m_right + ofsX
            End If
        Case 3  '右下ハンドル
            If snap Then
                m_right = CalcSnapX(m_right + ofsX)
                m_bottom = CalcSnapY(m_bottom + ofsY)
            Else
                m_right = m_right + ofsX
                m_bottom = m_bottom + ofsY
            End If
        Case 4  '左下ハンドル
            If snap Then
                m_left = CalcSnapX(m_left + ofsX)
                m_bottom = CalcSnapY(m_bottom + ofsY)
            Else
                m_left = m_left + ofsX
                m_bottom = m_bottom + ofsY
            End If
    End Select
    
End Sub

Public Sub DrawResizingShape(pic As PictureBox, ofsX, ofsY, handlePos, snap)

    pic.DrawMode = vbXorPen
    pic.FillColor = m_color
    pic.FillStyle = vbFSSolid
    
    Select Case handlePos
        Case 1  '左上ハンドル
            If snap Then
                pic.Line (CalcSnapX(m_left + ofsX), CalcSnapY(m_top + ofsY))-(m_right, m_bottom), RGB(0, 0, 0), B
            Else
                pic.Line (m_left + ofsX, m_top + ofsY)-(m_right, m_bottom), RGB(0, 0, 0), B
            End If
        Case 2  '右上ハンドル
            If snap Then
                pic.Line (m_left, CalcSnapY(m_top + ofsY))-(CalcSnapX(m_right + ofsX), m_bottom), RGB(0, 0, 0), B
            Else
                pic.Line (m_left, m_top + ofsY)-(m_right + ofsX, m_bottom), RGB(0, 0, 0), B
            End If
        Case 3  '右下ハンドル
            If snap Then
                pic.Line (m_left, m_top)-(CalcSnapX(m_right + ofsX), CalcSnapY(m_bottom + ofsY)), RGB(0, 0, 0), B
            Else
                pic.Line (m_left, m_top)-(m_right + ofsX, m_bottom + ofsY), RGB(0, 0, 0), B
            End If
        Case 4  '左下ハンドル
            If snap Then
                pic.Line (CalcSnapX(m_left + ofsX), m_top)-(m_right, CalcSnapY(m_bottom + ofsY)), RGB(0, 0, 0), B
            Else
                pic.Line (m_left + ofsX, m_top)-(m_right, m_bottom + ofsY), RGB(0, 0, 0), B
            End If
    End Select
    
End Sub

Public Sub FromXMLNode(shapeNode As IXMLDOMNode)

    Dim i As Long
    Dim cNode As IXMLDOMNode
    Dim nIndex As Long
    
    For i = 0 To shapeNode.childNodes.length - 1
        Set cNode = shapeNode.childNodes.Item(i)
        
        Select Case cNode.nodeName
            Case "Type"
                m_type = CLng(cNode.Text)
            Case "Left"
                m_left = CLng(cNode.Text)
            Case "Top"
                m_top = CLng(cNode.Text)
            Case "Right"
                m_right = CLng(cNode.Text)
            Case "Bottom"
                m_bottom = CLng(cNode.Text)
            Case "Color"
                m_color = CLng(cNode.Text)
            Case "ShapeText"
                m_shapeText = cNode.Text
            Case "NumDai"
                m_numDai = CLng(cNode.Text)
                ReDim m_daiNo(m_numDai - 1)
            Case "DaiNo"
                nIndex = CLng(cNode.Attributes.getNamedItem("Index").Text)
                m_daiNo(nIndex) = CLng(cNode.Text)
            Case "DaiDir"
                m_daiDir = CLng(cNode.Text)
        End Select
    Next
End Sub

Public Function ToXMLNode() As IXMLDOMNode

    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    Dim shapeNode As IXMLDOMNode
    Set shapeNode = xmlDoc.createNode(NODE_ELEMENT, "Shape", "")
    
    Dim typeNode As IXMLDOMNode
    Set typeNode = xmlDoc.createNode(NODE_ELEMENT, "Type", "")
    typeNode.Text = CStr(m_type)
    shapeNode.appendChild typeNode
    
    Dim leftNode As IXMLDOMNode
    Set leftNode = xmlDoc.createNode(NODE_ELEMENT, "Left", "")
    leftNode.Text = CStr(m_left)
    shapeNode.appendChild leftNode
    
    Dim topNode As IXMLDOMNode
    Set topNode = xmlDoc.createNode(NODE_ELEMENT, "Top", "")
    topNode.Text = CStr(m_top)
    shapeNode.appendChild topNode
    
    Dim rightNode As IXMLDOMNode
    Set rightNode = xmlDoc.createNode(NODE_ELEMENT, "Right", "")
    rightNode.Text = CStr(m_right)
    shapeNode.appendChild rightNode
    
    Dim bottomNode As IXMLDOMNode
    Set bottomNode = xmlDoc.createNode(NODE_ELEMENT, "Bottom", "")
    bottomNode.Text = CStr(m_bottom)
    shapeNode.appendChild bottomNode
    
    Dim colorNode As IXMLDOMNode
    Set colorNode = xmlDoc.createNode(NODE_ELEMENT, "Color", "")
    colorNode.Text = CStr(m_color)
    shapeNode.appendChild colorNode
    
    Dim tempNode As IXMLDOMNode
    Dim attrNode As IXMLDOMNode
    
    Select Case m_type
        Case 0, 1
            Set tempNode = xmlDoc.createNode(NODE_ELEMENT, "ShapeText", "")
            tempNode.Text = m_shapeText
            shapeNode.appendChild tempNode
            
            Set tempNode = xmlDoc.createNode(NODE_ELEMENT, "Rotate", "")
            tempNode.Text = CStr(m_rotate)
            shapeNode.appendChild tempNode
        Case 2
            Set tempNode = xmlDoc.createNode(NODE_ELEMENT, "NumDai", "")
            tempNode.Text = m_numDai
            shapeNode.appendChild tempNode
            
            Dim i As Long
            For i = 0 To m_numDai - 1
                Set attrNode = xmlDoc.createNode(NODE_ATTRIBUTE, "Index", "")
                attrNode.Text = CStr(i)
                Set tempNode = xmlDoc.createNode(NODE_ELEMENT, "DaiNo", "")
                tempNode.Attributes.setNamedItem attrNode
                tempNode.Text = CStr(m_daiNo(i))
                shapeNode.appendChild tempNode
            Next
            
            Set tempNode = xmlDoc.createNode(NODE_ELEMENT, "DaiDir", "")
            tempNode.Text = CStr(m_daiDir)
            shapeNode.appendChild tempNode
            
    End Select
    
    Set ToXMLNode = shapeNode
End Function

'----------------------------------------------------------------------
'クラスの初期化(コンストラクタ)
'
Private Sub Class_Initialize()
    m_left = 0
    m_top = 0
    m_right = 1500
    m_bottom = 1500
    m_color = RGB(255, 255, 255)
    
    m_fontBold = False
    m_fontItalic = False
    m_fontStrikeThru = False
    m_fontUnderLine = False
    m_fontSize = 9  'point
    m_fontName = "MS UI Gothic"
End Sub
