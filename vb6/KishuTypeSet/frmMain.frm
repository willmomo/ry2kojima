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
   StartUpPosition =   3  'Windows �̊���l
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
         Name            =   "�l�r �o�S�V�b�N"
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
         Name            =   "�l�r �o�S�V�b�N"
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
      Caption         =   "�t�@�C��(&F)"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "�J��(&O)..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "�ۑ�(&S)"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "���O��t���ĕۑ�(&A)..."
      End
      Begin VB.Menu mnuFileSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "�I��(&X)"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private m_strFileName As String     '�J���Ă���t�@�C���̖��O


'**********************************************************************
'�J���Ă���h�L�������g�ɂ��킹��Window Title��ݒ肷��
'**********************************************************************
Private Sub SetTitle()

    If m_strFileName = "" Then
        Me.Caption = "(����) - �@��E�^�C�v�ݒ�r���[�A"
    Else
        Me.Caption = m_strFileName & " - �@��E�^�C�v�ݒ�r���[�A"
    End If
    
End Sub


'**********************************************************************
'�V�����h�L�������g���J��
'**********************************************************************
Private Sub NewDocument()

    '�J���Ă���t�@�C��������ɂ���
    m_strFileName = ""
    
    '�Z������ɂ���
    With fgKishu
        .Cell(flexcpText, .FixedRows, .FixedCols, .Rows - 1) = ""
        .Cell(flexcpText, .FixedRows, .FixedCols + 1, .Rows - 1) = 0
    End With
    
    With fgType
        .Cell(flexcpText, .FixedRows, .FixedCols, .Rows - 1, .Cols - 1) = ""
    End With
    
    '�E�B���h�E�^�C�g�����X�V
    SetTitle
    
End Sub

 
'**********************************************************************
'�h�L�������g���J��
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
    
        '�E�B���h�E�^�C�g�����X�V
        SetTitle
    Else
        '�t�@�C���I�[�v���G���[
        MsgBox "�t�@�C�� '" & strFileName & "' ���I�[�v���ł��܂���B" & vbNewLine & vbNewLine & _
            String(80, "-") & vbNewLine & _
            errMsg, vbCritical Or vbOKOnly, "�t�@�C���I�[�v���G���["
    End If
End Sub


'**********************************************************************
'�h�L�������g��ۑ�����
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
        
        '����ɕۑ��ł����Ƃ��́A�Ǘ����Ă���t�@�C������ύX����
        m_strFileName = strFileName

        '�E�B���h�E�^�C�g�����X�V
        SetTitle
    Else
        
        '�ۑ��Ɏ��s�����Ƃ��́A�G���[���b�Z�[�W��\��
        MsgBox "�t�@�C�� '" & strFileName & "' �ɕۑ��ł��܂���B" & vbNewLine & vbNewLine & errMsg, _
            vbOKOnly Or vbCritical, "�t�@�C���ۑ��G���["
    End If

End Sub


'**********************************************************************
'�h�L�������g�����
'**********************************************************************
Private Sub CloseDocument()

    NewDocument
    
End Sub


'**********************************************************************
'�t�H�[����̃R���g���[���𐮗񂳂���
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
'�t�H�[����̃R���g���[��������������
'
'[���l]
'�ʒu���킹�́AResize�C�x���g�ōs��
'**********************************************************************
Private Sub InitializeComponent()

    Dim i As Long
    
    'fgKishu
    With fgKishu
        .Cols = 3
        .Rows = 96 + 1
        .FixedCols = 1
        .FixedRows = 1
        
        .Cell(flexcpText, 0, 0) = "�@��ԍ�"
        .Cell(flexcpText, 0, 1) = "�@�햼"
        .Cell(flexcpText, 0, 2) = "�^�C�v�ԍ�"
        .ColWidth(1) = 2700
        .ColEditMask(2) = "90"
        
        For i = 1 To 96
            .Cell(flexcpText, .FixedRows + i - 1, 0) = "�@��" & CStr(i)
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
    
        .Cell(flexcpText, 0, 0) = "�^�C�v�ԍ�"
        .Cell(flexcpText, 0, 1) = "�^�C�v��"
        .ColWidth(1) = 2700
        
        For i = 1 To 16
            .Cell(flexcpText, .FixedRows + i - 1, 0) = "�^�C�v" & CStr(i)
        Next
    
        .AllowUserResizing = flexResizeColumns
        .Editable = flexEDKbdMouse
    End With
    
    'openDlg
    With openDlg
        .CancelError = True
        .Filter = "XML�t�@�C��(*.xml)|*.xml|���ׂẴt�@�C��(*.*)|*.*"
        .Flags = cdlOFNFileMustExist Or cdlOFNHideReadOnly
    End With
    
    'saveDlg
    With saveDlg
        .CancelError = True
        .Filter = "XML�t�@�C��(*.xml)|*.xml|���ׂẴt�@�C��(*.*)|*.*"
        .Flags = cdlOFNOverwritePrompt
    End With
    
End Sub


'**********************************************************************
'Load�C�x���g
'**********************************************************************
Private Sub Form_Load()

    InitializeComponent
    
    NewDocument
    
End Sub


'**********************************************************************
'Resize�C�x���g
'**********************************************************************
Private Sub Form_Resize()
    
    AdjustComponent
    
End Sub


'**********************************************************************
'���j���[[�t�@�C��]-[�I��]����
'**********************************************************************
Private Sub mnuFileExit_Click()

    Unload Me
    
End Sub


'**********************************************************************
'���j���[[�t�@�C��]-[�J��]����
'**********************************************************************
Private Sub mnuFileOpen_Click()

    On Error Resume Next
    openDlg.ShowOpen
    If Err.Number = 0 Then
    
        '�I�������t�@�C�����J��
        OpenDocument openDlg.FileName
        
    End If
    On Error GoTo 0
    
End Sub


'**********************************************************************
'���j���[[�t�@�C��]-[�ۑ�]����
'**********************************************************************
Private Sub mnuFileSave_Click()

    If m_strFileName = "" Then
    
        '�h�L�������g���J���Ă��Ȃ��Ƃ��́A���O��t���ĕۑ������s
        mnuFileSaveAs_Click
    Else
    
        '�J���Ă���Ƃ��́A���̃t�@�C���ɕۑ�
        SaveDocument m_strFileName
    End If
    
End Sub


'**********************************************************************
'���j���[[�t�@�C��]-[���O��t���ĕۑ�]����
'**********************************************************************
Private Sub mnuFileSaveAs_Click()

    '���݂̃t�@�C�����������l�Ƃ��ĕ\��
    saveDlg.FileName = m_strFileName
    
    On Error Resume Next
    saveDlg.ShowSave
    If Err.Number = 0 Then
    
        SaveDocument saveDlg.FileName
        
    End If
    On Error GoTo 0
    
End Sub
