Attribute VB_Name = "mdlComboBoxHelper"
'----------------------------------------------------------------------
'   [概要]
'   ComboBoxコントロールを便利に扱う関数群
'
'   [更新日]
'   2003/11/01 : 新規作成

Option Explicit

'----------------------------------------------------------------------
'   [概要]
'   ComboBoxにアイテムを追加すると同時に関連データも追加する。
'
'   [引数]
'   cb      : ComboBoxコントロール
'   item    : ComboBoxに追加するアイテム
'   number  : アイテムに関連付けるデータ
'
'   [戻り値]
'   なし
'
'   [説明]

Public Sub CBHAppendItem(cb As ComboBox, item, Optional number = 0)
    With cb
        .AddItem item
        .ItemData(.NewIndex) = number
    End With
End Sub

