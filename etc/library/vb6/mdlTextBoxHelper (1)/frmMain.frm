VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "mdlTextBoxHelperのテスト"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   2880
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   240
      Width           =   1215
   End
   Begin VB.CommandButton cmdTBHSelAllTest 
      Caption         =   "TBHSelAllのテスト"
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

Private Sub cmdTBHSelAllTest_Click()
    'テキストを全選択
    TBHSelAll Text1
    Debug.Assert Text1.SelStart = 0 And Text1.SelLength = Len(Text1.Text)
    
    'テスト完了
    MsgBox "TBHSelAll 関数は正常動作しています。" & vbCrLf & _
        "(このダイアログを閉じるとフォーカスがText1に移動します" & _
        "その段階で、選択されて見えます)", vbOKOnly, "テスト終了"
        
    Text1.SetFocus
End Sub
