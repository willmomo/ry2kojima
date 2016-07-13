

WScript.Quit(DoMain())


Function Logging(strMsg)
	Const ForAppending = 8
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objTs = objFs.OpenTextFile("ikatako.txt", ForAppending, True)
	objTs.WriteLine(strMsg)
	objTs.Close
End Function

Function CountChar(strText, strChar)
	CountChar = 0
	lngStart = 1
	Do
		lngPos = InStr(lngStart, strText, strChar)
		If lngPos > 0 Then
			CountChar = CountChar + 1
		End If
		lngStart = lngPos + 1
	Loop While lngPos > 0
End Function

Function Listing(strPath)
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objFolder = objFs.GetFolder(strPath)
	
	''WScript.Echo strPath, objFolder.Files.Count, objFolder.SubFolders.Count

	For Each objOne In objFolder.Files
	
		If Year(objOne.DateLastModified) = 2013 And _
		   Month(objOne.DateLastModified) = 5 And _
		   Day(objOne.DateLastModified) = 16 Then
		   
		   strExt = LCase(objFs.GetExtensionName(objOne.Name))
		   
			If strExt = "png" Or strExt = "jpg" Or strExt = "bmp" Then
'			If CountChar(objOne.Name, ".") > 1 Then
				WScript.Echo "OUT", objOne.DateLastModified, objOne.Size, objOne.Path
				Logging("OUT " & objOne.DateLastModified & " " & objOne.Path)
'			Else
'				WScript.Echo "   ", objOne.DateLastModified, objOne.Size, objOne.Path
'				Logging("    " & objOne.DateLastModified & " " & objOne.Path)
'			End If
			End If
		End If
	Next
	
	For Each objOne In objFolder.SubFolders
		Listing(objOne.Path)
	Next
End Function

Function DoMain()
	Listing("\\ts00\skshare")
End Function
