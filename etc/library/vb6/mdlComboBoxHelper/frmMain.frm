VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlComboBoxHelper�̃e�X�g"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.ComboBox Combo1 
      Height          =   300
      Left            =   3000
      Sorted          =   -1  'True
      Style           =   2  '��ۯ���޳� ؽ�
      TabIndex        =   1
      Top             =   360
      Width           =   1215
   End
   Begin VB.CommandButton cmdCBHAppendItemTest 
      Caption         =   "CBHAppendItem�̃e�X�g"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   2415
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdCBHAppendItemTest_Click()
    Dim i As Long
    
    'CBHAppendItem�̃e�X�g
    CBHAppendItem Combo1, "0��"
    CBHAppendItem Combo1, "1��", 10
    CBHAppendItem Combo1, "3��", 30
    CBHAppendItem Combo1, "2��", 20
    
    For i = 0 To 3
        Debug.Assert Combo1.List(i) = CStr(i) & "��"
        Debug.Assert Combo1.ItemData(i) = i * 10
    Next
    
    '�e�X�g����
    MsgBox "CBHAppendItem �֐��͐��퓮�삵�Ă��܂��B", vbOKOnly, "�e�X�g�I��"
End Sub
