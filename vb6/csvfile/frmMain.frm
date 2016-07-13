VERSION 5.00
Object = "{F13C99C0-4D73-11D2-B8B2-0000C00A958C}#7.0#0"; "fpSpr70.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   5955
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   8415
   LinkTopic       =   "Form1"
   ScaleHeight     =   5955
   ScaleWidth      =   8415
   StartUpPosition =   3  'Windows の既定値
   Begin MSComDlg.CommonDialog openDlg 
      Left            =   7680
      Top             =   480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin FPSpreadADO.fpSpread fp 
      Height          =   5055
      Left            =   240
      TabIndex        =   0
      Top             =   480
      Width           =   7215
      _Version        =   458752
      _ExtentX        =   12726
      _ExtentY        =   8916
      _StockProps     =   64
      SpreadDesigner  =   "frmMain.frx":0000
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
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_strFileName As String

Private Sub InitializeComponents()
    'コントロールの初期化
    With openDlg
        .CancelError = True
    End With
    
    With fp
        .MaxRows = 0
        .MaxCols = 0
    End With
End Sub

Private Sub AdjustComponents()
    'コントロールの位置合わせ
    With fp
        .Left = 0
        .Top = 0
        .Width = Me.ScaleWidth
        .Height = Me.ScaleHeight
    End With
End Sub

Private Sub ReadCSVFile(strFileName As String)
    Dim i As Long
    Dim csvArray As Variant
    Dim csvFile As clsCSVFile
    Set csvFile = New clsCSVFile
    
    With fp
        .MaxCols = 0
        .MaxRows = 0
    End With
    
    m_strFileName = strFileName
    
    csvFile.OpenReadFile strFileName
    Do Until csvFile.IsEOF
        csvArray = csvFile.ReadCSV
        
        If fp.MaxCols <= UBound(csvArray) Then
            fp.MaxCols = UBound(csvArray) + 1
        End If
        
        fp.MaxRows = fp.MaxRows + 1
        
        For i = 0 To UBound(csvArray)
            fp.SetText i + 1, fp.MaxRows, CStr(csvArray(i))
        Next
    Loop
    csvFile.CloseFile
    
    Set csvFile = Nothing
End Sub

Private Sub WriteCSVFile(strFileName As String)
    Dim i As Long
    Dim c As Long
    Dim csvArray As Variant
    Dim csvFile As clsCSVFile
    Set csvFile = New clsCSVFile
    
    ReDim csvArray(fp.MaxCols - 1)
    
    m_strFileName = strFileName
    
    csvFile.OpenWriteFile strFileName
    For i = 1 To fp.MaxRows
        For c = 1 To fp.MaxCols
            fp.GetText c, i, csvArray(c - 1)
        Next
        csvFile.WriteCSV csvArray
    Next
    csvFile.CloseFile
    
    Set csvFile = Nothing
End Sub

Private Sub Form_Load()
    InitializeComponents
End Sub

Private Sub Form_Resize()
    AdjustComponents
End Sub

Private Sub mnuFileOpen_Click()
    On Error Resume Next
    openDlg.ShowOpen
    If Err = 0 Then
        ReadCSVFile openDlg.FileName
    End If
    On Error GoTo 0
End Sub

Private Sub mnuFileSave_Click()
    WriteCSVFile m_strFileName
End Sub
