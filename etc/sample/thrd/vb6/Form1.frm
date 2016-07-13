VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   7830
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13110
   LinkTopic       =   "Form1"
   ScaleHeight     =   7830
   ScaleWidth      =   13110
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.TextBox Text1 
      Height          =   6615
      Left            =   180
      MultiLine       =   -1  'True
      ScrollBars      =   3  '—¼•û
      TabIndex        =   1
      Top             =   900
      Width           =   12675
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

    Me.MousePointer = vbHourglass
    
    Dim t
    t = Now
    
    Text1.Text = ""

    Dim bsock As New BASP21Lib.Socket
    Dim buf As Variant
    
    bsock.Connect "localhost", 12346
    
    bsock.Write "cmd: getfile" & vbNewLine & "arg1: C:\WINDOWS\I386\DRIVER.CAB" & vbNewLine & vbNewLine
    
    Dim rc
    Do
        rc = bsock.ReadLine(buf)
        Text1.Text = Text1.Text & buf & vbNewLine
    Loop While rc = 0 And buf <> ""
    
    Dim s
    Do While bsock.Read(buf, 1) = 0
    
        'Text1.Text = Text1.Text & buf
        
        'Text1.Text = Text1.Text & LenB(buf)
        s = s + LenB(buf)
        
        'Debug.Print s
    Loop
    
    Text1.Text = Text1.Text & s & vbNewLine
     
    bsock.Close
    
    Debug.Print DateDiff("s", Now, t)
    
    Me.MousePointer = vbDefault
    
'    Dim xml As New DOMDocument
'    xml.loadXML Text1.Text
'
'    Debug.Print xml.parseError, xml.parseError.reason
    
End Sub
