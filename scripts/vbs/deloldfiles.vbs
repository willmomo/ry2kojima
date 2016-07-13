'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Option Explicit

Const MONTH_AGO = 2

WScript.Quit doMain

'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Function doMain()
	Dim objFso, objD
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")

	'wscriptでの実行をガードする
	If LCase(objFso.GetFileName(WScript.FullName)) = "wscript.exe" Then
		WScript.Echo "cscriptで実行してください。"
		doMain = -1
		Exit Function
	End If

	WScript.Echo "Delete the file that was created " & MONTH_AGO & " month(s) ago."
	
	'ファイルを一覧する
	Call listing(WScript.Arguments(0))

	''' 空っぽのフォルダがあれば、それを削除して回る
	For Each objD In objFso.GetFolder(WScript.Arguments(0)).SubFolders
		Call deleteEmptyFolder(objD.Path)
	Next
End Function


'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Sub listing(path)
	Dim objFso
	Dim objDir
	Dim objF
	Dim objD

	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	Set objDir = objFso.GetFolder(path)
	For Each objF In objDir.Files
		'作成日が、一ヶ月以上古いファイルだけ出力する
		'''If objF.DateCreated < DateValue("2013/06/21") Then
		If objF.DateCreated < DateAdd("m", -MONTH_AGO, Now) Then
			WScript.Echo objF.DateCreated & vbTab & objFso.BuildPath(path, objF.Name)

			''' 対象ファイルを削除します!!
			objFso.DeleteFile objFso.BuildPath(path, objF.Name)
		End If
	Next
	For Each objD In objDir.SubFolders
		'''WScript.Echo objD.Path
		Call listing(objD.Path)
	Next
End Sub


'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Sub deleteEmptyFolder(path)
	Dim objFso
	Dim objDir
	Dim objF
	Dim objD

	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	Set objDir = objFso.GetFolder(path)

	For Each objD In objDir.SubFolders
		Call deleteEmptyFolder(objD.Path)
	Next
	If objDir.Files.Count = 0 And objDir.SubFolders.Count = 0 Then
		WScript.Echo "削除 : " & path
		objDir.Delete
	End If
End Sub
