VERSION 5.00
Object = "{90CC2E02-A649-11D3-B0C5-005004A82F01}#1.0#0"; "BarCode.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6135
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10140
   LinkTopic       =   "Form1"
   ScaleHeight     =   6135
   ScaleWidth      =   10140
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   900
      TabIndex        =   2
      Top             =   3540
      Width           =   1215
   End
   Begin VB.PictureBox Picture1 
      Height          =   2655
      Left            =   180
      ScaleHeight     =   2595
      ScaleWidth      =   2895
      TabIndex        =   1
      Top             =   300
      Width           =   2955
   End
   Begin BarCodeLibCtl.BarCode bc 
      Height          =   2265
      Left            =   3600
      TabIndex        =   0
      Top             =   480
      Width           =   2265
      Type            =   20
      Unit            =   1
      Message         =   -1  'True
      BeginProperty MessageFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   15.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MessagePos      =   0
      AdjustSize      =   -1  'True
      ForeColor       =   0
      BackColor       =   16777215
      Rotate          =   0
      CheckDigit      =   -1  'True
      Bearer          =   0   'False
      BarRatio        =   100
      Value           =   "1234567890"
      BarWidth        =   "39.93 mm"
      BarHeight       =   "39.93 mm"
      AddNumber       =   0
      PDF417ErrorLevel=   -1
      PDF417Type      =   1
      QRCodeErrorLevel=   1
      QRCodeMask      =   -1
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    bc.TargetDpiX = 203
    bc.TargetDpiY = 203
    
    bc.Type = bcdTypeQRCode2
    Printer.PaintPicture bc.CreateBitmap(72), 0, 0
    Printer.EndDoc
End Sub
