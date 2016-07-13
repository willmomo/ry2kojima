Dim objArg, objFS, objFolder, objSubFolder, objTS

Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

Set objFolder = objFS.GetFolder(WScript.Arguments.Item(0))

Set objTS = objFS.CreateTextFile("wdu.csv", True)

For Each objSubFolder In objFolder.SubFolders
	WScript.Echo objSubFolder.Name & "," & objSubFolder.Size
	objTS.WriteLine objSubFolder.Name & "," & objSubFolder.Size
Next

objTS.Close
