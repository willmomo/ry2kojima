Option Explicit
'------------------------------------------------------------------------------
' VBScript�p��code snipet
'------------------------------------------------------------------------------

'----------------------------------------------------------------------
' �e���|�����t�H���_���擾
'----------------------------------------------------------------------
Function Path_GetTempPath()
	Const TemporaryFolder = 2	'GetSpecialFolder ���\�b�h�A���� option �̂��߂̕ϐ�
	Dim objFs
	Set objFs = CreateObject("Scripting.FileSystemObject")
	Path_GetTempPath = objFs.GetSpecialFolder(TemporaryFolder)
End Function


'----------------------------------------------------------------------
' �t�@�C�������݂�����폜����
'----------------------------------------------------------------------
Sub DeleteFileIfExists(strFileName)
	Dim objFs
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	
	If objFs.FileExists(strFileName) Then
		objFs.DeleteFile(strFileName)
	End If

	Set objFs = Nothing
End Sub

'------------------------------------------------------------------------------
' ���O(%TEMP%\<script name>.log)�ɏo��
'------------------------------------------------------------------------------
Sub LogWrite(msg)
	Const ForAppending = 8		'OpenTextFile ���\�b�h�A���� create �̂��߂̕ϐ�

	Dim objFs
	Dim objTs

	Set objFs = CreateObject("Scripting.FileSystemObject")
	Set objTs = objFs.OpenTextFile(objFs.BuildPath(Path_GetTempPath(), objFs.GetBaseName(WScript.ScriptName) & ".log"), ForAppending, True)
	objTs.WriteLine("[" & Now & "] " & msg)
	objTs.Close
End Sub

'----------------------------------------------------------------------
' ���O�ɏ�������
'----------------------------------------------------------------------
Sub WriteLog(msg)
	Const ForAppending = 8
	Dim objFs
	Dim objTs

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objTs = objFs.OpenTextFile(objFs.BuildPath(Path_GetTempPath, "�O����.log"), ForAppending, True)
	objTs.WriteLine(msg)
	objTs.Close
End Sub


'----------------------------------------------------------------------
' ���O�̍ŏI�s���擾
'----------------------------------------------------------------------
Function TailLog()
	Const ForReading = 1
	Dim objFs
	Dim objTs

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objTs = g_objFs.OpenTextFile(objFs.BuildPath(Path_GetTempPath, "�O����.log"), ForReading, True)
	Do Until objTs.AtEndOfStream
		TailLog = objTs.ReadLine
	Loop
	objTs.Close
End Function

'------------------------------------------------------------------------------
' �J�����g�f�B���N�g�����擾/�ݒ肷��
'------------------------------------------------------------------------------
Function CurrentDirectory(strCurrentDirectory)
	Dim objShell

	Set objShell = WScript.CreateObject("WScript.Shell")
	If strCurrentDirectory <> "" Then
		objShell.CurrentDirectory = strCurrentDirectory
	End If
	CurrentDirectory = objShell.CurrentDirectory
	Set objShell = Nothing
End Function

'------------------------------------------------------------------------------
' �R�}���h�����s����
'------------------------------------------------------------------------------
Function Run(strCommand, intWindowStyle, bWaitOnReturn)
	Dim objShell

	Set objShell = WScript.CreateObject("WScript.Shell")
	Run = objShell.Run(strCommand, intWindowStyle, bWaitOnReturn)
	Set objShell = Nothing
End Function

'------------------------------------------------------------------------------
' stop service
'------------------------------------------------------------------------------
Function ServiceStop(strServiceName)
	Dim serviceSet
	Dim service

	Set serviceSet = GetObject("winmgmts:").ExecQuery("select * from Win32_Service where Name='" & strServiceName & "'")
	
	For Each service In serviceSet
		service.StopService()
	Next
	
	Set serviceSet = Nothing
End Function

