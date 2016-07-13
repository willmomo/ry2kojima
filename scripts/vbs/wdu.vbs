
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

Function hr(nValue)
	If nValue < (1024) Then
		hr = nValue & " B "
	ElseIf nValue < (1024 * 1024) Then
		hr = (nValue / 1024) & " KB"
	ElseIf nValue < (1024 * 1024 * 1024) Then
		hr = (nValue / 1024 / 1024) & " MB"
	ElseIf nValue < (1024 * 1024 * 1024 * 1024) Then
		hr = (nValue / 1024 / 1024 / 1024) & " GB"
	Else
		hr = (nValue / 1024 / 1024 / 1024 / 1024) & " TB"
	End If
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
	Dim i
	For i = 0 To WScript.Arguments.Count - 1
		
		' �����Ŏw�肳�ꂽ�t�H���_���̃T�u�t�H���_�̃T�C�Y�����X�g�A�b�v
		Dim objFolder, objSubFolder
		Set objFolder = objFS.GetFolder(WScript.Arguments.Item(i))
		On Error Resume Next
		For Each objSubFolder In objFolder.SubFolders
			WScript.Echo objSubFolder.Name & "," & objSubFolder.Size & "," & hr(objSubFolder.Size)
			If Err.Number <> 0 Then
				WScript.Echo objSubFolder.Name & ",-1,-1"
				Err.Clear
			End If
		Next
		On Error GoTo 0
		
		' �J�����g�t�H���_�ɂ���t�@�C���̃T�C�Y���o��
		Dim objFile, currentFileSize
		currentFileSize = 0
		For Each objFile In objFolder.Files
			currentFileSize = currentFileSize + objFile.Size
		Next
		WScript.Echo ". (" & WScript.Arguments.Item(i) & ")," & currentFileSize & "," & hr(currentFileSize)
	Next
End Sub

' ���C���������s
DoMain
