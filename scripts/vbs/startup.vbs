'******************************************************************************
'*
'* スタートアップに関するメンテナンス用スクリプト
'*
'* created by : r.kojima in 2011/05/10
'* 2016/01/12 r.kojima : clr コマンド追加
'*
'* 使用例:
'*   >startup.vbs dir
'*   >startup.vbs clr <alluser | curuser>
'*   >startup.vbs del WatchProc.lnk Win操作.lnk バックアップ.lnk ホールコン通信.lnk 通信管理.lnk BackupTool.lnk システム選択メニュー.lnk
'*   >startup.vbs mkl cur patcher C:\patcher\patcher.exe "-i c:\mpc2\patcher_mpc2.ini"
'*
'******************************************************************************
Option Explicit

	Dim i, strArguments, strStartupTarget

	'引数が無いときは、使用法を表示して終了
	If WScript.Arguments.Count = 0 Then
		Call Usage
	End If

	'一個目の引数は、スクリプトへのコマンド
	Select Case LCase(WScript.Arguments(0))
	Case "dir"
		WScript.Echo DirStartupFile

	Case "del"
		For i = 1 To WScript.Arguments.Count - 1
			DeleteStartupFile(WScript.Arguments(i))
		Next

	Case "clr"
		If WScript.Arguments.Count <> 2 Then
			Call Usage
		End If

		Call ClearStartupFile(WScript.Arguments(2))
		
	Case "mkl"
		If WScript.Arguments.Count < 4 Then
			Call Usage
		End If

		strStartupTarget = LCase(WScript.Arguments(1))
		If strStartupTarget <> "all" And strStartupTarget <> "cur" Then
			Call Usage
		End If

		If WScript.Arguments.Count >= 5 Then
			strArguments = WScript.Arguments(4)
		End If

		Call CreateStartupFile(strStartupTarget, WScript.Arguments(2), WScript.Arguments(3), strArguments)

	End Select

'******************************************************************************
'*
'* 使用法の表示と終了
'*
'******************************************************************************
Sub Usage()
	WScript.Echo _
		"使い方：" & WScript.ScriptName & " [dir|del] [...]" & vbNewLine & _
		"dir" & vbNewLine & _
		"  スタートアップにあるファイルを一覧表示する。" & vbNewLine & _
		vbNewLine & _
		"del <ファイル名> ..." & vbNewLine & _
		"  指定したファイルをスタートアップから削除する。" & vbNewLine & _
		vbNewLine & _
		"mkl <all|cur> <ショートカット名> <実行ファイル名> [コマンドライン引数]" & vbNewLine & _
		"  指定したコマンドラインへのショートカットを作成する。" & vbNewLine & _
		"  all を指定すると、All Users 、cur を指定するとカレントユーザーに作成される。" & vbNewLine
	
	WScript.Quit -1
End Sub

'******************************************************************************
'*
'* ショートカットを作成する
'*
'******************************************************************************
Sub CreateShortcut(strPathName, strTargetPath, strWorkingDirectory, strArguments)
	Dim objShell, objShortcut
	Set objShell = WScript.CreateObject("WScript.Shell")
	
	Set objShortcut = objShell.CreateShortcut(strPathName)
	With objShortcut
		.TargetPath = strTargetPath
		.WorkingDirectory = strWorkingDirectory
		.Arguments = strArguments
		.Save
	End With
End Sub


'******************************************************************************
'*
'* スタートアップにショートカットを作成する
'*
'******************************************************************************
Sub CreateStartupFile(strStartupTarget, strShortcutName, strCommandLine, strArguments)
	Dim objShell, objFs, strShortcutPath
	
	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	
	If strStartupTarget = "all" Then
		strShortcutPath = objShell.SpecialFolders("AllUsersStartup")
	Else
		strShortcutPath = objShell.SpecialFolders("Startup")
	End If

	'ショートカットを作成
	Call CreateShortcut(strShortcutPath & "\" & strShortcutName & ".lnk", _
						strCommandLine, _
						objFs.GetParentFolderName(strCommandLine), _
						strArguments)
End Sub

'******************************************************************************
'*
'* All Users のスタートアップとCurrent User のスタートアップのディレクトリ一覧を表示
'*
'* [引数]
'*
'* [戻り値]
'*
'******************************************************************************
Function DirStartupFile()
	Dim objShell, objFs, strResult, objFile

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	For Each objFile In objFs.GetFolder(objShell.SpecialFolders("AllUsersStartup")).Files
		strResult = strResult & objFile.Path & vbNewLine
	Next

	For Each objFile In objFs.GetFolder(objShell.SpecialFolders("Startup")).Files
		strResult = strResult & objFile.Path & vbNewLine
	Next

	DirStartupFile = strResult
End Function

'******************************************************************************
'*
'* All Users のスタートアップとCurrent User のスタートアップから指定ファイルを削除する。
'*
'* [引数]
'* ファイル名を指定する。
'*
'* [戻り値]
'* 0 = 削除ファイルなし
'* 1 = All Users のスタートアップからファイルを削除
'* 2 = Current User のスタートアップからファイルを削除
'* 3 = All Users と Current User の両方のスタートアップからファイルを削除
'*
'******************************************************************************
Function DeleteStartupFile(strFileName)
	Dim objShell, objFs, strTarget

	DeleteStartupFile = 0

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	strTarget = objShell.SpecialFolders("AllUsersStartup") & "\" & strFileName
	If objFs.FileExists(strTarget) Then
		Call objFs.DeleteFile(strTarget, True)
		DeleteStartupFile = 1
	End If

	strTarget = objShell.SpecialFolders("Startup") & "\" & strFileName
	If objFs.FileExists(strTarget) Then
		Call objFs.DeleteFile(strTarget, True)
		DeleteStartupFile = DeleteStartupFile + 2
	End If
End Function

'******************************************************************************
'*
'* All Users のスタートアップとCurrent User のスタートアップを空にする
'*
'* [引数]
'* strMode = alluser or curuser
'*
'* [戻り値]
'*
'******************************************************************************
Function ClearStartupFile(strMode)
	Dim objShell, objFs, strTarget

	DeleteStartupFile = 0

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	If LCase(strMode) = "alluser" Then
		strTarget = objShell.SpecialFolders("AllUsersStartup") & "\" & strFileName
	ElseIf LCase(strMode) = "curuser" Then
		strTarget = objShell.SpecialFolders("Startup") & "\" & strFileName
	Else
		Exit Function
	End If
	
	For Each objFile In objFs.GetFolder(strTarget).Files
		Call objFs.DeleteFile(objFile.Path)
	Next
End Function

