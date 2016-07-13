Attribute VB_Name = "mdlSub"
Option Explicit

Public Declare Function wsft_start Lib "wsft.dll" (ByVal hwnd As Long) As Long
Public Declare Function wsft_end Lib "wsft.dll" () As Long
Public Declare Function wsft_getopt Lib "wsft.dll" (ByVal opt As String) As Long
Public Declare Function wsft_setopt Lib "wsft.dll" (ByVal opt As String, ByVal Value As Long) As Long
Public Declare Function wsft_readfile Lib "wsft.dll" (ByVal fid As Integer, ByVal rec As Integer, buf As Any, ByVal bufsize As Long) As Long
Public Declare Function wsft_writefile Lib "wsft.dll" (ByVal fid As Integer, ByVal rec As Integer, buf As Any, ByVal bufsize As Long) As Long
Public Declare Function wsft_dllversion Lib "wsft.dll" () As Long
Public Declare Function wsft_setservstr Lib "wsft.dll" (ByVal servstr As String, ByVal rport As Long, ByVal wport As Long) As Long
Public Declare Function wsft_read_rec Lib "wsft.dll" (ByVal fid As Integer, ByVal starec As Long, ByVal endrec As Long, buf As Any, ByVal recsize As Long) As Long

Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" _
    (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, _
    ByVal nSize As Long, ByVal lpFileName As String) As Long


Public Function VBGetPrivateProfileString(sAppName As String, sKeyName As String, sDefault As String, sName As String) As String
    Dim sBuf As String
    Dim nPos As Long
    
    sBuf = String(1024, vbNullChar)
    GetPrivateProfileString sAppName, sKeyName, sDefault, sBuf, 1024, sName
    nPos = InStr(sBuf, vbNullChar)
    If nPos > 0 Then
        VBGetPrivateProfileString = Left(sBuf, nPos - 1)
    Else
        VBGetPrivateProfileString = ""
    End If
End Function

'   wsftのエラー文字列

Public Function wsft_error(ByVal code As Long) As String
    Select Case code
    Case &H1
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:他機器から書込み排他中(ファイル情報取得要求時)"
    Case &H2
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:他機器から書込み排他中(読込み要求時)"
    Case &H3
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:他機器から書込み排他中(書込み準備要求時)"
    Case &H4
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:他機器から書込み排他中(データ電文送信時)"
    Case &H5
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:他機器から書込み排他中(書込み完了要求時)"
    Case &H1001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":バッファ確保エラー"
    Case &H1002
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":書込みフレーム抜けエラー"
    Case &H1003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":書込み受信タイムアウト"
    Case &H1004
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":ファイル番号エラー"
    Case &H1005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":アドレスエラー"
    Case &H1006
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":レコードエラー"
    Case &H1007
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":フレームエラー"
    Case &H1008
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":コマンドエラー"
    Case &H1009
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":バッファオーバーフローエラー"
    Case &H2001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":バッファエラー"
    Case &H2003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":受信サイズエラー"
    Case &H2005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":バッファサイズ確認エラー"
    Case &H2010
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":タイムアウトエラー(ファイル情報取得要求時)"
    Case &H2011
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":タイムアウトエラー(読込み要求時)"
    Case &H2012
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":タイムアウトエラー(書込み準備要求時)"
    Case &H2013
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":タイムアウトエラー(データ電文送信時)"
    Case &H2014
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":タイムアウトエラー(書込み完了要求時)"
    Case &H2020
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":送信エラー(ファイル情報取得要求時)"
    Case &H2021
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":送信エラー(読込み要求時)"
    Case &H2022
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":送信エラー(書込み準備要求時)"
    Case &H2023
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":送信エラー(データ電文送信時)"
    Case &H2024
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":送信エラー(書込み完了要求時)"
    Case &H2030
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":受信エラー(ファイル情報取得要求時)"
    Case &H2031
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":受信エラー(読込み要求時)"
    Case &H2032
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":受信エラー(書込み準備要求時)"
    Case &H2033
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":受信エラー(データ電文送信時)"
    Case &H2034
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":受信エラー(書込み完了要求時)"
    Case &H2040
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:サーバーが見つからない"
    Case &H3001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:winsockの初期化に失敗"
    Case &H3002
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:winsockのバージョンエラー"
    Case &H3003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:ソケットの作成に失敗"
    Case &H3004
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:初期化前に通信関数が使用された"
    Case &H3005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:メモリ確保に失敗"
    Case &H3006
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:INIファイルの設定エラー"
    Case &H3007
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":初期化エラー:ログ出力開始エラー"
    Case &H5001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":オプション設定エラー"
    Case &H5002
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":タイムアウト設定エラー"
    Case &H5003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":読込みポート番号エラー"
    Case &H5004
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":書込みポート番号エラー"
    Case &H5005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":リトライ回数エラー"
    Case &H5006
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":ホスト名エラー"
    Case &HF001&
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":プロトコルバージョン"
    Case Else
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":unknown"
    End Select
End Function
