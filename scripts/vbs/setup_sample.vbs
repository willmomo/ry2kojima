Option Explicit

WScript.Quit(DoMain())

'======================================================================

Dim g_logLevel

Sub LogEnter(msg)
	Call LogWrite(msg & " {")
	g_logLevel = g_logLevel + 1
End Sub

Sub LogLeave(msg, code)
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
' FileSystemObject.FileExists
'----------------------------------------------------------------------
Function FileSystemObject_FileExists(filespec)
	Call LogEnter("FileSystemObject_FileExists")
	Call LogWrite("filespec = " & filespec)
	
	Dim objFso
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	FileSystemObject_FileExists = objFso.FileExists(filespec)
	
	Call LogLeave("FileSystemObject_FileExists", FileSystemObject_FileExists)
End Function

'----------------------------------------------------------------------
' プログラム実行
'----------------------------------------------------------------------
Function Shell_Run(strCommand)
	Call LogEnter("Shell_Run")
	Call LogWrite("strCommand = " & strCommand)
	
	On Error Resume Next
	Const vbNormalFocus = 1			'通常のウィンドウ、かつ最前面のウィンドウにします
	Dim objShell
	Set objShell = WScript.CreateObject("WScript.Shell")
	Shell_Run = objShell.Run(strCommand, vbNormalFocus, True)
	If Err.Number <> 0 Then
		Shell_Run = Err.Number
		Call LogWrite("error " & Err.Number & ": " & Err.Description)
	End If
	
	Call LogLeave("Shell_Run", Shell_Run)
End Function

'----------------------------------------------------------------------
' BNSのインストール
'----------------------------------------------------------------------
Function InstallBNServer(verSrc, backup)
	Call LogEnter("InstallBNServer")
	Call LogWrite("verSrc = " & verSrc)
	Call LogWrite("backup = " & backup)
	
	InstallBNServer = Shell_Run("C:\BNVersionUp\BNVersionUp.exe " & verSrc & " " & backup)
	
	Call LogLeave("InstallBNServer", InstallBNServer)
End Function

'----------------------------------------------------------------------
' CSVConverter Service の停止
'----------------------------------------------------------------------
Function StopCSVConverterService()
	Call LogEnter("StopCSVConverterService")
	
	Dim cmdLine
	cmdLine = "C:\CSVConverter\CSVConverterStop.exe"
	StopCSVConverterService = 0
	If FileSystemObject_FileExists(cmdLine) Then
		InstallCSVConverter = Shell_Run(cmdLine)
	End If
	
	Call LogLeave("StopCSVConverterService", StopCSVConverterService)
End Function

'----------------------------------------------------------------------
' CSVConvererインストール
'----------------------------------------------------------------------
Function InstallCSVConverter()
	Call LogEnter("InstallCSVConverter")
	
	InstallCSVConverter = Shell_Run("C:\CSVConverter\CSVConverterInstaller.exe")
	
	Call LogLeave("InstallCSVConverter", InstallCSVConverter)
End Function

'----------------------------------------------------------------------
' メイン処理
'----------------------------------------------------------------------
Function DoMain()
	Call LogWrite("---------- " & WScript.ScriptFullName & " start ----------")
	Call LogEnter("DoMain")
	
	Dim verSrc
	Dim backup
	
	If WScript.Arguments.Count = 2 Then
		verSrc = WScript.Arguments.Item(0)
		backup = WScript.Arguments.Item(1)
		
		DoMain = InstallBNServer(verSrc, backup)
		DoMain = StopCSVConverterService()
		DoMain = InstallCSVConverter()
	Else
		Call LogWrite("引数異常")
		Call LogWrite(WScript.ScriptName & " <VERSRC> <BACKUP>")
		Call LogWrite("  VERSRC  <VerupBasePath> + <バージョン番号> + <TargetPath>")
		Call LogWrite("  BACKUP  「バックアップする」を選択したときは、1 そうでないときは 0 を指定")
		DoMain = -1
	End If
	
	Call LogLeave("DoMain", DoMain)
	Call LogWrite("---------- " & WScript.ScriptFullName & " end ----------")
End Function
