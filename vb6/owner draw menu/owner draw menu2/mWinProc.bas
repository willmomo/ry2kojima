Attribute VB_Name = "mWinProc"
'
'*************************************************************************
'
'                        サブクラス化汎用モジュール
'
'*************************************************************************
'
Option Explicit

' ■■ 変数宣言 ■■

Private P_Orghwnd As Long   ' サブクラス化されているウインドウのハンドル保持
Private P_OrgProc As Long   ' デフォルトウインドウプロシージャアドレス保持

' ■■ 定数宣言 ■■

Public Const GWL_WNDPROC = (-4)     ' ウインドウプロシージャの変更を要求する

' ■■ API関数宣言 ■■

' 指定ウインドウの属性を変更する関数
Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" ( _
                                                ByVal hwnd As Long, _
                                                ByVal nIndex As Long, _
                                                ByVal dwNewLong As Long) As Long

' 指定ウインドウの属性を取得する関数
Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" ( _
                                                ByVal hwnd As Long, _
                                                ByVal nIndex As Long) As Long

' 指定ウインドウプロシージャにメッセージを送る関数
Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" ( _
                                                ByVal lpPrevWndFunc As Long, _
                                                ByVal hwnd As Long, _
                                                ByVal msg As Long, _
                                                ByVal wParam As Long, _
                                                ByVal lParam As Long) As Long
                                                
'■■■ 定数宣言 ■■■

' ウインドウメッセージ
Public Const WM_DRAWITEM = &H2B        ' コントロールやメニューの外観が変更された時
Public Const WM_MEASUREITEM = &H2C     ' Windowsが項目の大きさを要求してきた時
Public Const WM_MENUCHAR = &H120       ' メニューアクティブ時にキーが押された時
    Public Const MNC_IGNORE = 0
    Public Const MNC_CLOSE = 1
    Public Const MNC_EXECUTE = 2
    Public Const MNC_SELECT = 3

' オーナードロータイプ
Public Const ODT_MENU = 1              ' メニュー
Public Const ODT_LISTBOX = 2           ' リストボックス
Public Const ODT_COMBOBOX = 3          ' コンボボックス
Public Const ODT_BUTTON = 4            ' ボタン
Public Const ODT_LISTVIEW = 102        ' リストビュー

' オーナードローアクション
Public Const ODA_DRAWENTIRE = &H1      ' 全体を描画
Public Const ODA_SELECT = &H2          ' フォーカス変化
Public Const ODA_FOCUS = &H4           ' 選択状態変化

' オーナードローステータス
Public Const ODS_SELECTED = &H1        ' 選択状態
Public Const ODS_GRAYED = &H2          ' 淡色表示
Public Const ODS_DISABLED = &H4        ' 使用禁止
Public Const ODS_CHECKED = &H8         ' チェック（メニューのみ有効）
Public Const ODS_FOCUS = &H10          ' フォーカスを持つ
Public Const ODS_DEFAULT = &H20        ' デフォルト
Public Const ODS_COMBOBOXEDIT = &H1000 ' コンボボックスエディット

'■■■ 構造体宣言 ■■■
Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

'MEASUREITEMSTRUCT 構造体
Type MEASUREITEMSTRUCT
    CtlType As Long     ' コントロールタイプ（定数「オーナードロータイプ」参照）
    CtlID As Long       ' コントロール ID。メニューの場合未使用
    itemID As Long      ' メニューの場合、メニューID
    itemWidth As Long   ' 幅
    itemHeight As Long  ' 高さ
    itemData As Long    ' アイテムデータ（オーナードローを設定した時にセット）
End Type

'DRAWITEMSTRUCT 構造体
Type DRAWITEMSTRUCT
    CtlType As Long     ' コントロールタイプ（定数「オーナードロータイプ」参照）
    CtlID As Long       ' コントロール ID
    itemID As Long      ' MEASUREITEMSTRUCT 構造体参照
    itemAction As Long  ' アクション（定数「オーナードローアクション」参照）
    itemState As Long   ' アイテム状態（定数「オーナードローステータス」参照）
    hwndItem As Long    ' ウインドウハンドル
    hdc As Long         ' デバイスコンテキスト
    rcItem As RECT      ' アイテムの描画領域
    itemData As Long    ' MEASUREITEMSTRUCT 構造体参照
End Type

