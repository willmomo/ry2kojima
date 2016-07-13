VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsNotifyIcon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const NIM_ADD = &H0
Private Const NIM_DELETE = &H2
Private Const NIM_MODIFY = &H1

Private Const NIF_ICON = &H2
Private Const NIF_MESSAGE = &H1
Private Const NIF_TIP = &H4

Private Type NOTIFYICONDATA
    cbSize As Long
    hwnd As Long
    uID As Long
    uFlags As Long
    uCallbackMessage As Long
    hIcon As Long
    'szTip As String * 64
    szTip(64 - 1) As Byte
End Type

Private Declare Function Shell_NotifyIcon Lib "shell32.dll" (ByVal dwMessage As Long, lpData As NOTIFYICONDATA) As Long


'メンバ変数
Private m_nid As NOTIFYICONDATA


'----------------------------------------------------------------------
' タスクトレイアイコンを追加する
'----------------------------------------------------------------------
Public Function Add(ownerForm As Form, ByVal uID As Long, icon As Picture, tipText As String) As Long

    Dim i As Long
    Dim work() As Byte
    
    With m_nid
        .cbSize = LenB(m_nid)
        .hwnd = ownerForm.hwnd
        .uID = uID
        
        If Not (icon Is Nothing) Then
            .hIcon = icon.Handle
            .uFlags = .uFlags Or NIF_ICON
        End If
        
        work = StrConv(tipText, vbFromUnicode)
        For i = 0 To UBound(work)
            .szTip(i) = work(i)
        Next
        .uFlags = .uFlags Or NIF_TIP
    End With
    
    Shell_NotifyIcon NIM_ADD, m_nid
    
End Function


'----------------------------------------------------------------------
' タスクトレイアイコンを削除する
'----------------------------------------------------------------------
Public Function Delete() As Long

    If m_nid.cbSize = LenB(m_nid) Then
        Shell_NotifyIcon NIM_DELETE, m_nid
        m_nid.cbSize = 0
    End If
    
End Function


'----------------------------------------------------------------------
' タスクトレイアイコンのアイコンを変更する
'----------------------------------------------------------------------
Public Function ModifyIcon(icon As Picture) As Long

    m_nid.hIcon = icon.Handle
    m_nid.uFlags = NIF_ICON
    
    Shell_NotifyIcon NIM_MODIFY, m_nid
    
End Function


'----------------------------------------------------------------------
' タスクトレイアイコンのテキストを変更する
'----------------------------------------------------------------------
Public Function ModifyText(tipText As String) As Long

    work = StrConv(tipText, vbFromUnicode)
    For i = 0 To UBound(work)
        .szTip(i) = work(i)
    Next
    .uFlags = .uFlags Or NIF_TIP
    
    Shell_NotifyIcon NIM_MODIFY, m_nid

End Function

