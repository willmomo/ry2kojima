VERSION 5.00
Object = "{C0A63B80-4B21-11D3-BD95-D426EF2C7949}#1.0#0"; "Vsflex7L.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   Caption         =   "データ復旧ツール"
   ClientHeight    =   7935
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   10920
   LinkTopic       =   "Form1"
   ScaleHeight     =   7935
   ScaleWidth      =   10920
   StartUpPosition =   3  'Windows の既定値
   Begin MSComDlg.CommonDialog cdlg 
      Left            =   10260
      Top             =   180
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VSFlex7LCtl.VSFlexGrid fgZenten 
      Height          =   5595
      Left            =   5640
      TabIndex        =   1
      Top             =   2040
      Width           =   5115
      _cx             =   9022
      _cy             =   9869
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "ＭＳ Ｐゴシック"
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
      Cols            =   5
      FixedRows       =   1
      FixedCols       =   4
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
      ShowComboButton =   1
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
   Begin VSFlex7LCtl.VSFlexGrid fgDai 
      Height          =   5595
      Left            =   60
      TabIndex        =   0
      Top             =   900
      Width           =   5235
      _cx             =   9234
      _cy             =   9869
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "ＭＳ Ｐゴシック"
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
      Cols            =   8
      FixedRows       =   1
      FixedCols       =   3
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
      ShowComboButton =   1
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
   Begin VB.Label lblUpperStat 
      Caption         =   "(上段状態)"
      Height          =   255
      Left            =   1020
      TabIndex        =   7
      Top             =   60
      Width           =   5115
   End
   Begin VB.Label lblLowerStat 
      Caption         =   "(下段状態)"
      Height          =   255
      Left            =   1020
      TabIndex        =   6
      Top             =   360
      Width           =   5115
   End
   Begin VB.Label lblLower 
      Caption         =   "下段表示："
      Height          =   255
      Left            =   60
      TabIndex        =   5
      Top             =   360
      Width           =   915
   End
   Begin VB.Label lblUpper 
      Caption         =   "上段表示："
      Height          =   255
      Left            =   60
      TabIndex        =   4
      Top             =   60
      Width           =   915
   End
   Begin VB.Label lblZenten 
      Caption         =   "全店データ(&Z)"
      Height          =   195
      Left            =   5700
      TabIndex        =   3
      Top             =   1380
      Width           =   1335
   End
   Begin VB.Label lblDai 
      Caption         =   "台データ(&D)"
      Height          =   195
      Left            =   60
      TabIndex        =   2
      Top             =   660
      Width           =   1695
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
      Begin VB.Menu mnuFileNew 
         Caption         =   "新規(&N)"
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "開く(&O)..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "保存(&S)"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "名前を付けて保存(&A)..."
      End
      Begin VB.Menu mnuFileSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "終了(&X)"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "表示(&V)"
      Begin VB.Menu mnuViewUpper 
         Caption         =   "上段(&U)"
         Begin VB.Menu mnuUpperNow 
            Caption         =   "現在のデータ"
            Checked         =   -1  'True
         End
         Begin VB.Menu mnuUpperRecovered 
            Caption         =   "復旧後のデータ"
         End
      End
      Begin VB.Menu mnuViewLower 
         Caption         =   "下段(&L)"
         Begin VB.Menu mnuLowerAbsolute 
            Caption         =   "絶対値の復旧データ"
            Checked         =   -1  'True
         End
         Begin VB.Menu mnuLowerRelative 
            Caption         =   "相対値の復旧データ"
         End
      End
   End
   Begin VB.Menu mnuAction 
      Caption         =   "動作(&A)"
      Begin VB.Menu mnuViewLink 
         Caption         =   "データ連動"
         Begin VB.Menu mnuDataLink 
            Caption         =   "する"
         End
         Begin VB.Menu mnuDataUnlink 
            Caption         =   "しない"
            Checked         =   -1  'True
         End
      End
      Begin VB.Menu mnuCopyAction 
         Caption         =   "一括入力"
         Begin VB.Menu mnuCopyAll 
            Caption         =   "すべての台に複製"
            Enabled         =   0   'False
         End
         Begin VB.Menu mnuCopySameSbt 
            Caption         =   "同じ種別の台に複製"
            Enabled         =   0   'False
         End
      End
      Begin VB.Menu mnuActSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuActRecover 
         Caption         =   "復旧(&R)"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Enum DAI_COL_NAME
    COL_OUT = 3
    COL_SAFE
    COL_START
    COL_GEN1URI
    COL_GEN2URI
