'------------------------------------------------------------------------------
' ���̃X�N���v�g�����݂���t�H���_�ɁAFREEDOM �� PRO2500 �̃~���[�����O������
' �X�N���v�g�B
'
' �ق��ł��g���₷���Ȃ�悤�ɁA������x�̔ėp�����������Ă���B
'------------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------------------------
' ���C������
'------------------------------------------------------------------------------
Sub doMain_FREEDOM()
	Dim objMirror, objFs, objNet, orgPath, orgDirs, objTs, strExcludeFile

	Set objNet = WScript.CreateObject("WScript.Network")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objMirror = New mirrorExe

	strExcludeFile = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName) & ".txt"
	Set objTs = objFs.CreateTextFile(strExcludeFile, True)
	objTs.WriteLine("\.dropbox.cache")
	objTs.WriteLine("\RECYCLER")
	objTs.WriteLine("\System Volume Information")
	objTs.Close
	
	objMirror.Mode = "backup-mirror"
	objMirror.LogPath = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName)
	objMirror.ExcludeOption = strExcludeFile
	
	orgDirs = Array("D:\", "E:\")

	For Each orgPath In orgDirs
		objMirror.OrgPath = orgPath
		objMirror.MirPath = objFs.BuildPath(objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), objNet.ComputerName), Replace(objMirror.OrgPath, ":", "-drv"))
		objMirror.Run
	Next
End Sub

'------------------------------------------------------------------------------
' ���C������
'------------------------------------------------------------------------------
Sub doMain_PRO2500()
	Dim objMirror, objFs, objNet, orgPath, orgDirs, objTs, strExcludeFile

	Set objNet = WScript.CreateObject("WScript.Network")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objMirror = New mirrorExe

	strExcludeFile = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName) & ".txt"
	Set objTs = objFs.CreateTextFile(strExcludeFile, True)
	objTs.WriteLine("\.dropbox.cache")
	objTs.WriteLine("\RECYCLER")
	objTs.WriteLine("\System Volume Information")
	objTs.Close

	objMirror.Mode = "backup-mirror"
	objMirror.LogPath = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName)
	objMirror.ExcludeOption = strExcludeFile
	
	orgDirs = Array("C:\fswiki", "D:\Users")

	For Each orgPath In orgDirs
		objMirror.OrgPath = orgPath
		objMirror.MirPath = objFs.BuildPath(objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), objNet.ComputerName), Replace(objMirror.OrgPath, ":", "-drv"))
		objMirror.Run
	Next
	
	objMirror.OrgPath = "\\LANDISK-B7C184\disk"
	objMirror.MirPath = objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), "LANDISK-B7C184\disk")
	objMirror.Run
End Sub

'------------------------------------------------------------------------------
' ���C������
'------------------------------------------------------------------------------
Sub doMain()
	Dim objNet

	Set objNet = WScript.CreateObject("WScript.Network")
	
	If objNet.ComputerName = "FREEDOM" Then
		doMain_FREEDOM
	ElseIf objNet.ComputerName = "PRO2500" Then
		doMain_PRO2500
	Else
		MsgBox objNet.ComputerName & " �́A���Ή���PC�ł��B", vbOKOnly Or vbExclamation, WScript.ScriptName
	End If
End Sub

'------------------------------------------------------------------------------

Call doMain()


