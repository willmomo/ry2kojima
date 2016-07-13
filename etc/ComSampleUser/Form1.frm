VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5130
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   5895
   LinkTopic       =   "Form1"
   ScaleHeight     =   5130
   ScaleWidth      =   5895
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command7 
      Caption         =   "Command7"
      Height          =   495
      Left            =   3480
      TabIndex        =   8
      Top             =   4320
      Width           =   1215
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Command1"
      Height          =   495
      Left            =   2880
      TabIndex        =   6
      Top             =   1680
      Width           =   1215
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Command2"
      Height          =   495
      Left            =   2880
      TabIndex        =   5
      Top             =   2520
      Width           =   1215
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Command3"
      Height          =   495
      Left            =   2880
      TabIndex        =   4
      Top             =   3360
      Width           =   1215
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Command3"
      Height          =   495
      Left            =   240
      TabIndex        =   3
      Top             =   3360
      Width           =   1215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   240
      TabIndex        =   2
      Top             =   2520
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   240
      TabIndex        =   1
      Top             =   1680
      Width           =   1215
   End
   Begin VB.Label Label2 
      Caption         =   "Label2"
      Height          =   495
      Left            =   120
      TabIndex        =   7
      Top             =   840
      Width           =   4575
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4455
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim cs As New COMSAMPLELib.ObjSample
    Dim buf
    Dim buf2(3)
    
    buf = cs.VariantRet
    
    cs.VariantArg buf2
    cs.VariantArg buf
    
    Label1.Caption = TypeName(buf) & " , " & TypeName(buf2)
    Label2.Caption = Join(buf, ",")
End Sub

Private Sub Command2_Click()
    Dim cs As New COMSAMPLELib.ObjSample
    Dim buf()
    
    cs.VariantPArg buf
    
    Label1.Caption = TypeName(buf)
End Sub

Private Sub Command3_Click()
    Dim cs As New COMSAMPLELib.ObjSample
    Dim buf As Long
    
    buf = 123
    cs.VariantPArg buf
    
    Label1.Caption = TypeName(buf)
    Label2.Caption = buf
End Sub

Private Sub Command4_Click()
    Dim cs As New COMSAMPLELib.ObjSample
    Dim buf As Long
    
    buf = 234
    cs.VariantArg buf
    
    Label1.Caption = TypeName(buf)
    Label2.Caption = buf
End Sub

Private Sub Command5_Click()
    Dim cs As New COMSAMPLELib.ObjSample
    Dim buf()
    
    cs.VariantArg buf
    
    Label1.Caption = TypeName(buf)
End Sub

Private Sub Command6_Click()
    Dim cs As New COMSAMPLELib.ObjSample
    Dim buf
    
    cs.VariantArg buf
    
    Label1.Caption = TypeName(buf)
End Sub

Private Sub Command7_Click()
    Dim o As New COMSAMPLELib.SharedMem
    Dim buf
    
    o.Open "okok.bin"
    buf = o.ReadByte(128)
    o.Close
End Sub
