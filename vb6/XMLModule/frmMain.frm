VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  'å≈íË(é¿ê¸)
   Caption         =   "Form1"
   ClientHeight    =   2250
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5730
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2250
   ScaleWidth      =   5730
   StartUpPosition =   3  'Windows ÇÃä˘íËíl
   Begin VB.TextBox txtFile 
      Height          =   270
      Left            =   1440
      TabIndex        =   8
      Text            =   "test.xml"
      Top             =   240
      Width           =   3975
   End
   Begin VB.CommandButton cmdClear 
      Caption         =   "ÉNÉäÉA(&C)"
      Height          =   495
      Left            =   2280
      TabIndex        =   6
      Top             =   1440
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.CommandButton cmdWrite 
      Caption         =   "èëÇ≠(&W)"
      Height          =   495
      Left            =   4200
      TabIndex        =   5
      Top             =   1440
      Width           =   1215
   End
   Begin VB.CommandButton cmdRead 
      Caption         =   "ì«Çﬁ(&R)"
      Height          =   495
      Left            =   240
      TabIndex        =   4
      Top             =   1440
      Width           =   1215
   End
   Begin VB.TextBox txtValue 
      Height          =   270
      Left            =   1440
      TabIndex        =   3
      Top             =   960
      Width           =   3975
   End
   Begin VB.TextBox txtXMLPath 
      Height          =   270
      Left            =   1440
      TabIndex        =   1
      Top             =   600
      Width           =   3975
   End
   Begin VB.Label lblFile 
      Caption         =   "XML File (&F)"
      Height          =   255
      Left            =   240
      TabIndex        =   7
      Top             =   240
      Width           =   1095
   End
   Begin VB.Label lblValue 
      Caption         =   "Value (&V)"
      Height          =   255
      Left            =   240
      TabIndex        =   2
      Top             =   960
      Width           =   1095
   End
   Begin VB.Label lblXMLPath 
      Caption         =   "XML Path (&P)"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   600
      Width           =   1095
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_xmlFile As clsXMLFile

Private Sub cmdClear_Click()
    txtValue.Text = ""
End Sub

Private Sub cmdRead_Click()
    txtValue.Text = m_xmlFile.ReadText(txtXMLPath.Text, "(default)")
    
    If Err.Number Then
        MsgBox Err.Description
    End If
End Sub

Private Sub cmdWrite_Click()
    If Not m_xmlFile.WriteText(txtXMLPath.Text, txtValue.Text) Then
        MsgBox Err.Description
    End If
    
    Exit Sub
End Sub

Private Sub Form_Load()
    Set m_xmlFile = New clsXMLFile
    m_xmlFile.OpenFile "clsText.xml"
    m_xmlFile.UseCache = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_xmlFile = Nothing
End Sub
