VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlTextBoxHelper�̃e�X�g"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   2880
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   240
      Width           =   1215
   End
   Begin VB.CommandButton cmdTBHSelAllTest 
      Caption         =   "TBHSelAll�̃e�X�g"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1695
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdTBHSelAllTest_Click()
    '�e�L�X�g��S�I��
    TBHSelAll Text1
    Debug.Assert Text1.SelStart = 0 And Text1.SelLength = Len(Text1.Text)
    
    '�e�X�g����
    MsgBox "TBHSelAll �֐��͐��퓮�삵�Ă��܂��B" & vbCrLf & _
        "(���̃_�C�A���O�����ƃt�H�[�J�X��Text1�Ɉړ����܂�" & _
        "���̒i�K�ŁA�I������Č����܂�)", vbOKOnly, "�e�X�g�I��"
        
    Text1.SetFocus
End Sub
