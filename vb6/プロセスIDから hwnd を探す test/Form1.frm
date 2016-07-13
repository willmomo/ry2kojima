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
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   1380
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   540
      Width           =   1875
   End
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

Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String, ByVal cch As Long) As Long

Private Sub Command1_Click()
    Dim hwndFromPID As Long
    
    hwndFromPID = GetWindowHandleFromPID(Text1)
    
    Dim s As String
    
    s = String(512, vbNullChar)
    GetWindowText hwndFromPID, s, 512
    Debug.Print Now, hwndFromPID, Left(s, InStr(s, vbNullChar) - 1)
    
End Sub
