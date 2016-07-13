'----------------------------------------------------------------------
' �w��t�H���_�̃t�@�C�������X�V�����Ƀ��l�[������
'----------------------------------------------------------------------
Option Explicit


Call Main	'�v���O�����X�^�[�g


'----------------------------------------------------------------------
' ���ׂẴt�@�C����񋓂���(�ċA�I�ł͂Ȃ�)
'----------------------------------------------------------------------
Sub RenAll(strSrc, strDst)
	Dim objFs, objCurDir, objSubDir, objFile, baseName, objDstDir
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objCurDir = objFs.GetFolder(strSrc)
	Set objDstDir = objFs.GetFolder(strDst)

	' �t�H���_���̃t�@�C����񋓂���
	For Each objFile In objCurDir.Files
		baseName = Year(objFile.DateLastModified) & Right("0" & Month(objFile.DateLastModified), 2) & Right("0" & Day(objFile.DateLastModified), 2) & "_" & Right("0" & Hour(objFile.DateLastModified), 2) & Right("0" & Minute(objFile.DateLastModified), 2) & Right("0" & Second(objFile.DateLastModified), 2)

		WScript.Echo objFile.Path & " �� " & objDstDir.Path & "\" & baseName & "." & objFs.GetExtensionName(objFile.Name)
		Call objFile.Copy(objDstDir.Path & "\" & baseName & "." & objFs.GetExtensionName(objFile.Name), False)
	Next
	
	Set objCurDir = Nothing
	Set objFs = Nothing
End Sub


'----------------------------------------------------------------------
' ���C�����[�`��
'----------------------------------------------------------------------
Sub Main
	Dim i
	
	Call RenAll(WScript.Arguments(0), WScript.Arguments(1))
End Sub
