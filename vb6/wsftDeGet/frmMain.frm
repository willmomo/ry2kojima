VERSION 5.00
Object = "{C0A63B80-4B21-11D3-BD95-D426EF2C7949}#1.0#0"; "Vsflex7L.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "wsft de get"
   ClientHeight    =   7800
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9975
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7800
   ScaleWidth      =   9975
   StartUpPosition =   3  'Windows の既定値
   Begin VB.ComboBox cmbFiles 
      Height          =   300
      Left            =   1800
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   9
      Top             =   2280
      Width           =   3015
   End
   Begin VB.ComboBox cmbTenpo 
      Height          =   300
      ItemData        =   "frmMain.frx":0000
      Left            =   1800
      List            =   "frmMain.frx":0002
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   1
      Top             =   120
      Width           =   3015
   End
   Begin VB.CommandButton cmdClear 
      Caption         =   "リストのクリア(&C)"
      Height          =   375
      Left            =   5160
      TabIndex        =   10
      Top             =   2280
      Width           =   2055
   End
   Begin VB.TextBox txtHost 
      Height          =   375
      Index           =   2
      Left            =   1800
      TabIndex        =   7
      Text            =   "as3"
      Top             =   1560
      Width           =   3015
   End
   Begin VB.TextBox txtHost 
      Height          =   375
      Index           =   1
      Left            =   1800
      TabIndex        =   5
      Text            =   "as2"
      Top             =   1080
      Width           =   3015
   End
   Begin VSFlex7LCtl.VSFlexGrid fg 
      Height          =   4695
      Left            =   120
      TabIndex        =   11
      Top             =   2760
      Width           =   9615
      _cx             =   16960
      _cy             =   8281
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
   Begin VB.TextBox txtHost 
      Height          =   375
      Index           =   0
      Left            =   1800
      TabIndex        =   3
      Text            =   "as1"
      Top             =   600
      Width           =   3015
   End
   Begin VB.CommandButton cmdRecv 
      Caption         =   "受信(&R)"
      Default         =   -1  'True
      Height          =   375
      Left            =   7680
      TabIndex        =   12
      Top             =   2280
      Width           =   2055
   End
   Begin VB.Label lblTenpo 
      Caption         =   "店舗選択(&T)"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1335
   End
   Begin VB.Label lblList 
      Caption         =   "ファイル一覧(&L)"
      Height          =   255
      Left            =   240
      TabIndex        =   8
      Top             =   2303
      Width           =   1455
   End
   Begin VB.Label lblHost 
      Caption         =   "通信相手(&3):"
      Height          =   255
      Index           =   2
      Left            =   360
      TabIndex        =   6
      Top             =   1620
      Width           =   1095
   End
   Begin VB.Label lblHost 
      Caption         =   "通信相手(&2):"
      Height          =   255
      Index           =   1
      Left            =   360
      TabIndex        =   4
      Top             =   1140
      Width           =   1095
   End
   Begin VB.Label lblHost 
      Caption         =   "通信相手(&1):"
      Height          =   255
      Index           =   0
      Left            =   360
      TabIndex        =   2
      Top             =   660
      Width           =   1095
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub InitTenpoList()
    Dim i As Long
    Dim sTmp As String
    Dim sTkn() As String
    
    CBHAppendItem cmbTenpo, "(カスタム)", 0
    For i = 1 To 999
        sTmp = VBGetPrivateProfileString("TenpoList", "Tenpo" & i, "", App.Path & "\wsftDeGet.ini")
        If sTmp = "" Then Exit For
        
        sTkn = Split(sTmp, ",")
        CBHAppendItem cmbTenpo, sTkn(0), i
    Next
    
    cmbTenpo.ListIndex = 0
End Sub

Private Sub InitFileList()
    Dim i As Long
    Dim sTmp As String
    Dim sTkn() As String
    
    CBHAppendItem cmbFiles, "(カスタム)", 0
    For i = 1 To 999
        sTmp = VBGetPrivateProfileString("FileList", "File" & i, "", App.Path & "\wsftDeGet.ini")
        If sTmp = "" Then Exit For
        
        sTkn = Split(sTmp, ",")
        CBHAppendItem cmbFiles, sTkn(0), i
    Next
    
    cmbFiles.ListIndex = 0
End Sub

Private Sub cmbFiles_Click()
    Dim i As Long
    Dim sTmp As String
    Dim sTkn() As String
    
    With cmbFiles
        If .ItemData(.ListIndex) = 0 Then
            fg.Cell(flexcpText, 1, 0, fg.Rows - 1, fg.Cols - 1) = ""
        Else
            sTmp = VBGetPrivateProfileString("FileList", "File" & .ItemData(.ListIndex), "", App.Path & "\wsftDeGet.ini")
            sTkn = Split(sTmp, ",")
            
            fg.Cell(flexcpText, 1, 0, fg.Rows - 1, fg.Cols - 1) = ""
            For i = 1 To UBound(sTkn)
                fg.Cell(flexcpText, i, 0) = sTkn(i)
            Next
        End If
    End With
