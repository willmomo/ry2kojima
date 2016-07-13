Option Explicit

WScript.Quit(DoMain())

'======================================================================

Dim g_logLevel

Function LogEnter(msg)
	Call LogWrite(msg & " {")
	g_logLevel = g_logLevel + 1
	LogEnter = g_logLevel
End Sub

Sub LogLeave(msg, code, logLevel)
	g_logLevel = logLevel
	g_logLevel = g_logLevel - 1
	Call LogWrite("} " & msg & " = " & code)
End Sub

'----------------------------------------------------------------------
' ログを出力する
'----------------------------------------------------------------------
Sub LogWrite(msg)
	On Error Resume Next
	Const TemporaryFolder = 2
	Const ForApending = 8
	Dim fso, fname, tfile, i, fnameSrc, fnameDst
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	fname = fso.GetSpecialFolder(TemporaryFolder) & "\" & fso.GetBaseName(WScript.ScriptName) & ".log"
	Set tfile = fso.OpenTextFile(fname, ForApending, True)
	tfile.WriteLine(Now & " " & String(g_logLevel * 2, " ") & msg)
	tfile.Close
	If fso.GetFile(fname).Size > 1048576 Then
		fnameDst = fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_9.log"
		If fso.FileExists(fnameDst) Then fso.DeleteFile fnameDst
		For i = 8 to 1 Step -1
			fnameSrc = fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_" & i & ".log"
			fnameDst = fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_" & (i + 1) & ".log"
			If fso.FileExists(fnameSrc) Then fso.MoveFile fnameSrc, fnameDst
		Next
		fso.MoveFile fname, fso.GetParentFolderName(fname) & "\" & fso.GetBaseName(fname) & "_1.log"
	End If
	WScript.StdOut.WriteLine String(g_logLevel * 2, " ") & msg
End Sub

'----------------------------------------------------------------------
' INIファイルに情報を書き込む
'----------------------------------------------------------------------
Function WriteIniString(strSection, strKey, strValue, strIniFile)
	Dim objFso
	Dim objTsRead
	Dim objTsWrite
	Dim strLine
	Dim strCurSection
	Dim strCurKey
	Dim strCurValue
	Dim nPosEqu
	Dim blnWritten
	
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	
	If Not objFso.FileExists(strIniFile) Then
		Set objTsRead = objFso.CreateTextFile(strIniFile, True)
		objTsRead.Close
	End If
	
	blnWritten = False
	
	Set objTsRead = objFso.OpenTextFile(strIniFile)
	Set objTsWrite = objFso.CreateTextFile(strIniFile & ".$$$", True)
	Do Until objTsRead.AtEndOfStream
		strLine = objTsRead.ReadLine
		
		If Left(strLine, 1) = "[" And Right(strLine, 1) = "]" Then
			'セクションの時の処理
			'セクションが変わる前に前の値の書き込みチェックを行う
			If blnWritten = False And UCase(strCurSection) = UCase(strSection) Then
				objTsWrite.WriteLine(strKey & "=" & strValue)
				blnWritten = True
			End If
			
			strCurSection = Mid(strLine, 2, Len(strLine) - 2)
			objTsWrite.WriteLine(strLine)
		ElseIf InStr(strLine, "=") > 1 Then
			'キーの時の処理
			nPosEqu = InStr(strLine, "=")
			strCurKey = Left(strLine, nPosEqu - 1)
			strCurValue = Mid(strLine, nPosEqu + 1)
			
			'LogWrite("[" & strCurSection & "][" & strCurKey & "][" & strCurValue & "]")
			
			If UCase(strCurSection) = UCase(strSection) And UCase(strCurKey) = UCase(strKey) Then
				objTsWrite.WriteLine(strCurKey & "=" & strValue)
				blnWritten = True
			Else
				objTsWrite.WriteLine(strLine)
			End If
		Else
			objTsWrite.WriteLine(strLine)
		End If
	Loop
	
	If blnWritten = False Then
		objTsWrite.WriteLine("[" & strSection & "]")
		objTsWrite.WriteLine(strKey & "=" & strValue)
	End If
	
	objTsWrite.Close
	objTsRead.Close
	
	objFso.DeleteFile strIniFile
	objFso.MoveFile strIniFile & ".$$$", strIniFile
End Function

