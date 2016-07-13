VERSION 5.00
Begin VB.Form frmNotifyIcon 
   Caption         =   "NotifyIcon Sample"
   ClientHeight    =   1995
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   6450
   LinkTopic       =   "Form1"
   ScaleHeight     =   1995
   ScaleWidth      =   6450
   StartUpPosition =   3  'Windows の既定値
   Tag             =   "1"
   Begin VB.Menu mnuPopup 
      Caption         =   "(popup-menu)"
      Begin VB.Menu mnuPopupExit 
         Caption         =   "終了(&X)"
      End
   End
End
Attribute VB_Name = "frmNotifyIcon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Const WM_MOUSEMOVE = &H200
Private Const WM_LBUTTONDOWN = &H201
Private Const WM_LBUTTONUP = &H202
Private Const WM_LBUTTONDBLCLK = &H203
Private Const WM_RBUTTONDOWN = &H204
Private Const WM_RBUTTONUP = &H205
Private Const WM_RBUTTONDBLCLK = &H206

Private Const NIM_ADD = &H0
Private Const NIM_DELETE = &H2
Private Const NIM_MODIFY = &H1

Private Const NIF_ICON = &H2
Private Const NIF_MESSAGE = &H1
Private Const NIF_TIP = &H4

Private Type NOTIFYICONDATA
    cbSize As Long
    hWnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage As Long
    hIcon As Long
    szTip As String * 64
End Type

Private Declare Function Shell_NotifyIcon Lib "shell32.dll" (ByVal dwMessage As Long, lpData As NOTIFYICONDATA) As Long


'メンバ変数
Private m_nid As NOTIFYICONDATA


'----------------------------------------------------------------------
' タスクトレイにアイコンを追加する
'
' IDは、FormのTagプロパティを参照。
' Iconは、FormのIconを参照。
' テキストは、FormのCaptionを参照。
' コールバックメッセージは、MouseMoveを代用。
'----------------------------------------------------------------------
Private Sub AddIconToTray()

    With m_nid
        .cbSize = LenB(m_nid)
        .hWnd = Me.hWnd
        .uID = Me.Tag
        .uFlags = NIF_ICON Or NIF_MESSAGE Or NIF_TIP
        .uCallbackMessage = WM_MOUSEMOVE
        .hIcon = Me.Icon.Handle
        .szTip = Me.Caption & vbNullChar
    End With
    
    Call Shell_NotifyIcon(NIM_ADD, m_nid)
    
End Sub


'----------------------------------------------------------------------
' タスクトレイからアイコンを削除する
'----------------------------------------------------------------------
Private Sub DeleteIconFromTray()
    Call Shell_NotifyIcon(NIM_DELETE, m_nid)
End Sub


'----------------------------------------------------------------------
' Load event
'----------------------------------------------------------------------
Private Sub Form_Load()
    Call AddIconToTray
End Sub


'----------------------------------------------------------------------
' MouseMove event
'
' タスクトレイからのメッセージとして処理する。
'----------------------------------------------------------------------
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)

    Dim uMsg As Long
    
    uMsg = X / Screen.TwipsPerPixelX
    
    Select Case uMsg
    Case WM_LBUTTONDBLCLK
    Case WM_LBUTTONDOWN
    Case WM_LBUTTONUP
    Case WM_RBUTTONDBLCLK
    Case WM_RBUTTONDOWN
    Case WM_RBUTTONUP
        Me.PopupMenu mnuPopup
    End Select
    
End Sub


'----------------------------------------------------------------------
' Unload event
'----------------------------------------------------------------------
Private Sub Form_Unload(Cancel As Integer)
    Call DeleteIconFromTray
End Sub


'----------------------------------------------------------------------
' (必要ならPopupMenuの処理)
'----------------------------------------------------------------------
Private Sub mnuPopupExit_Click()
    Unload Me
End Sub
