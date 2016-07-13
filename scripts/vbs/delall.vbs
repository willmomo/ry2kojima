Option Explicit

'----------------------------------------------------------------------
' 使用法を表示
'----------------------------------------------------------------------
Sub usage
	WScript.Echo "usage: delall フォルダ名 [...]"
End Sub

'----------------------------------------------------------------------
' 指定フォルダ以下を再帰的に削除
'----------------------------------------------------------------------
Sub delall(objPath)
	Dim obj

	For Each obj In objPath.SubFolders
		delall obj
	Next

	For Each obj In objPath.Files
		WScript.Echo "[F] " & obj.Path
		obj.Delete
	Next
	
	WScript.Echo "[D] " & objPath.Path
	objPath.Delete
End Sub

'----------------------------------------------------------------------
' メイン
'----------------------------------------------------------------------
Function doMain
	Dim objFs, arg

	doMain = -1
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	If LCase(objFs.GetFileName(WScript.FullName)) <> "cscript.exe" Then
		WScript.Echo "cscript で実行してください。"
		Exit Function
	End If

	If WScript.Arguments.Count = 0 Then
		usage
		Exit Function
	End If

	For Each arg In WScript.Arguments
		delall objFs.GetFolder(arg)
	Next

	doMain = 0
End Function

'======================================================================

WScript.Quit doMain
