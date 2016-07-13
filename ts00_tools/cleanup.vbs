'----------------------------------------------------------------------
' �f�t���O��vbscript�ŏ������镔���́A�ȉ��̏����Q��
' http://gallery.technet.microsoft.com/scriptcenter/c1b28ae3-9213-48aa-b654-0bf6765134da
'----------------------------------------------------------------------
Option Explicit

GetFileLog.SetLogPathToScriptFolder "log"
GetFileLog.LogIdent = "CLR"
GetFileLog.LogWrite "---------- " & WScript.ScriptName & " start ----------"

Dim ret
ret = DoMain
'' ret = TestMain

GetFileLog.LogWrite "---------- " & WScript.ScriptName & " end ----------"

'�I�����[���𑗐M
SendJobMail "�N���[���A�b�v", "����I��"

WScript.Quit ret

'======================================================================


'----------------------------------------------------------------------
' t2dbm/ip�̃o�b�N�A�b�v���폜
'
' �g����: Call Cleanup_dump("T2DUMP", "t2dbm")
' �g����: Call Cleanup_dump("IPDUMP", "ip")
' �g����: Call Cleanup_dump("HINKAIDUMP", "hinkaidbm")
'----------------------------------------------------------------------
Sub Cleanup_dump(title, folder)
	Const SAVE_DAYS = 30
	Const BASE_PATH = "E:\lain2"
	Dim dir, obj, days

	Call GetFileLog.LogWrite("# Cleanup " & title & " start.")

	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	GetFileLog.LogWrite " cleanup " & BASE_PATH & "\share\@�J��\" & folder & "\backup (" & SAVE_DAYS & " days save)"
	For Each obj In fso.GetFolder(BASE_PATH & "\share\@�J��\" & folder & "\backup").Files
		days = DateDiff("d", obj.DateCreated, Now)
		If days > SAVE_DAYS Then
			GetFileLog.LogWrite " " & obj.Name & " - " & obj.DateCreated & " - " & days
			obj.Delete True
		End If
	Next
	GetFileLog.LogWrite " folder size: " & FormatHumanReadable(fso.GetFolder(BASE_PATH & "\share\@�J��\" & folder & "\backup").Size)

	Call GetFileLog.LogWrite("# Cleanup " & title & " end.")
End Sub

'----------------------------------------------------------------------
' dokuwiki/trac�̃o�b�N�A�b�v���폜
'----------------------------------------------------------------------
Sub Cleanup_wikidump()
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Const SAVE_DAYS = 4

	Call GetFileLog.LogWrite("# Cleanup WIKI/TRAC DUMP start.")

	GetFileLog.LogWrite " cleanup E:\lain2\share\_dokuwiki_backup (" & SAVE_DAYS & " days save)"
	DeleteOldFiles "E:\lain2\share\_dokuwiki_backup", SAVE_DAYS
	GetFileLog.LogWrite " folder size: " & FormatHumanReadable(fso.GetFolder("E:\lain2\share\_dokuwiki_backup").Size)

	Call GetFileLog.LogWrite("# Cleanup WIKI/TRAC DUMP end.")
End Sub

'----------------------------------------------------------------------
' ts02�̃o�b�N�A�b�v���폜
'----------------------------------------------------------------------
Sub Cleanup_ts02()
	Const SAVE_DAYS = 70
	Dim fso, dir, obj, days

	Call GetFileLog.LogWrite("# Cleanup TS02 start.")

	Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	GetFileLog.LogWrite " cleanup \\ts02\pcbackup\ts00"
	On Error Resume Next
	Set dir = fso.GetFolder("\\ts02\pcbackup\ts00")
	If Err.Number = 0 Then
		On Error Goto 0
		For Each obj In dir.SubFolders
			days = DateDiff("d", obj.DateCreated, Now)

			''GetFileLog.LogWrite " " & obj.Name & " - " & obj.DateCreated & " - " & days
			
			If days > SAVE_DAYS Then
				GetFileLog.LogWrite " " & obj.Name & " - " & obj.DateCreated & " - " & days
				obj.Delete True
				
				If Err.Number <> 0 Then
					GetFileLog.LogWrite " Delete error: " & Err.Number & " : " & Err.Description
				End If
			End If
		Next
	Else
		GetFileLog.LogWrite " error " & Err.Number & " : " & Err.Description
		GetFileLog.LogWrite " Cannot get folder object : '\\ts02\pcbackup\ts00'"
	End If
	
	Call GetFileLog.LogWrite("# Cleanup TS02 end.")
End Sub

'----------------------------------------------------------------------
' PubFTP�̌Â��t�@�C�����폜
'----------------------------------------------------------------------
Sub Cleanup_pubftp()
	Const SAVE_DAYS = 60
	Const SERVER = "\\pubftp"
	Dim fso, dir, obj, shell, fullPath, one

	Call GetFileLog.LogWrite("# Cleanup PubFTP start.")

	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Set shell = WScript.CreateObject("WScript.Shell")

	If shell.Run("net use " & SERVER & " /user:sankoftp H6fkLd", 0, True) = 0 Then
	
		' ry2kojima �́A����I�ɏ����Ȃ��t�H���_�ł��B
		
		For Each one In Array("backup", "dorasu", "exabyte", "gens", "glory", "maru", "sanko", "systemhouse", "triterasu")
			fullPath = SERVER & "\" & one

			GetFileLog.LogWrite " cleanup " & fullPath

			'�t�H���_���̌Â��t�@�C�����폜����
			Call DeleteOldFiles(fullPath, SAVE_DAYS)

			'����ۂ̃t�H���_������΁A������폜����
			For Each obj In fso.GetFolder(fullPath).SubFolders
				Call DeleteEmptyFolder(obj.Path)
			Next
		Next
	Else
		GetFileLog.LogWrite SERVER & " �ɃA�N�Z�X�ł��܂���B"
	End If

	Call GetFileLog.LogWrite("# Cleanup PubFTP end.")
End Sub

'----------------------------------------------------------------------
' �Â����O�t�@�C�����폜
'----------------------------------------------------------------------
Sub Cleanup_log()
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	Call GetFileLog.LogWrite("# Cleanup LOG start.")
	
	Const SAVE_DAYS = 90
	Dim path
	
	path = "C:\tools\log"
	GetFileLog.LogWrite " cleanup " & path & " (" & SAVE_DAYS & " days save)"
	DeleteOldFolder path, SAVE_DAYS
	GetFileLog.LogWrite " folder size: " & FormatHumanReadable(fso.GetFolder(path).Size)
	
	path = "C:\tools\log_inc"
	GetFileLog.LogWrite " cleanup " & path & " (" & SAVE_DAYS & " days save)"
	DeleteOldFolder path, SAVE_DAYS
	GetFileLog.LogWrite " folder size: " & FormatHumanReadable(fso.GetFolder(path).Size)

	path = "C:\tools\log_mir"
	GetFileLog.LogWrite " cleanup " & path & " (" & SAVE_DAYS & " days save)"
	DeleteOldFolder path, SAVE_DAYS
	GetFileLog.LogWrite " folder size: " & FormatHumanReadable(fso.GetFolder(path).Size)

	Call GetFileLog.LogWrite("# Cleanup LOG end.")
End Sub

' �쐬���ƍX�V�����ׂĐV�����ق���Ԃ�
Function NewerDate(obj)
	If obj.DateCreated > obj.DateLastModified Then
		NewerDate = obj.DateCreated
		GetFileLog.LogWrite "���t�t�]" & vbTab & obj.DateCreated & vbTab & obj.DateLastModified & vbTab & obj.Path
	Else
		NewerDate = obj.DateLastModified
	End If
End Function

'------------------------------------------------------------------------------
' �w��������Â��t�H���_���폜
'------------------------------------------------------------------------------
Sub DeleteOldFolder(path, saveDays)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	
	Dim obj
	For Each obj In fso.GetFolder(path).SubFolders
		'�ŏI�X�V���� �w����ȏ�Â��t�H���_���폜
		''GetFileLog.LogWrite "  " & NewerDate(obj) & vbTab & DateDiff("d", NewerDate(obj), Now) & vbTab & obj.Path
		
		If DateDiff("d", NewerDate(obj), Now) > saveDays Then
			GetFileLog.LogWrite "  " & NewerDate(obj) & vbTab & obj.Path

			' �Ώۃt�@�C�����폜���܂�!!
			fso.DeleteFolder obj.Path
		End If
	Next
End Sub

'------------------------------------------------------------------------------
'�@�w��������Â��t�@�C�����폜
'------------------------------------------------------------------------------
Sub DeleteOldFiles(path, saveDays)
	Dim fso, dir, obj

	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Set dir = fso.GetFolder(path)

	For Each obj In dir.Files
		'�ŏI�X�V���� �w����ȏ�Â��t�@�C�����폜
		If DateDiff("d", NewerDate(obj), Now) > saveDays Then
			GetFileLog.LogWrite NewerDate(obj) & vbTab & obj.Path
			GetFileLog.LogWrite obj.DateCreated & vbTab & obj.DateLastAccessed & vbTab & obj.DateLastModified & vbTab & obj.Path

			' �Ώۃt�@�C�����폜���܂�!!
			obj.Delete
		End If
	Next

	'�T�u�t�H���_�����l�ɌÂ��t�@�C�����폜����
	For Each obj In dir.SubFolders
		Call DeleteOldFiles(obj.Path, saveDays)
	Next
End Sub

'----------------------------------------------------------------------
' ����ۂ̃t�H���_���ċA�I�ɍ폜
'----------------------------------------------------------------------
Sub DeleteEmptyFolder(path)
	Dim fso, dir, obj

	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Set dir = fso.GetFolder(path)

	For Each obj In dir.SubFolders
		Call DeleteEmptyFolder(obj.Path)
	Next
	If dir.Files.Count = 0 And dir.SubFolders.Count = 0 Then
		GetFileLog.LogWrite "�폜 : " & path
		dir.Delete
	End If
End Sub

'----------------------------------------------------------------------
' �j���ɂ��킹�ăf�t���O�����s����
'----------------------------------------------------------------------
Sub Defrag_drive()
	Dim cmd, shell

	Call GetFileLog.LogWrite("# Defrag drive start.")

	Set shell = WScript.CreateObject("WScript.Shell")

	Select Case(Weekday(Now))
		Case vbMonday	'���j��
			cmd = "defrag C:"
			driveLetter = "C"
		Case vbTuesday	'�Ηj��
			cmd = "defrag O:"
			driveLetter = "O"
		Case vbWednesday	'���j��
			cmd = "defrag Q:"
			driveLetter = "Q"
		Case vbThursday	'�ؗj��
			cmd = "defrag W:"
			driveLetter = "W"
		Case Else
			cmd = ""
	End Select

	If cmd <> "" Then
		GetFileLog.LogWrite " " & cmd
		''GetFileLog.LogWrite " return code: " & shell.Run(cmd, 1, True)
		GetFileLog.LogWrite " return code: " & RunDefrag(driveLetter)
	End If

	Call GetFileLog.LogWrite("# Defrag drive end.")
End Sub

'----------------------------------------------------------------------
' wmi���g����defrag�̏���
'----------------------------------------------------------------------
Function RunDefrag(driveLetter)
	strComputer = "."
	Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")

	Set colVolumes = objWMIService.ExecQuery("Select * from Win32_Volume Where Name = '" & driveLetter & ":\\'")

	For Each objVolume in colVolumes
		GetFileLog.LogWrite " Please wait while drive " & objVolume.Name & " is defragged."
		errResult = objVolume.Defrag()

		If errResult = 0 Then
			GetFileLog.LogWrite " Drive " & objVolume.Name & " successfully defragged."
		Else  
			GetFileLog.LogWrite " Drive " & objVolume.Name & " could not be defragged."
			GetFileLog.LogWrite " Error number " & errResult & " occurred."
			Err.Clear
		End If
	Next 
	RunDefrag = errResult
End Function

'----------------------------------------------------------------------
' �v���O���� �X�^�[�g
'----------------------------------------------------------------------
Function DoMain
	GetFileLog.LogWrite "job:cleanup"
	
	Call Cleanup_dump("T2DUMP", "t2dbm")
	Call Cleanup_dump("IPDUMP", "ip")
	Call Cleanup_dump("HINKAIDUMP", "hinkaidbm")
	Call Cleanup_wikidump
	Call Cleanup_ts02
	Call Cleanup_pubftp
	''Call Defrag_drive
	Call Cleanup_log
	
	DoMain = 0
End Function

'----------------------------------------------------------------------
' �e�X�g�p�̃��C��
'----------------------------------------------------------------------
Function TestMain
	GetFileLog.LogWrite "job:cleanup(TestMain)"
	Call Cleanup_pubftp
	TestMain = 0
End Function
