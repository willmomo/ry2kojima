'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Option Explicit

Const MONTH_AGO = 2

WScript.Quit doMain

'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Function doMain()
	Dim objFso, objD
	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")

	'wscript�ł̎��s���K�[�h����
	If LCase(objFso.GetFileName(WScript.FullName)) = "wscript.exe" Then
		WScript.Echo "cscript�Ŏ��s���Ă��������B"
		doMain = -1
		Exit Function
	End If

	WScript.Echo "Delete the file that was created " & MONTH_AGO & " month(s) ago."
	
	'�t�@�C�����ꗗ����
	Call listing(WScript.Arguments(0))

	''' ����ۂ̃t�H���_������΁A������폜���ĉ��
	For Each objD In objFso.GetFolder(WScript.Arguments(0)).SubFolders
		Call deleteEmptyFolder(objD.Path)
	Next
End Function


'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Sub listing(path)
	Dim objFso
	Dim objDir
	Dim objF
	Dim objD

	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	Set objDir = objFso.GetFolder(path)
	For Each objF In objDir.Files
		'�쐬�����A�ꃖ���ȏ�Â��t�@�C�������o�͂���
		'''If objF.DateCreated < DateValue("2013/06/21") Then
		If objF.DateCreated < DateAdd("m", -MONTH_AGO, Now) Then
			WScript.Echo objF.DateCreated & vbTab & objFso.BuildPath(path, objF.Name)

			''' �Ώۃt�@�C�����폜���܂�!!
			objFso.DeleteFile objFso.BuildPath(path, objF.Name)
		End If
	Next
	For Each objD In objDir.SubFolders
		'''WScript.Echo objD.Path
		Call listing(objD.Path)
	Next
End Sub


'------------------------------------------------------------------------------
'
'------------------------------------------------------------------------------
Sub deleteEmptyFolder(path)
	Dim objFso
	Dim objDir
	Dim objF
	Dim objD

	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	Set objDir = objFso.GetFolder(path)

	For Each objD In objDir.SubFolders
		Call deleteEmptyFolder(objD.Path)
	Next
	If objDir.Files.Count = 0 And objDir.SubFolders.Count = 0 Then
		WScript.Echo "�폜 : " & path
		objDir.Delete
	End If
End Sub
