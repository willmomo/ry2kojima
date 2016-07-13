VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Form1"
   ClientHeight    =   7785
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   10575
   LinkTopic       =   "Form1"
   ScaleHeight     =   7785
   ScaleWidth      =   10575
   StartUpPosition =   3  'Windows の既定値
   WindowState     =   2  '最大化
   Begin MSComDlg.CommonDialog fontDlg 
      Left            =   5640
      Top             =   1200
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   7380
      Width           =   1275
   End
   Begin VB.VScrollBar VScroll1 
      Height          =   1335
      Left            =   10200
      TabIndex        =   2
      Top             =   780
      Width           =   255
   End
   Begin VB.PictureBox Picture2 
      Height          =   6975
      Left            =   120
      ScaleHeight     =   6915
      ScaleWidth      =   9675
      TabIndex        =   0
      Top             =   60
      Width           =   9735
      Begin VB.PictureBox Picture1 
         Appearance      =   0  'ﾌﾗｯﾄ
         AutoRedraw      =   -1  'True
         BackColor       =   &H00C0FFFF&
         ForeColor       =   &H80000008&
         Height          =   4395
         Left            =   0
         ScaleHeight     =   4365
         ScaleWidth      =   6585
         TabIndex        =   1
         Top             =   0
         Width           =   6615
         Begin MSComDlg.CommonDialog colorDlg 
            Left            =   4860
            Top             =   1140
            _ExtentX        =   847
            _ExtentY        =   847
            _Version        =   393216
         End
      End
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "開く(&O)"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "保存(&S)"
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "編集(&E)"
      Begin VB.Menu mnuEditDelete 
         Caption         =   "削除(&D)"
         Shortcut        =   {DEL}
      End
      Begin VB.Menu mnuEditSep 
         Caption         =   "-"
         Index           =   0
      End
      Begin VB.Menu mnuEditResize 
         Caption         =   "図面サイズ変更(&R)"
      End
      Begin VB.Menu mnuEditJust 
         Caption         =   "ジャストフィット(&J)"
      End
   End
   Begin VB.Menu mnuPopup 
      Caption         =   "図形(&S)"
      Begin VB.Menu mnuInsertShape 
         Caption         =   "図形挿入(&I)"
      End
      Begin VB.Menu mnuShapeSep3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuShapeFont 
         Caption         =   "フォント(&F)..."
      End
      Begin VB.Menu mnuPopupBackColor 
         Caption         =   "背景色(&B)..."
      End
      Begin VB.Menu mnuShapeSep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuShapeType 
         Caption         =   "タイプ(&T)"
         Begin VB.Menu mnuShapeTypeRect 
            Caption         =   "四角(&R)"
         End
         Begin VB.Menu mnuShapeTypeCircle 
            Caption         =   "円(&C)"
         End
         Begin VB.Menu mnuShapeTypeDai 
            Caption         =   "台(&D)"
         End
      End
      Begin VB.Menu mnuOrder 
         Caption         =   "順序(&R)"
         Begin VB.Menu mnuOrderTop 
            Caption         =   "最前面へ移動(&T)"
         End
         Begin VB.Menu mnuOrderBack 
            Caption         =   "最背面へ移動(&K)"
         End
         Begin VB.Menu mnuOrderFront 
            Caption         =   "前面へ移動(&F)"
         End
         Begin VB.Menu mnuOrderBackOne 
            Caption         =   "背面へ移動(&B)"
         End
      End
      Begin VB.Menu mnuShapeSep2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuShapeProperty 
         Caption         =   "プロパティ(&R)..."
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const DEF_PICTURE1_WIDTH As Long = 1024 * 15
Private Const DEF_PICTURE1_HEIGHT As Long = 768 * 15

Private m_shapeList As New clsList
Private m_dragging As Boolean
Private m_resizing As Long
Private m_resizingShape As clsAnyShape
Private m_startPosX
Private m_startPosY
Private m_prevX
Private m_prevY

