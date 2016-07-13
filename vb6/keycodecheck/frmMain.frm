VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "キーコードチェッカ"
   ClientHeight    =   6045
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7785
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   6045
   ScaleWidth      =   7785
   StartUpPosition =   3  'Windows の既定値
   Begin VB.ListBox lstKeyCode 
      Enabled         =   0   'False
      Height          =   4920
      IntegralHeight  =   0   'False
      Left            =   240
      TabIndex        =   0
      Top             =   480
      Width           =   7215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    Dim s As String
    
    s = Time & vbTab & "0x" & Right("00" & KeyCode, 2) & vbTab
    s = s & "0x" & Right("00" & Hex(Shift), 2) & "["
    s = s & IIf((Shift And vbShiftMask) <> 0, "S", "s")
    s = s & IIf((Shift And vbCtrlMask) <> 0, "C", "c")
    s = s & IIf((Shift And vbAltMask) <> 0, "A", "a") & "]"
    
    lstKeyCode.AddItem s, 0
    If lstKeyCode.ListCount > 100 Then
        lstKeyCode.RemoveItem lstKeyCode.ListCount - 1
    End If
End Sub

Private Sub Form_Resize()
    lstKeyCode.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
End Sub
