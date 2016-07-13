VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlComboBoxHelperのテスト"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.ComboBox Combo1 
      Height          =   300
      Left            =   3000
      Sorted          =   -1  'True
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   1
      Top             =   360
      Width           =   1215
   End
   Begin VB.CommandButton cmdCBHAppendItemTest 
      Caption         =   "CBHAppendItemのテスト"
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
    
    'CBHAppendItemのテスト
    CBHAppendItem Combo1, "0個目"
    CBHAppendItem Combo1, "1個目", 10
    CBHAppendItem Combo1, "3個目", 30
    CBHAppendItem Combo1, "2個目", 20
    
    For i = 0 To 3
        Debug.Assert Combo1.List(i) = CStr(i) & "個目"
        Debug.Assert Combo1.ItemData(i) = i * 10
    Next
    
    'テスト完了
    MsgBox "CBHAppendItem 関数は正常動作しています。", vbOKOnly, "テスト終了"
End Sub