'------------------------------------------------------------------------------
' start service
'------------------------------------------------------------------------------
Function ServiceStart(strServiceName)
	Dim serviceSet
	Dim service

	Set serviceSet = GetObject("winmgmts:").ExecQuery("select * from Win32_Service where Name='" & strServiceName & "'")
	
	For Each service In serviceSet
		service.StartService()
	Next
	
	Set serviceSet = Nothing
End Function

'----------------------------------------------------------------------
' �t�H���_���쐬����
'----------------------------------------------------------------------
Function CreateFolder(folderspec)
	Dim objFs
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	
	''WScript.Echo folderspec
	
	'���s�ŏ�����
	CreateFolder = False
	
	If folderspec = "" Then
		CreateFolder = True
		Exit Function
	End If
	
	If objFs.FolderExists(folderspec) Then
		'���łɃt�H���_�����݂���Ƃ��́A����I������
		CreateFolder = True
	Else
		'�e�t�H���_�̍쐬�����݂Ă���
		If CreateFolder(objFs.GetParentFolderName(folderspec)) Then
			'�ړI�̃t�H���_���쐬����
			objFs.CreateFolder(folderspec)
			CreateFolder = True
		End If
	End If
End Function

'------------------------------------------------------------------------------
' Windows 2000 : 5
' Windows XP : 5.1
'------------------------------------------------------------------------------
Function GetOSVersion()
	Dim colTarget,objRow,str,aData,nData, I, nTarget

	Set colTarget = GetObject("winmgmts:\\.\root\cimv2").ExecQuery("select * from Win32_OperatingSystem")
	For Each objRow in colTarget
		str = objRow.Version
	Next
	
	aData = Split( str, "." )
	For I = 0 to Ubound( aData )
		if I > 1 then
			Exit For
		end if
		if I > 0 then
			nTarget = nTarget & "."
		end if
		nTarget = nTarget & aData(I)
	Next
	GetOSVersion = CDbl(nTarget)
End Function

Call ShowOSInfo
WScript.Quit

'------------------------------------------------------------------------------
'------------------------------------------------------------------------------
Public Sub ShowOSInfo()
	Dim objRow, strMsg, i
	
	For Each objRow In GetObject("winmgmts:\\.\root\cimv2").ExecQuery("select * from Win32_OperatingSystem")
		strMsg = strMsg & "BootDevice:  " & objRow.BootDevice & vbNewLine
		strMsg = strMsg & "BuildNumber: " & objRow.BuildNumber & vbNewLine
		strMsg = strMsg & "BuildType:   " & objRow.BuildType & vbNewLine
		strMsg = strMsg & "Caption:     " & objRow.Caption & vbNewLine
		strMsg = strMsg & "CodeSet:     " & objRow.CodeSet & vbNewLine
		strMsg = strMsg & "CountryCode: " & objRow.CountryCode & vbNewLine
		strMsg = strMsg & "CreationClassName: " & objRow.CreationClassName & vbNewLine
		strMsg = strMsg & "CSCreationClassName: " & objRow.CSCreationClassName & vbNewLine
		strMsg = strMsg & "CSDVersion: " & objRow.CSDVersion & vbNewLine
		strMsg = strMsg & "CSName: " & objRow.CSName & vbNewLine
		strMsg = strMsg & "CurrentTimeZone: " & objRow.CurrentTimeZone & vbNewLine
		strMsg = strMsg & "DataExecutionPrevention_Available: " & objRow.DataExecutionPrevention_Available & vbNewLine
		strMsg = strMsg & "DataExecutionPrevention_32BitApplications: " & objRow.DataExecutionPrevention_32BitApplications & vbNewLine
		strMsg = strMsg & "DataExecutionPrevention_Drivers: " & objRow.DataExecutionPrevention_Drivers & vbNewLine
		strMsg = strMsg & "DataExecutionPrevention_SupportPolicy: " & objRow.DataExecutionPrevention_SupportPolicy & vbNewLine
		strMsg = strMsg & "Debug: " & objRow.Debug & vbNewLine
		strMsg = strMsg & "Description: " & objRow.Description & vbNewLine
		strMsg = strMsg & "Distributed: " & objRow.Distributed & vbNewLine
		strMsg = strMsg & "EncryptionLevel: " & objRow.EncryptionLevel & vbNewLine
		strMsg = strMsg & "ForegroundApplicationBoost: " & objRow.ForegroundApplicationBoost & vbNewLine
		strMsg = strMsg & "FreePhysicalMemory: " & objRow.FreePhysicalMemory & vbNewLine
		strMsg = strMsg & "FreeSpaceInPagingFiles: " & objRow.FreeSpaceInPagingFiles & vbNewLine
		strMsg = strMsg & "FreeVirtualMemory: " & objRow.FreeVirtualMemory & vbNewLine
		strMsg = strMsg & "InstallDate: " & objRow.InstallDate & vbNewLine
		strMsg = strMsg & "LargeSystemCache: " & objRow.LargeSystemCache & vbNewLine
		strMsg = strMsg & "LastBootUpTime: " & objRow.LastBootUpTime & vbNewLine
		strMsg = strMsg & "LocalDateTime: " & objRow.LocalDateTime & vbNewLine
		strMsg = strMsg & "Locale: " & objRow.Locale & vbNewLine
		strMsg = strMsg & "Manufacturer: " & objRow.Manufacturer & vbNewLine
		strMsg = strMsg & "MaxNumberOfProcesses: " & objRow.MaxNumberOfProcesses & vbNewLine
		strMsg = strMsg & "MaxProcessMemorySize: " & objRow.MaxProcessMemorySize & vbNewLine
