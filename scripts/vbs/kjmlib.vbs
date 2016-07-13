Option Explicit
'------------------------------------------------------------------------------
' VBScript用のcode snipet
'------------------------------------------------------------------------------

'----------------------------------------------------------------------
' テンポラリフォルダを取得
'----------------------------------------------------------------------
Function Path_GetTempPath()
	Const TemporaryFolder = 2	'GetSpecialFolder メソッド、引数 option のための変数
	Dim objFs
	Set objFs = CreateObject("Scripting.FileSystemObject")
	Path_GetTempPath = objFs.GetSpecialFolder(TemporaryFolder)
End Function


'----------------------------------------------------------------------
' ファイルが存在したら削除する
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
' ログ(%TEMP%\<script name>.log)に出力
'------------------------------------------------------------------------------
Sub LogWrite(msg)
	Const ForAppending = 8		'OpenTextFile メソッド、引数 create のための変数

	Dim objFs
	Dim objTs

	Set objFs = CreateObject("Scripting.FileSystemObject")
	Set objTs = objFs.OpenTextFile(objFs.BuildPath(Path_GetTempPath(), objFs.GetBaseName(WScript.ScriptName) & ".log"), ForAppending, True)
	objTs.WriteLine("[" & Now & "] " & msg)
	objTs.Close
End Sub

'----------------------------------------------------------------------
' ログに書き込む
'----------------------------------------------------------------------
Sub WriteLog(msg)
	Const ForAppending = 8
	Dim objFs
	Dim objTs

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objTs = objFs.OpenTextFile(objFs.BuildPath(Path_GetTempPath, "前準備.log"), ForAppending, True)
	objTs.WriteLine(msg)
	objTs.Close
End Sub


'----------------------------------------------------------------------
' ログの最終行を取得
'----------------------------------------------------------------------
Function TailLog()
	Const ForReading = 1
	Dim objFs
	Dim objTs

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objTs = g_objFs.OpenTextFile(objFs.BuildPath(Path_GetTempPath, "前準備.log"), ForReading, True)
	Do Until objTs.AtEndOfStream
		TailLog = objTs.ReadLine
	Loop
	objTs.Close
End Function

'------------------------------------------------------------------------------
' カレントディレクトリを取得/設定する
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
' コマンドを実行する
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
' フォルダを作成する
'----------------------------------------------------------------------
Function CreateFolder(folderspec)
	Dim objFs
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	
	''WScript.Echo folderspec
	
	'失敗で初期化
	CreateFolder = False
	
	If folderspec = "" Then
		CreateFolder = True
		Exit Function
	End If
	
	If objFs.FolderExists(folderspec) Then
		'すでにフォルダが存在するときは、正常終了扱い
		CreateFolder = True
	Else
		'親フォルダの作成を試みてから
		If CreateFolder(objFs.GetParentFolderName(folderspec)) Then
			'目的のフォルダを作成する
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

