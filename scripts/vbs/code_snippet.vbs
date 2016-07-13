'----------------------------------------------------------------------
' code snippet
'----------------------------------------------------------------------
'----------------------------------------------------------------------
' %TEMP%を取得する
'----------------------------------------------------------------------
Function GetTempPath()
	Const TemporaryFolder = 2	'GetSpecialFolder メソッド、引数 option のための変数
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	GetTempPath = fso.GetSpecialFolder(TemporaryFolder)
End Function

'----------------------------------------------------------------------
' ファイル名部分を置き換える
'----------------------------------------------------------------------
Function ChangeFileName(path, filename)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	ChangeFileName = fso.BuildPath(fso.GetParentFolderName(path), filename)
End Function

'----------------------------------------------------------------------
' ログに書き込む
'----------------------------------------------------------------------
Sub WriteLog(msg)
	Const ForAppending = 8
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim path: path = GetTempPath
	Dim basename: basename = fso.GetBaseName(WScript.ScriptFullName)
	Dim ts: Set ts = fso.OpenTextFile(fso.BuildPath(path, basename) & ".log", ForAppending, True)
	ts.WriteLine(Now & " " & msg)
	ts.Close
End Sub

'----------------------------------------------------------------------
' コマンドの実行
'----------------------------------------------------------------------
Function Exec(strCommand)
	WriteLog(strCommand)
	Dim shl: Set shl = WScript.CreateObject("WScript.Shell")
	On Error Resume Next
	Exec = shl.Run(strCommand, , True)
	If Err.Number <> 0 Then
		Exec = Err.Number
		WriteLog("実行時エラー: " & Exec)
	End If
End Function

