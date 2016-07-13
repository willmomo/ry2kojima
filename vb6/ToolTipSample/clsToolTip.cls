VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsToolTip"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------------------------------------------------------
' ツールチップを処理するクラス
'
' Histroy:
' 2011/08/12 r.kojima UpdateTipTextを追加。ラベルバージョンは作っていません。
'------------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------------------------
' ツールチップAPIに必要な、型と定数

Private Type RECT
    Left        As Long
    Top         As Long
    Right       As Long
    Bottom      As Long
End Type

Private Type TOOLINFO
    cbSize      As Long
    uFlags      As Long
    hwnd        As Long
    uId         As Long
    rc          As RECT
    hInst       As Long
    lpszText    As String
    lParam      As Long
End Type

Private Const TOOLTIPS_CLASS = "tooltips_class32"

Private Const TTF_IDISHWND = &H1
Private Const TTF_CENTERTIP = &H2
Private Const TTF_RTLREADING = &H4
Private Const TTF_SUBCLASS = &H10
'-- IE3以降
Private Const TTF_TRACK = &H20
Private Const TTF_ABSOLUTE = &H80
Private Const TTF_TRANSPARENT = &H100
Private Const TTF_DI_SETITEM = &H8000        '// valid only on the TTN_NEEDTEXT callback
'--

Private Const TTS_ALWAYSTIP = &H1
Private Const TTS_NOPREFIX = &H2
'-- IE5以降 --
Private Const TTS_NOANIMATE = &H10
Private Const TTS_NOFADE = &H20
Private Const TTS_BALLOON = &H40
'--

'// Tool Tip Messages
Private Const WM_USER = &H400

Private Const TTM_ACTIVATE = (WM_USER + 1)
Private Const TTM_SETDELAYTIME = (WM_USER + 3)
Private Const TTM_ADDTOOL = (WM_USER + 4)
Private Const TTM_DELTOOL = (WM_USER + 5)
Private Const TTM_NEWTOOLRECT = (WM_USER + 6)
Private Const TTM_RELAYEVENT = (WM_USER + 7)
Private Const TTM_GETTOOLINFO = (WM_USER + 8)
Private Const TTM_SETTOOLINFO = (WM_USER + 9)
Private Const TTM_HITTEST = (WM_USER + 10)
Private Const TTM_GETTEXT = (WM_USER + 11)
Private Const TTM_UPDATETIPTEXT = (WM_USER + 12)
Private Const TTM_GETTOOLCOUNT = (WM_USER + 13)
Private Const TTM_ENUMTOOLS = (WM_USER + 14)
Private Const TTM_GETCURRENTTOOL = (WM_USER + 15)
Private Const TTM_WINDOWFROMPOINT = (WM_USER + 16)
'-- IE3以降
Private Const TTM_TRACKACTIVATE = (WM_USER + 17)     '// wParam = TRUE/FALSE start end  lparam = LPTOOLINFO
Private Const TTM_TRACKPOSITION = (WM_USER + 18)     '// lParam = dwPos
Private Const TTM_SETTIPBKCOLOR = (WM_USER + 19)
Private Const TTM_SETTIPTEXTCOLOR = (WM_USER + 20)
Private Const TTM_GETDELAYTIME = (WM_USER + 21)
Private Const TTM_GETTIPBKCOLOR = (WM_USER + 22)
Private Const TTM_GETTIPTEXTCOLOR = (WM_USER + 23)
Private Const TTM_SETMAXTIPWIDTH = (WM_USER + 24)
Private Const TTM_GETMAXTIPWIDTH = (WM_USER + 25)
Private Const TTM_SETMARGIN = (WM_USER + 26)         '// lParam = lprc
Private Const TTM_GETMARGIN = (WM_USER + 27)         '// lParam = lprc
Private Const TTM_POP = (WM_USER + 28)
'-- IE4以降
Private Const TTM_UPDATE = (WM_USER + 29)
'-- IE5以降
Private Const TTM_GETBUBBLESIZE = (WM_USER + 30)
Private Const TTM_ADJUSTRECT = (WM_USER + 31)
Private Const TTM_SETTITLE = (WM_USER + 32)          '// wParam = TTI_*, lParam = char* szTitle
'--

'------------------------------------------------------------------------------
' Windows API

Private Declare Function CreateWindowEx Lib "user32" Alias "CreateWindowExA" (ByVal dwExStyle As Long, ByVal lpClassName As String, ByVal lpWindowName As String, ByVal dwStyle As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hwndParent As Long, ByVal hMenu As Long, ByVal hInstance As Long, lpParam As Any) As Long
Private Declare Function DestroyWindow Lib "user32" (ByVal hwnd As Long) As Long
Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long
Private Declare Function SendMessageLL Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

'------------------------------------------------------------------------------
' メンバ変数

Private m_hwndMaster As Long    'ツールチップを保有する親ウィンドウ

