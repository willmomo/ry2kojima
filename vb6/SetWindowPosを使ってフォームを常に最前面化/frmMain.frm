VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "SetWindowPos���g���ăt�H�[������ɍőO�ʉ�"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton cmdEnd 
      Caption         =   "�I��(&X)"
      Height          =   495
      Left            =   1800
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
   End
   Begin VB.TextBox txtSample 
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Text            =   "�t�H�[�J�X�Ɋւ��Ē��ӂ��K�v"
      Top             =   720
      Width           =   4455
   End
   Begin VB.Label Label1 
      Caption         =   "SetWindowPos���g���ăt�H�[������ɍőO�ʉ�"
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4455
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'----------------------------------------------------------------------
' Click �C�x���g
'----------------------------------------------------------------------
Private Sub cmdEnd_Click()
    Unload Me
End Sub

'----------------------------------------------------------------------
' Load �C�x���g
'----------------------------------------------------------------------
Private Sub Form_Load()

    ' SWP_NOACTIVATE�����ČĂ΂Ȃ��ƁA�t�H�[�J�X���t�H�[���ɍs���Ă��܂��B
    ' �A�N�e�B�x�[�g�̏�����VB6�ɂ�点��K�v������񂶂�Ȃ����ȁB
    
    Call SetWindowPos(Me.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOACTIVATE)
End Sub

'----------------------------------------------------------------------
' GotFocus �C�x���g
'
' �e�L�X�g��S�I�����鏈��
'----------------------------------------------------------------------
Private Sub txtSample_GotFocus()
    txtSample.SelStart = 0
    txtSample.SelLength = Len(txtSample.Text)
End Sub
