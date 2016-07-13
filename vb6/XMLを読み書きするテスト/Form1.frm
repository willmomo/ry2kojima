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
   Begin VB.Timer Timer2 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   4140
      Top             =   1980
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Command3"
      Height          =   495
      Left            =   1500
      TabIndex        =   3
      Top             =   180
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      Height          =   2175
      Left            =   180
      MultiLine       =   -1  'True
      ScrollBars      =   2  'êÇíº
      TabIndex        =   2
      Top             =   780
      Width           =   3855
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   4140
      Top             =   2520
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   2820
      TabIndex        =   1
      Top             =   180
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   180
      TabIndex        =   0
      Top             =   180
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
    Command3.Enabled = False
    Command2.Enabled = True
    
    Timer1.Enabled = True
    
End Sub

Private Sub Command2_Click()

    Timer1.Enabled = False
    Timer2.Enabled = False
    
    Command1.Enabled = True
    Command3.Enabled = True
    Command2.Enabled = False
    
End Sub

Private Sub Command3_Click()

    Command1.Enabled = False
    Command3.Enabled = False
    Command2.Enabled = True
    
    Timer2.Enabled = True

End Sub

Private Sub Timer1_Timer()

    Dim xmlDoc As New DOMDocument
    
    If xmlDoc.Load(App.Path & "\mscorlib.xml") = True Then
        Text1.Text = Now & ": Load success. " & vbNewLine & Text1.Text
    Else
        Text1.Text = Now & ": Load error. " & vbNewLine & Text1.Text
    End If
    
End Sub

Private Sub Timer2_Timer()

    Dim xmlDoc As New DOMDocument
    
    If xmlDoc.Load(App.Path & "\mscorlib.xml") = True Then
    
        On Error Resume Next
        Call xmlDoc.save(App.Path & "\mscorlib.xml")
        
        If Err.Number = 0 Then
            Text1.Text = Now & ": Save success. " & vbNewLine & Text1.Text
        Else
            Text1.Text = Now & ": Save error. " & vbNewLine & Text1.Text
        End If
        On Error GoTo 0
        
    Else
        Text1.Text = Now & ": Load error. " & vbNewLine & Text1.Text
    End If

End Sub
