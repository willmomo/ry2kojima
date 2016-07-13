'----------------------------------------------------------------------
' 指定フォルダから、*.lnk を列挙する
'----------------------------------------------------------------------
Option Explicit


Call Main	' プログラムスタート


'----------------------------------------------------------------------
' .lnk ファイルの情報を検査する
'----------------------------------------------------------------------
Sub CheckLnkFile(objFile)
	Dim objFs, objShell, objLnk, strPath
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objLnk = objShell.CreateShortcut(objFile.Path)

	strPath = UCase("\\ts00\share\営業\_出荷ソフト２_20040531から運用開始")
	
'	If UCase(Left(objLnk.TargetPath, Len(strPath))) = strPath Then
		WScript.Echo String(64, "-")
		WScript.Echo objFile.Path
		WScript.Echo "      TargetPath: " & objLnk.TargetPath
		WScript.Echo "WorkingDirectory: " & objLnk.WorkingDirectory
'	End If
	
	Set objLnk = Nothing
	Set objShell = Nothing
	Set objFs = Nothing
End Sub


'----------------------------------------------------------------------
' *.lnk を列挙する
'----------------------------------------------------------------------
Sub FileListing(strDir)
	Dim objFs, objCurDir, objSubDir, objFile
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objCurDir = objFs.GetFolder(strDir)
	
	' フォルダ内のファイルを列挙する
	For Each objFile In objCurDir.Files
		If UCase(objFS.GetExtensionName(objFile.Name)) = "LNK" Then
			Call CheckLnkFile(objFile)
		End If
	Next

	' 再帰的にフォルダを下りていく
	On Error Resume Next
	For Each objSubDir In objCurDir.SubFolders
		If Err.Number = 0 Then
			Call FileListing(objSubDir.Path)
		Else
			WScript.Echo strDir & " のサブフォルダ列挙でエラー発生: " & Err.Description
			Err.Clear
			Exit Sub
		End If
	Next
	
	Set objCurDir = Nothing
	Set objFs = Nothing
End Sub


'----------------------------------------------------------------------
' メインルーチン
'----------------------------------------------------------------------
Sub Main
	Dim i
	
	' 一覧開始
	For i = 0 To WScript.Arguments.Count - 1
		Call FileListing(WScript.Arguments(i))
	Next
End Sub
