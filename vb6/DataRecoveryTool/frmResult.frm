VERSION 5.00
Object = "{C0A63B80-4B21-11D3-BD95-D426EF2C7949}#1.0#0"; "Vsflex7L.ocx"
Begin VB.Form frmResult 
   Caption         =   "復旧結果"
   ClientHeight    =   8490
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6780
   LinkTopic       =   "Form1"
   ScaleHeight     =   8490
   ScaleWidth      =   6780
   StartUpPosition =   2  '画面の中央
   Begin VSFlex7LCtl.VSFlexGrid fgDaiPrn 
      Height          =   1275
      Left            =   60
      TabIndex        =   3
      Top             =   5940
      Visible         =   0   'False
      Width           =   6675
      _cx             =   11774
      _cy             =   2249
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
      Cols            =   9
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
   Begin VB.CommandButton cmdPrint 
      Caption         =   "印刷"
      Height          =   495
      Left            =   4020
      TabIndex        =   2
      Top             =   7860
      Width           =   1215
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Height          =   495
      Left            =   2700
      TabIndex        =   1
      Top             =   7860
      Width           =   1215
   End
   Begin VSFlex7LCtl.VSFlexGrid fgZenten 
      Height          =   5595
      Left            =   60
      TabIndex        =   0
      Top             =   60
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
      AllowUserResizing=   1
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
      FormatString    =   $"frmResult.frx":0000
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
End
Attribute VB_Name = "frmResult"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Enum ZEN_COL_NAME
    COL_URIKIN = 4
End Enum

'
' 固定部分(台番号や販売機番号)の表示
'
Private Sub DrawFixed()
    Dim i As Long
    Dim r As Long
    Dim s As String
    
    With fgZenten
        .Rows = .FixedRows
        
        For i = 1 To MAX_ZENTENSU
            If f1002_get_kind(i) > 0 And f1002_get_sbt(i) <> 0 Then
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

'
' 各台/販売機の上段データ(元値/更新後値)を表示
'
Private Sub DrawUppderData()
    Dim i As Long
    Dim r As Long
    Dim idx As Long
    
    With fgZenten
        For i = .FixedRows To .Rows - 1 Step 2
            idx = .Cell(flexcpText, i, 0)
            .Cell(flexcpText, i, COL_URIKIN) = f4103_get_kingaku(idx)
        Next
    End With
End Sub

'
' 各台/販売機の下段データ(絶対修正値/相対修正値)を表示
'
Private Sub DrawLowerData()
    Dim i As Long
    Dim r As Long
    Dim idx As Long

    With fgZenten
        For i = .FixedRows To .Rows - 1 Step 2
            idx = .Cell(flexcpText, i, 0)
            
            .Cell(flexcpText, i + 1, COL_URIKIN) = indata_to_kingaku(idx, bld_result(idx - 1).result_data)
            
            If .Cell(flexcpText, i, COL_URIKIN) <> .Cell(flexcpText, i + 1, COL_URIKIN) Then
                .Cell(flexcpForeColor, i + 1, COL_URIKIN) = RGB(255, 0, 0)
            End If
        Next
    End With
End Sub

Private Sub cmdOK_Click()
    Unload Me
End Sub

Private Sub cmdPrint_Click()
    Dim i As Long
    
    Me.MousePointer = vbHourglass
    
    fgZenten.PrintGrid
    
''    With fgDaiPrn
''        .Rows = .FixedRows + err_count
''        For i = 0 To err_count - 1
''            .Cell(flexcpText, i + .FixedRows, 0 + .FixedCols) = i + 1
''            .Cell(flexcpText, i + .FixedRows, 1 + .FixedCols) = err_buffer(i).rdb
''            .Cell(flexcpText, i + .FixedRows, 2 + .FixedCols) = "0x" & Right("0000" & Hex(err_buffer(i).errflag), 4)
''            .Cell(flexcpText, i + .FixedRows, 3 + .FixedCols) = f1001_get_hdb(err_buffer(i).rdb)
''            .Cell(flexcpText, i + .FixedRows, 4 + .FixedCols) = f4102_get_out(err_buffer(i).rdb)
''            .Cell(flexcpText, i + .FixedRows, 5 + .FixedCols) = f4102_get_safe(err_buffer(i).rdb)
''            .Cell(flexcpText, i + .FixedRows, 6 + .FixedCols) = f4102_get_start(err_buffer(i).rdb)
''            .Cell(flexcpText, i + .FixedRows, 7 + .FixedCols) = f4102_get_gen1uri(err_buffer(i).rdb)
''            .Cell(flexcpText, i + .FixedRows, 8 + .FixedCols) = f4102_get_gen2uri(err_buffer(i).rdb)
''        Next
''
''        .PrintGrid
''    End With
    
    Me.MousePointer = vbDefault
End Sub

Private Sub Form_Load()
    With fgZenten
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
        
        .Editable = flexEDNone
        .AllowSelection = False
        
        .BackColor = RGB(255, 255, 192)
        .BackColorAlternate = vbWhite
        
        .ColFormat(4) = "$#,###"
    End With
    
    DrawFixed
    DrawUppderData
    DrawLowerData
End Sub

Private Sub Form_Resize()
    cmdOK.Left = (Me.ScaleWidth - (cmdOK.Width + ToTwipsX(5) + cmdPrint.Width)) / 2
    cmdOK.Top = Me.ScaleHeight - ToTwipsY(5) - cmdOK.Height
    
    cmdPrint.Left = GetRight(cmdOK) + ToTwipsX(5)
    cmdPrint.Top = cmdOK.Top
    
    fgZenten.Top = 0
    fgZenten.Left = 0
    fgZenten.Width = Me.ScaleWidth
    fgZenten.Height = Me.ScaleHeight - cmdOK.Height - ToTwipsY(10)
End Sub
