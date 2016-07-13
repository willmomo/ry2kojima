VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   5  '‰Â•ÏÂ°Ù ³¨ÝÄÞ³
   Caption         =   "Form2"
   ClientHeight    =   3915
   ClientLeft      =   60
   ClientTop       =   330
   ClientWidth     =   6990
   ControlBox      =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3915
   ScaleWidth      =   6990
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.TextBox Text1 
      Height          =   3495
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   3  '—¼•û
      TabIndex        =   0
      Top             =   120
      Width           =   4575
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Resize()
    Text1.Left = 0
    Text1.Top = 0
    Text1.Width = Me.ScaleWidth
    Text1.Height = Me.ScaleHeight
End Sub
