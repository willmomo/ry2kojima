Option Explicit

'----------------------------------------------------------------------
' �Â��t�@�C�����폜(days �͕����w��)
'----------------------------------------------------------------------
Function DeleteOldFiles(path, days)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	
	Dim f
	For Each f In fso.GetFolder(path).Files
		If (f.DateLastModified - Now) < days Then
			WScript.Echo f.DateLastModified, f.Path
		End If
	Next
End Function

'----------------------------------------------------------------------
' �Â��t�H���_���폜(days �͕����w��)
'----------------------------------------------------------------------
Function DeleteOldFolders(path, days)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	
	Dim f
	For Each f In fso.GetFolder(path).SubFolders
	WScript.Echo (f.DateLastModified - Now)
		If (f.DateLastModified - Now) < days Then
			WScript.Echo f.DateLastModified, f.Path
		End If
	Next
End Function

'======================================================================
DeleteOldFiles "C:\Users\kojima\Documents\temp", -90
DeleteOldFolders "C:\Users\kojima\Documents\temp", -30