''		For i = 0 To UBound(objRow.MUILanguages)
''			strMsg = strMsg & "MUILanguages: " & objRow.MUILanguages(i) & vbNewLine
''		Next
		strMsg = strMsg & "Name: " & objRow.Name & vbNewLine
		strMsg = strMsg & "NumberOfLicensedUsers: " & objRow.NumberOfLicensedUsers & vbNewLine
		strMsg = strMsg & "NumberOfProcesses: " & objRow.NumberOfProcesses & vbNewLine
		strMsg = strMsg & "NumberOfUsers: " & objRow.NumberOfUsers & vbNewLine
''		strMsg = strMsg & "OperatingSystemSKU: " & objRow.OperatingSystemSKU & vbNewLine
		strMsg = strMsg & "Organization: " & objRow.Organization & vbNewLine
''		strMsg = strMsg & "OSArchitecture: " & objRow.OSArchitecture & vbNewLine
		strMsg = strMsg & "OSLanguage: " & objRow.OSLanguage & vbNewLine
		strMsg = strMsg & "OSProductSuite: " & objRow.OSProductSuite & vbNewLine
		strMsg = strMsg & "OSType: " & objRow.OSType & vbNewLine
		strMsg = strMsg & "OtherTypeDescription: " & objRow.OtherTypeDescription & vbNewLine
