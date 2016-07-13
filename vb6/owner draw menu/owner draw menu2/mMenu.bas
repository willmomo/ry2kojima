Attribute VB_Name = "mMenu"
'
'*************************************************************************
'
'                        メニュー関連モジュール
'
'*************************************************************************
'
Option Explicit

'■■ 定数宣言 ■■
'---------------------------------------------------------
'     MENUITEMINFO 構造体の fType メンバで使用する
'---------------------------------------------------------
Public Const MF_STRING = &H0&
Public Const MF_BITMAP = &H4&
Public Const MF_MENUBARBREAK = &H20&
Public Const MF_MENUBREAK = &H40&
Public Const MF_OWNERDRAW = &H100&
Public Const MF_SEPARATOR = &H800&
Public Const MF_RIGHTJUSTIFY = &H4000&

Public Const MFT_STRING = MF_STRING                 ' テキスト文字列アイテム   : dwTypeData = 文字列のポインタ
                                                    '                          : cch        = 文字列サイズ
Public Const MFT_BITMAP = MF_BITMAP                 ' ビットマップアイテム     : dwTypeData = ビットマップハンドル
Public Const MFT_MENUBARBREAK = MF_MENUBARBREAK     ' バーブレークアイテム
Public Const MFT_MENUBREAK = MF_MENUBREAK           ' ブレークアイテム
Public Const MFT_OWNERDRAW = MF_OWNERDRAW           ' オーナードローアイテム   : dwTypeData = ユーザー32ビット値
Public Const MFT_RADIOCHECK = &H200&                ' hbmpChecked が NULLの場合 ラジオボタンマーク
Public Const MFT_SEPARATOR = MF_SEPARATOR           ' セパレータアイテム
Public Const MFT_RIGHTORDER = &H2000&               ' 右から左への文字（アラビア語）Windows95/NT 5.0 以降
Public Const MFT_RIGHTJUSTIFY = MF_RIGHTJUSTIFY     '

' MFT_BITMAP, MFT_SEPARATOR, MFT_STRING これらのフラグは同時に使用できない

'---------------------------------------------------------
'    MENUITEMINFO 構造体の fState メンバで使用する
'---------------------------------------------------------
Public Const MF_UNCHECKED = &H0&
Public Const MF_ENABLED = &H0&
Public Const MF_GRAYED = &H1&
Public Const MF_DISABLED = &H2&
Public Const MF_CHECKED = &H8&
Public Const MF_POPUP = &H10&
Public Const MF_UNHILITE = &H0&
Public Const MF_HILITE = &H80&
Public Const MF_DEFAULT = &H1000&

Public Const MFS_GRAYED = &H3&                      ' 選択不可（灰色）アイテム
Public Const MFS_DISABLED = MFS_GRAYED              ' 選択不可（灰色）アイテム
Public Const MFS_CHECKED = MF_CHECKED               ' チェックアイテム
Public Const MFS_HILITE = MF_HILITE                 ' 強調アイテム
Public Const MFS_ENABLED = MF_ENABLED               ' 選択可アイテム
Public Const MFS_UNCHECKED = MF_UNCHECKED           ' 非チェックアイテム
Public Const MFS_UNHILITE = MF_UNHILITE             ' 非強調アイテム
Public Const MFS_DEFAULT = MF_DEFAULT               ' デフォルトアイテム（太字?）

'---------------------------------------------------------
'     MENUITEMINFO 構造体の fMask メンバで使用する
'---------------------------------------------------------
Public Const MIIM_STATE = &H1       ' fState メンバの取得または設定
Public Const MIIM_ID = &H2          ' wID メンバの取得または設定
Public Const MIIM_SUBMENU = &H4     ' hSubMenu メンバの取得または設定
Public Const MIIM_CHECKMARKS = &H8  ' hbmpChecked,hbmpUnchecked メンバの取得または設定
Public Const MIIM_TYPE = &H10       ' fType,dwTypeData メンバの取得または設定
Public Const MIIM_DATA = &H20       ' dwItemData メンバの取得または設定

'■■ 構造体宣言 ■■
Type MENUITEMINFO
    cbSize As Long          ' この構造体のサイズ
    fMask As Long           ' 取得または設定するメンバの指定
    fType As Long           ' メニューアイテムのタイプ
    fState As Long          ' メニューアイテムの状態
    wID As Long             ' メニューアイテム識別ID
    hSubMenu As Long        ' サブメニューハンドル（サブメニューを持たない場合NULL）
    hbmpChecked As Long     ' チェック時ビットマップハンドル
    hbmpUnchecked As Long   ' 非チェック時ビットマップハンドル
    dwItemData As Long      ' メニューアイテムに関連したアプリケーション定義情報
    dwTypeData As String    ' メニューアイテムの内容
    cch As Long             ' テキストの長さ
