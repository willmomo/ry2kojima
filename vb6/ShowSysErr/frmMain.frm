VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  '�Œ�(����)
   Caption         =   "GetLastError�̒l�𕶎���ŕ\��"
   ClientHeight    =   2325
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6330
   BeginProperty Font 
      Name            =   "�l�r �o�S�V�b�N"
      Size            =   12
      Charset         =   128
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2325
   ScaleWidth      =   6330
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton cmdShowError 
      Caption         =   "�G���[���e�̕\��(&S)"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   9
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   3600
      TabIndex        =   2
      Top             =   240
      Width           =   2535
   End
   Begin VB.TextBox txtErrCode 
      Alignment       =   1  '�E����
      Height          =   390
      Left            =   1560
      TabIndex        =   1
      Top             =   292
      Width           =   1215
   End
   Begin VB.Label lblCaution 
      Caption         =   "���ӁF�K���ȃG���[�ԍ�������ƁA���ʂ��Ƃ�����܂��B"
      ForeColor       =   &H000000FF&
      Height          =   315
      Left            =   180
      TabIndex        =   4
      Top             =   1800
      Width           =   5955
   End
   Begin VB.Label lblErrMsg 
      BackColor       =   &H80000013&
      BorderStyle     =   1  '����
      Caption         =   "Label2"
      Height          =   735
      Left            =   180
      TabIndex        =   3
      Top             =   900
      Width           =   5955
   End
   Begin VB.Label Label1 
      Caption         =   "�G���[�ԍ�(&E)"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   9
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   180
      TabIndex        =   0
      Top             =   360
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function FormatMessage Lib "kernel32" Alias "FormatMessageA" ( _
    ByVal dwFlags As Long, _
    lpSource As Any, _
    ByVal dwMessageId As Long, _
    ByVal dwLanguageId As Long, _
    ByVal lpBuffer As String, _
    ByVal nSize As Long, _
    Arguments As Long) As Long

Private Const FORMAT_MESSAGE_FROM_SYSTEM = &H1000

Private Sub UpdateErrorMessage(ByVal lngErrCode As Long)
    Dim strBuffer As String
    Dim lngRet As Long
    
    strBuffer = String(512, vbNullChar)
    
    lngRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, ByVal 0, lngErrCode, 0, strBuffer, Len(strBuffer), 0)
    
    If lngRet = 0 Then
        MsgBox "�G���[�R�[�h '" & lngErrCode & "' �̃��b�Z�[�W�́A�p�ӂ���Ă��܂���B"
        
        lblErrMsg.Caption = ""
    Else
        strBuffer = Left(strBuffer, InStr(strBuffer, vbNullChar) - 1)
    
        lblErrMsg.Caption = strBuffer
    End If
End Sub

Private Sub cmdShowError_Click()
    UpdateErrorMessage txtErrCode.Text
End Sub

Private Sub Form_Load()
    txtErrCode.Text = 0
    UpdateErrorMessage txtErrCode.Text
End Sub

Private Sub txtErrCode_GotFocus()
    txtErrCode.SelStart = 0
    txtErrCode.SelLength = Len(txtErrCode.Text)
End Sub
