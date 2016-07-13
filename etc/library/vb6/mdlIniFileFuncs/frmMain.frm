VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlIniFileFuncs�̃e�X�g"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton mcdIIFGetPrivateProfileStringTest 
      Caption         =   "IIFGetPrivateProfileString�̃e�X�g"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   3015
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub mcdIIFGetPrivateProfileStringTest_Click()
    Dim sValue As String
    Dim rc As Long
    
    '���ʂɎ擾�ł���l
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test1", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 0 And sValue = "�����́AIFFGetPrivateProfileString �֐��̃e�X�g�p�L�[�ł��B"
    
    '���ɒ����l
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test2", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 0 And sValue = "VeryLong" & String(480, "*")
    
    '�f�t�H���g�l���擾�ł���ꍇ
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test3", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 1 And sValue = "default value"
    
    '�f�t�H���g�l�Ɠ����l���擾�ł���ꍇ
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test4", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 0 And sValue = "default value"

    '�e�X�g����
    MsgBox "IFFGetPrivateProfileString �֐��͐��퓮�삵�Ă��܂��B", vbOKOnly, "�e�X�g�I��"
End Sub
