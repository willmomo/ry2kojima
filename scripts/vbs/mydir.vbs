' mydir
'

'----------------------------------------------------------------------
' 範囲内チェック
'----------------------------------------------------------------------
Function InRange(minValue, Value, maxValue)
	InRnage = IIf(minValue <= Value And Value <= maxValue, True, False)
End Function

'----------------------------------------------------------------------
' Folder オブジェクト内のファイルを再帰的に表示
'----------------------------------------------------------------------
Sub ListingFiles(objFolder)
	On Error Resume Next

	Dim objSubFolder, objFile

	For Each objSubFolder In objFolder.SubFolders
		ListingFiles(objSubFolder)
	Next

	For Each objFile In objFolder.Files
		If objFile.Size > (1024 * 1024 * 1024 / 10) Then
			WScript.Echo CLng(objFile.Size / 1024 / 1024) & " MB " & objFile.Path
		End If
	Next
End Sub


Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

For i = 0 To WScript.Arguments.Count - 1
	ListingFiles objFS.GetFolder(WScript.Arguments(i))
Next