''		strMsg = strMsg & "PAEEnabled: " & objRow.PAEEnabled & vbNewLine
		strMsg = strMsg & "PlusProductID: " & objRow.PlusProductID & vbNewLine
		strMsg = strMsg & "PlusVersionNumber: " & objRow.PlusVersionNumber & vbNewLine
		strMsg = strMsg & "Primary: " & objRow.Primary & vbNewLine
		strMsg = strMsg & "ProductType: " & objRow.ProductType & vbNewLine
		strMsg = strMsg & "RegisteredUser: " & objRow.RegisteredUser & vbNewLine
		strMsg = strMsg & "SerialNumber: " & objRow.SerialNumber & vbNewLine
		strMsg = strMsg & "ServicePackMajorVersion: " & objRow.ServicePackMajorVersion & vbNewLine
		strMsg = strMsg & "ServicePackMinorVersion: " & objRow.ServicePackMinorVersion & vbNewLine
		strMsg = strMsg & "SizeStoredInPagingFiles: " & objRow.SizeStoredInPagingFiles & vbNewLine
		strMsg = strMsg & "Status: " & objRow.Status & vbNewLine
		strMsg = strMsg & "SuiteMask: " & objRow.SuiteMask & vbNewLine
		strMsg = strMsg & "SystemDevice: " & objRow.SystemDevice & vbNewLine
		strMsg = strMsg & "SystemDirectory: " & objRow.SystemDirectory & vbNewLine
		strMsg = strMsg & "SystemDrive: " & objRow.SystemDrive & vbNewLine
		strMsg = strMsg & "TotalSwapSpaceSize: " & objRow.TotalSwapSpaceSize & vbNewLine
		strMsg = strMsg & "TotalVirtualMemorySize: " & objRow.TotalVirtualMemorySize & vbNewLine
		strMsg = strMsg & "TotalVisibleMemorySize: " & objRow.TotalVisibleMemorySize & vbNewLine
		strMsg = strMsg & "Version: " & objRow.Version & vbNewLine
		strMsg = strMsg & "WindowsDirectory: " & objRow.WindowsDirectory & vbNewLine
	Next
	
	WScript.Echo strMsg
End Sub

'------------------------------------------------------------------------------
' WScript.Shell.Run
'
' last modified: 2011/07/25
'------------------------------------------------------------------------------
Public Function kjmShellRun(strCommand, intWindowStyle, bWaitOnReturn)
	Dim obj
	
	Set obj = WScript.CreateObject("WScript.Shell")
	kjmShellRun = obj.Run(strCommand, intWindowStyle, bWaitOnReturn)
End Function

' intWinodwStyle �p�萔
Public Const SW_HIDE = 0	' �E�B���h�E���\���ɂ��A�ʂ̃E�B���h�E���A�N�e�B�u�ɂ��܂��B

'------------------------------------------------------------------------------
' WScript.Shell.Exec
'
' last modified: 2011/07/25
'------------------------------------------------------------------------------
Public Function kjmShellExec(strCommand)
	Dim obj, objExec
	
	Set obj = WScript.CreateObject("WScript.Shell")
	Set objExec = obj.Exec(strCommand)
	
	Do While objExec.Status = 0
		WScript.Sleep 100
	Loop
	
	kjmShellExec = objExec.Status
End Function

'----------------------------------------------------------------------
' �t�@�C�������݂�����ړ�����
'
' �X�N���v�g�ɃR�s�[���Ďg�����A
' �ȉ��̈�s��ǉ����āA�^���I�� #include "MoveFileIfExists.vbs" ����������B
' ExecuteGlobal WScript.CreateObject("Scripting.FileSystemObject").OpenTextFile("MoveFileIfExists.vbs").ReadAll
'----------------------------------------------------------------------
Sub MoveFileIfExists(strExistingFileName, strNewFileName)
	Dim objFs
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	
	If objFs.FileExists(strExistingFileName) Then
		objFs.MoveFile strExistingFileName, strNewFileName
	End If

	Set objFs = Nothing
