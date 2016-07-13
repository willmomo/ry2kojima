VERSION 5.00
Object = "{C0A63B80-4B21-11D3-BD95-D426EF2C7949}#1.0#0"; "Vsflex7L.ocx"
Begin VB.Form frmMain 
   Caption         =   "環境チェッカー"
   ClientHeight    =   4695
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   10785
   LinkTopic       =   "Form1"
   ScaleHeight     =   4695
   ScaleWidth      =   10785
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton cmdCheck 
      Caption         =   "環境チェック"
      Height          =   495
      Left            =   60
      TabIndex        =   1
      Top             =   60
      Width           =   1215
   End
   Begin VSFlex7LCtl.VSFlexGrid fg 
      Height          =   3615
      Left            =   180
      TabIndex        =   0
      Top             =   780
      Width           =   8415
      _cx             =   14843
      _cy             =   6376
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
      Cols            =   3
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
   Begin VB.Label lblFile 
      Caption         =   "使用ファイル："
      Height          =   255
      Left            =   1380
      TabIndex        =   2
      Top             =   240
      Width           =   9135
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
      Begin VB.Menu mnuFilePrint 
         Caption         =   "印刷(&P)"
      End
      Begin VB.Menu mnuSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "終了(&X)"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "ヘルプ(&H)"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "バージョン情報(&A)..."
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub AppendMsg(GroupMsg, KomokuMsg, TestMsg)
    fg.Rows = fg.Rows + 1
    
    fg.Cell(flexcpText, fg.Rows - 1, 0) = GroupMsg
    fg.Cell(flexcpText, fg.Rows - 1, 1) = KomokuMsg
    fg.Cell(flexcpText, fg.Rows - 1, 2) = TestMsg
End Sub

Private Sub UpdateStatus(StatusMsg, Optional ErrorStat = False)
    fg.Cell(flexcpText, fg.Rows - 1, 3) = StatusMsg
    
    If ErrorStat Then
        fg.Cell(flexcpBackColor, fg.Rows - 1, 2, , fg.Cols - 1) = RGB(255, 0, 0)
    End If
End Sub

Private Sub CheckUsers(nodes As IXMLDOMNodeList)
    Dim i As Long
    Dim j As Long
    Dim ui3 As USER_INFO_3_VB
    Dim user_name As String
    
    For i = 0 To nodes.Length - 1
        user_name = nodes(i).Attributes.getNamedItem("name").Text
        
        'ユーザーの有無チェック
        AppendMsg "ユーザーチェック", user_name, "ユーザー有無"
        If VB_NetUserGetInfo(user_name, ui3) = NERR_Success Then
            UpdateStatus "ok"

            'パスワード期限チェック
            AppendMsg "ユーザーチェック", user_name, "パスワード期限"
            If (ui3.Flags And UF_DONT_EXPIRE_PASSWD) <> 0 Then
                UpdateStatus "ok"
            Else
                UpdateStatus "パスワードが無期限ではありません。", True
            End If
        Else
            UpdateStatus "ユーザーが存在しません。", True
        End If
    Next
End Sub

Private Sub CheckShares(nodes As IXMLDOMNodeList)
    Dim i As Long
    Dim j As Long
    Dim si2 As SHARE_INFO_2_VB
    Dim share_name As String
    Dim dont_expire_password As Long
    Dim node As IXMLDOMNode
    
    For i = 0 To nodes.Length - 1
        share_name = nodes(i).Attributes.getNamedItem("name").Text
        
        'ユーザーの有無チェック
        AppendMsg "共有チェック", share_name, "共有フォルダの有無"
        If VB_NetShareGetInfo(share_name, si2) = NERR_Success Then
            fg.Cell(flexcpText, fg.Rows - 1, 3) = "ok"
            
            Set node = GetNamedChildNode(nodes(i), "path")
            
            ' 共有されている物理パスのチェック
            AppendMsg "共有チェック", share_name, "物理パスのチェック"
            If UCase(node.Text) = UCase(si2.Path) Then
                fg.Cell(flexcpText, fg.Rows - 1, 3) = "ok (" & si2.Path & ")"
            Else
                fg.Cell(flexcpText, fg.Rows - 1, 3) = "共有フォルダの場所が違います。"
                fg.Cell(flexcpBackColor, fg.Rows - 1, 2, , fg.Cols - 1) = RGB(255, 0, 0)
            End If
                
            ' 共有されている物理パスが存在するかどうかのチェック
            AppendMsg "共有チェック", share_name, "物理パスの存在チェック"
            Dim objFS As FileSystemObject
            Set objFS = New FileSystemObject
            
            If objFS.FolderExists(si2.Path) Then
                fg.Cell(flexcpText, fg.Rows - 1, 3) = "ok"
            Else
                fg.Cell(flexcpText, fg.Rows - 1, 3) = "物理パスが存在しません。"
                fg.Cell(flexcpBackColor, fg.Rows - 1, 2, , fg.Cols - 1) = RGB(255, 0, 0)
            End If
            Set objFS = Nothing
        Else
            fg.Cell(flexcpText, fg.Rows - 1, 3) = "共有フォルダがが存在しません。"
            fg.Cell(flexcpBackColor, fg.Rows - 1, 2, , fg.Cols - 1) = RGB(255, 0, 0)
        End If
    Next
End Sub

Private Sub cmdCheck_Click()
    Dim xml As DOMDocument
    
    Set xml = New DOMDocument
    
    If xml.Load(App.Path & "\checklist.xml") Then
        fg.Rows = 1
        
        CheckUsers xml.getElementsByTagName("checklist/users/user")
        CheckShares xml.getElementsByTagName("checklist/shares/share")
        
        fg.AutoSize 0, fg.Cols - 1
    End If
    
    Set xml = Nothing
End Sub

Private Sub Form_Load()
    frmMain.Caption = App.Title
    
    lblFile.Caption = "使用ファイル：" & App.Path & "\checklist.xml"
    
    fg.Cols = 4
    fg.Rows = 1
    fg.FixedCols = 0
    fg.FixedRows = 1
    fg.AllowUserResizing = flexResizeBoth
    'fg.GridLinesFixed = flexGridExplorer
    'fg.ExplorerBar = flexExMove
    
    fg.MergeCells = flexMergeRestrictAll
    fg.MergeCol(-1) = True
    
    fg.OutlineBar = flexOutlineBarComplete
    
    fg.Cell(flexcpText, 0, 0) = "グループ"
    fg.Cell(flexcpText, 0, 1) = "対象"
    fg.Cell(flexcpText, 0, 2) = "項目"
    fg.Cell(flexcpText, 0, 3) = "ステータス"
End Sub

Private Sub Form_Resize()
    fg.Left = 0
    fg.Top = cmdCheck.Top + cmdCheck.Height + (cmdCheck.Top)
    fg.Width = Me.ScaleWidth
    fg.Height = Max(0, Me.ScaleHeight - fg.Top)
End Sub

Private Sub mnuFileExit_Click()
    Unload Me
End Sub

Private Sub mnuFilePrint_Click()
    fg.PrintGrid , , 2
End Sub

Private Sub mnuHelpAbout_Click()
    frmAbout.Show vbModal, Me
End Sub
