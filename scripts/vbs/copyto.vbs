Option Explicit

'���s�T���v��
Call CopyTo("J:\drvimg", "F:\drvimg")

'----------------------------------------------------------------------
' �����t�H���_ src ���� dst �Ƀt�@�C�����������ێ����Ȃ���R�s�[����
'
' 2015/09/11 r.kojima
'----------------------------------------------------------------------
Function CopyTo(srcPath, dstPath)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim dir: Set dir = fso.GetFolder(srcPath)
	Dim obj, dst
	'�T�u�t�H���_��񋓂���ɃR�s�[����
	For Each obj In dir.SubFolders
		Call CopyTo(obj.Path, fso.BuildPath(dstPath, obj.Name))
	Next
	'�t�@�C����񋓂��R�s�[����
	For Each obj In dir.Files
		dst = fso.BuildPath(dstPath, obj.Name)
		If Not fso.FileExists(dst) Then
			WScript.Echo "�R�s�[���Ȃ���"
			WScript.Echo "��: " & obj.Path
			WScript.Echo "��: " & dst
		End If
	Next
End Function
