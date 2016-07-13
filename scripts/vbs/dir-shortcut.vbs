'----------------------------------------------------------------------
' �t�@�C�����ꗗ����X�N���v�g
' ���������邩�́A�A�i�^����B
' �K���ɏ��������Ă��������B
'----------------------------------------------------------------------
Option Explicit


'----------------------------------------------------------------------
' �悭�g���I�u�W�F�N�g�̐���
'----------------------------------------------------------------------
Dim g_objFS, g_objShell
Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objShell = WScript.CreateObject("WScript.Shell")


If ChkArg = False Then Usage	'�����`�F�b�N
Call Main						'���̊֐����N�_�ɏ������L�q���Ă���


'----------------------------------------------------------------------
' �g�p�@��\�����ďI������
'----------------------------------------------------------------------
Sub Usage()
	Dim strUsage
	
	strUsage = "�g�p�@�F" & WScript.ScriptName & " <�����t�H���_>..."
	
	WScript.Echo strUsage
	WScript.Quit
End Sub


'----------------------------------------------------------------------
' �����̃`�F�b�N
'
' �������s�K�؂ȏꍇ�́AFalse��Ԃ��悤�ɂ���B
'----------------------------------------------------------------------
Function ChkArg()
	If WScript.Arguments.Count = 0 Then
		ChkArg = False
		Exit Function
	End If
	
	ChkArg = True
End Function


'----------------------------------------------------------------------
' �t�@�C���ɑ΂��ĉ���������
'----------------------------------------------------------------------
Sub DoAnything(objFile)
	Dim objLnk
	Set objLnk = g_objShell.CreateShortcut(objFile.Path)

	If UCase(g_objFS.GetDriveName(objLnk.TargetPath)) = "\\HQPC208\�J��" Then
		WScript.Echo String(64, "-")
		WScript.Echo objFile.Path
		WScript.Echo "      TargetPath: " & objLnk.TargetPath
		WScript.Echo "WorkingDirectory: " & objLnk.WorkingDirectory
		''WScript.Echo "       DriveName: " & g_objFS.GetDriveName(objLnk.TargetPath)
	End If
	
	Set objLnk = Nothing
End Sub


'----------------------------------------------------------------------
' �t�@�C���ɑ΂��ăt�B���^��������
'----------------------------------------------------------------------
Function IsTarget(objFile)
	If UCase(g_objFS.GetExtensionName(objFile.Name)) = "LNK" Then
		IsTarget = True
	Else
		IsTarget = False
	End If
End Function


'----------------------------------------------------------------------
' �t�@�C�����ꗗ����
'----------------------------------------------------------------------
Function FileListing(objFolder)
	On Error Resume Next
	
	Dim objSubFolder, objFile

	' �ċA�I�Ƀt�H���_���~��Ă���
	For Each objSubFolder In objFolder.SubFolders
		If Err.Number = 0 Then
			FileListing(objSubFolder)
		Else
			WScript.Echo "Folder access error: " & Err.Description & ": " & objFolder
			Exit For
		ENd If
	Next

	' �t�H���_���̃t�@�C����񋓂��ď����o�͂���
	For Each objFile In objFolder.Files
		If Err.Number = 0 Then
			If IsTarget(objFile) Then
				Call DoAnything(objFile)
			End If
		Else
			WScript.Echo "File access error: " & Err.Description & ": " & objFolder
			Exit For
		ENd If
	Next
End Function


'----------------------------------------------------------------------
' ���C�����[�`��
'----------------------------------------------------------------------
Sub Main
	Dim i
	
	' �ꗗ�J�n!
	For i = 0 To WScript.Arguments.Count - 1
		FileListing(g_objFS.GetFolder(WScript.Arguments(i)))
	Next
End Sub

