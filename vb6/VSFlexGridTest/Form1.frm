VERSION 5.00
Object = "{C0A63B80-4B21-11D3-BD95-D426EF2C7949}#1.0#0"; "Vsflex7L.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6960
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   12750
   LinkTopic       =   "Form1"
   ScaleHeight     =   6960
   ScaleWidth      =   12750
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VSFlex7LCtl.VSFlexGrid fgTest 
      Height          =   6375
      Left            =   7080
      TabIndex        =   0
      Top             =   60
      Width           =   5595
      _cx             =   9869
      _cy             =   11245
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "‚l‚r ‚oƒSƒVƒbƒN"
         Size            =   9
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   0   'False
      AllowBigSelection=   -1  'True
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   2
      ShowComboButton =   -1  'True
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7LCtl.VSFlexGrid fgEvents 
      Height          =   6375
      Left            =   60
      TabIndex        =   1
      Top             =   60
      Width           =   6675
      _cx             =   11774
      _cy             =   11245
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "‚l‚r ‚oƒSƒVƒbƒN"
         Size            =   9
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   1
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   -1  'True
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_NowKeyAction As Boolean

Private Sub push_front(ar)
    fgEvents.AddItem Join(ar, vbTab), 1
    fgEvents.AutoSize 0, fgEvents.Cols - 1
End Sub

Private Sub fgTest_AfterCollapse(ByVal Row As Long, ByVal State As Integer)
    push_front Array(Now, "AfterCollapse", Row, State)
End Sub

Private Sub fgTest_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    push_front Array(Now, "AfterEdit", Row, Col)
End Sub

Private Sub fgTest_AfterMoveColumn(ByVal Col As Long, Position As Long)
    push_front Array(Now, "AfterMoveColumn", Col, Position)
End Sub

Private Sub fgTest_AfterMoveRow(ByVal Row As Long, Position As Long)
    push_front Array(Now, "AfterMoveRow", Row, Position)
End Sub

Private Sub fgTest_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    push_front Array(Now, "AfterRowColChange", OldRow, OldCol, NewRow, NewCol)
    
    If m_NowKeyAction And (Abs(NewRow - OldRow) = 1) Then
        If ((NewRow - fgTest.FixedRows) Mod 2) = 0 Then
            fgTest.Row = NewRow + IIf(NewRow > OldRow, 1, -1)
        End If
    Else
        If ((NewRow - fgTest.FixedRows) Mod 2) = 0 Then
            fgTest.Row = NewRow + 1
        End If
    End If
End Sub

Private Sub fgTest_AfterScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long)
    push_front Array(Now, "AfterScroll", OldTopRow, OldLeftCol, NewTopRow, NewLeftCol)
End Sub

Private Sub fgTest_AfterSelChange(ByVal OldRowSel As Long, ByVal OldColSel As Long, ByVal NewRowSel As Long, ByVal NewColSel As Long)
    push_front Array(Now, "AfterSelChange", OldRowSel, OldColSel, NewRowSel, NewColSel)
End Sub

Private Sub fgTest_AfterSort(ByVal Col As Long, Order As Integer)
    push_front Array(Now, "AfterSort", Col, Order)
End Sub

Private Sub fgTest_AfterUserFreeze()
    push_front Array(Now, "AfterUserFreeze")
End Sub

Private Sub fgTest_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    push_front Array(Now, "AfterUserResize", Row, Col)
End Sub

Private Sub fgTest_BeforeCollapse(ByVal Row As Long, ByVal State As Integer, Cancel As Boolean)
    push_front Array(Now, "BeforeCollapse", Row, State, Cancel)
End Sub

Private Sub fgTest_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    push_front Array(Now, "BeforeEdit", Row, Col, Cancel)
End Sub

Private Sub fgTest_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal Y As Single, Cancel As Boolean)
    push_front Array(Now, "BeforeMouseDown", Button, Shift, X, Y, Cancel)
End Sub

Private Sub fgTest_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    push_front Array(Now, "BeforeMoveColumn", Col, Position)
End Sub

Private Sub fgTest_BeforeMoveRow(ByVal Row As Long, Position As Long)
    push_front Array(Now, "BeforeMoveRow", Row, Position)
End Sub

Private Sub fgTest_BeforePageBreak(ByVal Row As Long, BreakOK As Boolean)
    push_front Array(Now, "BeforePageBreak", Row, BreakOK)
End Sub

Private Sub fgTest_BeforeRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long, Cancel As Boolean)
    push_front Array(Now, "BeforeRowColChange", OldRow, OldCol, NewRow, NewCol, Cancel)
End Sub

Private Sub fgTest_BeforeScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long, Cancel As Boolean)
    push_front Array(Now, "BeforeScroll", OldTopRow, OldLeftCol, NewTopRow, NewLeftCol, Cancel)
End Sub

Private Sub fgTest_BeforeScrollTip(ByVal Row As Long)
    push_front Array(Now, "BeforeScrollTip", Row)
End Sub

Private Sub fgTest_BeforeSelChange(ByVal OldRowSel As Long, ByVal OldColSel As Long, ByVal NewRowSel As Long, ByVal NewColSel As Long, Cancel As Boolean)
    push_front Array(Now, "BeforeSelChange", OldRowSel, OldColSel, NewRowSel, NewColSel, Cancel)
