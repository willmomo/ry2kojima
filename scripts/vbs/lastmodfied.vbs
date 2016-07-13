
' サブフォルダのサイズをリストアップする。
'

Option Explicit

' cscriptで動かされているかどうか、チェックする
'
Sub CheckRunOnCScript()

	' FileSystemObjectを作成する
	Dim objFS
	Set objFS = WScript.CreateObject("Scripting.FileSystemObject")
	
	' cscript以外で動いているとき
	If LCase(objFS.GetFileName(WScript.FullName)) <> "cscript.exe" Then
	
		' エラーメッセージを表示
		WScript.Echo "cscript を使って実行してください。" & vbNewLine & _
					 "例) cscript """ & WScript.ScriptFullName & """"
					 
		' 終了
		WScript.Quit(1)
	End If
End Sub

' オプションを調べる
' 異常の時は、ヘルプを表示して終了
'
Sub CheckArgument()
	If WScript.Arguments.Count = 0 Then
		WScript.Echo "使用法：wdu フォルダ名"
		WScript.Quit(1)
	End If
End Sub

' フォルダの中で更新日付が、もっとも新しいFileオブジェクトを返す
'
Function LatestLastModified(folderName)
	Dim objLatestFile, objTempFile
	Set objLatestFile = Nothing

	' FileSystemObjectを作成
	Dim objFS
	Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

	' Folderオブジェクトを作成
	Dim objFolder
	Set objFolder = objFS.GetFolder(folderName)

	' サブフォルダの中から、もっとも更新日付の新しいFileオブジェクトを探す
	Dim objSubFolder
	For Each objSubFolder In objFolder.SubFolders
		Set objTempFile = LatestLastModified(objSubFolder.Path)
		If objLatestFile Is Nothing Then
			Set objLatestFile = objTempFile
		ElseIf objTempFile Is Nothing Then
			' 何もしない
		ElseIf objLatestFile.DateLastModified < objTempFile.DateLastModified Then
			Set objLatestFile = objTempFile
		End If
	Next

	' フォルダ直下にあるファイルの中から、もっとも更新日付の新しいFileオブジェクトを探す
	Dim objFile
	For Each objFile In objFolder.Files
		If objLatestFile Is Nothing Then
			Set objLatestFile = objFile
		ElseIf objLatestFile.DateLastModified < objFile.DateLastModified Then
			Set objLatestFile = objFile
		End If
	Next

	' 最も更新日付の新しいファイルオブジェクトを返す
	Set LatestLastModified = objLatestFile
End Function

' メイン処理
'
Sub DoMain()
	' cscriptで動いているかどうか調べる
	CheckRunOnCScript

	' オプションを調べる
	CheckArgument

	' FileSystemObjectを作成
	Dim obJFS
	Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

	' 引数で指定されたフォルダをすべてリストアップ
	Dim i, objLatestFile
	For i = 0 To WScript.Arguments.Count - 1
		
		' 引数で指定されたフォルダ内のサブフォルダのサイズをリストアップ
		Dim objFolder, objSubFolder
		Set objFolder = objFS.GetFolder(WScript.Arguments.Item(i))
		For Each objSubFolder In objFolder.SubFolders
			Set objLatestFile = LatestLastModified(objSubFolder.Path)
			If Not (objLatestFile Is Nothing) Then
				WScript.Echo objLatestFile.DateLastModified & "," & objLatestFile.Path
			End If
		Next
	Next
End Sub

' メイン処理実行
DoMain
