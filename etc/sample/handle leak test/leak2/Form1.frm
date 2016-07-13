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
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   1740
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
    
    Dim s As Date, e As Date
    
    Dim doc As New MSXML.DOMDocument
    Dim fso As New FileSystemObject
    Dim ts As TextStream
    
    Set ts = fso.OpenTextFile("c:\list.txt")
    
    'doc.async = False
    
    s = Now
    Do Until ts.AtEndOfStream
        
        On Error Resume Next
        If doc.Load(ts.ReadLine) Then
        End If
        
    Loop
    e = Now
    
    Command1.Enabled = True
    
    MsgBox e - s
    
End Sub
