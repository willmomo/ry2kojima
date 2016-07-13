VERSION 5.00
Object = "{92D71E90-25A8-11CF-A640-9986B64D9618}#6.0#0"; "Olch2x32.ocx"
Object = "{B02F3647-766B-11CE-AF28-C3A2FBE76A13}#3.0#0"; "SPR32X30.ocx"
Object = "{6FBA474E-43AC-11CE-9A0E-00AA0062BB4C}#1.0#0"; "sysinfo.ocx"
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   5970
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13875
   LinkTopic       =   "Form1"
   ScaleHeight     =   5970
   ScaleWidth      =   13875
   StartUpPosition =   3  'Windows ÇÃä˘íËíl
   Begin SysInfoLib.SysInfo sinfo 
      Left            =   6720
      Top             =   120
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
   End
   Begin VB.ComboBox cmbChartType 
      Height          =   300
      Left            =   10080
      Style           =   2  'ƒﬁ€ØÃﬂ¿ﬁ≥› ÿΩƒ
      TabIndex        =   2
      Top             =   120
      Width           =   1215
   End
   Begin VB.ComboBox cmbData 
      Height          =   300
      ItemData        =   "frmMain.frx":0000
      Left            =   8760
      List            =   "frmMain.frx":0002
      Style           =   2  'ƒﬁ€ØÃﬂ¿ﬁ≥› ÿΩƒ
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
   Begin FPSpread.vaSpread fp 
      Height          =   5295
      Left            =   8760
      TabIndex        =   0
      Top             =   480
      Width           =   4935
      _Version        =   196608
      _ExtentX        =   8705
      _ExtentY        =   9340
      _StockProps     =   64
      SpreadDesigner  =   "frmMain.frx":0004
   End
   Begin OlectraChart2D.Chart2D c2d 
      Height          =   5175
      Left            =   120
      TabIndex        =   3
      Top             =   600
      Width           =   8490
      _Version        =   393216
      _Revision       =   1
      _ExtentX        =   14975
      _ExtentY        =   9128
      _StockProps     =   0
      ControlProperties=   "frmMain.frx":0190
   End
   Begin VB.Label Label1 
      Caption         =   "ëSìXèWåv"
      BeginProperty Font 
         Name            =   "ÇlÇr ÇoÉSÉVÉbÉN"
         Size            =   15.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   1335
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'----------------------------------------------------------------------
' íËêî

Private Const s_db_name As String = "teij.mdb"

'----------------------------------------------------------------------
' [äTóv]
' y-axisÇí â›å`éÆÇ…Ç∑ÇÈ
'----------------------------------------------------------------------

Private Sub SetCurrencyFormat()
    With c2d.ChartArea.Axes("Y").LabelFormat
        .Category = oc2dCategoryCurrency
        .Currency.DecimalPlaces = 0
    End With
End Sub

'----------------------------------------------------------------------
' [äTóv]
' y-axisêîílâ›å`éÆÇ…Ç∑ÇÈ
'----------------------------------------------------------------------

Private Sub SetNumberFormat()
    With c2d.ChartArea.Axes("Y").LabelFormat
        .Category = oc2dCategoryNumber
        .Number.DecimalPlaces = 0
    End With
End Sub

Private Sub UpdateGraph(ByVal n As Long)
    Dim i As Long
    Dim v As Variant
    
    With c2d
        .IsBatched = True
        
        With .ChartGroups(1)
            frmMain.fp.GetText n, 1, v
            .SeriesLabels(n).Text = v
        End With
        
        With .ChartGroups(1).Data
            For i = 1 To 16
                frmMain.fp.GetText n, i + 1, v
                
                If v = 0 Or v = "" Then
                    .Y(n, i) = .HoleValue
                Else
                    .Y(n, i) = v
                End If
            Next
        End With
        
        .IsBatched = False
    End With

End Sub

Private Sub UpdateData(ByVal Col As Long, ByVal Row As Long)
    Dim db As Connection
    Dim rs As Recordset
    Dim sql As String
    Dim npos As Long
    
    fp.Col = Col
    fp.Row = Row
    If fp.Text = "" Then
        fp.Col = Col
        fp.Col2 = Col
        fp.Row = 2
        fp.Row2 = fp.MaxRows
        fp.BlockMode = True
        fp.Text = ""
        fp.BlockMode = False
        
        UpdateGraph Col
    Else
        Set db = New Connection
        Set rs = New Recordset
        
        db.Open "Driver={Microsoft Access Driver (*.mdb)}; DBQ=" & App.Path & "\" & s_db_name
        
        sql = "select kado, urikin, (d_keikin/urikin)*100 as ewari from Teij where âcã∆ì˙=#" & fp.Text & "# order by rec;"
        
        rs.Open sql, db, adOpenForwardOnly, adLockReadOnly
        
        npos = 2
        Do Until rs.EOF
            If cmbData.ListIndex = 0 Then
                fp.SetText Col, npos, rs("urikin")
            ElseIf cmbData.ListIndex = 1 Then
                fp.SetText Col, npos, rs("kado")
            Else
                fp.SetText Col, npos, rs("ewari")
            End If
            npos = npos + 1
            
            rs.MoveNext
        Loop
        
        rs.Close
        db.Close
        
        Set rs = Nothing
        Set db = Nothing
    
        UpdateGraph Col
    End If

