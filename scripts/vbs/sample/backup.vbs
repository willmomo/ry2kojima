'----------------------------------------------------------------------
' MIRROR ラッパークラス
'----------------------------------------------------------------------
Class CMIRROR
	Private m_orgPath
	Private m_mirPath
	Private m_excludeOpt

	Public Property Let OrgPath(newValue)
		m_orgPath = newValue
	End Property
	
	Public Property Get OrgPath()
		OrgPath = m_orgPath
	End Property
	
	Public Property Let MirPath(newValue)
		m_mirPath = newValue
	End Property
	
	Public Property Get MirPath()
		MirPath = m_mirPath
	End Property
	
	Public Property Let ExcludeOption(newValue)
		m_excludeOpt = newValue
	End Property

	Public Property Get ExcludeOption()
		ExcludeOption = m_excludeOption
	End Property

	Public Sub Run()
		Dim strCmd, objShell
		
		strCmd = "c:\tools\mirror.exe"
		strCmd = strCmd & " """ & m_orgPath & """"
		strCmd = strCmd & " """ & m_mirPath & """"

		If m_excludeOpt <> "" Then
			strCmd = strCmd & " --exclude=""" & m_excludeOpt & """"
		End If

		Set objShell = WScript.CreateObject("WScript.Shell")
		Call objShell.Run(strCmd, 1, True)
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

Sub DoMain()
	Dim objNTBackup, objMirror, strDate
	
	Set objNTBackup = New CNTBackup
	Set objMirror = New CMIRROR
	
	strDate = Replace(Date, "/", "")

	objMirror.ExcludeOption = "exclude-list.txt"
	objMirror.OrgPath = "O:\share"
	objMirror.MirPath = "P:\share"
	objMirror.Run
	
	objMirror.OrgPath = "Q:\drvimg"
	objMirror.MirPath = "R:\drvimg"
	objMirror.Run
	objMirror.OrgPath = "Q:\fixhome"
	objMirror.MirPath = "R:\fixhome"
	objMirror.Run
	objMirror.OrgPath = "Q:\fixshare"
	objMirror.MirPath = "R:\fixshare"
	objMirror.Run
	objMirror.OrgPath = "Q:\hqpc209"
	objMirror.MirPath = "R:\hqpc209"
	objMirror.Run
	
	objMirror.OrgPath = "S:\home"
	objMirror.MirPath = "T:\home"
	objMirror.Run
	objMirror.OrgPath = "S:\img1"
	objMirror.MirPath = "T:\img1"
	objMirror.Run
	objMirror.OrgPath = "S:\img2"
	objMirror.MirPath = "T:\img2"
	objMirror.Run

	objNTBackup.BackupTarget = "O:\share Q:\drvimg Q:\fixhome Q:\fixshare Q:\hqpc209 S:\home S:\img1 S:\img2"
	objNTBackup.BackupFile = "\\ts01\pcbackup\_server\ts00_" & strDate & ".bkf"
	objNTBackup.Mode = "incremental"
	objNTBackup.Description = "TS00 incremental backup(" & Now & ")"
	objNTBackup.Run
End Sub


Call DoMain
