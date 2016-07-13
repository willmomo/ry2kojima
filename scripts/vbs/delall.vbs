Option Explicit

'----------------------------------------------------------------------
' �g�p�@��\��
'----------------------------------------------------------------------
Sub usage
	WScript.Echo "usage: delall �t�H���_�� [...]"
End Sub

'----------------------------------------------------------------------
' �w��t�H���_�ȉ����ċA�I�ɍ폜
'----------------------------------------------------------------------
Sub delall(objPath)
	Dim obj

	For Each obj In objPath.SubFolders
		delall obj
	Next

	For Each obj In objPath.Files
		WScript.Echo "[F] " & obj.Path
		obj.Delete
	Next
	
	WScript.Echo "[D] " & objPath.Path
	objPath.Delete
End Sub

'----------------------------------------------------------------------
' ���C��
'----------------------------------------------------------------------
Function doMain
	Dim objFs, arg

	doMain = -1
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	If LCase(objFs.GetFileName(WScript.FullName)) <> "cscript.exe" Then
		WScript.Echo "cscript �Ŏ��s���Ă��������B"
		Exit Function
	End If

	If WScript.Arguments.Count = 0 Then
		usage
		Exit Function
	End If

	For Each arg In WScript.Arguments
		delall objFs.GetFolder(arg)
	Next

	doMain = 0
End Function

'======================================================================

WScript.Quit doMain