End Sub

Private Function EigyoDays() As String
    Dim db As Connection
    Dim rs As Recordset
    Dim sql As String
    Dim pos As Long
    
    Set db = New Connection
    Set rs = New Recordset
    
    db.Open "Driver={Microsoft Access Driver (*.mdb)}; DBQ=" & App.Path & "\" & s_db_name
    
    sql = "select âcã∆ì˙ from Teij group by âcã∆ì˙ order by âcã∆ì˙;"
    
    rs.Open sql, db, adOpenKeyset, adLockOptimistic
    
    Do Until rs.EOF
        If pos <> 0 Then
            EigyoDays = EigyoDays & vbTab
        End If
        
        EigyoDays = EigyoDays & CStr(rs("âcã∆ì˙"))
        pos = pos + 1
        rs.MoveNext
    Loop
    
    rs.Close
    db.Close
    
    Set rs = Nothing
    Set db = Nothing
End Function

Private Sub cmbChartType_Click()
    If cmbChartType.ListIndex = 0 Then
        c2d.ChartGroups(1).ChartType = oc2dTypePlot
    Else
        c2d.ChartGroups(1).ChartType = oc2dTypeBar
    End If
End Sub

Private Sub cmbData_Click()
    If cmbData.ListIndex = 0 Then
        'y-axisÇí â›å^Ç…Ç∑ÇÈ
        SetCurrencyFormat
    Else
        'y-axisÇêîílå^Ç…Ç∑ÇÈ
        SetNumberFormat
    End If
    
    UpdateData 1, 1
    UpdateData 2, 1
    UpdateData 3, 1
End Sub

Private Sub Form_Load()
    Dim i As Long
    
    cmbData.AddItem "îÑè„"
    cmbData.AddItem "â“ìÆ"
    cmbData.AddItem "ã@äBäÑêî"
    
    cmbChartType.AddItem "ê‹ê¸ÉOÉâÉt"
    cmbChartType.AddItem "ñ_ÉOÉâÉt"
    
    With fp
        .MaxRows = 17
        .MaxCols = 3
        
        For i = 9 To 24
            .SetText 0, i - 7, i & "éû"
        Next
        
        .ColWidth(1) = 11
        .ColWidth(2) = 11
        .ColWidth(3) = 11
        
        .Col = 1: .Row = 1: .Col2 = 3: .Row2 = 1
        .BlockMode = True
        .CellType = CellTypeComboBox
        .TypeComboBoxList = vbTab & EigyoDays
        .TypeComboBoxEditable = False
        .BlockMode = False
    End With
    
    Me.Top = sinfo.WorkAreaTop
    Me.Left = sinfo.WorkAreaLeft
    Me.Width = sinfo.WorkAreaWidth
    Me.Height = 432 * Screen.TwipsPerPixelY
    
    With c2d.ChartGroups(1)
        
        .SeriesLabels.RemoveAll
        .SeriesLabels.Add "----/--/--"
        .SeriesLabels.Add "----/--/--"
        .SeriesLabels.Add "----/--/--"
        
        .Data.NumSeries = 3
        .Data.NumPoints(1) = 16
        
        For i = 1 To .Data.NumPoints(1)
            .Data.Y(1, i) = .Data.HoleValue
            .Data.Y(2, i) = .Data.HoleValue
            .Data.Y(3, i) = .Data.HoleValue
        Next
    End With
    
    'y-axisÇí â›å^Ç…Ç∑ÇÈ
    SetCurrencyFormat
    
    c2d.ChartArea.Axes("X").AnnotationMethod = oc2dAnnotateValueLabels
    For i = 1 To c2d.ChartGroups(1).Data.NumPoints(1)
        c2d.ChartArea.Axes("X").ValueLabels.Add i, (i + 8) & "éû"
    Next
    c2d.ChartArea.Axes("X").AnnotationRotation = oc2dRotate90Degrees
    
    cmbData.ListIndex = 0
    cmbChartType.ListIndex = 0
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Dim f As Form
    
    For Each f In Forms
        Unload f
    Next
End Sub

Private Sub fp_Change(ByVal Col As Long, ByVal Row As Long)
    If Row = 1 Then
        UpdateData Col, Row
    End If
End Sub
