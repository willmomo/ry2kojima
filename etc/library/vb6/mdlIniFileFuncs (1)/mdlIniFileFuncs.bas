Attribute VB_Name = "mdlIniFileFuncs"
'----------------------------------------------------------------------
'   [概要]
'   INIファイルにアクセスする関数群
'
'   [更新日]
'   2003/11/01 : 新規作成

Option Explicit

'----------------------------------------------------------------------
'   外部プロシージャの参照宣言

Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

'----------------------------------------------------------------------
'   [概要]
'   文字列終端のNULLをカットする

Private Sub RemoveNulls(sValue As String)
    Dim lngPos As Long
    lngPos = InStr(sValue, vbNullChar)
    If lngPos > 0 Then
        sValue = Left(sValue, lngPos - 1)
    End If
End Sub

'----------------------------------------------------------------------
'   [概要]
'   INIファイルから文字列を取得
'
'   [引数]
'   sSection    : セクション名
'   sKey        : キー名
'   sDefault    : キーが無いときのデフォルト値
'   sValue      : 値
'   sFile       : INIファイル名
'
'   [戻り値]
'   0   : 正常終了
'   1   : 正常終了(デフォルト値を使用)
'
'   [備考]
'   関数は必ず成功しますが、キーを読み込んだ場合と、デフォルト値を使っ
'   た場合で戻り値が違います。
'
'   sSection, sKeyにvbNullStringを指定した場合の動作は不定です。

Public Function IFFGetPrivateProfileString(sSection As String, sKey As String, sDefault As String, sValue As String, sFile As String) As Long
    Dim rc As Long
    Dim bufSize As Long
    Dim sTemp As String
    
    '十分に長い値も考慮して読み込み
    Do
        bufSize = bufSize + 256
        sValue = String(bufSize, vbNullChar)
        rc = GetPrivateProfileString(sSection, sKey, sDefault, sValue, bufSize, sFile)
    Loop While rc = bufSize - 1
    
    RemoveNulls sValue  '行末のNULL文字を削除
    
    '読み込んだ(とされる)文字列がデフォルト値と同じときは
    'デフォルト値を変えて読み込んでみる。
    If sValue = sDefault Then
        sTemp = String(rc + 2, vbNullChar)
        rc = GetPrivateProfileString(sSection, sKey, "!" & sDefault, sTemp, rc + 2, sFile)
    
        RemoveNulls sTemp
        
        '1回目と2回目の値が違うときは、キーが存在しないのでデフォルト値を読み込んでいるはず。
        If sValue <> sTemp Then
            IFFGetPrivateProfileString = 1
        End If
    End If
End Function
