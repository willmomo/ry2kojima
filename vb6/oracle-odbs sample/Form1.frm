VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows ÇÃä˘íËíl
   Begin VB.Timer Timer1 
      Interval        =   3000
      Left            =   3720
      Top             =   360
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   1800
      TabIndex        =   0
      Top             =   1320
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Command1.Enabled = False
    
    Dim cnn As New ADODB.Connection
    Dim rec As New ADODB.Recordset
    Dim s As Long, e As Long, ss As Long, ee As Long
    
    InitSQL3
    
    cnn.Open "Provider=MSDAORA;" & _
        "Data Source=mpc-new-asahi-tino;", "gmc", "mpcadmin"
    'Data SourceÇ…ÇÕÅATNSÇéwíËÇ∑ÇÈ
    
    Dim fno As Integer
    Dim ln As Long
    
    fno = FreeFile
    Open "d:\work\sql.txt" For Input As fno
    
    ss = GetTickCount
    Do Until EOF(fno)
        Line Input #fno, g_strSQL
        ln = ln + 1
        
        s = GetTickCount
        rec.Open g_strSQL, cnn, adOpenKeyset, adLockOptimistic
        rec.Close
        e = GetTickCount
    
        Debug.Print Now, ln, (e - s - 200) & " ms"
    Loop
    ee = GetTickCount
    Debug.Print Now, (ee - ss) & " ms"
    Debug.Print Now, "---------"
    
    Close fno
    
    cnn.Close
    
    Set rec = Nothing
    Set cnn = Nothing
    
    Command1.Enabled = True
End Sub

Private Sub Timer1_Timer()

    Command1_Click
    
End Sub
