VERSION 5.00
Object = "{065E6FD1-1BF9-11D2-BAE8-00104B9E0792}#3.0#0"; "ssa3d30.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   1  '�Œ�(����)
   Caption         =   "Form1"
   ClientHeight    =   3225
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   7800
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3225
   ScaleWidth      =   7800
   StartUpPosition =   3  'Windows �̊���l
   Begin Threed.SSRibbon SSRibbon1 
      Height          =   855
      Index           =   0
      Left            =   3840
      TabIndex        =   7
      Top             =   240
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   1508
      _Version        =   196610
      Caption         =   "SSRibbon1"
   End
   Begin VB.CommandButton Command3 
      Caption         =   "�T�u�E�B���h�E���J��"
      Height          =   495
      Left            =   1560
      TabIndex        =   6
      Top             =   1440
      Width           =   2055
   End
   Begin VB.CommandButton Command2 
      Caption         =   "�T�u�E�B���h�E���J��"
      Height          =   495
      Left            =   1560
      TabIndex        =   4
      Top             =   600
      Width           =   2055
   End
   Begin VB.CommandButton Command1 
      Caption         =   "�I���"
      Height          =   495
      Left            =   120
      TabIndex        =   5
      Top             =   2520
      Width           =   3495
   End
   Begin Threed.SSRibbon SSRibbon1 
      Height          =   855
      Index           =   1
      Left            =   5520
      TabIndex        =   8
      Top             =   240
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   1508
      _Version        =   196610
      Caption         =   "SSRibbon1"
   End
   Begin VB.Label Label4 
      BackColor       =   &H00C0FFC0&
      Caption         =   "�X��덷��"
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   1920
      Width           =   1215
   End
   Begin VB.Label Label3 
      BackColor       =   &H00C0FFC0&
      Caption         =   "��덷��"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00C0FFC0&
      Caption         =   "�Ŏ~����"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   720
      Width           =   1215
   End
   Begin VB.Label Label1 
      BackColor       =   &H008080FF&
      Caption         =   "�X����"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_toolTip As New clsToolTip

Private Sub Command1_Click()
    Unload frmSub
    Unload Me
End Sub

Private Sub Command2_Click()
    frmSub.Show vbModal, Me
End Sub

Private Sub Command3_Click()
    frmSub.Show vbModeless, Me
End Sub

Private Sub Form_Load()
    ' �c�[���`�b�v���쐬����
    If m_toolTip.createToolTip(Me.hwnd, False) = False Then
        MsgBox "�c�[���`�b�v�̍쐬�Ɏ��s!!"
        End
    End If

    ' -1 �ȊO���w�肷��ƃ}���`���C���ɂȂ�B
    m_toolTip.MaxTipWidth = 1000

    ' �\������c�[���`�b�v��o�^����B
    m_toolTip.AddTool Command1, "�A�v���P�[�V�������I�����܂��B"
    m_toolTip.AddTool Command2, "�T�u�E�B���h�E��\�����āA���낢��ȃR���g���[���Ƀc�[���`�b�v��ǉ��B" & vbNewLine & "���[�_���ŊJ���܂��B"
    m_toolTip.AddTool Command3, "�T�u�E�B���h�E��\�����āA���낢��ȃR���g���[���Ƀc�[���`�b�v��ǉ��B" & vbNewLine & "���[�h���X�ŊJ���܂��B"
    m_toolTip.AddToolForLabel Label1, 1, "�J�����삩��̎��ʐ�(����ʂ��܂ށj"
    m_toolTip.AddToolForLabel Label2, 2, "�����Ŏ~���s�����ʐ�" & vbNewLine & "���J�����삩��̎��ʐ�(����ʂ��܂܂Ȃ��j"
    m_toolTip.AddToolForLabel Label3, 3, "�݌v�̌덷�ʐ��B" & vbNewLine & "���␳����ʁ{���ʕ��o���|���ʁ|�e��v���l"
    m_toolTip.AddToolForLabel Label4, 4, "�J�����삩��̌덷�ʐ��B"
    
    m_toolTip.AddTool SSRibbon1(0), "�R���g���[��(0)�̏�Ԃ�" & vbNewLine & SSRibbon1(0).Value & vbNewLine & "�ł��B"
    m_toolTip.AddTool SSRibbon1(1), "�R���g���[��(1)�̏�Ԃ�" & vbNewLine & SSRibbon1(1).Value & vbNewLine & "�ł��B"
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' �c�[���`�b�v��j������
    m_toolTip.destroyToolTip
End Sub

Private Sub SSRibbon1_Click(Index As Integer, Value As Integer)
    m_toolTip.UpdateTipText SSRibbon1(Index), Now & vbNewLine & "�R���g���[��(" & Index & ")�̏�Ԃ�" & vbNewLine & SSRibbon1(Index).Value & vbNewLine & "�ł��B"
End Sub
