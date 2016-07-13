VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5340
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9480
   LinkTopic       =   "Form1"
   ScaleHeight     =   5340
   ScaleWidth      =   9480
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   8280
      Top             =   2220
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      RemoteHost      =   "localhost"
      RemotePort      =   12345
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   7560
      TabIndex        =   0
      Top             =   540
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
    
    Winsock1.SendData "14,15,16,14"
    
    Dim ansData
    
    Do While Winsock1.BytesReceived = 0
    Loop
        
    Winsock1.GetData ansData, vbString
    
    Debug.Print ansData
    
    Command1.Enabled = True
    
End Sub