End Enum

Private Enum ZEN_COL_NAME
    COL_URIKIN = 4
End Enum

Private m_fgDai_NowKeyAction As Boolean
Private m_fgZen_NowKeyAction As Boolean

Private Sub DrawFixed()
    Dim i As Long
    Dim r As Long
    Dim s As String
    
    With fgDai
        .Rows = .FixedRows + f1000_get_daisu() * 2
        
        For i = 0 To f1000_get_daisu() - 1
            r = i * 2 + .FixedRows
            
            .Cell(flexcpText, r, 0, r + 1) = i + 1
            .Cell(flexcpText, r, 1, r + 1) = f1001_get_hdb(i + 1)
            .Cell(flexcpText, r, 2, r + 1) = f1008_get_sbtname(f1001_get_sbt(i + 1))
        Next
    End With
    
    With fgZenten
        .Rows = .FixedRows
        
        For i = 1 To MAX_ZENTENSU
            If f1002_get_kind(i) > 0 Then
                s = CStr(i) & vbTab & _
                    f1002_get_disp1(i) & vbTab & _
                    f1002_get_disp2(i) & vbTab & _
                    f1002_get_kind_string(i)

                .AddItem s
                .AddItem s
            End If
        Next
    End With
End Sub

Private Sub DrawUppderData()
    Dim i As Long
    Dim r As Long
    
    With fgDai
        For i = 0 To f1000_get_daisu() - 1
            r = i * 2 + .FixedRows
            
            If mnuUpperNow.Checked Then
                .Cell(flexcpText, r, COL_OUT) = f4102_get_out(i + 1)
                .Cell(flexcpText, r, COL_SAFE) = f4102_get_safe(i + 1)
                .Cell(flexcpText, r, COL_START) = f4102_get_start(i + 1)
                .Cell(flexcpText, r, COL_GEN1URI) = f4102_get_gen1uri(i + 1)
                .Cell(flexcpText, r, COL_GEN2URI) = f4102_get_gen2uri(i + 1)
            Else
                .Cell(flexcpText, r, COL_OUT) = f4102_get_out(i + 1) + g_daiRel(i).out
                .Cell(flexcpText, r, COL_SAFE) = f4102_get_safe(i + 1) + g_daiRel(i).safe
                .Cell(flexcpText, r, COL_START) = f4102_get_start(i + 1) + g_daiRel(i).start
                .Cell(flexcpText, r, COL_GEN1URI) = f4102_get_gen1uri(i + 1) + g_daiRel(i).gen1uri
                .Cell(flexcpText, r, COL_GEN2URI) = f4102_get_gen2uri(i + 1) + g_daiRel(i).gen2uri
            End If
        Next
    End With
    
    With fgZenten
        For i = .FixedRows To .Rows - 1 Step 2
            If mnuUpperNow.Checked Then
                .Cell(flexcpText, i, COL_URIKIN) = f4103_get_indata((i - .FixedRows) / 2 + 1)
            Else
                .Cell(flexcpText, i, COL_URIKIN) = f4103_get_indata((i - .FixedRows) / 2 + 1) + g_zenRel((i - .FixedRows) / 2).urikin
            End If
        Next
    End With
End Sub

Private Sub DrawLowerData()
    Dim i As Long
    Dim r As Long
    
    With fgDai
        For i = 0 To f1000_get_daisu() - 1
            r = i * 2 + .FixedRows + 1
            
            If mnuLowerAbsolute.Checked Then
                .Cell(flexcpText, r, COL_OUT) = f4102_get_out(i + 1) + g_daiRel(i).out
                .Cell(flexcpText, r, COL_SAFE) = f4102_get_safe(i + 1) + g_daiRel(i).safe
                .Cell(flexcpText, r, COL_START) = f4102_get_start(i + 1) + g_daiRel(i).start
                .Cell(flexcpText, r, COL_GEN1URI) = f4102_get_gen1uri(i + 1) + g_daiRel(i).gen1uri
                .Cell(flexcpText, r, COL_GEN2URI) = f4102_get_gen2uri(i + 1) + g_daiRel(i).gen2uri
            Else
                .Cell(flexcpText, r, COL_OUT) = g_daiRel(i).out
                .Cell(flexcpText, r, COL_SAFE) = g_daiRel(i).safe
                .Cell(flexcpText, r, COL_START) = g_daiRel(i).start
                .Cell(flexcpText, r, COL_GEN1URI) = g_daiRel(i).gen1uri
                .Cell(flexcpText, r, COL_GEN2URI) = g_daiRel(i).gen2uri
            End If
        Next
    End With

    With fgZenten
        For i = .FixedRows To .Rows - 1 Step 2
            If mnuLowerAbsolute.Checked Then
                .Cell(flexcpText, i + 1, COL_URIKIN) = f4103_get_indata((i - .FixedRows) / 2 + 1) + g_zenRel((i - .FixedRows) / 2).urikin
            Else
                .Cell(flexcpText, i + 1, COL_URIKIN) = g_zenRel((i - .FixedRows) / 2).urikin
            End If
        Next
    End With