End Sub

Private Sub cmbTenpo_Click()
    Dim i As Long
    Dim sTmp As String
    Dim sTkn() As String
    
    With cmbTenpo
        If .ItemData(.ListIndex) = 0 Then
            For i = 0 To 2
                txtHost(i).Enabled = True
            Next
        Else
            sTmp = VBGetPrivateProfileString("TenpoList", "Tenpo" & .ItemData(.ListIndex), "", App.Path & "\wsftDeGet.ini")
            sTkn = Split(sTmp, ",")
            
            For i = 0 To 2
                If UBound(sTkn) - 1 >= i Then
                    txtHost(i).Text = sTkn(i + 1)
                Else
                    txtHost(i).Text = ""
                End If
                txtHost(i).Enabled = False
            Next
        End If
    End With
End Sub

Private Sub cmdClear_Click()
    fg.Cell(flexcpText, 1, 0, fg.Rows - 1, fg.Cols - 1) = ""
End Sub

Private Sub cmdRecv_Click()
    Dim buf() As Byte
    Dim ret As Long
    Dim fh As Integer
    Dim fno As Integer
    Dim nSize As Long
    Dim nTarget As Long
    Dim r As Long
    Dim nErrCnt As Long
    
    cmdRecv.Enabled = False
    
    With fg
        .Cell(flexcpText, 1, 2, .Rows - 1, .Cols - 1) = ""
        
        For r = 1 To .Rows - 1
            
            .ShowCell r, 0
            
            'ファイル番号取得
            If ToInteger(.Cell(flexcpText, r, 0), fno) = False Then
                Exit For
            End If
            
            'ターゲット番号取得
            If ToLong(.Cell(flexcpText, r, 1), nTarget) = False Then
                nTarget = 1
            End If
            
            .Cell(flexcpText, r, 3) = "ホスト情報設定中..."
            .Refresh
            
            ret = wsft_setservstr(txtHost(nTarget - 1).Text, 20001, 20002)
            If ret <> 0 Then
                cmdRecv.Enabled = True
                .Cell(flexcpText, r, 3) = wsft_error(ret)
                .Refresh
                Exit Sub
            End If
            
            .Cell(flexcpText, r, 3) = "ファイル情報取得中..."
            .Refresh
            
            ret = wsft_readfile(fno, 0, nSize, 0)
            If ret <> 0 Then
                .Cell(flexcpText, r, 3) = wsft_error(ret)
                .Refresh
                
                nErrCnt = nErrCnt + 1
            Else
                .Cell(flexcpText, r, 2) = nSize
                .Cell(flexcpText, r, 3) = "受信中..."
                .Refresh
                
                ReDim buf(nSize - 1)
                
                ret = wsft_readfile(fno, 0, buf(0), nSize)
                If ret = 0 Then
                    fh = FreeFile
                    Open "mf" & fno & "_" & nTarget & ".bin" For Binary As fh
                    Put fh, , buf
                    Close fh
                    .Cell(flexcpText, r, 3) = "正常終了"
                Else
                    .Cell(flexcpText, r, 3) = wsft_error(ret)
                End If
                .Refresh
            End If
            
            If nErrCnt >= 3 Then Exit For
        Next
    End With
    
    cmdRecv.Enabled = True
End Sub

Private Sub Form_Load()
    Dim ret As Long
    
    ret = wsft_start(Me.hwnd)
    If ret <> 0 Then
        MsgBox wsft_error(ret), vbOKOnly Or vbCritical, "wsft_start error"
        Unload Me
    End If
    
    With fg
        .Rows = 50
        .Cols = 4
        .FixedRows = 1
        .FixedCols = 0
        .AllowUserResizing = flexResizeBoth
        .Editable = flexEDKbdMouse
        
        .Cell(flexcpText, 0, 0) = "ファイル番号"
        .Cell(flexcpText, 0, 1) = "対象"
        .Cell(flexcpText, 0, 2) = "サイズ"
        .Cell(flexcpText, 0, 3) = "状態"
        
        .ColWidth(3) = 5000
    End With
    
    '店舗コンボボックスを初期化
    InitTenpoList
    
    'ファイルリストに初期値を入れておく
    InitFileList
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    wsft_end
End Sub

Private Sub txtHost_GotFocus(Index As Integer)
    TBHSelAll txtHost(Index)
End Sub