'==============================================================================
'	�������� kjmlib.vbs ����̃R�s�[
'==============================================================================
'------------------------------------------------------------------------------
' mirror.exe ���b�p�[�N���X
'
' update: 2011/07/26
'------------------------------------------------------------------------------
Class mirrorExe
	'--------------------------------------------------------------------------
	' �R�s�[���t�H���_�̐ݒ�Ǝ擾
	Private m_orgPath
	
	Public Property Let OrgPath(newValue)
		m_orgPath = newValue
	End Property
	
	Public Property Get OrgPath()
		OrgPath = m_orgPath
	End Property

	'--------------------------------------------------------------------------
	' �R�s�[��t�H���_�̐ݒ�Ǝ擾
	Private m_mirPath
	
	Public Property Let MirPath(newValue)
		m_mirPath = newValue
	End Property
	
	Public Property Get MirPath()
		MirPath = m_mirPath
	End Property

	'--------------------------------------------------------------------------
	' --LogPath �I�v�V�����̐ݒ�Ǝ擾
	Private m_logPath
	
	Public Property Let LogPath(newValue)
		m_logPath = newValue
	End Property
	
	Public Property Get LogPath()
		LogPath = m_logPath
	End Property
	
	'--------------------------------------------------------------------------
	' --exclude �I�v�V�����̐ݒ�Ǝ擾
	Private m_excludeOpt

	Public Property Let ExcludeOption(newValue)
		m_excludeOpt = newValue
	End Property

	Public Property Get ExcludeOption()
		ExcludeOption = m_excludeOpt
	End Property

	'--------------------------------------------------------------------------
	' --mode �I�v�V�����̐ݒ�Ǝ擾
	Private m_mode

	Public Property Let Mode(newValue)
		m_mode = newValue
	End Property

	Public Property Get Mode()
		Mode = m_mode
	End Property

	'--------------------------------------------------------------------------
	' --test �I�v�V�����̐ݒ�Ǝ擾
	Private m_test

	Public Property Let Test(newValue)
		m_test = newValue
	End Property

	Public Property Get Test()
		Test = m_test
	End Property

	'--------------------------------------------------------------------------
	' mirror.exe �̎��s�B
	'
	Public Sub Run()
		Dim strCmd
		
		' ���s�t�@�C�����̎w��A�p�X���ʂ��Ă�����̂Ƃ���B
		strCmd = "mirror.exe"
		
		' �R�s�[���t�H���_�̎w��
		If InStr(OrgPath, " ") > 0 Then
			strCmd = strCmd & " """ & OrgPath & """"
		Else
			strCmd = strCmd & " " & OrgPath
		End If
		
		' �R�s�[��t�H���_�̎w��
		If InStr(MirPath, " ") > 0 Then
			strCmd = strCmd & " """ & MirPath & """"
		Else
			strCmd = strCmd & " " & MirPath
		End If

		' --exclude �I�v�V�����̎w��
		If ExcludeOption <> "" Then
			If InStr(ExcludeOption, " ") > 0 Then
				strCmd = strCmd & " --exclude=""" & ExcludeOption & """"
			Else
				strCmd = strCmd & " --exclude=" & ExcludeOption
			End If
		End If

		' --LogPath �I�v�V�����̎w��
		If LogPath <> "" Then
			If InStr(LogPath, " ") > 0 Then
				strCmd = strCmd & " --logpath=""" & LogPath & """"
			Else
				strCmd = strCmd & " --logpath=" & LogPath
			End If
		End If

		' --mode �I�v�V�����̎w��
		If Mode <> "" Then strCmd = strCmd & " --mode=" & Mode

		' --test �I�v�V�����̎w��
		If Test <> False Then strCmd = strCmd & " --test"
		
		''WScript.Echo strCmd

		' �E�B���h�E���ŐV�̃T�C�Y�ƈʒu�ŕ\�����܂��B�A�N�e�B�u�ȃE�B���h�E�ɂȂ�܂��B
		''Call WScript.CreateObject("WScript.Shell").Run(strCmd, 1, True)
		
		' �E�B���h�E���ŐV�̃T�C�Y�ƈʒu�ŕ\�����܂��B�A�N�e�B�u�ȃE�B���h�E�͐؂�ւ��܂���B
		Call WScript.CreateObject("WScript.Shell").Run(strCmd, 4, True)
		
		' �E�B���h�E���ŏ����E�B���h�E�Ƃ��ĕ\�����܂��B�A�N�e�B�u�ȃE�B���h�E�͐؂�ւ��܂���B
		''Call WScript.CreateObject("WScript.Shell").Run(strCmd, 7, True)
	End Sub

	'--------------------------------------------------------------------------
	' �N���X�̃C�j�V�����C�Y
	'
	Private Sub Class_Initialize()
		m_test = False
	End Sub
End Class

'==============================================================================
'	�����܂� kjmlib.vbs ����̃R�s�[
'==============================================================================
