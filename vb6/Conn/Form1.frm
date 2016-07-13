VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "mswinsck.ocx"
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
   Begin MSWinsockLib.Winsock Winsock1 
      Index           =   0
      Left            =   2160
      Top             =   1320
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_wsockNum As Long

Private Sub Form_Load()
    m_wsockNum = 0
    Winsock1(0).LocalPort = 1906
    Winsock1(0).Listen
End Sub

Private Sub Winsock1_Close(Index As Integer)
    Winsock1(Index).Close
End Sub

Private Sub Winsock1_ConnectionRequest(Index As Integer, ByVal requestID As Long)
    Debug.Print Index
    If Index = 0 Then
        m_wsockNum = m_wsockNum + 1
        Load Winsock1(m_wsockNum)
        Winsock1(m_wsockNum).LocalPort = 0
        Winsock1(m_wsockNum).Accept requestID
    End If
End Sub

Private Sub Winsock1_DataArrival(Index As Integer, ByVal bytesTotal As Long)
    Dim dat As String
    
    Winsock1(Index).GetData dat
    Winsock1(Index).SendData CStr(Rnd)
End Sub

Private Sub Winsock1_Error(Index As Integer, ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
    Debug.Print "error", Number, Description
End Sub
