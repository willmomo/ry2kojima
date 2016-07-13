'----------------------------------------------------------------------
' VBSでログを出力する関数のサンプル
'
' 2009/06/11 r.kojima
'----------------------------------------------------------------------
Option Explicit

'----------------------------------------------------------------------
' 日付を YYYYMMDD の形でフォーマットする
'----------------------------------------------------------------------
Function ToYYYYMMDD(expr)
	ToYYYYMMDD = Right("0000" & Year(expr), 4)
	ToYYYYMMDD = ToYYYYMMDD & Right("00" & Month(expr), 2)
	ToYYYYMMDD = ToYYYYMMDD & Right("00" & Day(expr), 2)
End Function

'----------------------------------------------------------------------
' ログを出力する
'----------------------------------------------------------------------
Sub LogWrite(msg)
	Const TemporaryFolder = 2
	Const ForApending = 8
	Dim fso, fname, tfile
	
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	fname = fso.GetSpecialFolder(TemporaryFolder) & "\" & fso.GetBaseName(WScript.ScriptName) & ToYYYYMMDD(Now) & ".log"
	Set tfile = fso.OpenTextFile(fname, ForApending, True)
	tfile.WriteLine(Now & " " & msg)
	tfile.Close
	
	Set tfile = Nothing
	Set fso = Nothing
End Sub



'----------------------------------------------------------------------
' テストコード
'----------------------------------------------------------------------
LogWrite("プログラムスタート")
