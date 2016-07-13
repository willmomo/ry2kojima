Attribute VB_Name = "mdlIniFunc"
'------------------------------------------------------------------------------
' INIファイルにアクセスする関数
'
' 2012/02/23 r.kojima new
'------------------------------------------------------------------------------
Option Explicit

Private Declare Function GetPrivateProfileSection Lib "kernel32" Alias "GetPrivateProfileSectionA" ( _
    ByVal lpAppName As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

' バッファを拡張する時のサイズ
' INIファイル関係の関数は、バッファを拡張しながら何度も呼ぶ必要があるので、
' モジュール内で、共通に使用する。
Private Const BUFF_GROW_SIZE As Long = 512

'------------------------------------------------------------------------------
' GetPrivateProfileSection を VB用に実装した関数
'
' lpAppName   セクション名
' lpFileName  INIファイル名
'
' 取得したセクション情報文字列。Split(buffer, vbNulLChar) で、分解可能。
'------------------------------------------------------------------------------
Public Function VBGetPrivateProfileSection(lpAppName As String, lpFileName As String) As String
    Dim buf As String
    Dim bufCnt As Long
    Dim ret As Long
    
    ' GetPrivateProfileString は、戻り値が、(バッファサイズ - 2)の間は、
    ' バッファが足りない可能性があるので、拡張しながら繰り返し呼び出す。
    Do
        bufCnt = bufCnt + BUFF_GROW_SIZE
        buf = String(bufCnt, vbNullChar)
        ret = GetPrivateProfileSection(lpAppName, buf, bufCnt, lpFileName)
    Loop While ret = bufCnt - 2
    
    ' 取得した情報の最後には、二つの NULL が入るので、その部分で文字列として切りだす。
    ' buf を vbNullChar で埋めているため、文字列が取得できなくても、
    ' 以下のコードは、正常に動作する。
    VBGetPrivateProfileSection = Left(buf, InStr(buf, String(2, vbNullChar)) - 1)
End Function
