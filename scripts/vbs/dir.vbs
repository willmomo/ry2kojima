Option Explicit


WScript.Quit Main


Function Exec(cmdLine)
	Dim obj

	Set obj = WScript.CreateObject("WScript.Shell")
	Exec = obj.Run(cmdLine, 0, True)
End Function


Function FileCompare(f1, f2)
	Dim cmdLine

	cmdLine = "fc /b """ & f1 & """ """ & f2 & """"
	FileCompare = Exec(cmdLine)
End Function


Function Dir(objFolder)
	Dim fso, re, obj, maybeOriginal

	Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	Set re = New RegExp
	re.Pattern = "(.+) \(\d\)(.*)"

	For Each obj In objFolder.SubFolders
		If re.Test(obj.Name) And obj.SubFolders.Count = 0 Then
			WScript.Echo "delete: " & obj.Path
			obj.Delete
		Else
			Call Dir(obj)
		End If
	Next

	For Each obj In objFolder.Files
		If re.Test(obj.Name) Then
			maybeOriginal = fso.BuildPath(obj.ParentFolder, re.Replace(obj.Name, "$1$2"))
			If fso.FileExists(maybeOriginal) Then
				If FileCompare(maybeOriginal, obj.Path) = 0 Then
					WScript.Echo "delete: " & obj.Path
					obj.Delete
				End If
			End If
		End If
	Next
End Function


Function Main
	Dim objFs

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Call Dir(objFs.GetFolder(WScript.Arguments(0)))
End Function


