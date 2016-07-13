VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlSafeConvertのテスト"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton cmdSafeCLngTest 
      Caption         =   "SafeCLngのテスト"
      Height          =   495
      Left            =   240
      TabIndex        =   1
      Top             =   960
      Width           =   1695
   End
   Begin VB.CommandButton cmdSafeCInt 
      Caption         =   "SafeCIntのテスト"
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
    
    '変換可能なプラスの値
    Debug.Assert SafeCInt(10, intValue) = 0
    Debug.Assert intValue = 10
    
    '変換可能なマイナスの値
    Debug.Assert SafeCInt(-10, intValue) = 0
    Debug.Assert intValue = -10
    
    'オーバーフローする値
    rc = SafeCInt(50000, intValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    '変換不可能な値
    rc = SafeCInt("Not Value", intValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    'テスト完了
    MsgBox "SafeCInt 関数は正常動作しています。", vbOKOnly, "テスト終了"
End Sub

Private Sub cmdSafeCLngTest_Click()
    Dim lngValue As Long
    Dim rc As Long
    
    '変換可能なプラスの値
    Debug.Assert SafeCLng(40000, lngValue) = 0
    Debug.Assert lngValue = 40000
    
    '変換可能なマイナスの値
    Debug.Assert SafeCLng(-40000, lngValue) = 0
    Debug.Assert lngValue = -40000
    
    'オーバーフローする値
    rc = SafeCLng(3000000000#, lngValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    '変換不可能な値
    rc = SafeCLng("Not Value", lngValue)
    Debug.Assert rc <> 0
    Debug.Print Now, rc, Error(rc)
    
    'テスト完了
    MsgBox "SafeCLng 関数は正常動作しています。", vbOKOnly, "テスト終了"
End Sub