End Sub

Private Sub fgTest_BeforeSort(ByVal Col As Long, Order As Integer)
    push_front Array(Now, "BeforeSort", Col, Order)
End Sub

Private Sub fgTest_BeforeUserResize(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    push_front Array(Now, "BeforeUserResize", Row, Col, Cancel)
End Sub

Private Sub fgTest_CellButtonClick(ByVal Row As Long, ByVal Col As Long)
    push_front Array(Now, "CellButtonClick", Row, Col)
End Sub

Private Sub fgTest_CellChanged(ByVal Row As Long, ByVal Col As Long)
    push_front Array(Now, "CellChange", Row, Col)
End Sub

Private Sub fgTest_ChangeEdit()
    push_front Array(Now, "ChangeEdit")
End Sub

Private Sub fgTest_Click()
    ''push_front Array(Now, "Click")
End Sub

Private Sub fgTest_Compare(ByVal Row1 As Long, ByVal Row2 As Long, Cmp As Integer)
    push_front Array(Now, "Compare", Row1, Row2, Cmp)
End Sub

Private Sub fgTest_DblClick()
    push_front Array(Now, "DblClick")
End Sub

Private Sub fgTest_DragDrop(Source As Control, X As Single, Y As Single)
    push_front Array(Now, "DragDrop", Source, X, Y)
End Sub

Private Sub fgTest_DragOver(Source As Control, X As Single, Y As Single, State As Integer)
    push_front Array(Now, "DragOver", Source, X, Y, State)
End Sub

Private Sub fgTest_DrawCell(ByVal hDC As Long, ByVal Row As Long, ByVal Col As Long, ByVal Left As Long, ByVal Top As Long, ByVal Right As Long, ByVal Bottom As Long, Done As Boolean)
    push_front Array(Now, "DrawCell", hDC, Row, Col, Left, Top, Right, Bottom, Done)
End Sub

Private Sub fgTest_EnterCell()
    push_front Array(Now, "EnterCell")
End Sub

Private Sub fgTest_Error(ByVal ErrorCode As Long, ShowMsgBox As Boolean)
    push_front Array(Now, "Error", ErrorCode, ShowMsgBox)
End Sub

Private Sub fgTest_GetHeaderRow(ByVal Row As Long, HeaderRow As Long)
    push_front Array(Now, "GEtHeaderRow", Row, HeaderRow)
End Sub

Private Sub fgTest_GotFocus()
    push_front Array(Now, "GotFocus")
End Sub

Private Sub fgTest_KeyDown(KeyCode As Integer, Shift As Integer)
    push_front Array(Now, "KeyDown", KeyCode, Shift)
    
    m_NowKeyAction = True
End Sub

Private Sub fgTest_KeyDownEdit(ByVal Row As Long, ByVal Col As Long, KeyCode As Integer, ByVal Shift As Integer)
    push_front Array(Now, "KeyDownEdit", Row, Col, KeyCode, Shift)
End Sub

Private Sub fgTest_KeyPress(KeyAscii As Integer)
    ''push_front Array(Now, "KeyPress", KeyAscii)
End Sub

Private Sub fgTest_KeyPressEdit(ByVal Row As Long, ByVal Col As Long, KeyAscii As Integer)
    push_front Array(Now, "KeyPressEdit", Row, Col, KeyAscii)
End Sub

Private Sub fgTest_KeyUp(KeyCode As Integer, Shift As Integer)
    push_front Array(Now, "KeyUp", KeyCode, Shift)
    
    m_NowKeyAction = False
End Sub

Private Sub fgTest_KeyUpEdit(ByVal Row As Long, ByVal Col As Long, KeyCode As Integer, ByVal Shift As Integer)
    push_front Array(Now, "KeyUpEdit", Row, Col, KeyCode, Shift)
End Sub

Private Sub fgTest_LeaveCell()
    push_front Array(Now, "LeaveCell")
End Sub

Private Sub fgTest_LostFocus()
    push_front Array(Now, "LostFocus")
End Sub

Private Sub fgTest_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    push_front Array(Now, "MouseDown", Button, Shift, X, Y)
End Sub

Private Sub fgTest_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ''push_front Array(Now, "MouseMove", Button, Shift, X, Y)
End Sub

Private Sub fgTest_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    push_front Array(Now, "MouseUp", Button, Shift, X, Y)
End Sub

Private Sub fgTest_RowColChange()
    push_front Array(Now, "RowColChange")
End Sub

Private Sub fgTest_SelChange()
    push_front Array(Now, "SelChange")
End Sub

Private Sub fgTest_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    push_front Array(Now, "StartEdit", Row, Col, Cancel)
End Sub

Private Sub fgTest_StartPage(ByVal hDC As Long, ByVal Page As Long, Cancel As Boolean)
    push_front Array(Now, "StartPage", hDC, Page, Cancel)
End Sub

Private Sub fgTest_Validate(Cancel As Boolean)
    push_front Array(Now, "Validate", Cancel)
End Sub

Private Sub fgTest_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    push_front Array(Now, "ValidateEdit", Row, Col, Cancel)
End Sub

Private Sub Form_Load()
    fgTest.Col = 1
    fgTest.Row = 2
End Sub
