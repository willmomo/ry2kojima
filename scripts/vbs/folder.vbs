
Set objFS = WScript.CreateObject( "Scripting.FileSystemObject" )
Set objFolder = objFS.GetFolder( WScript.Arguments.Item(0) )

WScript.Echo "�t�H���_: " & objFolder.Path

folderSize = objFolder.Size
If folderSize > (1024 * 1024 * 1024) Then
	WScript.Echo "  �T�C�Y: " & (folderSize / (1024 * 1024 * 1024)) & " GB"
ElseIf folderSize > (1024 * 1024) Then
	WScript.Echo "  �T�C�Y: " & (folderSize / (1024 * 1024)) & " MB"
ElseIf folderSize > 1024 Then
	WScript.Echo "  �T�C�Y: " & (folderSize / 1024) & " KB"
Else
	WScript.Echo "  �T�C�Y: " & folderSize & " B"
End If