'----------------------------------------------------------------------
' INIファイルから情報を読み込む
'----------------------------------------------------------------------
Function GetIniString(strSection, strKey, strDefault, strIniFile)
	Dim objFso
	Dim objTs
	Dim strLine
	Dim strCurSection
	Dim strCurKey
	Dim strCurValue
	Dim nPosEqu
	
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	
	GetIniString = strDefault
	If objFso.FileExists(strIniFile) Then
		Set objTs = objFso.OpenTextFile(strIniFile)
		Do Until objTs.AtEndOfStream
			strLine = Trim(objTs.ReadLine)
			If Left(strLine, 1) = ";" Or Left(strLine, 1) = "#" Then
				' ; と # で始まる行はコメントとみなす
			ElseIf Left(strLine, 1) = "[" And Right(strLine, 1) = "]" Then
				'セクションの時の処理
				strCurSection = Mid(strLine, 2, Len(strLine) - 2)
			ElseIf InStr(strLine, "=") > 1 Then
				'キーの時の処理
				nPosEqu = InStr(strLine, "=")
				strCurKey = Left(strLine, nPosEqu - 1)
				strCurValue = Mid(strLine, nPosEqu + 1)
				
				'LogWrite("[" & strCurSection & "][" & strCurKey & "][" & strCurValue & "]")
				
				If UCase(strCurSection) = UCase(strSection) And UCase(strCurKey) = UCase(strKey) Then
					GetIniString = strCurValue
					Exit Do
				End If
			End If
		Loop
		objTs.Close
	End If
End Function

'----------------------------------------------------------------------
' sntpw をスタートアップから外す
'----------------------------------------------------------------------
Function RemoveStartup()
	Dim logLevel
	logLevel = LogEnter("RemoveStartup")
	
	Dim objShell
	Dim objFso
	Dim strPath
	
	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")

	strPath = objFso.BuildPath(objShell.SpecialFolders("AllUsersStartup"), "sntpw.lnk")
	If objFso.FileExists(strPath) Then
		Call LogWrite(strPath & " が見つかったので削除します。")
		''Call objFso.DeleteFile(strPath)
	End If

	strPath = objFso.BuildPath(objShell.SpecialFolders("Startup"), "sntpw.lnk")
	If objFso.FileExists(strPath) Then
		Call LogWrite(strPath & " が見つかったので削除します。")
		''Call objFso.DeleteFile(strPath)
	End If
	
	Call LogLeave("RemoveStartup", RemoveStartup, logLevel)
End Function

'----------------------------------------------------------------------
' sntpwの停止
'----------------------------------------------------------------------
Function StopSntpw()
	Dim logLevel
	logLevel = LogEnter("StopSntpw")
	
	WriteIniFile "debug", "command", "exit", "C:\sntpw\sntpw.ini"
	
	Call LogLeave("StopSntpw", StopSntpw, logLevel)
End Function

'----------------------------------------------------------------------
' sntpd.ini のコピー
'----------------------------------------------------------------------
Function CopySntpdIni()
	Dim logLevel
	logLevel = LogEnter("CopySntpdIni")
	
	Dim objFso
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	objFso.CopyFile "C:\sntpw\sntpw.ini" "C:\sntpw\sntpd.ini"
	
	Call LogLeave("CopySntpdIni", CopySntpdIni, logLevel)
End Function

'----------------------------------------------------------------------
' sntpd.exe のコピー
'----------------------------------------------------------------------
Function CopySntpd()
	Dim logLevel
	logLevel = LogEnter("CopySntpd")
	
	Dim objFso
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	objFso.CopyFile "sntpd.exe", "C:\sntpw\sntpd.exe"
	
	Const vbNormalFocus = 1			'通常のウィンドウ、かつ最前面のウィンドウにします
	Dim objShell
	Set objShell = WScript.CreateObject("WScript.Shell")
	objShell.Run "C:\sntpw\sntpd.exe install", vbNormalFocus, True)
	
	LogLeave "CopySntpd", CopySntpd, logLevel
End Function

'----------------------------------------------------------------------
' メイン処理
'----------------------------------------------------------------------
Function DoMain()
	Call LogWrite("---------- " & WScript.ScriptFullName & " start ----------")
	Dim logLevel
	logLevel = LogEnter("DoMain")
	
	'1 sntpw をスタートアップから外す
	RemoveStartup
	
	'2 sntpw を停止する
	StopSntpw
	
	'3 sntpw.ini を sntpd.ini にコピーする
	CopySntpdIni
	
	'4 sntpd をコピーする(コピー先は C:\sntpw)
	CopySntpd
	
	Call LogLeave("DoMain", DoMain, logLevel)
	Call LogWrite("---------- " & WScript.ScriptFullName & " end ----------")
End Function
