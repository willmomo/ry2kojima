'----------------------------------------------------------------------
' NTBACKUP���I�u�W�F�N�g�w���ŋN�����邱�Ƃ̂ł���N���X�̃T���v��
'
' 2009/07/01 r.kojima
'----------------------------------------------------------------------
Option Explicit

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
		
		If m_strMode <> "" Then
			strCmd = strCmd & " /M " & m_strMode
		End If
		
		If m_strDescription <> "" Then
			strCmd = strCmd & " /D """ & m_strDescription & """"
		End If

		Set objShell = WScript.CreateObject("WScript.Shell")
		Call objShell.Run(strCmd, 1, True)
	End Sub
End Class
