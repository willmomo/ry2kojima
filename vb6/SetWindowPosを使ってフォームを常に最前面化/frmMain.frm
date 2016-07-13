VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "SetWindowPosを使ってフォームを常に最前面化"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton cmdEnd 
      Caption         =   "終了(&X)"
      Height          =   495
      Left            =   1800
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
   End
   Begin VB.TextBox txtSample 
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Text            =   "フォーカスに関して注意が必要"
      Top             =   720
      Width           =   4455
   End
   Begin VB.Label Label1 
      Caption         =   "SetWindowPosを使ってフォームを常に最前面化"
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4455
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'----------------------------------------------------------------------
' Click イベント
'----------------------------------------------------------------------
Private Sub cmdEnd_Click()
    Unload Me
End Sub

'----------------------------------------------------------------------
' Load イベント
'----------------------------------------------------------------------
Private Sub Form_Load()

    ' SWP_NOACTIVATEをつけて呼ばないと、フォーカスがフォームに行ってしまう。
    ' アクティベートの処理をVB6にやらせる必要があるんじゃないかな。
    
    Call SetWindowPos(Me.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOACTIVATE)
End Sub

'----------------------------------------------------------------------
' GotFocus イベント
'
' テキストを全選択する処理
'----------------------------------------------------------------------
Private Sub txtSample_GotFocus()
    txtSample.SelStart = 0
    txtSample.SelLength = Len(txtSample.Text)
End Sub
