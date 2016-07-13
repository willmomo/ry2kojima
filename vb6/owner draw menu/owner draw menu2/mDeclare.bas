Attribute VB_Name = "mDeclare"
'
'*************************************************************************
'
'                        その他APIモジュール
'
'*************************************************************************
'
Option Explicit
' メモリー領域をコピーする関数
Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDest As Any, pSource As Any, ByVal dwLength As Long)
' デバイスコンテキストのハンドルを取得
Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
' デバイスコンテキストを解放
Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hdc As Long) As Long
' 背景色を設定
Declare Function SetBkColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
' テキストの色を設定
Declare Function SetTextColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
' 画面のデザインに使われている色を取得
Declare Function GetSysColor Lib "user32" (ByVal nIndex As Long) As Long
    Public Const COLOR_MENU = 4             ' メニューの背景色
    Public Const COLOR_MENUTEXT = 7         ' メニュー内のテキストの色
    Public Const COLOR_HIGHLIGHT = 13       ' 選択された項目の色
    Public Const COLOR_HIGHLIGHTTEXT = 14   ' 選択された項目のテキストの色
    Public Const COLOR_GRAYTEXT = 17        ' 淡色状態 (無効状態) のテキストの色
' テキストを表示
Declare Function DrawText Lib "user32" Alias "DrawTextA" (ByVal hdc As Long, ByVal lpStr As String, ByVal nCount As Long, lpRect As RECT, ByVal wFormat As Long) As Long
    Public Const DT_CALCRECT = &H400    ' テキストを表示するために必要なサイズ取得
    Public Const DT_LEFT = &H0
    Public Const DT_RIGHT = &H2
    Public Const DT_SINGLELINE = &H20
    Public Const DT_VCENTER = &H4
    Public Const DT_EXPANDTABS = &H40

' 背景モードを設定
Declare Function SetBkMode Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long
    Public Const TRANSPARENT = 1    ' 背景をそのまま残す
    Public Const OPAQUE = 2         ' 描画される前に、現在の背景色で背景を塗りつぶし
' 塗りつぶし長方形の描画
Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long
' 論理ブラシを作成
Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long
' オブジェクトを削除
Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