Private m_hTooltip As Long      'ツールチップのウィンドウ

'------------------------------------------------------------------------------
' ツールチップの最大幅を設定
'
' この幅を設定することで、multi line (改行コード) に対応する。
'------------------------------------------------------------------------------
Public Property Let MaxTipWidth(ByVal nWidth As Long)
    SendMessageLL m_hTooltip, TTM_SETMAXTIPWIDTH, 0, nWidth
End Property

'------------------------------------------------------------------------------
' ツールチップの最大幅を取得
'------------------------------------------------------------------------------
Public Property Get MaxTipWidth() As Long
    MaxTipWidth = SendMessageLL(m_hTooltip, TTM_GETMAXTIPWIDTH, 0, 0)
End Property

'------------------------------------------------------------------------------
' ツールチップの作成
'------------------------------------------------------------------------------
Public Function createToolTip(ByVal hwnd As Long, ByVal balloonStyle As Boolean) As Boolean
    Dim windowStyle As Long
    
    m_hwndMaster = hwnd
    
    windowStyle = TTS_ALWAYSTIP
    If balloonStyle = True Then windowStyle = windowStyle Or TTS_BALLOON
    
    '-- ツールチップウィンドウの作成 (BALLOONスタイルのみ Comctl32.dll >= 5.80)
    m_hTooltip = CreateWindowEx(0, TOOLTIPS_CLASS, vbNullString, windowStyle, 0, 0, 0, 0, hwnd, 0, App.hInstance, ByVal 0&)
                              
    createToolTip = IIf(m_hTooltip <> 0, True, False)
End Function

'------------------------------------------------------------------------------
' ツールチップの破棄
'
' 未初期化状態で呼んでも、正しく動作します。
'------------------------------------------------------------------------------
Public Function destroyToolTip()
    If m_hTooltip <> 0 Then
        DestroyWindow m_hTooltip
        m_hTooltip = 0
    End If
End Function

'------------------------------------------------------------------------------
' ツールチップの追加(汎用)
'------------------------------------------------------------------------------
Public Function AddTool(ctrl As Control, strText As String) As Long
    Dim udtToolInfo As TOOLINFO

    '-- 追加するツールの設定(Picture2用)
    With udtToolInfo
        .cbSize = Len(udtToolInfo)
        .uFlags = TTF_SUBCLASS Or TTF_IDISHWND
        .hwnd = m_hwndMaster
        .uId = ctrl.hwnd
        .lpszText = strText
    End With
    
    '-- ツールの追加
    AddTool = SendMessage(m_hTooltip, TTM_ADDTOOL, 0, udtToolInfo)
End Function

'------------------------------------------------------------------------------
' ツールチップの追加(ラベル用)
'
' Control.hWnd が実装されていない Label コントロールのためのメソッド。
' nId は、ユニークに設定する必要がある。
'------------------------------------------------------------------------------
Public Function AddToolForLabel(lbl As Label, ByVal nId As Long, strText As String) As Long
    Dim udtToolInfo As TOOLINFO

    '-- 追加するツールの設定(Picture2用)
    With udtToolInfo
        .cbSize = Len(udtToolInfo)
        .uFlags = TTF_SUBCLASS
        .hwnd = m_hwndMaster
        .uId = nId
        .lpszText = strText
        .rc.Left = lbl.Left / Screen.TwipsPerPixelX
        .rc.Top = lbl.Top / Screen.TwipsPerPixelY
        .rc.Right = .rc.Left + lbl.Width / Screen.TwipsPerPixelX
        .rc.Bottom = .rc.Top + lbl.Height / Screen.TwipsPerPixelY
    End With
    
    '-- ツールの追加
    AddToolForLabel = SendMessage(m_hTooltip, TTM_ADDTOOL, 0, udtToolInfo)
End Function

'------------------------------------------------------------------------------
' ツールチップの追加(汎用)
'------------------------------------------------------------------------------
Public Function UpdateTipText(ctrl As Control, strText As String) As Long
    Dim udtToolInfo As TOOLINFO

    '-- 追加するツールの設定(Picture2用)
    With udtToolInfo
        .cbSize = Len(udtToolInfo)
        .hwnd = m_hwndMaster
        .uId = ctrl.hwnd
        .lpszText = strText
    End With
    
    '-- ツールの追加
    UpdateTipText = SendMessage(m_hTooltip, TTM_UPDATETIPTEXT, 0, udtToolInfo)
End Function

'------------------------------------------------------------------------------
' コンストラクタ
'------------------------------------------------------------------------------
Private Sub Class_Initialize()
    m_hwndMaster = 0
    m_hTooltip = 0
End Sub

'------------------------------------------------------------------------------
' デストラクタ
'------------------------------------------------------------------------------
Private Sub Class_Terminate()
    destroyToolTip
End Sub

