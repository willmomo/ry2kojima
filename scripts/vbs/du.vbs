
Set objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set objArgs = WScript.Arguments

For i = 0 To objArgs.Count - 1
	Set objFolder = objFS.GetFolder(objArgs(i))

	nTotal = objFolder.Size
	WScript.Echo nTotal & "," & objFolder.Path

	For Each objSubFolder In objFolder.SubFolders
		nSize = objSubFolder.Size
		WScript.Echo nSize & "," & objSubFolder.Path
		nTotal = nTotal - nSize
	Next

	WScript.Echo nTotal & "," & objFolder.Path & "\."
Next

