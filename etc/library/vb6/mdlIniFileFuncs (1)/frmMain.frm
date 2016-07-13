VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlIniFileFuncsのテスト"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton mcdIIFGetPrivateProfileStringTest 
      Caption         =   "IIFGetPrivateProfileStringのテスト"
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
    
    '普通に取得できる値
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test1", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 0 And sValue = "これらは、IFFGetPrivateProfileString 関数のテスト用キーです。"
    
    '非常に長い値
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test2", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 0 And sValue = "VeryLong" & String(480, "*")
    
    'デフォルト値を取得できる場合
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test3", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 1 And sValue = "default value"
    
    'デフォルト値と同じ値を取得できる場合
    rc = IFFGetPrivateProfileString("IniFileFuncs", "Test4", "default value", sValue, App.Path & "\IniFileFuncsTest.ini")
    Debug.Assert rc = 0 And sValue = "default value"

    'テスト完了
    MsgBox "IFFGetPrivateProfileString 関数は正常動作しています。", vbOKOnly, "テスト終了"
End Sub
