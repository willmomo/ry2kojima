VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   360
      TabIndex        =   0
      Top             =   360
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim strSample As String
    Dim strCol() As String
    Dim strMsg As String
    Dim i As Long
    
    strSample = "��1,""��2:,"",""��3:"""""",��4"
    strCol = CsvSplit(strSample, ",")
    
    strMsg = strSample & " �� "
    For i = 0 To UBound(strCol)
        strMsg = strMsg & "[ " & strCol(i) & " ]"
    Next
    MsgBox strMsg
    
    strSample = "��1"
    MsgBox strSample & " �� " & CsvEncode(strSample)
    
    strSample = "��2:,"
    MsgBox strSample & " �� " & CsvEncode(strSample)
    
    strSample = "��3:"""
    MsgBox strSample & " �� " & CsvEncode(strSample)
    
    strSample = "G��'75"
    MsgBox strSample & " �� " & SQLEncode(strSample)
End Sub
