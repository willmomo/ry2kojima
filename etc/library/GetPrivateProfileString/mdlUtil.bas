Attribute VB_Name = "mdlUtil"
Option Explicit

Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" ( _
    ByVal lpApplicationName As String, _
    ByVal lpKeyName As Any, _
    ByVal lpDefault As String, _
    ByVal lpReturnedString As String, _
    ByVal nSize As Long, _
    ByVal lpFileName As String _
) As Long


'----------------------------------------------------------------------
' API関数 GetPrivateProfileString のラップ関数
'
' [引数]
' strSection    セクション名
' strKey        キー名
' strDefault    セクションまたは、キーが無い時の戻り値
' strFileName   INIファイルの名前
'
' [戻り値]
' 指定したINIファイル、セクション、キーから取得できる文字列。
' strSectionにvbNullStringを指定したときは、全セクション名をvbNullCharで区切った文字列。
' strKeyにvbNullStringを指定したときは、セクション内の全キー名をvbNullCharで区切った文字列。
'
'----------------------------------------------------------------------
Public Function VBGetPrivateProfileString(strSection As String, strKey As String, strDefault As String, strFileName As String) As String

    Dim lngPos As Long
    Dim lngRet As Long
    Dim lngBufSize As Long
    Dim strRet As String
    
    '最低でもデフォルト文字列+256文字分のバッファを確保して処理を開始
    lngBufSize = 256 + LenB(strDefault)
    
    'GetPrivateProfileStringの戻り値が、バッファのサイズ-1 or -2の時は、
    'バッファのサイズが足りないので、拡張しながら繰り返す
    Do
        lngBufSize = lngBufSize * 2
        strRet = String(lngBufSize, vbNullChar)
        lngRet = GetPrivateProfileString(strSection, strKey, strDefault, strRet, lngBufSize, strFileName)
    Loop While (lngRet = lngBufSize - 1) Or (lngRet = lngBufSize - 2)
    
    '文字列終端の'\0'を取り除く
    'strSection, strKeyにvbNullString(NULL) or "" を指定したときは、二つのvbNullChar('\0')が終端になっている
    If (strSection = vbNullString) Or (strKey = vbNullString) Then
        lngPos = InStr(strRet, String(2, vbNullChar))
    Else
        lngPos = InStr(strRet, vbNullChar)
    End If
    If lngPos > 0 Then
        strRet = Left(strRet, lngPos - 1)
    End If
    
    VBGetPrivateProfileString = strRet
End Function