Private Sub AdjustComponents()
    
    VScroll1.Move Me.ScaleWidth - VScroll1.Width, 0, VScroll1.Width, Me.ScaleHeight - HScroll1.Height
    HScroll1.Move 0, Me.ScaleHeight - HScroll1.Height, Me.ScaleWidth - VScroll1.Width, HScroll1.Height

    Picture2.Move 0, 0, Me.ScaleWidth - VScroll1.Width, Me.ScaleHeight - HScroll1.Height
    
    'Picture1.Move 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight
    Picture1.Left = 0
    Picture1.Top = 0
    
    VScroll1.Min = 0
    If (Picture1.Height - Picture2.ScaleHeight) < 0 Then
        VScroll1.Max = 0
    Else
        VScroll1.Max = (Picture1.Height - Picture2.ScaleHeight) / Screen.TwipsPerPixelY
    End If
    
    HScroll1.Min = 0
    If (Picture1.Width - Picture2.ScaleWidth) < 0 Then
        HScroll1.Max = 0
    Else
        HScroll1.Max = (Picture1.Width - Picture2.ScaleWidth) / Screen.TwipsPerPixelX
    End If
    
End Sub

Private Sub InitializeComponents()
    
    With fontDlg
        .Flags = cdlCFScreenFonts
        '.CancelError
    End With
    
    With colorDlg
        .CancelError = True
    End With
    
    Picture1.Left = 0
    Picture1.Top = 0
    Picture1.Width = DEF_PICTURE1_WIDTH
    Picture1.Height = DEF_PICTURE1_HEIGHT
    
End Sub

Private Sub DrawGrid()
    Dim y As Long
    Dim x As Long
    
    Picture1.DrawMode = vbCopyPen
    For y = 0 To Picture1.ScaleHeight Step 10 * Screen.TwipsPerPixelY
        For x = 0 To Picture1.ScaleWidth Step 10 * Screen.TwipsPerPixelX
            Picture1.PSet (x, y), RGB(0, 0, 0)
        Next
    Next
End Sub

Private Sub DrawShape()

    Dim objNode As clsNode
    
    Set objNode = m_shapeList.Back
    
    Do Until objNode Is Nothing
        objNode.Value.DrawShape Picture1
        Set objNode = objNode.PrevNode
    Loop

End Sub

Private Sub DrawMovingShape(ofsX, ofsY, snap)

    Dim objNode As clsNode
    
    Set objNode = m_shapeList.Back
    
    Do Until objNode Is Nothing
        If objNode.Value.Selected Then
            objNode.Value.DrawMovingShape Picture1, ofsX, ofsY, snap
        End If
        Set objNode = objNode.PrevNode
    Loop

End Sub

Private Sub MoveShape(ofsX, ofsY, snap)

    Dim objNode As clsNode
    
    Set objNode = m_shapeList.Back
    
    Do Until objNode Is Nothing
        If objNode.Value.Selected Then
            
            objNode.Value.Left = CalcSnapX(objNode.Value.Left + ofsX)
            objNode.Value.Right = CalcSnapX(objNode.Value.Right + ofsX)
            objNode.Value.Top = CalcSnapY(objNode.Value.Top + ofsY)
            objNode.Value.Bottom = CalcSnapY(objNode.Value.Bottom + ofsY)
            
        End If
        Set objNode = objNode.PrevNode
    Loop

End Sub

Private Function CalcSnapX(x)
    CalcSnapX = Int(x / (10 * Screen.TwipsPerPixelX)) * (10 * Screen.TwipsPerPixelX)
End Function

Private Function CalcSnapY(y)
    CalcSnapY = Int(y / (10 * Screen.TwipsPerPixelY)) * (10 * Screen.TwipsPerPixelY)
End Function

Private Sub Snapping(ByVal x As Single, ByVal y As Single, ByRef snpX As Long, ByRef snpY As Long)

    snpX = Int(x / (10 * Screen.TwipsPerPixelX)) * (10 * Screen.TwipsPerPixelX)
    snpY = Int(y / (10 * Screen.TwipsPerPixelY)) * (10 * Screen.TwipsPerPixelY)
    
End Sub

Private Function PtInShape(x, y) As clsAnyShape
    Set PtInShape = Nothing
    
    Dim objNode As clsNode
    Set objNode = m_shapeList.Front
    Do Until objNode Is Nothing
        If objNode.Value.PtInShape(x, y) Then
            Set PtInShape = objNode.Value
            Exit Do
        End If
        Set objNode = objNode.NextNode
    Loop
End Function

Private Sub ResetSelect()
    Dim objNode As clsNode
    Set objNode = m_shapeList.Front
    Do Until objNode Is Nothing
        objNode.Value.Selected = False
        Set objNode = objNode.NextNode
    Loop
End Sub

