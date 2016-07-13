VERSION 5.00
Begin VB.Form frmNotifyIcon 
   BorderStyle     =   1  '固定(実線)
   Caption         =   "NotifyIcon"
   ClientHeight    =   3090
   ClientLeft      =   150
   ClientTop       =   840
   ClientWidth     =   4680
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.Menu mnuPopup 
      Caption         =   "(PopupMenu)"
      Begin VB.Menu mnuAppProperty 
         Caption         =   "設定(&P)"
      End
      Begin VB.Menu mnuSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuAppExit 
         Caption         =   "終了(&X)"
      End
   End
End
Attribute VB_Name = "frmNotifyIcon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' [概要]
' タスクトレイアイコンを表示するフォーム。
'
' [説明]
' タスクトレイアイコンを管理するフォームの雛形。
' タスクトレイアイコンを表示したいアプリケーションは、このフォームを
' ロードする。
'----------------------------------------------------------------------

Option Explicit

Private Const WM_NULL = &H0
Private Const WM_MOUSEMOVE = &H200
Private Const WM_LBUTTONDBLCLK = &H203
Private Const WM_LBUTTONDOWN = &H201
Private Const WM_LBUTTONUP = &H202
Private Const WM_RBUTTONDBLCLK = &H206
Private Const WM_RBUTTONDOWN = &H204
Private Const WM_RBUTTONUP = &H205

Private Const NIM_ADD         As Long = &H0
Private Const NIM_MODIFY      As Long = &H1
Private Const NIM_DELETE      As Long = &H2

Private Const NIF_MESSAGE     As Long = &H1
Private Const NIF_ICON        As Long = &H2
Private Const NIF_TIP         As Long = &H4

Private Type NOTIFYICONDATA
    cbSize As Long
    hwnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage As Long
    hIcon As Long
    szTip As String * 64
End Type

Private Type POINTAPI
    x As Long
    y As Long
End Type

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Const TPM_LEFTALIGN = &H0&

Private Declare Function Shell_NotifyIcon Lib "shell32.dll" _
    Alias "Shell_NotifyIconA" ( _
        ByVal dwMessage As Long, lpData As NOTIFYICONDATA _
    ) As Long
    
Private Declare Function SetForegroundWindow Lib "user32" (ByVal hwnd As Long) As Long

'Private Declare Function TrackPopupMenu Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wFlags As Long, _
    ByVal x As Long, _
    ByVal y As Long, _
    ByVal nReserved As Long, _
    ByVal hwnd As Long, _
    lprc As RECT _
) As Long

Private Declare Function TrackPopupMenu Lib "user32" ( _
    ByVal hMenu As Long, _
    ByVal wFlags As Long, _
    ByVal x As Long, _
    ByVal y As Long, _
    ByVal nReserved As Long, _
    ByVal hwnd As Long, _
    lprc As Any _
) As Long

Private Declare Function PostMessage Lib "user32" _
    Alias "PostMessageA" ( _
        ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long _
    ) As Long
    
Private Declare Function GetSubMenu Lib "user32" (ByVal hMenu As Long, ByVal nPos As Long) As Long

Private Declare Function GetMenu Lib "user32" (ByVal hwnd As Long) As Long

Private Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long

'----------------------------------------------------------------------
' メンバー変数

Private m_nid As NOTIFYICONDATA

'----------------------------------------------------------------------
' [概要]
' Loadイベント
'
' [備考]
' タスクトレイアイコンを登録する。
' アイコン、テキストを適宜修正する。
' コールバックにMouseMoveを登録するため、このフォームは常に非表示で
' 使用する。
'----------------------------------------------------------------------

Private Sub Form_Load()
    On Error GoTo ErrHandler
    
    m_nid.cbSize = Len(m_nid)
    m_nid.hwnd = Me.hwnd
    m_nid.uID = 0
    m_nid.uFlags = NIF_MESSAGE Or NIF_ICON Or NIF_TIP
    m_nid.uCallbackMessage = WM_MOUSEMOVE
    m_nid.hIcon = Me.Icon
    m_nid.szTip = "NotifyIcon Test" & vbNullChar
    
    Shell_NotifyIcon NIM_ADD, m_nid
    
    Exit Sub

ErrHandler:

End Sub

'----------------------------------------------------------------------
' [概要]
' MouseMoveイベント
'
' [備考]
' タスクトレイアイコンからのコールバックで使用する。
' 引数を x を Screen.TwipsPerPixelX で割った値が、メッセージの内容
'
' Article ID: Q162613
' HOWTO: Manipulate Icons in the System Tray with Visual Basic
' も参照
'----------------------------------------------------------------------

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
    On Error GoTo ErrHandler
    
    Dim msg As Long
    Dim pt As POINTAPI
    
    msg = x / Screen.TwipsPerPixelX
    
    Select Case msg
    Case WM_LBUTTONDOWN
    
        'アイコン上で左クリックされたときの処理
        
    Case WM_RBUTTONDOWN
    
        'アイコン上で右クリックされたときの処理
        
        SetForegroundWindow Me.hwnd
        
        GetCursorPos pt
        
        TrackPopupMenu GetSubMenu(GetMenu(Me.hwnd), 0), TPM_LEFTALIGN, pt.x, pt.y, 0, Me.hwnd, vbNullString
        
        PostMessage Me.hwnd, WM_NULL, 0, 0
        
    End Select
    
    Exit Sub
    
ErrHandler:

End Sub

'----------------------------------------------------------------------
' [概要]
' Unload イベント
'
' [備考]
' タスクトレイアイコンを削除する
'----------------------------------------------------------------------

Private Sub Form_Unload(Cancel As Integer)
    On Error GoTo ErrHandler
    
    Shell_NotifyIcon NIM_DELETE, m_nid
    
    Exit Sub
    
ErrHandler:

End Sub

'----------------------------------------------------------------------
' [概要]
' メニューから、終了が選ばれたときの処理
'----------------------------------------------------------------------

Private Sub mnuAppExit_Click()
    On Error GoTo ErrHandler
    
    '自分を登録した親アプリケーションを終了させる
    
    Unload Form1
    
    Exit Sub
    
ErrHandler:

End Sub

'----------------------------------------------------------------------
' [概要]
' メニューから、設定が選ばれたときの処理
'----------------------------------------------------------------------

Private Sub mnuAppProperty_Click()
    On Error GoTo ErrHandler
    
    '自分を登録した親フォームを表示させる
    
    Form1.Visible = True
    
    Exit Sub
    
ErrHandler:

End Sub
