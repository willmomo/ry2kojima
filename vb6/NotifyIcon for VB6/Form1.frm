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
   Visible         =   0   'False
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    
    Load frmNotifyIcon

End Sub

Private Sub Form_Resize()

    If Me.WindowState = vbMinimized Then
        
        Me.Visible = False
        Me.WindowState = vbNormal
        
    End If
        
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Unload frmNotifyIcon
End Sub
