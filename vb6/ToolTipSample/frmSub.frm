VERSION 5.00
Begin VB.Form frmSub 
   BorderStyle     =   3  '�Œ��޲�۸�
   Caption         =   "Form1"
   ClientHeight    =   4005
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5985
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4005
   ScaleWidth      =   5985
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.FileListBox File1 
      Height          =   450
      Left            =   1440
      TabIndex        =   13
      Top             =   3000
      Width           =   1215
   End
   Begin VB.DirListBox Dir1 
      Height          =   510
      Left            =   120
      TabIndex        =   12
      Top             =   3240
      Width           =   1215
   End
   Begin VB.DriveListBox Drive1 
      Height          =   300
      Left            =   120
      TabIndex        =   11
      Top             =   2760
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   3120
      TabIndex        =   10
      Text            =   "Text1"
      Top             =   2040
      Width           =   1215
   End
   Begin VB.VScrollBar VScroll1 
      Height          =   1215
      Left            =   5520
      TabIndex        =   9
      Top             =   600
      Width           =   255
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   3120
      TabIndex        =   8
      Top             =   1440
      Width           =   1335
   End
   Begin VB.ListBox List1 
      Height          =   420
      Left            =   3120
      TabIndex        =   7
      Top             =   840
      Width           =   1215
   End
   Begin VB.ComboBox Combo1 
      Height          =   300
      Left            =   4560
      TabIndex        =   6
      Text            =   "Combo1"
      Top             =   240
      Width           =   1215
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Option1"
      Height          =   495
      Left            =   3120
      TabIndex        =   5
      Top             =   120
      Width           =   1215
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Check1"
      Height          =   495
      Left            =   120
      TabIndex        =   4
      Top             =   2040
      Width           =   2775
   End
   Begin VB.Frame Frame1 
      Caption         =   "Frame1"
      Height          =   1095
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   2775
      Begin VB.CommandButton Command1 
         Caption         =   "Command1"
         Height          =   495
         Left            =   960
         TabIndex        =   3
         Top             =   360
         Width           =   1215
      End
   End
   Begin VB.PictureBox Picture1 
      Height          =   495
      Left            =   120
      ScaleHeight     =   435
      ScaleWidth      =   1155
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   495
      Left            =   1560
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
End
Attribute VB_Name = "frmSub"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_toolTip As New clsToolTip

Private Sub Form_Load()
    ' �c�[���`�b�v���쐬����
    If m_toolTip.createToolTip(Me.hwnd, False) = False Then
        MsgBox "�c�[���`�b�v�̍쐬�Ɏ��s!!"
        End
    End If
    
    m_toolTip.MaxTipWidth = 1000
    
    m_toolTip.AddTool Picture1, "����́y" & Picture1.Name & "�z"
    m_toolTip.AddToolForLabel Label1, 1, "����́y" & Label1.Name & "�z"
    m_toolTip.AddTool Frame1, "����́y" & Frame1.Name & "�z"
    m_toolTip.AddTool Command1, "����́y" & Command1.Name & "�z"
    m_toolTip.AddTool Check1, "����́y" & Check1.Name & "�z"
    m_toolTip.AddTool Option1, "����́y" & Option1.Name & "�z"
    m_toolTip.AddTool Combo1, "����́y" & Combo1.Name & "�z"
    m_toolTip.AddTool List1, "����́y" & List1.Name & "�z"
    m_toolTip.AddTool HScroll1, "����́y" & HScroll1.Name & "�z"
    m_toolTip.AddTool VScroll1, "����́y" & VScroll1.Name & "�z"
    m_toolTip.AddTool Text1, "����́y" & Text1.Name & "�z"
    m_toolTip.AddTool Drive1, "����́y" & Drive1.Name & "�z"
    m_toolTip.AddTool Dir1, "����́y" & Dir1.Name & "�z"
    m_toolTip.AddTool File1, "����́y" & File1.Name & "�z"
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' �c�[���`�b�v��j������
    m_toolTip.destroyToolTip
End Sub