Private Function SelectShape(x As Single, y As Single, AppendMode) As clsAnyShape
    If Not AppendMode Then
        ResetSelect
    End If
    
    Set SelectShape = Nothing
    
    Dim objNode As clsNode
    
    Set objNode = m_shapeList.Front
    Do Until objNode Is Nothing
        If objNode.Value.PtInShape(x, y) Then
            objNode.Value.Selected = True
            Set SelectShape = objNode.Value
            
            Picture1.Cls
            DrawGrid
            DrawShape
            Picture1.Refresh
        
            Exit Do
        End If
        
        Set objNode = objNode.NextNode
    Loop
    
End Function

Private Function SelectedNode() As clsNode
    Set SelectedNode = Nothing
    
    Dim objNode As clsNode
    Set objNode = m_shapeList.Front
    Do Until objNode Is Nothing
        If objNode.Value.Selected Then
            Set SelectedNode = objNode
            Exit Do
        End If
        Set objNode = objNode.NextNode
    Loop
End Function

Private Function SelectedShape() As clsAnyShape
    Set SelectedShape = Nothing
    
    Dim objNode As clsNode
    Set objNode = m_shapeList.Front
    Do Until objNode Is Nothing
        If objNode.Value.Selected Then
            Set SelectedShape = objNode.Value
            Exit Do
        End If
        Set objNode = objNode.NextNode
    Loop
End Function

Private Sub Form_Load()

    Randomize
    
    InitializeComponents
    
    Picture1.Cls
    DrawGrid
    DrawShape
    Picture1.Refresh
End Sub

Private Sub Form_Resize()

    AdjustComponents
    
    Picture1.Cls
    DrawGrid
    DrawShape
    Picture1.Refresh
End Sub

Private Sub HScroll1_Change()

    Picture1.Left = -HScroll1.Value * Screen.TwipsPerPixelX
    
End Sub

Private Sub mnuEditDelete_Click()

    Dim curNode As clsNode
    Set curNode = m_shapeList.Front
    
    Do Until curNode Is Nothing
    
        Dim targetNode As clsNode
        Set targetNode = curNode
        
        Set curNode = curNode.NextNode
        
        If targetNode.Value.Selected Then
            m_shapeList.Remove targetNode
        End If
        
    Loop
    
    Picture1.Cls
    DrawGrid
    DrawShape
    Picture1.Refresh
End Sub

Private Sub mnuEditJust_Click()
    Dim rcUnion As RECT
    Dim rcTemp As RECT
    Dim rcTemp2 As RECT
    
    Dim curNode As clsNode
    Set curNode = m_shapeList.Front
    
    Dim objAnyShape As clsAnyShape
    Do Until curNode Is Nothing
        Set objAnyShape = curNode.Value
        
        SetRect rcTemp, objAnyShape.Left, objAnyShape.Top, objAnyShape.Right, objAnyShape.Bottom
        
        Picture1.Line (rcTemp.Left, rcTemp.Top)-(rcTemp.Right, rcTemp.Bottom), , B
        
        UnionRect rcUnion, rcTemp2, rcTemp
        rcTemp2 = rcUnion
        
        Set curNode = curNode.NextNode
    Loop
    
    Picture1.Left = 0
    Picture1.Top = 0
    Picture1.Width = (rcTemp.Right - rcTemp.Left)
    Picture1.Height = (rcTemp.Bottom - rcTemp.Top)

        
    VScroll1.Min = 0
    If (Picture1.Height - Picture2.ScaleHeight) < 0 Then
        VScroll1.Max = 0
    Else
        VScroll1.Max = (Picture1.Height - Picture2.ScaleHeight) / Screen.TwipsPerPixelY
    End If
    
    HScroll1.Min = 0
    If (Picture1.Width - Picture2.ScaleWidth) < 0 Then
        HScroll1.Max = 0
    Else
        HScroll1.Max = (Picture1.Width - Picture2.ScaleWidth) / Screen.TwipsPerPixelX
    End If
    
    Picture1.Cls
    DrawGrid
    DrawShape
    Picture1.Refresh
End Sub