'----------------------------
' オーナードローメニュー表示
'----------------------------
'tDRAWITEMSTRUCT    :DRAWITEMSTRUCT 構造体
Private Sub psubDRAWITEM(tDRAWITEMSTRUCT As DRAWITEMSTRUCT)
    Dim lngPrevTextColor As Long ' 設定前のテキスト色
    Dim lngPrevBackMode As Long  ' 設定前の背景モード
    Dim lngBackColor As Long     ' 背景色
    Dim hBrush As Long           ' ブラシハンドル
    Dim tRCText As RECT          ' RECT構造体-文字
    Dim strImageKey As String    ' イメージリストキー
    Dim lngImageFlag As Long     ' イメージ描画フラグ
    
    With tDRAWITEMSTRUCT
        ' ステータス別の設定
        ' デフォルト（通常のメニュー項目の場合）
        lngPrevTextColor = SetTextColor(.hdc, GetSysColor(COLOR_MENUTEXT))
        lngBackColor = GetSysColor(COLOR_MENU)
        If .itemState And (ODS_GRAYED Or ODS_DISABLED) Then
            ' 選択不可項目
            lngPrevTextColor = SetTextColor(.hdc, GetSysColor(COLOR_GRAYTEXT))
        ElseIf .itemState And ODS_SELECTED Then
            ' 選択されているメニュー項目の場合
            lngPrevTextColor = SetTextColor(.hdc, GetSysColor(COLOR_HIGHLIGHTTEXT))
            lngBackColor = GetSysColor(COLOR_HIGHLIGHT)
        End If
        
        ' 背景の塗りつぶしブラシ作成
        hBrush = CreateSolidBrush(lngBackColor)
        ' 背景の塗りつぶし
        Call FillRect(.hdc, .rcItem, hBrush)
        ' 背景の塗りつぶしブラシ破棄
        Call DeleteObject(hBrush)
        
        ' 各RECT構造体初期化
        tRCText = .rcItem
        tRCText.Left = tRCText.Left + 22
        ' 背景モードの設定
        lngPrevBackMode = SetBkMode(.hdc, TRANSPARENT)
        ' メニュー文字列描画
        DrawText .hdc, tMenu(.itemData).strMenu, -1, tRCText, DT_EXPANDTABS Or DT_LEFT Or DT_VCENTER Or DT_SINGLELINE
        ' アイコンの描画
        strImageKey = tMenu(.itemData).strMenu
        If InStr(strImageKey, Chr(9)) Then strImageKey = Left(strImageKey, InStr(strImageKey, Chr(9)) - 1)
        lngImageFlag = imlTransparent
        If .itemState And ODS_CHECKED Then lngImageFlag = imlSelected
        On Error Resume Next
        Form1.ImageList1.ListImages(strImageKey).Draw .hdc, .rcItem.Left + 2, .rcItem.Top, lngImageFlag
        On Error GoTo 0
        ' テキスト色、背景色、背景モードをもとに戻す
        SetTextColor .hdc, lngPrevTextColor
        SetBkMode .hdc, lngPrevBackMode
    End With
End Sub

'--------------------------
' 独自ウインドウプロシージャ
'--------------------------
Public Function WindowProc(ByVal hwnd As Long, _
                           ByVal uMsg As Long, _
                           ByVal wParam As Long, _
                           ByVal lParam As Long) As Long

    Dim hdc As Long     ' デバイスコンテキストハンドル
    Dim tRC As RECT     ' RECT構造体
    Dim tMEASUREITEMSTRUCT As MEASUREITEMSTRUCT
    Dim tDRAWITEMSTRUCT As DRAWITEMSTRUCT
    
    Select Case uMsg
    ' メニューサイズのセット
    Case WM_MEASUREITEM
        ' lParamの指すMEASUREITEMSTRUCT構造体の内容をコピー
        CopyMemory tMEASUREITEMSTRUCT, ByVal lParam, ByVal Len(tMEASUREITEMSTRUCT)
        ' ウインドウハンドルよりデバイスコンテキストハンドル取得
        hdc = GetDC(hwnd)
        ' 文字列サイズ取得
        With tMenu(tMEASUREITEMSTRUCT.itemData)
            Call DrawText(hdc, .strMenu, -1, tRC, DT_CALCRECT Or DT_EXPANDTABS)
        End With
        ' サイズの設定
        tMEASUREITEMSTRUCT.itemWidth = tRC.Right + 22
        tMEASUREITEMSTRUCT.itemHeight = tRC.Bottom + 8
        ' デバイスコンテキストハンドルの開放
        ReleaseDC hwnd, hdc
        ' 設定したMEASUREITEMSTRUCT構造体を書き戻す
        CopyMemory ByVal lParam, tMEASUREITEMSTRUCT, ByVal Len(tMEASUREITEMSTRUCT)
        Exit Function
    ' オーナードロー描画
    Case WM_DRAWITEM
        CopyMemory tDRAWITEMSTRUCT, ByVal lParam, ByVal Len(tDRAWITEMSTRUCT)
        Call psubDRAWITEM(tDRAWITEMSTRUCT)
        Exit Function
    End Select
    
    ' 元のウインドウプロシージャの呼び出し
    WindowProc = CallWindowProc(P_OrgProc, hwnd, uMsg, wParam, lParam)
End Function

'------------------
' サブクラス化開始
'------------------
Public Sub SubClass(ByVal hwnd As Long)
    Dim lngRet As Long
    ' 既にサブクラス化されているかチェック
    If P_OrgProc <> 0 Then
        ' 既ににサブクラス化されている場合、サブクラス化終了
        Call UnSubClass
    End If
    ' サブクラス化開始
    P_OrgProc = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WindowProc)
    P_Orghwnd = hwnd
End Sub

'------------------
' サブクラス化終了
'------------------
Public Sub UnSubClass()
    Dim lngRet As Long
    ' サブクラス化されている場合
    If P_OrgProc <> 0 Then
        ' ウインドウプロシージャを元のデフォルトウインドウプロシージャに設定
        lngRet = SetWindowLong(P_Orghwnd, GWL_WNDPROC, P_OrgProc)
        P_Orghwnd = 0
        P_OrgProc = 0
    End If
End Sub