End Sub

Private Sub fgDai_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    If m_fgDai_NowKeyAction And (Abs(NewRow - OldRow) = 1) Then
        If ((NewRow - fgDai.FixedRows) Mod 2) = 0 Then
            fgDai.Row = NewRow + IIf(NewRow > OldRow, 1, -1)
            If (fgDai.Row - fgDai.FixedRows) < 1 Then
                fgDai.Row = 1 + fgDai.FixedRows
            End If
        End If
    Else
        If ((NewRow - fgDai.FixedRows) Mod 2) = 0 Then
            fgDai.Row = NewRow + 1
        End If
    End If
End Sub

Private Sub fgDai_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    If (Row Mod 2) <> 0 Then
        Cancel = True
    End If
End Sub

Private Sub fgDai_GotFocus()
    mnuCopyAll.Enabled = True
    mnuCopySameSbt.Enabled = True
End Sub

Private Sub fgDai_KeyDown(KeyCode As Integer, Shift As Integer)
    m_fgDai_NowKeyAction = True
End Sub

Private Sub fgDai_KeyUp(KeyCode As Integer, Shift As Integer)
    m_fgDai_NowKeyAction = False
End Sub

Private Sub fgDai_LostFocus()
    mnuCopyAll.Enabled = False
    mnuCopySameSbt.Enabled = False
End Sub

Private Sub fgDai_SelChange()
    If fgDai.RowIsVisible(fgDai.Row) = False Then
        fgDai.ShowCell fgDai.Row, fgDai.Col
    End If
End Sub

Private Sub fgDai_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Dim rdb As Long
    Dim newRel As Long
    
    If IsNumeric(fgDai.EditText) = False Then
        Cancel = True
        Exit Sub
    End If
    
    If mnuLowerAbsolute.Checked = True And fgDai.EditText < 0 Then
        Cancel = True
        Exit Sub
    End If
        
    rdb = ((Row - fgDai.FixedRows) \ 2) + 1
    
    Select Case Col
    Case 3
        If mnuLowerAbsolute.Checked = True Then
            newRel = fgDai.EditText - f4102_get_out(rdb)
        Else
            newRel = fgDai.EditText
        End If
        set_out_rel rdb, newRel, mnuDataLink.Checked
    Case 4
        If mnuLowerAbsolute.Checked = True Then
            newRel = fgDai.EditText - f4102_get_safe(rdb)
        Else
            newRel = fgDai.EditText
        End If
        set_safe_rel rdb, newRel, mnuDataLink.Checked
    Case 5
        If mnuLowerAbsolute.Checked = True Then
            newRel = fgDai.EditText - f4102_get_start(rdb)
        Else
            newRel = fgDai.EditText
        End If
        set_start_rel rdb, newRel, mnuDataLink.Checked
    Case 6
        If mnuLowerAbsolute.Checked = True Then
            newRel = fgDai.EditText - f4102_get_gen1uri(rdb)
        Else
            newRel = fgDai.EditText
        End If
        set_gen1uri_rel rdb, newRel, mnuDataLink.Checked
    Case 7
        If mnuLowerAbsolute.Checked = True Then
            newRel = fgDai.EditText - f4102_get_gen2uri(rdb)
        Else
            newRel = fgDai.EditText
        End If
        set_gen2uri_rel rdb, newRel, mnuDataLink.Checked
    End Select
    
    DrawUppderData
    DrawLowerData
End Sub

