VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   3240
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" ( _
    ByVal lpApplicationName As String, _
    ByVal lpKeyName As Any, _
    ByVal lpString As Any, _
    ByVal lpFileName As String _
) As Long

Private Sub Command1_Click()
    Dim ret As Long
    
    ret = WritePrivateProfileString("sec1", "key2", vbNullString, ".\delkey.ini")
    Debug.Print ret, Err.LastDllError
    
    ret = WritePrivateProfileString("sec2", "key3", vbNullString, ".\delkey.ini")
    Debug.Print ret, Err.LastDllError
    
    ret = WritePrivateProfileString("sec2", "key7", vbNullString, ".\delkey.ini")
    Debug.Print ret, Err.LastDllError
    
End Sub

