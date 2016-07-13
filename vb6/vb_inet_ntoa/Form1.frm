VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function lstrlen Lib "kernel32" Alias "lstrlenA" (ByVal lpString As Any) As Long
Private Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyA" (ByVal lpString1 As Any, ByVal lpString2 As Any) As Long

Private Declare Function ntohl Lib "wsock32.dll" (ByVal netlong As Long) As Long
Private Declare Function ntohs Lib "wsock32.dll" (ByVal netshort As Integer) As Integer
Private Declare Function htonl Lib "wsock32.dll" (ByVal hostlong As Long) As Long
Private Declare Function htons Lib "wsock32.dll" (ByVal hostshort As Integer) As Integer
Private Declare Function inet_addr Lib "wsock32.dll" (ByVal cp As String) As Long
Private Declare Function inet_ntoa Lib "wsock32.dll" (ByVal addr As Long) As Long

Private Function vb_inet_ntoa(ByVal addr As Long) As String
    Dim ptr As Long
    ptr = inet_ntoa(addr)   '文字列へのポインタを取得
    vb_inet_ntoa = String$(lstrlen(ByVal ptr), 0)   '文字列長をAPIで求め、作成する
    lstrcpy ByVal vb_inet_ntoa, ByVal ptr   '確保したバッファにコピー
End Function

Private Function MakeString(ByVal ptr As Long) As String
    MakeString = String$(lstrlen(ByVal ptr), 0)
    lstrcpy ByVal MakeString, ByVal ptr
End Function

Private Sub Form_Load()
    Dim a As Long
    Dim b As Integer
    
    a = &H12345678
    b = &H1245
    
    Debug.Print Hex(a), Hex(b)
    
    Debug.Print Hex(ntohl(a)), Hex(ntohs(b))
    
    Debug.Print Hex(htonl(a)), Hex(htons(b))
    
    Debug.Print Hex(inet_addr("192.168.150.255")), Hex(inet_addr("hoge hoge"))
    
    Debug.Print "[" + vb_inet_ntoa(inet_addr("192.168.150.255")) + "]"
End Sub
