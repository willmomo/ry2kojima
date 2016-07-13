VERSION 5.00
Object = "{C0A63B80-4B21-11D3-BD95-D426EF2C7949}#1.0#0"; "Vsflex7L.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   9165
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   12240
   LinkTopic       =   "Form1"
   ScaleHeight     =   9165
   ScaleWidth      =   12240
   StartUpPosition =   3  'Windows の既定値
   Begin MSComDlg.CommonDialog saveDlg 
      Left            =   1500
      Top             =   7620
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComDlg.CommonDialog openDlg 
      Left            =   600
      Top             =   7620
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VSFlex7LCtl.VSFlexGrid fgType 
      Height          =   6495
      Left            =   6240
      TabIndex        =   1
      Top             =   420
      Width           =   4995
      _cx             =   8811
      _cy             =   11456
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
   Begin VSFlex7LCtl.VSFlexGrid fgKishu 
      Height          =   6555
      Left            =   240
      TabIndex        =   0
      Top             =   420
      Width           =   4995
      _cx             =   8811
      _cy             =   11562
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
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
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
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private m_strFileName As String     '開いているファイルの名前


'**********************************************************************
'開いているドキュメントにあわせてWindow Titleを設定する
'**********************************************************************
Private Sub SetTitle()

    If m_strFileName = "" Then
        Me.Caption = "(無題) - 機種・タイプ設定ビューア"
    Else
        Me.Caption = m_strFileName & " - 機種・タイプ設定ビューア"
    End If
    
End Sub


'**********************************************************************
'新しいドキュメントを開く
'**********************************************************************
Private Sub NewDocument()

    '開いているファイル名を空にする
    m_strFileName = ""
    
    'セルを空にする
    With fgKishu
        .Cell(flexcpText, .FixedRows, .FixedCols, .Rows - 1) = ""
        .Cell(flexcpText, .FixedRows, .FixedCols + 1, .Rows - 1) = 0
    End With
    
    With fgType
        .Cell(flexcpText, .FixedRows, .FixedCols, .Rows - 1, .Cols - 1) = ""
    End With
    
    'ウィンドウタイトルを更新
    SetTitle
    
End Sub

 
'**********************************************************************
'ドキュメントを開く
'**********************************************************************
Private Sub OpenDocument(strFileName As String)

    Dim objKishuTypeSet As clsKishuTypeSet
    Set objKishuTypeSet = New clsKishuTypeSet
    
    Dim errMsg As String
    If objKishuTypeSet.LoadFile(strFileName, errMsg) Then
    
        Dim i As Long
        
        With fgKishu
            For i = .FixedRows To .Rows - 1
                .Cell(flexcpText, i, .FixedCols) = objKishuTypeSet.KishuSet(i - .FixedRows + 1).KishuName
                .Cell(flexcpText, i, .FixedCols + 1) = objKishuTypeSet.KishuSet(i - .FixedRows + 1).TypeNo
            Next
        End With
    
        With fgType
            For i = .FixedRows To .Rows - 1
                .Cell(flexcpText, i, .FixedCols) = objKishuTypeSet.TypeSet(i - .FixedRows + 1).TypeName
            Next
        End With
        
        m_strFileName = strFileName
    
        'ウィンドウタイトルを更新
        SetTitle
    Else
        'ファイルオープンエラー
        MsgBox "ファイル '" & strFileName & "' がオープンできません。" & vbNewLine & vbNewLine & _
            String(80, "-") & vbNewLine & _
            errMsg, vbCritical Or vbOKOnly, "ファイルオープンエラー"
    End If
End Sub


'**********************************************************************
'ドキュメントを保存する
'**********************************************************************
Private Sub SaveDocument(strFileName As String)

    Dim objKishuTypeSet As clsKishuTypeSet
    Set objKishuTypeSet = New clsKishuTypeSet
    
    Dim i As Long
    
    With fgKishu
        For i = 1 To 96
            objKishuTypeSet.KishuSet(i).KishuName = .Cell(flexcpText, .FixedRows + i - 1, 1)
            objKishuTypeSet.KishuSet(i).TypeNo = .Cell(flexcpText, .FixedRows + i - 1, 2)
        Next
    End With

    With fgType
        For i = 1 To 16
            objKishuTypeSet.TypeSet(i).TypeName = .Cell(flexcpText, .FixedRows + i - 1, 1)
        Next
    End With
    
    Dim errMsg As String
    If objKishuTypeSet.SaveFile(strFileName, errMsg) Then
        
        '正常に保存できたときは、管理しているファイル名を変更する
        m_strFileName = strFileName

        'ウィンドウタイトルを更新
        SetTitle
    Else
        
        '保存に失敗したときは、エラーメッセージを表示
        MsgBox "ファイル '" & strFileName & "' に保存できません。" & vbNewLine & vbNewLine & errMsg, _
            vbOKOnly Or vbCritical, "ファイル保存エラー"
    End If

