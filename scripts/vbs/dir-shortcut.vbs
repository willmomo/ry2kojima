'----------------------------------------------------------------------
' ファイルを一覧するスクリプト
' 何をさせるかは、アナタ次第。
' 適当に書き換えてください。
'----------------------------------------------------------------------
Option Explicit


'----------------------------------------------------------------------
' よく使うオブジェクトの生成
'----------------------------------------------------------------------
Dim g_objFS, g_objShell
Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objShell = WScript.CreateObject("WScript.Shell")


If ChkArg = False Then Usage	'引数チェック
Call Main						'この関数を起点に処理を記述していく


'----------------------------------------------------------------------
' 使用法を表示して終了する
'----------------------------------------------------------------------
Sub Usage()
	Dim strUsage
	
	strUsage = "使用法：" & WScript.ScriptName & " <検索フォルダ>..."
	
	WScript.Echo strUsage
	WScript.Quit
End Sub


'----------------------------------------------------------------------
' 引数のチェック
'
' 引数が不適切な場合は、Falseを返すようにする。
'----------------------------------------------------------------------
Function ChkArg()
	If WScript.Arguments.Count = 0 Then
		ChkArg = False
		Exit Function
	End If
	
	ChkArg = True
End Function


'----------------------------------------------------------------------
' ファイルに対して何かをする
'----------------------------------------------------------------------
Sub DoAnything(objFile)
	Dim objLnk
	Set objLnk = g_objShell.CreateShortcut(objFile.Path)

	If UCase(g_objFS.GetDriveName(objLnk.TargetPath)) = "\\HQPC208\開発" Then
		WScript.Echo String(64, "-")
		WScript.Echo objFile.Path
		WScript.Echo "      TargetPath: " & objLnk.TargetPath
		WScript.Echo "WorkingDirectory: " & objLnk.WorkingDirectory
		''WScript.Echo "       DriveName: " & g_objFS.GetDriveName(objLnk.TargetPath)
	End If
	
	Set objLnk = Nothing
End Sub


'----------------------------------------------------------------------
' ファイルに対してフィルタをかける
'----------------------------------------------------------------------
Function IsTarget(objFile)
	If UCase(g_objFS.GetExtensionName(objFile.Name)) = "LNK" Then
		IsTarget = True
	Else
		IsTarget = False
	End If
End Function


'----------------------------------------------------------------------
' ファイルを一覧する
'----------------------------------------------------------------------
Function FileListing(objFolder)
	On Error Resume Next
	
	Dim objSubFolder, objFile

	' 再帰的にフォルダを降りていく
	For Each objSubFolder In objFolder.SubFolders
		If Err.Number = 0 Then
			FileListing(objSubFolder)
		Else
			WScript.Echo "Folder access error: " & Err.Description & ": " & objFolder
			Exit For
		ENd If
	Next

	' フォルダ内のファイルを列挙して情報を出力する
	For Each objFile In objFolder.Files
		If Err.Number = 0 Then
			If IsTarget(objFile) Then
				Call DoAnything(objFile)
			End If
		Else
			WScript.Echo "File access error: " & Err.Description & ": " & objFolder
			Exit For
		ENd If
	Next
End Function


'----------------------------------------------------------------------
' メインルーチン
'----------------------------------------------------------------------
Sub Main
	Dim i
	
	' 一覧開始!
	For i = 0 To WScript.Arguments.Count - 1
		FileListing(g_objFS.GetFolder(WScript.Arguments(i)))
	Next
End Sub

