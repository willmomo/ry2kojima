VERSION 5.00
Begin VB.Form Form2 
   Caption         =   "Form2"
   ClientHeight    =   6885
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10020
   LinkTopic       =   "Form2"
   ScaleHeight     =   6885
   ScaleWidth      =   10020
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command3 
      Caption         =   "Command3"
      Height          =   495
      Left            =   3480
      TabIndex        =   5
      Top             =   3720
      Width           =   1215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   1920
      TabIndex        =   4
      Top             =   3720
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   360
      TabIndex        =   3
      Top             =   3720
      Width           =   1215
   End
   Begin VB.Label Label3 
      Caption         =   "Label1"
      Height          =   735
      Left            =   120
      TabIndex        =   2
      Top             =   2400
      Width           =   3855
   End
   Begin VB.Label Label2 
      Caption         =   "Label1"
      Height          =   735
      Left            =   120
      TabIndex        =   1
      Top             =   1320
      Width           =   3855
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   735
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   3855
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_errId As Long

Private Sub InitUI()
    Dim objDb As New ADODB.Connection
    Dim objRs As New ADODB.Recordset
    Dim work As Variant
    
    objDb.Open "Driver={Microsoft Access Driver (*.mdb)}; DBQ=ErrDlg.mdb"
    objRs.Open "select * from T_ERRDLG where ERRID=" & m_errId, objDb
    
    Me.BackColor = objRs("FormBackColor")
    
    Label1.BackColor = objRs("Label1BackColor")
    Label1.Caption = objRs("Label1Caption")
    work = Split(objRs("Label1Rect"), ",")
    Label1.Left = work(0) * Screen.TwipsPerPixelX
    Label1.Top = work(1) * Screen.TwipsPerPixelY
    Label1.Width = work(2) * Screen.TwipsPerPixelX
    Label1.Height = work(3) * Screen.TwipsPerPixelY
    
    Label2.BackColor = objRs("Label2BackColor")
    Label2.Caption = objRs("Label2Caption")
    work = Split(objRs("Label2Rect"), ",")
    Label2.Left = work(0) * Screen.TwipsPerPixelX
    Label2.Top = work(1) * Screen.TwipsPerPixelY
    Label2.Width = work(2) * Screen.TwipsPerPixelX
    Label2.Height = work(3) * Screen.TwipsPerPixelY
    
    Label3.BackColor = objRs("Label3BackColor")
    Label3.Caption = objRs("Label3Caption")
    work = Split(objRs("Label3Rect"), ",")
    Label3.Left = work(0) * Screen.TwipsPerPixelX
    Label3.Top = work(1) * Screen.TwipsPerPixelY
    Label3.Width = work(2) * Screen.TwipsPerPixelX
    Label3.Height = work(3) * Screen.TwipsPerPixelY
    
    Command1.Visible = objRs("Btn1Visible")
    Command1.Caption = objRs("Btn1Caption")
    work = Split(objRs("Btn1Rect"), ",")
    Command1.Left = work(0) * Screen.TwipsPerPixelX
    Command1.Top = work(1) * Screen.TwipsPerPixelY
    Command1.Width = work(2) * Screen.TwipsPerPixelX
    Command1.Height = work(3) * Screen.TwipsPerPixelY
    
    Command2.Visible = objRs("Btn2Visible")
    Command2.Caption = objRs("Btn2Caption")
    work = Split(objRs("Btn2Rect"), ",")
    Command2.Left = work(0) * Screen.TwipsPerPixelX
    Command2.Top = work(1) * Screen.TwipsPerPixelY
    Command2.Width = work(2) * Screen.TwipsPerPixelX
    Command2.Height = work(3) * Screen.TwipsPerPixelY
    
    Command3.Visible = objRs("Btn3Visible")
    Command3.Caption = objRs("Btn3Caption")
    work = Split(objRs("Btn3Rect"), ",")
    Command3.Left = work(0) * Screen.TwipsPerPixelX
    Command3.Top = work(1) * Screen.TwipsPerPixelY
    Command3.Width = work(2) * Screen.TwipsPerPixelX
    Command3.Height = work(3) * Screen.TwipsPerPixelY
    
    objRs.Close
    objDb.Close
End Sub

Private Sub Form_Load()
    m_errId = 1
    
    InitUI
End Sub
