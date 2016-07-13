Attribute VB_Name = "Module1"
Option Explicit

'-------------------------------------------------------
'
'-------------------------------------------------------
Public Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long

Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long
Public Const WM_COPYDATA = &H4A

Type KAIIN_DATA
        kno As Long
        ptup As Long
        mondai(2) As String * 100
End Type

Type COPYDATASTRUCT
        dwData As Long
        cbData As Long
        lpData As Long  'Å¶íçà”Å@ëóêMêÊÇÕStringå^Ç…ÇµÇ‹Ç∑ÅB
End Type




Public Function nvl(src As String)

    If src = "" Then
        nvl = vbNullChar
    Else
        nvl = src
    End If

End Function
