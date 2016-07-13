
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

Function hr(nValue)
	If nValue < (1024) Then
		hr = nValue & " B "
	ElseIf nValue < (1024 * 1024) Then
		hr = (nValue / 1024) & " KB"
	ElseIf nValue < (1024 * 1024 * 1024) Then
		hr = (nValue / 1024 / 1024) & " MB"
	ElseIf nValue < (1024 * 1024 * 1024 * 1024) Then
		hr = (nValue / 1024 / 1024 / 1024) & " GB"
	Else
		hr = (nValue / 1024 / 1024 / 1024 / 1024) & " TB"
	End If
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
	Dim i
	For i = 0 To WScript.Arguments.Count - 1
		
		' 引数で指定されたフォルダ内のサブフォルダのサイズをリストアップ
		Dim objFolder, objSubFolder
		Set objFolder = objFS.GetFolder(WScript.Arguments.Item(i))
		On Error Resume Next
		For Each objSubFolder In objFolder.SubFolders
			WScript.Echo objSubFolder.Name & "," & objSubFolder.Size & "," & hr(objSubFolder.Size)
			If Err.Number <> 0 Then
				WScript.Echo objSubFolder.Name & ",-1,-1"
				Err.Clear
			End If
		Next
		On Error GoTo 0
		
		' カレントフォルダにあるファイルのサイズを出力
		Dim objFile, currentFileSize
		currentFileSize = 0
		For Each objFile In objFolder.Files
			currentFileSize = currentFileSize + objFile.Size
		Next
		WScript.Echo ". (" & WScript.Arguments.Item(i) & ")," & currentFileSize & "," & hr(currentFileSize)
	Next
End Sub

' メイン処理実行
DoMain