Private Sub mnuEditResize_Click()
    dlgCanvasProp.CanvasWidth = Picture1.Width / Screen.TwipsPerPixelX
    dlgCanvasProp.CanvasHeight = Picture1.Height / Screen.TwipsPerPixelY
    dlgCanvasProp.Show vbModal, Me
    If dlgCanvasProp.Result = vbOK Then
        Picture1.Left = 0
        Picture1.Top = 0
        Picture1.Width = dlgCanvasProp.CanvasWidth * Screen.TwipsPerPixelX
        Picture1.Height = dlgCanvasProp.CanvasHeight * Screen.TwipsPerPixelY
        
        VScroll1.Min = 0
        If (Picture1.Height - Picture2.ScaleHeight) < 0 Then
            VScroll1.Max = 0
        Else
            VScroll1.Max = (Picture1.Height - Picture2.ScaleHeight) / Screen.TwipsPerPixelY
        End If
        
        HScroll1.Min = 0
        If (Picture1.Width - Picture2.ScaleWidth) < 0 Then
            HScroll1.Max = 0
        Else
            HScroll1.Max = (Picture1.Width - Picture2.ScaleWidth) / Screen.TwipsPerPixelX
        End If
        
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If
End Sub

Private Sub mnuFileOpen_Click()
    Dim i As Long
    Dim nodeList As IXMLDOMNodeList
    
    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    If xmlDoc.Load("panelconf.xml") Then
        Set nodeList = xmlDoc.getElementsByTagName("Shapes/Shape")
        
        For i = 0 To nodeList.length - 1
            
            Dim objNode As clsNode
            Set objNode = m_shapeList.CreateAnyShapeNode()
            
            Dim objShape As clsAnyShape
            Set objShape = objNode.Value
            
            objShape.FromXMLNode nodeList.Item(i)
            
            m_shapeList.PushBack objNode
            
        Next

        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If
    
    Set xmlDoc = Nothing
End Sub

Private Sub mnuFileSave_Click()

    Dim xmlDoc As DOMDocument
    Set xmlDoc = New DOMDocument
    
    Dim rootNode As IXMLDOMNode
    Set rootNode = xmlDoc.createNode(NODE_ELEMENT, "Shapes", "")
    
    Dim objNode As clsNode
    Set objNode = m_shapeList.Front
    Do Until objNode Is Nothing
    
        rootNode.appendChild objNode.Value.ToXMLNode()
        
        Set objNode = objNode.NextNode
    Loop
    
    xmlDoc.appendChild rootNode
    
    xmlDoc.save "panelconf.xml"
End Sub

Private Sub mnuInsertShape_Click()

    Dim newNode As clsNode
    Set newNode = m_shapeList.CreateAnyShapeNode
    
    m_shapeList.PushFront newNode
    
    Picture1.Cls
    DrawGrid
    DrawShape
    Picture1.Refresh
End Sub

Private Sub mnuOrderBack_Click()

    Dim objNode As clsNode
    Set objNode = SelectedNode
    If Not objNode Is Nothing Then
        m_shapeList.Remove objNode
        m_shapeList.PushBack objNode
        
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If
    
End Sub

Private Sub mnuOrderBackOne_Click()

    Dim objNode As clsNode
    Set objNode = SelectedNode
    If Not objNode Is Nothing Then
    
        If Not objNode.NextNode Is Nothing Then
            If Not objNode.NextNode.NextNode Is Nothing Then
                m_shapeList.Remove objNode
                m_shapeList.Insert objNode.NextNode.NextNode, objNode
            Else
                m_shapeList.Remove objNode
                m_shapeList.PushBack objNode
            End If
            
            Picture1.Cls
            DrawGrid
            DrawShape
            Picture1.Refresh
        End If
    End If

End Sub

Private Sub mnuOrderFront_Click()

    Dim objNode As clsNode
    Set objNode = SelectedNode
    If Not objNode Is Nothing Then
    
        If Not objNode.PrevNode Is Nothing Then
            m_shapeList.Remove objNode
            m_shapeList.Insert objNode.PrevNode, objNode
            
            Picture1.Cls
            DrawGrid
            DrawShape
            Picture1.Refresh
        End If
    End If

End Sub

Private Sub mnuOrderTop_Click()

    Dim objNode As clsNode
    Set objNode = SelectedNode
    If Not objNode Is Nothing Then
        m_shapeList.Remove objNode
        m_shapeList.PushFront objNode
        
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If
    
End Sub

Private Sub mnuPopupBackColor_Click()

    On Error Resume Next
    colorDlg.ShowColor
    If Err.Number = 0 Then
        
        Dim objShape As clsAnyShape
        Set objShape = SelectedShape
        If Not objShape Is Nothing Then
            objShape.Color = colorDlg.Color
        
            Picture1.Cls
            DrawGrid
            DrawShape
            Picture1.Refresh
        End If
    End If
    