End Sub


'**********************************************************************
'ドキュメントを閉じる
'**********************************************************************
Private Sub CloseDocument()

    NewDocument
    
End Sub


'**********************************************************************
'フォーム上のコントロールを整列させる
'**********************************************************************
Private Sub AdjustComponent()

    'fgKishu
    With fgKishu
        .Left = 0
        .Top = 0
        .Width = Me.ScaleWidth / 2
        .Height = Me.ScaleHeight
    End With
    
    'fgType
    With fgType
        .Left = fgKishu.Left + fgKishu.Width
        .Top = 0
        .Width = Me.ScaleWidth / 2
        .Height = Me.ScaleHeight
    End With
    
End Sub


'**********************************************************************
'フォーム上のコントロールを初期化する
'
'[備考]
'位置合わせは、Resizeイベントで行う
'**********************************************************************
Private Sub InitializeComponent()

    Dim i As Long
    
    'fgKishu
    With fgKishu
        .Cols = 3
        .Rows = 96 + 1
        .FixedCols = 1
        .FixedRows = 1
        
        .Cell(flexcpText, 0, 0) = "機種番号"
        .Cell(flexcpText, 0, 1) = "機種名"
        .Cell(flexcpText, 0, 2) = "タイプ番号"
        .ColWidth(1) = 2700
        .ColEditMask(2) = "90"
        
        For i = 1 To 96
            .Cell(flexcpText, .FixedRows + i - 1, 0) = "機種" & CStr(i)
        Next
        
        .AllowUserResizing = flexResizeColumns
        .Editable = flexEDKbdMouse
    End With
    
    'fgType
    With fgType
        .Cols = 2
        .Rows = 16 + 1
        .FixedCols = 1
        .FixedRows = 1
    
        .Cell(flexcpText, 0, 0) = "タイプ番号"
        .Cell(flexcpText, 0, 1) = "タイプ名"
        .ColWidth(1) = 2700
        
        For i = 1 To 16
            .Cell(flexcpText, .FixedRows + i - 1, 0) = "タイプ" & CStr(i)
        Next
    
        .AllowUserResizing = flexResizeColumns
        .Editable = flexEDKbdMouse
    End With
    
    'openDlg
    With openDlg
        .CancelError = True
        .Filter = "XMLファイル(*.xml)|*.xml|すべてのファイル(*.*)|*.*"
        .Flags = cdlOFNFileMustExist Or cdlOFNHideReadOnly
    End With
    
    'saveDlg
    With saveDlg
        .CancelError = True
        .Filter = "XMLファイル(*.xml)|*.xml|すべてのファイル(*.*)|*.*"
        .Flags = cdlOFNOverwritePrompt
    End With
    
End Sub


'**********************************************************************
'Loadイベント
'**********************************************************************
Private Sub Form_Load()

    InitializeComponent
    
    NewDocument
    
End Sub


'**********************************************************************
'Resizeイベント
'**********************************************************************
Private Sub Form_Resize()
    
    AdjustComponent
    
End Sub


'**********************************************************************
'メニュー[ファイル]-[終了]処理
'**********************************************************************
Private Sub mnuFileExit_Click()

    Unload Me
    
End Sub


'**********************************************************************
'メニュー[ファイル]-[開く]処理
'**********************************************************************
Private Sub mnuFileOpen_Click()

    On Error Resume Next
    openDlg.ShowOpen
    If Err.Number = 0 Then
    
        '選択したファイルを開く
        OpenDocument openDlg.FileName
        
    End If
    On Error GoTo 0
    
End Sub


'**********************************************************************
'メニュー[ファイル]-[保存]処理
'**********************************************************************
Private Sub mnuFileSave_Click()

    If m_strFileName = "" Then
    
        'ドキュメントを開いていないときは、名前を付けて保存を実行
        mnuFileSaveAs_Click
    Else
    
        '開いているときは、そのファイルに保存
        SaveDocument m_strFileName
    End If
    
End Sub


'**********************************************************************
'メニュー[ファイル]-[名前を付けて保存]処理
'**********************************************************************
Private Sub mnuFileSaveAs_Click()

    '現在のファイル名を初期値として表示
    saveDlg.FileName = m_strFileName
    
    On Error Resume Next
    saveDlg.ShowSave
    If Err.Number = 0 Then
    
        SaveDocument saveDlg.FileName
        
    End If
    On Error GoTo 0
    
End Sub
