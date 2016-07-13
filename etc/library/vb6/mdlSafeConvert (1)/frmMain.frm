VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlSafeConvert�̃e�X�g"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton cmdSafeCLngTest 
      Caption         =   "SafeCLng�̃e�X�g"
      Height          =   495
      Left            =   240
      TabIndex        =   1
      Top             =   960
      Width           =   1695
   End
   Begin VB.CommandButton cmdSafeCInt 
      Caption         =   "SafeCInt�̃e�X�g"
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

Private Sub cmdSafeCInt_Click()
    Dim intValue As Integer
    Dim rc As Long
    
    '�ϊ��\�ȃv���X�̒l
    Debug.Assert SafeCInt(10, intValue) = 0
    Debug.Assert intValue = 10
    
    '�ϊ��\�ȃ}�C�i�X�̒l
    Debug.Assert SafeCInt(-10, intValue) = 0
    Debug.Assert intValue = -10
    
    '�I�[�o�[�t���[����l
    rc = SafeCInt(50000, intValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    '�ϊ��s�\�Ȓl
    rc = SafeCInt("Not Value", intValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    '�e�X�g����
    MsgBox "SafeCInt �֐��͐��퓮�삵�Ă��܂��B", vbOKOnly, "�e�X�g�I��"
End Sub

Private Sub cmdSafeCLngTest_Click()
    Dim lngValue As Long
    Dim rc As Long
    
    '�ϊ��\�ȃv���X�̒l
    Debug.Assert SafeCLng(40000, lngValue) = 0
    Debug.Assert lngValue = 40000
    
    '�ϊ��\�ȃ}�C�i�X�̒l
    Debug.Assert SafeCLng(-40000, lngValue) = 0
    Debug.Assert lngValue = -40000
    
    '�I�[�o�[�t���[����l
    rc = SafeCLng(3000000000#, lngValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    '�ϊ��s�\�Ȓl
    rc = SafeCLng("Not Value", lngValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    '�e�X�g����
    MsgBox "SafeCLng �֐��͐��퓮�삵�Ă��܂��B", vbOKOnly, "�e�X�g�I��"
End Sub
