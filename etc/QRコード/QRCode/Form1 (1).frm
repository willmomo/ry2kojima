VERSION 5.00
Object = "{90CC2E02-A649-11D3-B0C5-005004A82F01}#1.0#0"; "BarCode.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5235
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4200
   LinkTopic       =   "Form1"
   ScaleHeight     =   5235
   ScaleWidth      =   4200
   StartUpPosition =   3  'Windows ÇÃä˘íËíl
   Begin VB.CommandButton cmd_print 
      Caption         =   "àÛç¸"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   2400
      Width           =   1815
   End
   Begin VB.TextBox txt 
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   1920
      Width           =   1815
   End
   Begin BarCodeLibCtl.BarCode BarCode1 
      Height          =   405
      Left            =   2040
      TabIndex        =   10
      Top             =   2400
      Visible         =   0   'False
      Width           =   405
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
      Value           =   "12345"
      BarWidth        =   "7.25 mm"
      BarHeight       =   "7.25 mm"
      AddNumber       =   0
      PDF417ErrorLevel=   -1
      PDF417Type      =   1
      QRCodeErrorLevel=   1
      QRCodeMask      =   -1
   End
   Begin VB.Label Label4 
      Caption         =   "ÉRÅ[ÉhÇïœä∑"
      Height          =   255
      Index           =   2
      Left            =   2640
      TabIndex        =   13
      Top             =   4440
      Width           =   1455
   End
   Begin VB.Label Label4 
      Caption         =   "ÉoÅ[ÉWÉáÉìÇP"
      Height          =   255
      Index           =   1
      Left            =   360
      TabIndex        =   12
      Top             =   4440
      Width           =   1455
   End
   Begin VB.Label Label4 
      Caption         =   "ÉoÅ[ÉWÉáÉìÇQ"
      Height          =   255
      Index           =   0
      Left            =   2640
      TabIndex        =   11
      Top             =   3000
      Width           =   1215
   End
   Begin VB.Image Image3 
      Height          =   975
      Left            =   2640
      Stretch         =   -1  'True
      Top             =   3360
      Width           =   1095
   End
   Begin VB.Image Image1 
      Height          =   975
      Left            =   240
      Stretch         =   -1  'True
      Top             =   3360
      Width           =   1095
   End
   Begin VB.Image Image2 
      Height          =   975
      Left            =   2640
      Stretch         =   -1  'True
      Top             =   1920
      Width           =   1095
   End
   Begin VB.Label Label2 
      BackColor       =   &H00000000&
      Caption         =   "10p"
      BeginProperty Font 
         Name            =   "ÇlÇr ÇoÉSÉVÉbÉN"
         Size            =   36
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   1215
      Left            =   0
      TabIndex        =   9
      Top             =   600
      Width           =   1335
   End
   Begin VB.Label lblPoint 
      Alignment       =   2  'íÜâõëµÇ¶
      BackStyle       =   0  'ìßñæ
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   495
      Left            =   80
      TabIndex        =   8
      Top             =   840
      Width           =   1215
   End
   Begin VB.Label lblTani 
      Alignment       =   1  'âEëµÇ¶
      BackStyle       =   0  'ìßñæ
      Caption         =   "pt"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   14.25
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   1320
      Width           =   1095
   End
   Begin VB.Line Line1 
      X1              =   0
      X2              =   3960
      Y1              =   600
      Y2              =   600
   End
   Begin VB.Line Line2 
      X1              =   3960
      X2              =   3960
      Y1              =   1800
      Y2              =   600
   End
   Begin VB.Line Line3 
      X1              =   3960
      X2              =   0
      Y1              =   1800
      Y2              =   1800
   End
   Begin VB.Line Line4 
      X1              =   1320
      X2              =   1320
      Y1              =   600
      Y2              =   1800
   End
   Begin VB.Line Line5 
      X1              =   1320
      X2              =   3960
      Y1              =   980
      Y2              =   980
   End
   Begin VB.Line Line6 
      X1              =   0
      X2              =   0
      Y1              =   600
      Y2              =   1800
   End
   Begin VB.Image ImgShohin 
      Height          =   795
      Left            =   1350
      Picture         =   "Form1.frx":0000
      Stretch         =   -1  'True
      Top             =   990
      Width           =   2600
   End
   Begin VB.Label lblShohin 
      Alignment       =   2  'íÜâõëµÇ¶
      BackStyle       =   0  'ìßñæ
      Caption         =   "NODATA"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   12
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   1320
      TabIndex        =   6
      Top             =   660
      Width           =   2625
   End
   Begin VB.Label lblPrintDate 
      Appearance      =   0  'Ã◊Øƒ
      BackColor       =   &H80000005&
      BackStyle       =   0  'ìßñæ
      Caption         =   "2004/08/19 12:00"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   9.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1560
      TabIndex        =   5
      Top             =   0
      Width           =   2295
   End
   Begin VB.Label lblKaiinno 
      Appearance      =   0  'Ã◊Øƒ
      BackColor       =   &H80000005&
      BackStyle       =   0  'ìßñæ
      Caption         =   "123456789"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   9.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1560
      TabIndex        =   4
      Top             =   240
      Width           =   2415
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Ã◊Øƒ
      BackColor       =   &H80000005&
      BackStyle       =   0  'ìßñæ
      Caption         =   "èoóÕì˙éûÅ@ÅF"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   9.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   0
      Width           =   1455
   End
   Begin VB.Label Label3 
      Appearance      =   0  'Ã◊Øƒ
      BackColor       =   &H80000005&
      BackStyle       =   0  'ìßñæ
      Caption         =   "âÔàıî‘çÜÅ@ÅF"
      BeginProperty Font 
         Name            =   "ÇlÇr ÉSÉVÉbÉN"
         Size            =   9.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   240
      Width           =   1455
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit



Private Sub cmd_print_Click()
    Dim tmpPic As New StdPicture
    BarCode1.Value = txt.Text
    'QRÉRÅ[ÉhâªÇ≥ÇπÇÈï∂éöóÒÇÃê›íË
    
    
    BarCode1.Type = bcdTypeQRCode2
    Image2.Picture = BarCode1.CreateBitmap(100)
    
    
    BarCode1.Type = bcdTypeQRCode1
    Image1.Picture = BarCode1.CreateBitmap(100)
    
    
    BarCode1.Value = StrConv(txt.Text, vbUnicode)
    BarCode1.Type = bcdTypeQRCode2
    Image3.Picture = BarCode1.CreateBitmap(100)
    
    Form1.PrintForm

End Sub

