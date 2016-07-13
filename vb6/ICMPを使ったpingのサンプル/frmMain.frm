VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  '固定(実線)
   Caption         =   "ICMPを使ったpingのサンプル"
   ClientHeight    =   1800
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   3795
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1800
   ScaleWidth      =   3795
   StartUpPosition =   3  'Windows の既定値
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   1080
      TabIndex        =   1
      Text            =   "127.0.0.1"
      Top             =   180
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "ping!"
      Default         =   -1  'True
      Height          =   495
      Left            =   2400
      TabIndex        =   2
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label Label2 
      BorderStyle     =   1  '実線
      Height          =   855
      Left            =   120
      TabIndex        =   3
      Top             =   840
      Width           =   3495
   End
   Begin VB.Label Label1 
      Caption         =   "IPアドレス："
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   975
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim Reply As ICMP_ECHO_REPLY
    Dim lngSuccess As Long
    Dim strIPAddress As String
    
    'Get the sockets ready.
    If SocketsInitialize() Then
      
        'Address to ping
        strIPAddress = Text1.Text
        
        'Ping the IP that is passing the address and get a reply.
        lngSuccess = ping(strIPAddress, Reply)
          
        'Display the results.
        Label2.Caption = ""
        Label2.Caption = Label2.Caption & "Address to Ping: " & strIPAddress & vbNewLine
        Label2.Caption = Label2.Caption & "Raw ICMP code: " & lngSuccess & vbNewLine
        Label2.Caption = Label2.Caption & "Ping Response Message : " & EvaluatePingResponse(lngSuccess) & vbNewLine
        Label2.Caption = Label2.Caption & "Time : " & Reply.RoundTripTime & " ms"
          
        'Clean up the sockets.
        SocketsCleanup
      
    Else
    
        'Winsock error failure, initializing the sockets.
        Debug.Print WINSOCK_ERROR
    
    End If
End Sub

Private Sub Text1_GotFocus()
    Text1.SelStart = 0
    Text1.SelLength = Len(Text1.Text)
End Sub
