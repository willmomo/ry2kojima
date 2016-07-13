Attribute VB_Name = "mdlTextBoxHelper"
'----------------------------------------------------------------------
'   [概要]
'   TextBoxコントロールを便利に扱う関数群
'
'   [更新日]
'   2003/11/01 : 新規作成

Option Explicit

'----------------------------------------------------------------------
'   [概要]
'   テキストを全選択する
'
'   [引数]
'   tb  : TextBoxコントロール
'
'   [戻り値]
'   なし
'
'   [説明]
'   TextBoxにフォーカスがないと選択されて見えないことに注意。

Public Sub TBHSelAll(tb As TextBox)
    With tb
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub
