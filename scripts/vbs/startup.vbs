'******************************************************************************
'*
'* �X�^�[�g�A�b�v�Ɋւ��郁���e�i���X�p�X�N���v�g
'*
'* created by : r.kojima in 2011/05/10
'* 2016/01/12 r.kojima : clr �R�}���h�ǉ�
'*
'* �g�p��:
'*   >startup.vbs dir
'*   >startup.vbs clr <alluser | curuser>
'*   >startup.vbs del WatchProc.lnk Win����.lnk �o�b�N�A�b�v.lnk �z�[���R���ʐM.lnk �ʐM�Ǘ�.lnk BackupTool.lnk �V�X�e���I�����j���[.lnk
'*   >startup.vbs mkl cur patcher C:\patcher\patcher.exe "-i c:\mpc2\patcher_mpc2.ini"
'*
'******************************************************************************
Option Explicit

	Dim i, strArguments, strStartupTarget

	'�����������Ƃ��́A�g�p�@��\�����ďI��
	If WScript.Arguments.Count = 0 Then
		Call Usage
	End If

	'��ڂ̈����́A�X�N���v�g�ւ̃R�}���h
	Select Case LCase(WScript.Arguments(0))
	Case "dir"
		WScript.Echo DirStartupFile

	Case "del"
		For i = 1 To WScript.Arguments.Count - 1
			DeleteStartupFile(WScript.Arguments(i))
		Next

	Case "clr"
		If WScript.Arguments.Count <> 2 Then
			Call Usage
		End If

		Call ClearStartupFile(WScript.Arguments(2))
		
	Case "mkl"
		If WScript.Arguments.Count < 4 Then
			Call Usage
		End If

		strStartupTarget = LCase(WScript.Arguments(1))
		If strStartupTarget <> "all" And strStartupTarget <> "cur" Then
			Call Usage
		End If

		If WScript.Arguments.Count >= 5 Then
			strArguments = WScript.Arguments(4)
		End If

		Call CreateStartupFile(strStartupTarget, WScript.Arguments(2), WScript.Arguments(3), strArguments)

	End Select

'******************************************************************************
'*
'* �g�p�@�̕\���ƏI��
'*
'******************************************************************************
Sub Usage()
	WScript.Echo _
		"�g�����F" & WScript.ScriptName & " [dir|del] [...]" & vbNewLine & _
		"dir" & vbNewLine & _
		"  �X�^�[�g�A�b�v�ɂ���t�@�C�����ꗗ�\������B" & vbNewLine & _
		vbNewLine & _
		"del <�t�@�C����> ..." & vbNewLine & _
		"  �w�肵���t�@�C�����X�^�[�g�A�b�v����폜����B" & vbNewLine & _
		vbNewLine & _
		"mkl <all|cur> <�V���[�g�J�b�g��> <���s�t�@�C����> [�R�}���h���C������]" & vbNewLine & _
		"  �w�肵���R�}���h���C���ւ̃V���[�g�J�b�g���쐬����B" & vbNewLine & _
		"  all ���w�肷��ƁAAll Users �Acur ���w�肷��ƃJ�����g���[�U�[�ɍ쐬�����B" & vbNewLine
	
	WScript.Quit -1
End Sub

'******************************************************************************
'*
'* �V���[�g�J�b�g���쐬����
'*
'******************************************************************************
Sub CreateShortcut(strPathName, strTargetPath, strWorkingDirectory, strArguments)
	Dim objShell, objShortcut
	Set objShell = WScript.CreateObject("WScript.Shell")
	
	Set objShortcut = objShell.CreateShortcut(strPathName)
	With objShortcut
		.TargetPath = strTargetPath
		.WorkingDirectory = strWorkingDirectory
		.Arguments = strArguments
		.Save
	End With
End Sub


'******************************************************************************
'*
'* �X�^�[�g�A�b�v�ɃV���[�g�J�b�g���쐬����
'*
'******************************************************************************
Sub CreateStartupFile(strStartupTarget, strShortcutName, strCommandLine, strArguments)
	Dim objShell, objFs, strShortcutPath
	
	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	
	If strStartupTarget = "all" Then
		strShortcutPath = objShell.SpecialFolders("AllUsersStartup")
	Else
		strShortcutPath = objShell.SpecialFolders("Startup")
	End If

	'�V���[�g�J�b�g���쐬
	Call CreateShortcut(strShortcutPath & "\" & strShortcutName & ".lnk", _
						strCommandLine, _
						objFs.GetParentFolderName(strCommandLine), _
						strArguments)
End Sub

'******************************************************************************
'*
'* All Users �̃X�^�[�g�A�b�v��Current User �̃X�^�[�g�A�b�v�̃f�B���N�g���ꗗ��\��
'*
'* [����]
'*
'* [�߂�l]
'*
'******************************************************************************
Function DirStartupFile()
	Dim objShell, objFs, strResult, objFile

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	For Each objFile In objFs.GetFolder(objShell.SpecialFolders("AllUsersStartup")).Files
		strResult = strResult & objFile.Path & vbNewLine
	Next

	For Each objFile In objFs.GetFolder(objShell.SpecialFolders("Startup")).Files
		strResult = strResult & objFile.Path & vbNewLine
	Next

	DirStartupFile = strResult
End Function

'******************************************************************************
'*
'* All Users �̃X�^�[�g�A�b�v��Current User �̃X�^�[�g�A�b�v����w��t�@�C�����폜����B
'*
'* [����]
'* �t�@�C�������w�肷��B
'*
'* [�߂�l]
'* 0 = �폜�t�@�C���Ȃ�
'* 1 = All Users �̃X�^�[�g�A�b�v����t�@�C�����폜
'* 2 = Current User �̃X�^�[�g�A�b�v����t�@�C�����폜
'* 3 = All Users �� Current User �̗����̃X�^�[�g�A�b�v����t�@�C�����폜
'*
'******************************************************************************
Function DeleteStartupFile(strFileName)
	Dim objShell, objFs, strTarget

	DeleteStartupFile = 0

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	strTarget = objShell.SpecialFolders("AllUsersStartup") & "\" & strFileName
	If objFs.FileExists(strTarget) Then
		Call objFs.DeleteFile(strTarget, True)
		DeleteStartupFile = 1
	End If

	strTarget = objShell.SpecialFolders("Startup") & "\" & strFileName
	If objFs.FileExists(strTarget) Then
		Call objFs.DeleteFile(strTarget, True)
		DeleteStartupFile = DeleteStartupFile + 2
	End If
End Function

'******************************************************************************
'*
'* All Users �̃X�^�[�g�A�b�v��Current User �̃X�^�[�g�A�b�v����ɂ���
'*
'* [����]
'* strMode = alluser or curuser
'*
'* [�߂�l]
'*
'******************************************************************************
Function ClearStartupFile(strMode)
	Dim objShell, objFs, strTarget

	DeleteStartupFile = 0

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	If LCase(strMode) = "alluser" Then
		strTarget = objShell.SpecialFolders("AllUsersStartup") & "\" & strFileName
	ElseIf LCase(strMode) = "curuser" Then
		strTarget = objShell.SpecialFolders("Startup") & "\" & strFileName
	Else
		Exit Function
	End If
	
	For Each objFile In objFs.GetFolder(strTarget).Files
		Call objFs.DeleteFile(objFile.Path)
	Next
End Function