Private Sub fgZenten_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    If m_fgZen_NowKeyAction And (Abs(NewRow - OldRow) = 1) Then
        If ((NewRow - fgZenten.FixedRows) Mod 2) = 0 Then
            fgZenten.Row = NewRow + IIf(NewRow > OldRow, 1, -1)
            If (fgZenten.Row - fgZenten.FixedRows) < 1 Then
                fgZenten.Row = 1 + fgZenten.FixedRows
            End If
        End If
    Else
        If ((NewRow - fgZenten.FixedRows) Mod 2) = 0 Then
            fgZenten.Row = NewRow + 1
        End If
    End If
End Sub

Private Sub fgZenten_KeyDown(KeyCode As Integer, Shift As Integer)
    m_fgZen_NowKeyAction = True
End Sub

Private Sub fgZenten_KeyUp(KeyCode As Integer, Shift As Integer)
    m_fgZen_NowKeyAction = False
End Sub

Private Sub Form_Load()
    Dim i As Long
    Dim r As Long
    Dim s As String
    
    lblUpperStat.Caption = mnuUpperNow.Caption
    lblLowerStat.Caption = mnuLowerAbsolute.Caption
    
    With fgDai
        .Redraw = flexRDNone
        
        .Cell(flexcpText, 0, 0) = "ID"
        .Cell(flexcpText, 0, 1) = "台番号"
        .Cell(flexcpText, 0, 2) = "種別"
        .Cell(flexcpText, 0, 3) = "アウト"
        .Cell(flexcpText, 0, 4) = "セーフ"
        .Cell(flexcpText, 0, 5) = "スタート"
        .Cell(flexcpText, 0, 6) = "現金1"
        .Cell(flexcpText, 0, 7) = "現金2"
        
        .MergeCells = flexMergeRestrictRows
        .MergeCol(0) = True
        .MergeCol(1) = True
        .MergeCol(2) = True
        
        .Editable = flexEDKbdMouse
        .AllowSelection = False
        
        .BackColor = RGB(255, 255, 192)
        .BackColorAlternate = vbWhite
        
        .Redraw = flexRDDirect
    End With
    
    With fgZenten
        .Redraw = flexRDNone
        
        .Cell(flexcpText, 0, 0) = "ID"
        .Cell(flexcpText, 0, 1) = "グループ番号"
        .Cell(flexcpText, 0, 2) = "個別番号"
        .Cell(flexcpText, 0, 3) = "種類"
        .Cell(flexcpText, 0, 4) = "現金"
        
        .MergeCells = flexMergeRestrictRows
        .MergeCol(0) = True
        .MergeCol(1) = True
        .MergeCol(2) = True
        .MergeCol(3) = True
        
        .Editable = flexEDKbdMouse
        .AllowSelection = False
        
        .BackColor = RGB(255, 255, 192)
        .BackColorAlternate = vbWhite
        
        .Redraw = flexRDDirect
    End With
    
    DrawFixed
    DrawUppderData
    DrawLowerData
    
    fgDai.Col = fgDai.FixedCols
    fgDai.Row = fgDai.FixedRows + 1

    fgZenten.Col = fgZenten.FixedCols
    fgZenten.Row = fgZenten.FixedRows + 1
End Sub

Private Sub Form_Resize()
    fgDai.Width = (Me.ScaleWidth / 2) - fgDai.Left - (fgDai.Left / 2)
    fgDai.Height = Me.ScaleHeight - fgDai.Top - ToTwipsY(4)
    
    fgZenten.Top = fgDai.Top
    fgZenten.Left = GetRight(fgDai) + ToTwipsX(4)
    fgZenten.Width = fgDai.Width
    fgZenten.Height = fgDai.Height
    
    lblZenten.Top = lblDai.Top
    lblZenten.Left = fgZenten.Left
End Sub

Private Sub mnuActRecover_Click()
    DataRecover
End Sub

