'----------------------------------------------------------------------
' VBSでログを出力する関数のサンプル
'
' 2014/05/21 r.kojima
' 2014/10/03 r.kojima 最大サイズをConst化
'----------------------------------------------------------------------
Option Explicit

'----------------------------------------------------------------------
' ログを出力する
' 最大サイズでシフトする。
'----------------------------------------------------------------------
Sub LogWrite(msg)
	On Error Resume Next
	Const TemporaryFolder = 2
	Const ForApending = 8
	'----------
	Const MAX_SIZE = 1048576
	Dim fso, fname, tfile, i, fnameSrc, fnameDst, logPath
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	
	'用途に合わせて保存先を直接変更して使う
	
	'%TEMP%を使う場合(標準)
	''logPath = fso.GetSpecialFolder(TemporaryFolder)
	
	'スクリプトの存在する場所
	logPath = fso.GetParentFolderName(WScript.ScriptFullName)
	
	'フォルダを作ってから保存する場合
	logPath = fso.BuildPath(logPath, "log")
	
	If Not fso.FolderExists(logPath) Then fso.CreateFolder(logPath)
	fname = fso.BuildPath(logPath, fso.GetBaseName(WScript.ScriptName) & ".log")
	Set tfile = fso.OpenTextFile(fname, ForApending, True)
	tfile.WriteLine(Now & " " & msg)
	tfile.Close
	If fso.GetFile(fname).Size > MAX_SIZE Then
		fnameDst = fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_9.log"
		If fso.FileExists(fnameDst) Then fso.DeleteFile fnameDst
		For i = 8 to 1 Step -1
			fnameSrc = fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_" & i & ".log"
			fnameDst = fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_" & (i + 1) & ".log"
			If fso.FileExists(fnameSrc) Then fso.MoveFile fnameSrc, fnameDst
		Next
		fso.MoveFile fname, fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_1.log"
	End If
	WScript.StdOut.WriteLine msg
End Sub



'----------------------------------------------------------------------
' テストコード
'----------------------------------------------------------------------
Do While True
	LogWrite("プログラムスタート")
Loop
