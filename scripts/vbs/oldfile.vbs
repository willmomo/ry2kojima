Dim objArg, objFS, objFolder, objSubFolder

Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

FileListing(objFS.GetFolder(WScript.Arguments.Item(0)))


Function FileListing(objFolder)
	Dim objSubFolder, objFile

	For Each objSubFolder In objFolder.SubFolders
		FileListing(objSubFolder)
	Next

	For Each objFile In objFolder.Files
		If Int((Now - objFile.DateLastModified) / 365) Then
			WScript.Echo objFile.Path & "," & objFile.DateLastModified & "," & objFile.DateLastAccessed
		End If
	Next
End Function