Private Sub mnuCopySameSbt_Click()
    Dim rdb As Long
    Dim sbt As Long
    Dim src As Long
    Dim i As Long
    
    rdb = fgDai.Cell(flexcpText, fgDai.Row, 0)
    sbt = f1001_get_sbt(rdb)
    
    Select Case fgDai.Col
        Case COL_OUT
            src = g_daiRel(rdb - 1).out
        Case COL_SAFE
            src = g_daiRel(rdb - 1).safe
        Case COL_START
            src = g_daiRel(rdb - 1).start
        Case COL_GEN1URI
            src = g_daiRel(rdb - 1).gen1uri
        Case COL_GEN2URI
            src = g_daiRel(rdb - 1).gen2uri
        Case Else
            Exit Sub
    End Select
    
    For i = 0 To f1000_get_daisu - 1
        If f1001_get_sbt(i + 1) = sbt Then
            Select Case fgDai.Col
                Case COL_OUT
                    set_out_rel i + 1, src, mnuDataLink.Checked
                Case COL_SAFE
                    set_safe_rel i + 1, src, mnuDataLink.Checked
                Case COL_START
                    set_start_rel i + 1, src, mnuDataLink.Checked
                Case COL_GEN1URI
                    set_gen1uri_rel i + 1, src, mnuDataLink.Checked
                Case COL_GEN2URI
                    set_gen2uri_rel i + 1, src, mnuDataLink.Checked
                Case Else
                    Exit Sub
            End Select
        End If
    Next

    fgDai.Redraw = flexRDNone
    fgZenten.Redraw = flexRDNone
    
    DrawUppderData
    DrawLowerData

    fgDai.Redraw = flexRDDirect
    fgZenten.Redraw = flexRDDirect
End Sub

Private Sub mnuDataLink_Click()
    mnuDataLink.Checked = True
    mnuDataUnlink.Checked = False
End Sub

Private Sub mnuDataUnlink_Click()
    mnuDataLink.Checked = False
    mnuDataUnlink.Checked = True
End Sub

Private Sub mnuFileExit_Click()
    Unload Me
End Sub

'[ファイル]-[新規]の処理
'
Private Sub mnuFileNew_Click()
    NewDocument
    
    DrawFixed
    DrawUppderData
    DrawLowerData
    
    fgDai.Col = fgDai.FixedCols
    fgDai.Row = fgDai.FixedRows + 1

    fgZenten.Col = fgZenten.FixedCols
    fgZenten.Row = fgZenten.FixedRows + 1
End Sub

'[ファイル]-[開く]の処理
'
Private Sub mnuFileOpen_Click()
    cdlg.CancelError = True
    cdlg.DefaultExt = ".csv"
    cdlg.Filter = "CSVファイル (*.csv)|*.csv|すべてのファイル (*.*)|*.*"
    cdlg.Flags = cdlOFNHideReadOnly
    cdlg.FileName = GetDocumentName
    On Error Resume Next
    cdlg.ShowOpen
    If Err.Number = cdlCancel Then
        Exit Sub
    End If
    On Error GoTo 0
    
    OpenDocument cdlg.FileName
    
    DrawFixed
    DrawUppderData
    DrawLowerData
    
    fgDai.Col = fgDai.FixedCols
    fgDai.Row = fgDai.FixedRows + 1

    fgZenten.Col = fgZenten.FixedCols
    fgZenten.Row = fgZenten.FixedRows + 1
End Sub

'[ファイル]-[保存]の処理
'
Private Sub mnuFileSave_Click()
    SaveDocument GetDocumentName
End Sub

'[ファイル]-[名前を付けて保存]の処理
'
Private Sub mnuFileSaveAs_Click()
    cdlg.CancelError = True
    cdlg.DefaultExt = ".csv"
    cdlg.Filter = "CSVファイル (*.csv)|*.csv|すべてのファイル (*.*)|*.*"
    cdlg.Flags = cdlOFNHideReadOnly
    cdlg.FileName = GetDocumentName
    On Error Resume Next
    cdlg.ShowSave
    If Err.Number = cdlCancel Then
        Exit Sub
    End If
    On Error GoTo 0
    
    SaveDocument cdlg.FileName
End Sub

Private Sub mnuLowerAbsolute_Click()
    mnuLowerAbsolute.Checked = True
    mnuLowerRelative.Checked = False
    
    lblLowerStat.Caption = mnuLowerAbsolute.Caption
    
    DrawLowerData
End Sub

Private Sub mnuLowerRelative_Click()
    mnuLowerAbsolute.Checked = False
    mnuLowerRelative.Checked = True
    
    lblLowerStat.Caption = mnuLowerRelative.Caption
    
    DrawLowerData
End Sub

Private Sub mnuUpperNow_Click()
    mnuUpperNow.Checked = True
    mnuUpperRecovered.Checked = False
    
    lblUpperStat.Caption = mnuUpperNow.Caption
    
    DrawUppderData
End Sub

Private Sub mnuUpperRecovered_Click()
    mnuUpperNow.Checked = False
    mnuUpperRecovered.Checked = True
    
    lblUpperStat.Caption = mnuUpperRecovered.Caption
    
    DrawUppderData
End Sub
