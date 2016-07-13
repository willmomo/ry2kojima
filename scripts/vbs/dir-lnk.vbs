'----------------------------------------------------------------------
' �w��t�H���_����A*.lnk ��񋓂���
'----------------------------------------------------------------------
Option Explicit


Call Main	' �v���O�����X�^�[�g


'----------------------------------------------------------------------
' .lnk �t�@�C���̏�����������
'----------------------------------------------------------------------
Sub CheckLnkFile(objFile)
	Dim objFs, objShell, objLnk, strPath
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objLnk = objShell.CreateShortcut(objFile.Path)

	strPath = UCase("\\ts00\share\�c��\_�o�׃\�t�g�Q_20040531����^�p�J�n")
	
'	If UCase(Left(objLnk.TargetPath, Len(strPath))) = strPath Then
		WScript.Echo String(64, "-")
		WScript.Echo objFile.Path
		WScript.Echo "      TargetPath: " & objLnk.TargetPath
		WScript.Echo "WorkingDirectory: " & objLnk.WorkingDirectory
'	End If
	
	Set objLnk = Nothing
	Set objShell = Nothing
	Set objFs = Nothing
End Sub


'----------------------------------------------------------------------
' *.lnk ��񋓂���
'----------------------------------------------------------------------
Sub FileListing(strDir)
	Dim objFs, objCurDir, objSubDir, objFile
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objCurDir = objFs.GetFolder(strDir)
	
	' �t�H���_���̃t�@�C����񋓂���
	For Each objFile In objCurDir.Files
		If UCase(objFS.GetExtensionName(objFile.Name)) = "LNK" Then
			Call CheckLnkFile(objFile)
		End If
	Next

	' �ċA�I�Ƀt�H���_������Ă���
	On Error Resume Next
	For Each objSubDir In objCurDir.SubFolders
		If Err.Number = 0 Then
			Call FileListing(objSubDir.Path)
		Else
			WScript.Echo strDir & " �̃T�u�t�H���_�񋓂ŃG���[����: " & Err.Description
			Err.Clear
			Exit Sub
		End If
	Next
	
	Set objCurDir = Nothing
	Set objFs = Nothing
End Sub


'----------------------------------------------------------------------
' ���C�����[�`��
'----------------------------------------------------------------------
Sub Main
	Dim i
	
	' �ꗗ�J�n
	For i = 0 To WScript.Arguments.Count - 1
		Call FileListing(WScript.Arguments(i))
	Next
End Sub
