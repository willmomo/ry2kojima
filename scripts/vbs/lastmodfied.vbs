
' �T�u�t�H���_�̃T�C�Y�����X�g�A�b�v����B
'

Option Explicit

' cscript�œ�������Ă��邩�ǂ����A�`�F�b�N����
'
Sub CheckRunOnCScript()

	' FileSystemObject���쐬����
	Dim objFS
	Set objFS = WScript.CreateObject("Scripting.FileSystemObject")
	
	' cscript�ȊO�œ����Ă���Ƃ�
	If LCase(objFS.GetFileName(WScript.FullName)) <> "cscript.exe" Then
	
		' �G���[���b�Z�[�W��\��
		WScript.Echo "cscript ���g���Ď��s���Ă��������B" & vbNewLine & _
					 "��) cscript """ & WScript.ScriptFullName & """"
					 
		' �I��
		WScript.Quit(1)
	End If
End Sub

' �I�v�V�����𒲂ׂ�
' �ُ�̎��́A�w���v��\�����ďI��
'
Sub CheckArgument()
	If WScript.Arguments.Count = 0 Then
		WScript.Echo "�g�p�@�Fwdu �t�H���_��"
		WScript.Quit(1)
	End If
End Sub

' �t�H���_�̒��ōX�V���t���A�����Ƃ��V����File�I�u�W�F�N�g��Ԃ�
'
Function LatestLastModified(folderName)
	Dim objLatestFile, objTempFile
	Set objLatestFile = Nothing

	' FileSystemObject���쐬
	Dim objFS
	Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

	' Folder�I�u�W�F�N�g���쐬
	Dim objFolder
	Set objFolder = objFS.GetFolder(folderName)

	' �T�u�t�H���_�̒�����A�����Ƃ��X�V���t�̐V����File�I�u�W�F�N�g��T��
	Dim objSubFolder
	For Each objSubFolder In objFolder.SubFolders
		Set objTempFile = LatestLastModified(objSubFolder.Path)
		If objLatestFile Is Nothing Then
			Set objLatestFile = objTempFile
		ElseIf objTempFile Is Nothing Then
			' �������Ȃ�
		ElseIf objLatestFile.DateLastModified < objTempFile.DateLastModified Then
			Set objLatestFile = objTempFile
		End If
	Next

	' �t�H���_�����ɂ���t�@�C���̒�����A�����Ƃ��X�V���t�̐V����File�I�u�W�F�N�g��T��
	Dim objFile
	For Each objFile In objFolder.Files
		If objLatestFile Is Nothing Then
			Set objLatestFile = objFile
		ElseIf objLatestFile.DateLastModified < objFile.DateLastModified Then
			Set objLatestFile = objFile
		End If
	Next

	' �ł��X�V���t�̐V�����t�@�C���I�u�W�F�N�g��Ԃ�
	Set LatestLastModified = objLatestFile
End Function

' ���C������
'
Sub DoMain()
	' cscript�œ����Ă��邩�ǂ������ׂ�
	CheckRunOnCScript

	' �I�v�V�����𒲂ׂ�
	CheckArgument

	' FileSystemObject���쐬
	Dim obJFS
	Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

	' �����Ŏw�肳�ꂽ�t�H���_�����ׂă��X�g�A�b�v
	Dim i, objLatestFile
	For i = 0 To WScript.Arguments.Count - 1
		
		' �����Ŏw�肳�ꂽ�t�H���_���̃T�u�t�H���_�̃T�C�Y�����X�g�A�b�v
		Dim objFolder, objSubFolder
		Set objFolder = objFS.GetFolder(WScript.Arguments.Item(i))
		For Each objSubFolder In objFolder.SubFolders
			Set objLatestFile = LatestLastModified(objSubFolder.Path)
			If Not (objLatestFile Is Nothing) Then
				WScript.Echo objLatestFile.DateLastModified & "," & objLatestFile.Path
			End If
		Next
	Next
End Sub

' ���C���������s
DoMain
