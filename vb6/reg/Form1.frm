VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   7725
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   11415
   LinkTopic       =   "Form1"
   ScaleHeight     =   7725
   ScaleWidth      =   11415
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.Frame fraRegW32Time 
      Caption         =   "Windows Time Service"
      Height          =   1935
      Left            =   120
      TabIndex        =   13
      Top             =   3600
      Width           =   4695
      Begin VB.Frame fraRegW32TimeStart 
         Caption         =   "�X�^�[�g�A�b�v�̎��"
         Height          =   735
         Left            =   120
         TabIndex        =   17
         Top             =   240
         Width           =   4455
         Begin VB.OptionButton optRegW32TimeStart 
            Caption         =   "����"
            Height          =   375
            Index           =   2
            Left            =   2760
            TabIndex        =   20
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton optRegW32TimeStart 
            Caption         =   "�蓮"
            Height          =   375
            Index           =   1
            Left            =   1560
            TabIndex        =   19
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton optRegW32TimeStart 
            Caption         =   "����"
            Height          =   375
            Index           =   0
            Left            =   360
            TabIndex        =   18
            Top             =   240
            Width           =   1215
         End
      End
      Begin VB.Frame fraRegLocalNTP 
         Caption         =   "���[�J����NTP�T�[�o�[��"
         Height          =   735
         Left            =   120
         TabIndex        =   14
         Top             =   1080
         Width           =   4455
         Begin VB.OptionButton optRegLocalNTP 
            Caption         =   "����"
            Height          =   375
            Index           =   0
            Left            =   360
            TabIndex        =   16
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton optRegLocalNTP 
            Caption         =   "���Ȃ�"
            Height          =   375
            Index           =   1
            Left            =   1560
            TabIndex        =   15
            Top             =   240
            Width           =   1215
         End
      End
   End
   Begin VB.Frame fraRegAutoEndTasks 
      Caption         =   "�n���O�A�b�v�����A�v���������I�ɏI��"
      Height          =   735
      Left            =   120
      TabIndex        =   10
      Top             =   2760
      Width           =   4695
      Begin VB.OptionButton optRegAutoEndTasks 
         Caption         =   "�����Ȃ�"
         Height          =   375
         Index           =   1
         Left            =   1680
         TabIndex        =   12
         Top             =   240
         Width           =   1215
      End
      Begin VB.OptionButton optRegAutoEndTasks 
         Caption         =   "������"
         Height          =   375
         Index           =   0
         Left            =   480
         TabIndex        =   11
         Top             =   240
         Width           =   1215
      End
   End
   Begin VB.Frame fraRegPersistBrowsers 
      Caption         =   "�E�B���h�E��Ԃ�ێ�"
      Height          =   735
      Left            =   120
      TabIndex        =   7
      Top             =   1920
      Width           =   4695
      Begin VB.OptionButton optRegPersistBrowsers 
         Caption         =   "����"
         Height          =   375
         Index           =   0
         Left            =   480
         TabIndex        =   9
         Top             =   240
         Width           =   1215
      End
      Begin VB.OptionButton optRegPersistBrowsers 
         Caption         =   "���Ȃ�"
         Height          =   375
         Index           =   1
         Left            =   1680
         TabIndex        =   8
         Top             =   240
         Width           =   1215
      End
   End
   Begin VB.Frame fraRegNoSaveSettings 
      Caption         =   "�G�N�X�v���[���̐ݒ��ۑ�"
      Height          =   735
      Left            =   120
      TabIndex        =   4
      Top             =   1080
      Width           =   4695
      Begin VB.OptionButton optRegNoSaveSettings 
         Caption         =   "����"
         Height          =   375
         Index           =   0
         Left            =   480
         TabIndex        =   6
         Top             =   240
         Width           =   1215
      End
      Begin VB.OptionButton optRegNoSaveSettings 
         Caption         =   "���Ȃ�"
         Height          =   375
         Index           =   1
         Left            =   1680
         TabIndex        =   5
         Top             =   240
         Width           =   1215
      End
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   8640
      TabIndex        =   3
      Top             =   4680
      Width           =   1215
   End
   Begin VB.Frame fraRegNoDesktop 
      Caption         =   "�f�X�N�g�b�v�̃A�C�R����\��"
      Height          =   735
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   4695
      Begin VB.OptionButton optRegNoDesktop 
         Caption         =   "���Ȃ�"
         Height          =   375
         Index           =   1
         Left            =   1680
         TabIndex        =   2
         Top             =   240
         Width           =   1215
      End
      Begin VB.OptionButton optRegNoDesktop 
         Caption         =   "����"
         Height          =   375
         Index           =   0
         Left            =   480
         TabIndex        =   1
         Top             =   240
         Width           =   1215
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim m_reg As SousaRegAccess

Private Sub LoadRegs()
    optRegNoDesktop(IIf(m_reg.NoDesktop <> 1, 0, 1)).Value = True
    optRegNoSaveSettings(IIf(m_reg.NoSaveSettings <> 1, 0, 1)).Value = True
    optRegPersistBrowsers(IIf(m_reg.PersistBrowsers <> 0, 0, 1)).Value = True
    optRegAutoEndTasks(IIf(m_reg.AutoEndTasks = "1", 0, 1)).Value = True
    optRegLocalNTP(IIf(m_reg.LocalNTP = 1, 0, 1)).Value = True
    
    Select Case m_reg.W32TimeStart
    Case 2
        optRegW32TimeStart(0).Value = True
    Case 3
        optRegW32TimeStart(1).Value = True
    Case 4
        optRegW32TimeStart(2).Value = True
    Case Else
    End Select
End Sub

Private Sub SaveRegs()
    m_reg.NoDesktop = IIf(optRegNoDesktop(0).Value = True, 0, 1)
    m_reg.NoSaveSettings = IIf(optRegNoSaveSettings(0).Value = True, 0, 1)
    m_reg.PersistBrowsers = IIf(optRegPersistBrowsers(0).Value = True, 1, 0)
    m_reg.AutoEndTasks = IIf(optRegAutoEndTasks(0).Value = True, "1", "0")
    m_reg.LocalNTP = IIf(optRegLocalNTP(0).Value = True, 1, 0)
    
    If optRegW32TimeStart(0).Value = True Then
        m_reg.W32TimeStart = 2
    ElseIf optRegW32TimeStart(1).Value = True Then
        m_reg.W32TimeStart = 3
    ElseIf optRegW32TimeStart(2).Value = True Then
        m_reg.W32TimeStart = 4
    End If
End Sub

Private Sub Command1_Click()
    SaveRegs
End Sub

Private Sub Form_Load()
    Set m_reg = New SousaRegAccess
    
    LoadRegs
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_reg = Nothing
End Sub

