VERSION 5.00
Begin VB.Form frmWait 
   BackColor       =   &H80000003&
   BorderStyle     =   0  '�Ȃ�
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '��ʂ̒���
   Begin VB.Timer tim 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   60
      Top             =   60
   End
   Begin VB.Label lblMsg 
      Alignment       =   2  '��������
      BackColor       =   &H80000003&
      Caption         =   "�`�F�b�N���E�E�E"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   27.75
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000013&
      Height          =   1215
      Left            =   120
      TabIndex        =   0
      Top             =   840
      Width           =   4395
   End
End
Attribute VB_Name = "frmWait"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_startTime As Date
Private m_hasError As Long      '0=����I�� 1=�G���[ 2=�^�C���A�E�g 3=������

Public Property Get Error() As Long
    Error = m_hasError
End Property

Private Sub Form_Load()

    '
    ' ������Ԃ͖�����
    '
    m_hasError = 3
    
    '
    ' ���g��Load���ꂽ�Ƃ����`�F�b�N�J�n���ԂƂ���
    '
    m_startTime = Now
    
    '
    ' �`�F�b�N�^�C�}�[ �X�^�[�g
    '
    tim.Enabled = True
    
End Sub

Private Sub tim_Timer()
    Dim rc As Long
    Static m_lastCheck As Date
    
    '
    ' 2���o�߂��Ă��t�@�C�����폜����Ȃ��Ƃ��́A�G���[�Ƃ��ďI������B
    '
    If DateDiff("s", m_startTime, Now) > 120 Then
        m_hasError = 2
        tim.Enabled = False
        Unload Me
    End If
    
    '
    ' �O��`�F�b�N����A10�b�o�߂�����AS�̏�Ԃ��`�F�b�N
    ' �`�F�b�N��������߂����Ƃ��́A�G���[��OFF�ɂ��ďI������B
    '
    If DateDiff("s", m_lastCheck, Now) > 10 Then
        rc = CheckRecovery
        
        If rc = 1 Then      ' ��������
            m_hasError = 0
            tim.Enabled = False
            Unload Me
        ElseIf rc = 2 Then  '�����ُ�
            m_hasError = 1
            tim.Enabled = False
            Unload Me
        End If
    End If
End Sub
