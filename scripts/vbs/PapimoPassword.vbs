'------------------------------------------------------------------------------
' PAPIMOにアップロードするZIPファイルのパスワードを求めるスクリプト
'
' 2011/06/27 r.kojima
'
' zipのパスワードについては
' [YYYYMMDD][加盟店コード]
' ※YYYYMMDDは以下の暗号化を行う事とする。
' 
' Y → 40を加算しテキスト化
' Y → 50を加算しテキスト化
' Y → 60を加算しテキスト化
' Y → 70を加算しテキスト化
' M → 80を加算しテキスト化
' M → 90を加算しテキスト化
' D → 100を加算しテキスト化
' D → 110を加算しテキスト化
'
' 例) Yが2の時、2+40 = 42 -> 0x2a 「*」となる。
'
' オリジナルのドキュメントは、
' \\Ts00\share\@開発\document\_Projects\A1000296-00_パピモ遊ガイド　台データ表示対応 (杉浦)\0001_要件定義\外注とのやりとり\IFPC.課題一覧20110311.xls
' になります。
'------------------------------------------------------------------------------

Set fso = WScript.CreateObject("Scripting.FileSystemObject")

For i = 0 To WScript.Arguments.Count - 1
	yyyymmdd = Mid(fso.GetFileName(WScript.Arguments(0)), 9, 8)

	zipPassword = ""

	For j = 0 To 7
		zipPassword = zipPassword & Chr(Int(Mid(yyyymmdd, j + 1, 1)) + (40 + 10 * j))
	Next

	WScript.Echo fso.GetFileName(WScript.Arguments(i)) & " のパスワード [" & zipPassword & Left(fso.GetFileName(WScript.Arguments(0)), 8) & "]"
Next