End Sub

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
		ExcludeOption = m_excludeOption
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
		If ExcludeOpt <> "" Then
			If InStr(ExcludeOpt, " ") > 0 Then
				strCmd = strCmd & " --exclude=""" & ExcludeOpt & """"
			Else
				strCmd = strCmd & " --exclude=" & ExcludeOpt
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

'----------------------------------------------------------------------
' NTBACKUP�̃��b�p�[�N���X
'
' [�g����]
' Dim objNTBackup
' Set objNTBackup = New CNTBackup
' objNTBackup.BackupTarget = "D:\work"
' objNTBackup.BackupFile = "\\nas\share\_backup\hoge.bkf"
' objNTBackup.Mode = "normal"
' objNTBackup.Description = "WORK�̃o�b�N�A�b�v(" & Now & ")"
' objNTBackup.Run
'----------------------------------------------------------------------
Class CNTBackup
	Private m_strBackupTarget
	Private m_strBackupFile
	Private m_strMode
	Private m_strDescription
	
	Public Property Let BackupTarget(newValue)
		m_strBackupTarget = newValue
	End Property
	
	Public Property Get BackupTarget()
		BackupTarget = m_strBackupTarget
	End Property
	
	Public Property Let BackupFile(newValue)
		m_strBackupFile = newValue
	End Property
	
	Public Property Get BackupFile()
		BackupFile = m_strBackupFile
	End Property
	
	Public Property Let Mode(newValue)
		m_strMode = newValue
	End Property
	
	Public Property Get Mode()
		Mode = m_strMode
	End Property
	
	Public Property Let Description(newValue)
		m_strDescription = newValue
	End Property
	
	Public Sub Run()
		Dim strCmd, objShell
		
		strCmd = "ntbackup backup"
		strCmd = strCmd & " " & m_strBackupTarget
		strCmd = strCmd & " /F " & m_strBackupFile
		strCmd = strCmd & " /M " & m_strMode
		strCmd = strCmd & " /D """ & m_strDescription & """"

		Set objShell = WScript.CreateObject("WScript.Shell")
		Call objShell.Run(strCmd, 1, True)
	End Sub
End Class

'----------------------------------------------------------------------
' XCOPY ���b�p�[�N���X
'----------------------------------------------------------------------
Class CXCopy
	Private m_srcPath
	Private m_dstPath
	Private m_switchM	'/M �� True or False
	Private m_switchE	'/E �� True or False
	Private m_switchC	'/C �� True or False
	Private m_switchH	'/H �� True or False
	Private m_switchR	'/R �� True or False
	Private m_switchY	'/Y �� True or False
	
	Public Property Let SrcPath(newValue)
		m_srcPath = newValue
	End Property
	
	Public Property Get SrcPath()
		SrcPath = m_srcPath
	End Property
	
	Public Property Let DstPath(newValue)
		m_dstPath = newValue
	End Property
	
	Public Property Get DstPath()
		DstPath = m_dstPath
	End Property
	
	Public Property Let SwitchM(newValue)
		m_switchM = newValue
	End Property
	
	Public Property Get SwitchM()
		SwitchM = m_switchM
	End Property
	
	Public Property Let SwitchE(newValue)
		m_switchE = newValue
	End Property
	
	Public Property Get SwitchE()
		SwitchE = m_switchE
	End Property
	
	Public Property Let SwitchC(newValue)
		m_switchC = newValue
	End Property
	
	Public Property Get SwitchC()
		SwitchC = newValue
	End Property
	
	Public Property Let SwitchH(newValue)
		m_switchH = newValue
	End Property
	
	Public Property Get SwitchH()
		SwitchH = newValue
	End Property
	
	Public Property Let SwitchR(newValue)
		m_switchR = newValue
	End Property
	
	Public Property Get SwitchR()
		SwitchR = newValue
	End Property
	
	Public Property Let SwitchY(newValue)
		m_switchY = newValue
	End Property
	
	Public Property Get SwitchY()
		SwitchY = newValue
	End Property
	
	Public Sub Run()
		Dim strCmd, objShell
		
		strCmd = "xcopy.exe"
		If m_switchM = True Then
			strCmd = strCmd & " /M"
		End If
		If m_switchE = True Then
			strCmd = strCmd & " /E"
		End If
		If m_switchC = True Then
			strCmd = strCmd & " /C"
		End If
		If m_switchH = True Then
			strCmd = strCmd & " /H"
		End If
		If m_switchR = True Then
			strCmd = strCmd & " /R"
		End If
		If m_switchY = True Then
			strCmd = strCmd & " /Y"
		End If
		strCmd = strCmd & " """ & m_srcPath & """"
		strCmd = strCmd & " """ & m_dstPath & """"

		Set objShell = WScript.CreateObject("WScript.Shell")
		Call objShell.Run(strCmd, 1, True)
	End Sub
End Class
