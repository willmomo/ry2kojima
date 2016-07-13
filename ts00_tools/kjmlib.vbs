'----------------------------------------------------------------------
' human readable format にする
'----------------------------------------------------------------------
Function FormatHumanReadable(expr)
	If expr > (1024 * 1024) Then
		FormatHumanReadable = Int(expr / 1024 / 1024) & "MB"
	ElseIf expr > 1024 Then
		FormatHumanReadable = Int(expr / 1024) & "KB"
	Else
		FormatHumanReadable = expr & "B"
	End If
End Function

'----------------------------------------------------------------------
' IIf
'----------------------------------------------------------------------
Function IIf(expr, truepart, falsepart)
	If expr Then
		IIf = truepart
	Else
		IIf = falsepart
	End If
End Function

'----------------------------------------------------------------------
' イベントログに出力
'----------------------------------------------------------------------
Sub LogEvent(flag, msg)
	Dim objShell
	Set objShell = WScript.CreateObject("WScript.Shell")
	objShell.LogEvent flag, msg
End Sub

Sub LogInfoEvent(msg)
	Const EVENT_SUCCESS = 0
	LogEvent EVENT_SUCCESS, msg
End Sub

Sub LogErrorEvent(msg)
	Const EVENT_ERROR = 1
	LogEvent EVENT_ERROR, msg
End Sub

'----------------------------------------------------------------------
' フォルダの削除
'----------------------------------------------------------------------
Sub DeleteFolder(strPath)
	Dim objFs
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	If objFs.FolderExists(strPath) Then
		objFs.DeleteFolder strPath
	End If
End Sub

'----------------------------------------------------------------------
' ファイルの削除
'----------------------------------------------------------------------
Sub DeleteFile(strPath)
	Dim objFs
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	If objFs.FileExists(strPath) Then
		objFs.DeleteFile strPath
	End If
End Sub

'------------------------------------------------------------------
' YYYYMMDD 形式の Now を返す
'------------------------------------------------------------------
Function NowYYYYMMDD
	Dim expr
	expr = Now
	NowYYYYMMDD = Year(expr) & ZeroPad(2, Month(expr)) & ZeroPad(2, Day(expr))
End Function

'----------------------------------------------------------------------
' プログラムの実行
'----------------------------------------------------------------------
Function Run(cmdLine, nStyle, bWait)
	Dim objShell
	Set objShell = WScript.CreateObject("WScript.Shell")

''	g_log.LogWrite "Run cmdLine = '" & cmdLine & "'"
	Run = objShell.Run(cmdLine, nStyle, bWait)
End Function

'----------------------------------------------------------------------
' シャットダウンコマンドで再起動
'----------------------------------------------------------------------
Function Reboot()
	Dim cmdLine
	cmdLine = "shutdown -r -t 0"
	Reboot = Run(cmdLine, 1, True)
End Function

'----------------------------------------------------------------------
'
'----------------------------------------------------------------------
Function ZeroPad(cols, expr)
	ZeroPad = Right(String(cols, "0") & expr, cols)
End Function

'----------------------------------------------------------------------
' 汎用ログのシングルトン
'----------------------------------------------------------------------
Dim g__FileLog_singleton__
Function GetFileLog()
	If IsEmpty(g__FileLog_singleton__) Then Set g__FileLog_singleton__ = New FileLog
	Set GetFileLog = g__FileLog_singleton__
End Function

'----------------------------------------------------------------------
' 汎用ログクラス
'----------------------------------------------------------------------
Class FileLog
	'ログの識別子
	Private m_logIdent

	Public Property Get LogIdent
		LogIdent = m_logIdent
	End Property

	Public Property Let LogIdent(expr)
		m_logIdent = expr
	End Property

	'ログの保存先
	Private m_logPath

	Public Property Get LogPath
		LogPath = m_logPath
	End Property

	Public Property Let LogPath(expr)
		m_logPath = expr
	End Property

	'ログの保存先をテンポラリフォルダにする
	Public Sub SetLogPathToTemporaryFolder(expr)
		Const TemporaryFolder = 2
		Dim fso
		Set fso = WScript.CreateObject("Scripting.FileSystemObject")
		m_logPath = fso.GetSpecialFolder(TemporaryFolder)
		If expr <> "" Then m_logPath = fso.BuildPath(m_logPath, expr)
	End Sub

	'ログの保存先をスクリプトファイルと同じフォルダにする
	Public Sub SetLogPathToScriptFolder(expr)
		Dim fso
		Set fso = WScript.CreateObject("Scripting.FileSystemObject")
		m_logPath = fso.GetParentFolderName(WScript.ScriptFullName)
		If expr <> "" Then m_logPath = fso.BuildPath(m_logPath, expr)
	End Sub

	'----------------------------------------------------------------------
	' ログを出力する
	' 最大サイズでシフトする。
	'----------------------------------------------------------------------
	Sub LogWrite(msg)
		On Error Resume Next
		Const ForApending = 8
		'----------
		Dim fso, fname, tfile, i, fnameSrc, fnameDst
		Set fso = WScript.CreateObject("Scripting.FileSystemObject")
		If Not fso.FolderExists(m_logPath) Then fso.CreateFolder(m_logPath)
		fname = fso.BuildPath(m_logPath, fso.GetBaseName(WScript.ScriptName) & ".log")
		Set tfile = fso.OpenTextFile(fname, ForApending, True)
		tfile.WriteLine(FormattedNow & " [" & m_logIdent & "] " & msg)
		tfile.Close
		WScript.StdOut.WriteLine FormattedNow & " [" & m_logIdent & "] " & msg
		LogShift
	End Sub

	'----------------------------------------------------------------------
	' 最大サイズでシフトする
	'----------------------------------------------------------------------
	Sub LogShift
		Const MAX_SIZE = 1048576
		Dim fso, fname
		Set fso = WScript.CreateObject("Scripting.FileSystemObject")
		fname = fso.BuildPath(m_logPath, fso.GetBaseName(WScript.ScriptName) & ".log")
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
	End Sub

	'------------------------------------------------------------------
	' Now を固定長にフォーマットする
	'------------------------------------------------------------------
	Function FormattedNow
		Dim expr
		expr = Now
		FormattedNow = Year(expr) & "/" & ZeroPad(2, Month(expr)) & "/" & ZeroPad(2, Day(expr)) & " " & ZeroPad(2, Hour(expr)) & ":" & ZeroPad(2, Minute(expr)) & ":" & ZeroPad(2, Second(expr))
	End Function
End Class