' intWinodwStyle 用定数
Public Const SW_HIDE = 0	' ウィンドウを非表示にし、別のウィンドウをアクティブにします。

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
' ファイルが存在したら移動する
'
' スクリプトにコピーして使うか、
' 以下の一行を追加して、疑似的に #include "MoveFileIfExists.vbs" を処理する。
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
' mirror.exe ラッパークラス
'
' update: 2011/07/26
'------------------------------------------------------------------------------
Class mirrorExe
	'--------------------------------------------------------------------------
	' コピー元フォルダの設定と取得
	Private m_orgPath
	
	Public Property Let OrgPath(newValue)
		m_orgPath = newValue
	End Property
	
	Public Property Get OrgPath()
		OrgPath = m_orgPath
	End Property

	'--------------------------------------------------------------------------
	' コピー先フォルダの設定と取得
	Private m_mirPath
	
	Public Property Let MirPath(newValue)
		m_mirPath = newValue
	End Property
	
	Public Property Get MirPath()
		MirPath = m_mirPath
	End Property

	'--------------------------------------------------------------------------
	' --LogPath オプションの設定と取得
	Private m_logPath
	
	Public Property Let LogPath(newValue)
		m_logPath = newValue
	End Property
	
	Public Property Get LogPath()
		LogPath = m_logPath
	End Property
	
	'--------------------------------------------------------------------------
	' --exclude オプションの設定と取得
	Private m_excludeOpt

	Public Property Let ExcludeOption(newValue)
		m_excludeOpt = newValue
	End Property

	Public Property Get ExcludeOption()
		ExcludeOption = m_excludeOption
	End Property

	'--------------------------------------------------------------------------
	' --mode オプションの設定と取得
	Private m_mode

	Public Property Let Mode(newValue)
		m_mode = newValue
	End Property

	Public Property Get Mode()
		Mode = m_mode
	End Property

	'--------------------------------------------------------------------------
	' --test オプションの設定と取得
	Private m_test

	Public Property Let Test(newValue)
		m_test = newValue
	End Property

	Public Property Get Test()
		Test = m_test
	End Property

	'--------------------------------------------------------------------------
	' mirror.exe の実行。
	'
	Public Sub Run()
		Dim strCmd
		
		' 実行ファイル名の指定、パスが通っているものとする。
		strCmd = "mirror.exe"
		
		' コピー元フォルダの指定
		If InStr(OrgPath, " ") > 0 Then
			strCmd = strCmd & " """ & OrgPath & """"
		Else
			strCmd = strCmd & " " & OrgPath
		End If
		
		' コピー先フォルダの指定
		If InStr(MirPath, " ") > 0 Then
			strCmd = strCmd & " """ & MirPath & """"
		Else
			strCmd = strCmd & " " & MirPath
		End If

		' --exclude オプションの指定
		If ExcludeOpt <> "" Then
			If InStr(ExcludeOpt, " ") > 0 Then
				strCmd = strCmd & " --exclude=""" & ExcludeOpt & """"
			Else
				strCmd = strCmd & " --exclude=" & ExcludeOpt
			End If
		End If

		' --LogPath オプションの指定
		If LogPath <> "" Then
			If InStr(LogPath, " ") > 0 Then
				strCmd = strCmd & " --logpath=""" & LogPath & """"
			Else
				strCmd = strCmd & " --logpath=" & LogPath
			End If
		End If

		' --mode オプションの指定
		If Mode <> "" Then strCmd = strCmd & " --mode=" & Mode

		' --test オプションの指定
		If Test <> False Then strCmd = strCmd & " --test"
		
		''WScript.Echo strCmd

		' ウィンドウを最新のサイズと位置で表示します。アクティブなウィンドウになります。
		''Call WScript.CreateObject("WScript.Shell").Run(strCmd, 1, True)
		
		' ウィンドウを最新のサイズと位置で表示します。アクティブなウィンドウは切り替わりません。
		Call WScript.CreateObject("WScript.Shell").Run(strCmd, 4, True)
		
		' ウィンドウを最小化ウィンドウとして表示します。アクティブなウィンドウは切り替わりません。
		''Call WScript.CreateObject("WScript.Shell").Run(strCmd, 7, True)
	End Sub

	'--------------------------------------------------------------------------
	' クラスのイニシャライズ
	'
	Private Sub Class_Initialize()
		m_test = False
	End Sub
End Class

'----------------------------------------------------------------------
' NTBACKUPのラッパークラス
'
' [使い方]
' Dim objNTBackup
' Set objNTBackup = New CNTBackup
' objNTBackup.BackupTarget = "D:\work"
' objNTBackup.BackupFile = "\\nas\share\_backup\hoge.bkf"
' objNTBackup.Mode = "normal"
' objNTBackup.Description = "WORKのバックアップ(" & Now & ")"
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
' XCOPY ラッパークラス
'----------------------------------------------------------------------
Class CXCopy
	Private m_srcPath
	Private m_dstPath
	Private m_switchM	'/M の True or False
	Private m_switchE	'/E の True or False
	Private m_switchC	'/C の True or False
	Private m_switchH	'/H の True or False
	Private m_switchR	'/R の True or False
	Private m_switchY	'/Y の True or False
	
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