End Sub

Private Sub mnuShapeProperty_Click()

    Dim objShape As clsAnyShape
    Set objShape = SelectedShape

    If Not objShape Is Nothing Then
        If objShape.ShapeType = 2 Then
        
            Set dlgDaiProperty.TargetShape = objShape
            dlgDaiProperty.Show vbModal, Me
            If dlgDaiProperty.Result = vbOK Then
                
                Picture1.Cls
                DrawGrid
                DrawShape
                Picture1.Refresh
            End If
        Else
            Set dlgShapeProperty.TargetShape = objShape
            dlgShapeProperty.Show vbModal, Me
            If dlgShapeProperty.Result = vbOK Then
                Picture1.Cls
                DrawGrid
                DrawShape
                Picture1.Refresh
            End If
        End If
    End If
    
End Sub

Private Sub mnuShapeTypeCircle_Click()

    Dim objShape As clsAnyShape
    Set objShape = SelectedShape
    If Not objShape Is Nothing Then
        objShape.ShapeType = 1
    
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If

End Sub

Private Sub mnuShapeTypeDai_Click()

    Dim objShape As clsAnyShape
    Set objShape = SelectedShape
    If Not objShape Is Nothing Then
        objShape.ShapeType = 2
    
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If

End Sub

Private Sub mnuShapeTypeRect_Click()

    Dim objShape As clsAnyShape
    Set objShape = SelectedShape
    If Not objShape Is Nothing Then
        objShape.ShapeType = 0
    
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    End If

End Sub

Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
    Dim objShape As clsAnyShape
    
    If Button = vbLeftButton Then
        Set objShape = PtInShape(x, y)
        
        If Not objShape Is Nothing Then
            
            If Not objShape.Selected Then
                Call SelectShape(x, y, IIf(Shift = vbShiftMask, True, False))
            End If
        
            m_resizing = objShape.PtInHandle(x, y)
            
            m_startPosX = x
            m_startPosY = y
            
            m_prevX = x
            m_prevY = y
            
            If m_resizing > 0 Then
                Set m_resizingShape = objShape
                m_resizingShape.DrawResizingShape Picture1, x - m_startPosX, y - m_startPosY, m_resizing, True
            Else
                DrawMovingShape x - m_startPosX, y - m_startPosY, True
                m_dragging = True
            End If
            
        Else
            ResetSelect
            Picture1.Cls
            DrawGrid
            DrawShape
            Picture1.Refresh
            
        End If
        
    ElseIf Button = vbRightButton Then
        Set objShape = SelectShape(x, y, False)
        
        If Not objShape Is Nothing Then
            Me.PopupMenu mnuPopup
        End If
    End If
    
End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)

    If m_resizing > 0 Then
    
        m_resizingShape.DrawResizingShape Picture1, m_prevX - m_startPosX, m_prevY - m_startPosY, m_resizing, True
        m_resizingShape.DrawResizingShape Picture1, x - m_startPosX, y - m_startPosY, m_resizing, True
        
        m_prevX = x
        m_prevY = y
    ElseIf m_dragging Then
    
        DrawMovingShape m_prevX - m_startPosX, m_prevY - m_startPosY, True
        DrawMovingShape x - m_startPosX, y - m_startPosY, True
        
        m_prevX = x
        m_prevY = y
        
    End If
    
End Sub

Private Sub Picture1_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)

    If m_resizing > 0 Then
    
        m_resizingShape.DrawResizingShape Picture1, m_prevX - m_startPosX, m_prevY - m_startPosY, m_resizing, True
        m_resizingShape.ResizeShape m_prevX - m_startPosX, m_prevY - m_startPosY, m_resizing, True
        
        Set m_resizingShape = Nothing
        m_resizing = 0
        
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
    ElseIf m_dragging Then
    
        DrawMovingShape m_prevX - m_startPosX, m_prevY - m_startPosY, True
        
        m_dragging = False
    
        MoveShape x - m_startPosX, y - m_startPosY, True
        
        Picture1.Cls
        DrawGrid
        DrawShape
        Picture1.Refresh
        
    End If
    
End Sub

Private Sub VScroll1_Change()

    Picture1.Top = -VScroll1.Value * Screen.TwipsPerPixelY
    
End Sub
