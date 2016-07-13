VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Table2Csv"
   ClientHeight    =   5925
   ClientLeft      =   165
   ClientTop       =   555
   ClientWidth     =   7470
   LinkTopic       =   "Form1"
   ScaleHeight     =   5925
   ScaleWidth      =   7470
   StartUpPosition =   3  'Windows の既定値
   Begin VB.TextBox txtTables 
      Height          =   1095
      Left            =   1560
      MultiLine       =   -1  'True
      ScrollBars      =   2  '垂直
      TabIndex        =   7
      Top             =   480
      Width           =   5775
   End
   Begin VB.TextBox txtConnString 
      Height          =   270
      Left            =   1560
      TabIndex        =   5
      Top             =   120
      Width           =   5775
   End
   Begin VB.TextBox Text1 
      Height          =   2895
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  '垂直
      TabIndex        =   3
      Top             =   2880
      Width           =   7215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "中止"
      Height          =   495
      Left            =   6120
      TabIndex        =   1
      Top             =   1800
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "作成"
      Default         =   -1  'True
      Height          =   495
      Left            =   4680
      TabIndex        =   0
      Top             =   1800
      Width           =   1215
   End
   Begin VB.Label Label3 
      Caption         =   "テーブルリスト："
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   480
      Width           =   1335
   End
   Begin VB.Label Label2 
      Caption         =   "ADO接続文字列："
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   1455
   End
   Begin VB.Label Label1 
      Appearance      =   0  'ﾌﾗｯﾄ
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   2520
      Width           =   7215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_abort As Boolean

Private Function FieldTypeToText(fieldType As DataTypeEnum) As String
    Select Case fieldType
    Case adNumeric
        FieldTypeToText = "adNumeric"
    Case adVarChar
        FieldTypeToText = "adVarChar"
    Case adDBTimeStamp
        FieldTypeToText = "adDBTimeStamp"
    Case Else
        FieldTypeToText = CStr(fieldType)
    End Select
End Function

Private Sub ExportTable(db As ADODB.Connection, tableName As Variant)
    Dim objFs As New FileSystemObject
    Dim ts As TextStream
    Dim cnt As Long
    Dim i As Long
    Dim rs As New ADODB.Recordset
    
    Set ts = objFs.OpenTextFile(tableName & ".csv", ForWriting, True)
    
    Label1.Caption = tableName & " [" & cnt & "] 出力中・・・"
    DoEvents
    
    rs.Open "select * from " & tableName, db, adOpenForwardOnly
    
    For i = 0 To rs.Fields.Count - 1
        If i <> 0 Then ts.Write ","
        ts.Write rs.Fields(i).Name
    Next
    ts.WriteLine
    
    For i = 0 To rs.Fields.Count - 1
        If i <> 0 Then ts.Write ","
        ts.Write FieldTypeToText(rs.Fields(i).Type)
    Next
    ts.WriteLine
    
    Do Until rs.EOF
        If m_abort = True Then Exit Do
        
        For i = 0 To rs.Fields.Count - 1
            If i <> 0 Then ts.Write ","
            If rs.Fields(i).Type = adNumeric Then
                If IsNull(rs(i)) Then
                    ts.Write "(Null)"
                Else
                    ts.Write rs(i)
                End If
            Else
                ts.Write "'" & rs(i) & "'"
            End If
        Next
        ts.WriteLine
        
        cnt = cnt + 1
        If (cnt Mod 2000&) = 0& Then
            Label1.Caption = tableName & " [" & cnt & "] 出力中・・・"
            DoEvents
        End If
        
        rs.MoveNext
    Loop
    
    rs.Close

    ts.Close
    
    Label1.Caption = ""
    DoEvents
End Sub

Private Sub Command1_Click()
    Dim db As New ADODB.Connection
    Dim s As Variant
    Dim t1 As Date
    Dim t2 As Date
    
    txtConnString.Enabled = False
    txtTables.Enabled = False
    Command1.Enabled = False
    Text1.Text = ""
    Command2.Enabled = True
    Command2.SetFocus
    
    m_abort = False
    
    db.Open txtConnString.Text
    
    For Each s In Split(txtTables.Text, ",")
        If m_abort = True Then Exit For
        
        t1 = Now
        ExportTable db, s
        t2 = Now
        
        Text1.Text = Text1.Text & s & vbTab & "所要時間：" & DateDiff("s", t1, t2) & " 秒" & vbNewLine
    Next
    
    db.Close
    
    Command2.Enabled = False
    txtConnString.Enabled = True
    txtTables.Enabled = True
    Command1.Enabled = True
    Command1.SetFocus
End Sub

Private Sub Command2_Click()
    m_abort = True
End Sub

Private Sub Form_Load()
    Command2.Enabled = False
    txtConnString.Text = "Driver={Oracle in OraHome92};UID=pos;PWD=posadmin;DBQ=pos"
    txtTables.Text = "MASTER_BUMON,MASTER_BUMON_GROUP,MASTER_KEIHIN,MASTER_KEIHIN_KARI_SETTEI,MASTER_ITEMKEY,MASTER_KEIHIN_BARCODE,MASTER_KEIHIN_GROUP,MASTER_KOUKAN_TANKA,MASTER_SHIIRESAKI,MASTER_TOKUSHU_KEIHIN"
End Sub

Private Sub Form_Resize()
    If Me.WindowState <> vbMinimized Then
        txtConnString.Width = Me.ScaleWidth - txtConnString.Left - 120
        txtTables.Width = Me.ScaleWidth - txtTables.Left - 120
        Command2.Left = Me.ScaleWidth - Command2.Width - 120
        Command1.Left = Command2.Left - Command1.Width - 120
        Label1.Width = Me.ScaleWidth - Label1.Left - 120
    End If
End Sub
