VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   Caption         =   "Owner Draw - [ Menu ]"
   ClientHeight    =   3495
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   233
   ScaleMode       =   3  '�߸��
   ScaleWidth      =   312
   StartUpPosition =   3  'Windows �̊���l
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   1920
      Top             =   1920
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   17
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0000
            Key             =   "�t�@�C��(&F)"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0454
            Key             =   "�\��(&V)"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0568
            Key             =   "����(&O)"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":067C
            Key             =   "�V�K�쐬(&C)"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0790
            Key             =   "�J��(&O)"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":08A4
            Key             =   "�ۑ�(&S)"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":09B8
            Key             =   "���(&P)"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0ACC
            Key             =   "�v���r���[(&V)"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1010
            Key             =   "�傫���A�C�R��(&G)"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1124
            Key             =   "�������A�C�R��(&M)"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1238
            Key             =   "�ꗗ(&L)"
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":134C
            Key             =   "�ڍ�(&V)"
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1460
            Key             =   "��(&L)"
         EndProperty
         BeginProperty ListImage14 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1574
            Key             =   "����(&C)"
         EndProperty
         BeginProperty ListImage15 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1688
            Key             =   "�E(&R)"
         EndProperty
         BeginProperty ListImage16 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":179C
            Key             =   "�őO�ʂɈړ�(&B)"
         EndProperty
         BeginProperty ListImage17 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":18B0
            Key             =   "�Ŕw�ʂɈړ�(&S)"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "�t�@�C��(&F)"
      Begin VB.Menu mnuFile_Create 
         Caption         =   "�V�K�쐬(&C)"
      End
      Begin VB.Menu mnuFile_Open 
         Caption         =   "�J��(&O)"
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFile_Save 
         Caption         =   "�ۑ�(&S)"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFile_Line1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFile_Print 
         Caption         =   "���(&P)"
         Shortcut        =   ^L
      End
      Begin VB.Menu mnuFile_Preview 
         Caption         =   "�v���r���[(&V)"
      End
      Begin VB.Menu mnuFile_Line2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFile_End 
         Caption         =   "�I��(&X)"
      End
   End
   Begin VB.Menu mnuDisp 
      Caption         =   "�\��(&V)"
      Begin VB.Menu mnuDisp_View 
         Caption         =   "�傫���A�C�R��(&G)"
         Checked         =   -1  'True
         Index           =   0
      End
      Begin VB.Menu mnuDisp_View 
         Caption         =   "�������A�C�R��(&M)"
         Index           =   1
      End
      Begin VB.Menu mnuDisp_View 
         Caption         =   "�ꗗ(&L)"
         Index           =   2
      End
      Begin VB.Menu mnuDisp_View 
         Caption         =   "�ڍ�(&V)"
         Index           =   3
      End
   End
   Begin VB.Menu mnuFormat 
      Caption         =   "����(&O)"
      Begin VB.Menu mnuFormat_Stand 
         Caption         =   "����(&A)"
         Begin VB.Menu mnuFormat_Stand_Left 
            Caption         =   "��(&L)"
         End
         Begin VB.Menu mnuFormat_Stand_Center 
            Caption         =   "����(&C)"
         End
         Begin VB.Menu mnuFormat_Stand_Right 
            Caption         =   "�E(&R)"
         End
      End
      Begin VB.Menu mnuFormat_Order 
         Caption         =   "����(&O)"
         Begin VB.Menu mnuFormat_Order_Befor 
            Caption         =   "�őO�ʂɈړ�(&B)"
            Shortcut        =   ^J
         End
         Begin VB.Menu mnuFormat_Order_After 
            Caption         =   "�Ŕw�ʂɈړ�(&S)"
            Shortcut        =   ^K
         End
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    ' ���j���[�̃I�[�i�[�h���[�ݒ�
    Call SetMenuOwnerDraw(GetMenu(Me.hwnd))
    ' �T�u�N���X���̊J�n
    Call SubClass(Me.hwnd)
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    ' �T�u�N���X���̏I��
    Call UnSubClass
End Sub

Private Sub mnuDisp_View_Click(Index As Integer)
    mnuDisp_View(0).Checked = False
    mnuDisp_View(1).Checked = False
    mnuDisp_View(2).Checked = False
    mnuDisp_View(3).Checked = False
    mnuDisp_View(Index).Checked = True
End Sub