End Type

'■■ API関数宣言 ■■
' ウィンドウに割り当てられたメニューのハンドルを取得
Declare Function GetMenu Lib "user32" (ByVal hwnd As Long) As Long
' サブメニューのハンドルを取得
Declare Function GetSubMenu Lib "user32" (ByVal hMenu As Long, ByVal nPos As Long) As Long
' メニューの項目数取得
Declare Function GetMenuItemCount Lib "user32" (ByVal hMenu As Long) As Long
' メニュー項目のIDを取得
Declare Function GetMenuItemID Lib "user32" (ByVal hMenu As Long, ByVal nPos As Long) As Long
' メニュー項目に関する情報を取得
Declare Function GetMenuItemInfo Lib "user32" Alias "GetMenuItemInfoA" (ByVal hMenu As Long, ByVal un As Long, ByVal b As Boolean, lpMenuItemInfo As MENUITEMINFO) As Long
' メニュー項目に関する情報を設定
Declare Function SetMenuItemInfo Lib "user32" Alias "SetMenuItemInfoA" (ByVal hMenu As Long, ByVal un As Long, ByVal bool As Boolean, lpcMenuItemInfo As MENUITEMINFO) As Long
' メニューバーを再描画
Declare Function DrawMenuBar Lib "user32" (ByVal hwnd As Long) As Long

'■■ メニュー情報を保存する為の宣言 ■■
Type tMenuStruct
    hMenu As Long               ' メニューハンドル
    lngPosition As Long         ' メニューポジション(一番上の項目を0とした場合の位置)
    strMenu As String           ' メニュー文字列
End Type

Public tMenu() As tMenuStruct   ' メニュー情報を保存する配列
Public lngtMenuCount As Long    ' tMenu 配列の数

'---------------------------------------
' メニュー情報保存 & オーナードロー設定
'---------------------------------------
Private Sub psubGetMenuItemInfo(ByVal hMenu As Long, ByVal lngID As Long, ByVal booKey As Boolean, ByVal lngPosition As Long)
    Dim tMENUINFO As MENUITEMINFO   ' MENUITEMINFO構造体
    Dim strMenu As String           ' メニュー文字列
    
    With tMENUINFO
        .cbSize = Len(tMENUINFO)
        .fMask = MIIM_TYPE Or MIIM_DATA
        .dwTypeData = Space(255)
        .cch = 255
        ' メニュー項目情報取得
        If GetMenuItemInfo(hMenu, lngID, booKey, tMENUINFO) <> 0 Then
            ' セパレータの場合はオーナードローにしない
            If .fType And MF_SEPARATOR Then Exit Sub
            ' メニューテキストを取得
            strMenu = Left(.dwTypeData, InStr(.dwTypeData, Chr(0)) - 1)
            ' メニュー情報を保存する配列の確保
            ReDim Preserve tMenu(lngtMenuCount)
            ' メニュー情報を保存
            tMenu(lngtMenuCount).hMenu = hMenu
            tMenu(lngtMenuCount).lngPosition = lngPosition
            tMenu(lngtMenuCount).strMenu = strMenu
            ' オーナードローフラグ & アイテムデータ セット
            .fType = .fType Or MF_OWNERDRAW
            .dwItemData = lngtMenuCount
            Call SetMenuItemInfo(hMenu, lngID, booKey, tMENUINFO)
            lngtMenuCount = lngtMenuCount + 1
        End If
    End With
End Sub

'------------------------------------------------------
' 指定されたウインドウのメニューをオーナードローに設定
'------------------------------------------------------
Public Sub SetMenuOwnerDraw(ByVal hMenu As Long)
    Dim lngCount As Long    ' メニューの項目数
    Dim i As Long           ' ループカウンタ
    Dim lngID As Long       ' メニュー項目識別ID
    
    ' メニューの項目数取得
    lngCount = GetMenuItemCount(hMenu)
    ' メニュー項目ループ
    For i = 0 To lngCount - 1
        ' メニュー項目のID取得
        lngID = GetMenuItemID(hMenu, i)
        If lngID = -1 Then
            ' サブメニュー項目の場合
            ' メニュー情報保存 & オーナードロー設定
            Call psubGetMenuItemInfo(hMenu, i, True, i)
            ' 再度メニュー項目列挙
            Call SetMenuOwnerDraw(GetSubMenu(hMenu, i))
        Else
            ' メニュー項目の場合
            ' メニュー情報保存 & オーナードロー設定
            Call psubGetMenuItemInfo(hMenu, lngID, False, i)
        End If
    Next i
End Sub





